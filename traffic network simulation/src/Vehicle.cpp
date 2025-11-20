#include "Vehicle.hpp"
#include "map.hpp" 
#include "edge.hpp" 
#include "node.hpp" 
#include <cmath>
#include <list> 

Vehicle::Vehicle(VehicleType type, const std::vector<int>& p, const std::map<int, node*>& nodeMap, const MapBounds& bounds, map& worldMap)
    : path(p), pathIndex(0), m_type(type), finished(false), m_isStopped(false), m_currentEdge(nullptr),
      m_totalDistanceKm(0.0)
{
    switch(m_type) {
        case VehicleType::Car:
            m_maxSpeed = 120.f; shape.setRadius(5.f); shape.setFillColor(sf::Color::Red); break;
        case VehicleType::Bus:
            m_maxSpeed = 80.f; shape.setRadius(8.f); shape.setFillColor(sf::Color::Blue); break;
        case VehicleType::Truck:
            m_maxSpeed = 90.f; shape.setRadius(7.f); shape.setFillColor(sf::Color(139, 69, 19)); break;
    }
    shape.setOrigin(sf::Vector2f(shape.getRadius(), shape.getRadius())); 
    
    if (path.empty()) {
        finished = true; m_currentNodeId = -1; m_targetNodeId = -1; return;
    }
    
    m_currentSpeed = m_maxSpeed;
    m_currentNodeId = path[0]; 
    position = getNodePosition(m_currentNodeId, nodeMap, bounds); 
    shape.setPosition(position);
    
    if (path.size() > 1) {
        m_targetNodeId = path[1]; 
        targetPos = getNodePosition(m_targetNodeId, nodeMap, bounds); 
        m_currentEdge = worldMap.getEdge(m_currentNodeId, m_targetNodeId);
        if (m_currentEdge) {
            m_currentEdge->addVehicle();
        }
    } else {
        targetPos = position; m_targetNodeId = -1; finished = true; 
    }
}

sf::Vector2f Vehicle::getNodePosition(int nodeId, const std::map<int, node*>& nodeMap, const MapBounds& bounds) {
    node* n = nodeMap.at(nodeId); 
    return project(n->get_coord(), bounds); 
}

bool Vehicle::update(sf::Time dt, 
                    const std::map<int, node*>& nodeMap, 
                    const MapBounds& bounds, 
                    map& worldMap,
                    const std::list<Vehicle>& allVehicles)
{
    if (finished) return false; 
    
    const float STOP_DISTANCE = shape.getRadius() * 2.f + 12.f; 
    const float SLOW_DISTANCE = STOP_DISTANCE + 40.f; 
    
    if (m_targetNodeId == -1) { 
        finished = true;
        return false;
    }
    
    node* targetNode = nodeMap.at(m_targetNodeId);
    junction* targetJunc = dynamic_cast<junction*>(targetNode);
    
    m_currentSpeed = m_maxSpeed;
    bool mustStop = false;
    bool shouldSlow = false;
    
    sf::Vector2f direction = targetPos - position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (distance < 0.1f) distance = 0.0f;
    
    // Logic xu ly tai giao lo
    if (targetJunc) {
        // Kiem tra den giao thong
        if (targetJunc->hasTrafficLight()) {
            if (!targetJunc->canEnter()) {
                mustStop = true;
            }
        }
        
        // Kiem tra va cham voi xe khac phia truoc
        if (!mustStop) {
            for (const auto& other : allVehicles) {
                if (&other == this) continue; 
                if (other.getCurrentTargetNodeId() == this->m_targetNodeId) {
                    sf::Vector2f otherPos = other.getPosition();
                    sf::Vector2f otherToTargetDir = targetPos - otherPos;
                    float otherDistToTarget = std::sqrt(otherToTargetDir.x * otherToTargetDir.x + otherToTargetDir.y * otherToTargetDir.y);
                    sf::Vector2f toOther = otherPos - position;
                    float distToOther = std::sqrt(toOther.x * toOther.x + toOther.y * toOther.y);

                    if (otherDistToTarget < distance) {
                        if (distToOther < STOP_DISTANCE) {
                            mustStop = true; 
                            break; 
                        }
                        if (distToOther < SLOW_DISTANCE) {
                            shouldSlow = true;
                            float slowFactor = (distToOther - STOP_DISTANCE) / (SLOW_DISTANCE - STOP_DISTANCE);
                            m_currentSpeed = m_maxSpeed * std::max(0.1f, slowFactor);
                        }
                    }
                }
            }
        }
    }
    
    // Giam toc do khi den gan dich
    if (distance < SLOW_DISTANCE && !mustStop && !shouldSlow) {
        float slowFactor = distance / SLOW_DISTANCE;
        m_currentSpeed = m_maxSpeed * std::max(0.3f, slowFactor);
        shouldSlow = true;
    }
    
    float moveAmount = m_currentSpeed * dt.asSeconds();
    m_isStopped = false;
    
    if (mustStop) {
        if (distance < STOP_DISTANCE - 1.0f) {
             position = targetPos - ((direction / distance) * STOP_DISTANCE);
        }
        else if (distance <= STOP_DISTANCE) {
             m_isStopped = true;
             if (targetJunc) targetJunc->addWaitingVehicle();
        }
        else if (moveAmount >= (distance - STOP_DISTANCE)) {
            position = targetPos - ((direction / distance) * STOP_DISTANCE);
            m_isStopped = true;
            if (targetJunc) targetJunc->addWaitingVehicle();
        }
        else {
            position += (direction / distance) * moveAmount;
        }
    }
    else if (moveAmount >= distance) {
        // Da den mot diem nut, chuyen sang diem tiep theo trong lo trinh
        position = targetPos;
        pathIndex++; 
        
        if (m_currentEdge) {
            m_totalDistanceKm += m_currentEdge->get_weight();
            m_currentEdge->removeVehicle(); 
        }
        
        if (targetJunc && m_isStopped) { 
            targetJunc->removeWaitingVehicle();
        }
        
        m_currentNodeId = m_targetNodeId;

        if (pathIndex >= path.size()) {
            finished = true;
            return false;
        } else {
            m_targetNodeId = path[pathIndex]; 
            targetPos = getNodePosition(m_targetNodeId, nodeMap, bounds); 
            m_currentEdge = worldMap.getEdge(m_currentNodeId, m_targetNodeId);
            if (m_currentEdge) m_currentEdge->addVehicle();
        }
    }
    else {
        position += (direction / distance) * moveAmount;
    }

    shape.setPosition(position);
    return true; 
}

bool Vehicle::isStopped() const {
    return m_isStopped;
}

sf::Vector2f Vehicle::getPosition() const {
    return position;
}

int Vehicle::getCurrentTargetNodeId() const {
    return m_targetNodeId;
}

void Vehicle::draw(sf::RenderTarget& target) {
    if (finished) return;
    target.draw(shape);
}

bool Vehicle::isFinished() const {
    return finished;
}

double Vehicle::getTotalDistanceKm() const {
    return m_totalDistanceKm;
}