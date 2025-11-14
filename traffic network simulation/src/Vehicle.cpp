#include "Vehicle.hpp"
#include "map.hpp" 
#include "edge.hpp" 
#include "node.hpp" 
#include <cmath>
#include <list> 

Vehicle::Vehicle(VehicleType type, const std::vector<int>& p, const std::map<int, node*>& nodeMap, const MapBounds& bounds, map& worldMap)
    : path(p), pathIndex(0), m_type(type), finished(false), m_isStopped(false), m_currentEdge(nullptr)
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
    
    // ===== 1. CHUẨN BỊ =====
    // Tăng khoảng cách dừng an toàn
    const float STOP_DISTANCE = shape.getRadius() * 2.f + 12.f; 
    const float SLOW_DISTANCE = STOP_DISTANCE + 40.f; // Vùng giảm tốc
    
    if (m_targetNodeId == -1) { 
        finished = true;
        return false;
    }
    
    node* targetNode = nodeMap.at(m_targetNodeId);
    junction* targetJunc = dynamic_cast<junction*>(targetNode);
    
    // ===== 2. KIỂM TRA ĐIỀU KIỆN DỪNG =====
    m_currentSpeed = m_maxSpeed;
    bool mustStop = false;
    bool shouldSlow = false;
    
    sf::Vector2f direction = targetPos - position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (distance < 0.1f) distance = 0.0f;
    
    if (targetJunc) {
        // 2.1: Kiểm tra đèn giao thông
        if (targetJunc->hasTrafficLight()) {
            if (!targetJunc->canEnter()) {
                mustStop = true;
            }
        }
        
        // ===== SỬA LỖI XUNG ĐỘT Ở ĐÂY =====
        // 2.2: KIỂM TRA VA CHẠM (Xe đi trước)
        if (!mustStop) {
            for (const auto& other : allVehicles) {
                if (&other == this) continue; // Bỏ qua chính mình
                
                // Chỉ kiểm tra nếu xe khác cũng đang đi đến cùng nút (cùng đoạn đường)
                if (other.getCurrentTargetNodeId() == this->m_targetNodeId) {
                    
                    sf::Vector2f otherPos = other.getPosition();
                    
                    // Tính khoảng cách của XE KHÁC đến mục tiêu
                    sf::Vector2f otherToTargetDir = targetPos - otherPos;
                    float otherDistToTarget = std::sqrt(otherToTargetDir.x * otherToTargetDir.x + otherToTargetDir.y * otherToTargetDir.y);

                    // Tính khoảng cách trực tiếp giữa 2 xe
                    sf::Vector2f toOther = otherPos - position;
                    float distToOther = std::sqrt(toOther.x * toOther.x + toOther.y * toOther.y);

                    // Nếu xe 'other' ở phía trước (gần mục tiêu hơn xe 'this')
                    if (otherDistToTarget < distance) {
                        
                        // Và 2 xe quá gần nhau (vi phạm khoảng cách an toàn)
                        if (distToOther < STOP_DISTANCE) {
                            mustStop = true; // Dừng khẩn cấp
                            break; 
                        }
                        
                        // Nếu không quá gần, nhưng trong vùng "cẩn thận"
                        if (distToOther < SLOW_DISTANCE) {
                            shouldSlow = true;
                            // Giảm tốc độ dựa trên khoảng cách, càng gần càng chậm
                            float slowFactor = (distToOther - STOP_DISTANCE) / (SLOW_DISTANCE - STOP_DISTANCE);
                            m_currentSpeed = m_maxSpeed * std::max(0.1f, slowFactor);
                        }
                    }
                }
            }
        }
        // ===================================
    }
    
    // ===== 3. LOGIC GIẢM TỐC DẦN KHI TỚI ĐÍCH (Smoother) =====
    // Chỉ giảm tốc nếu không có xe khác cản đường (ưu tiên tránh va chạm)
    if (distance < SLOW_DISTANCE && !mustStop && !shouldSlow) {
        float slowFactor = distance / SLOW_DISTANCE;
        m_currentSpeed = m_maxSpeed * std::max(0.3f, slowFactor);
        shouldSlow = true;
    }
    
    // ===== 4. XỬ LÝ DI CHUYỂN =====
    float moveAmount = m_currentSpeed * dt.asSeconds();
    m_isStopped = false;
    
    // 4.1: Phải dừng lại (vì đèn đỏ hoặc có xe cản)
    if (mustStop) {
        // Nếu xe đang lố vạch dừng, lùi lại
        if (distance < STOP_DISTANCE - 1.0f) {
             position = targetPos - ((direction / distance) * STOP_DISTANCE);
        }
        // Nếu xe ở ngay vạch dừng
        else if (distance <= STOP_DISTANCE) {
             m_isStopped = true;
             if (targetJunc) targetJunc->addWaitingVehicle();
        }
        // Nếu sắp va chạm vạch dừng (phanh gấp)
        else if (moveAmount >= (distance - STOP_DISTANCE)) {
            position = targetPos - ((direction / distance) * STOP_DISTANCE);
            m_isStopped = true;
            if (targetJunc) targetJunc->addWaitingVehicle();
        }
        // Nếu đang di chuyển bình thường tới vạch dừng
        else {
            position += (direction / distance) * moveAmount;
        }
    }
    // 4.2: Đã đến nơi (Snap vào target)
    else if (moveAmount >= distance) {
        position = targetPos;
        pathIndex++; 
        
        if (m_currentEdge) {
            m_currentEdge->removeVehicle(); 
        }
        
        // Xóa khỏi hàng đợi nếu đang chờ
        if (targetJunc && m_isStopped) { // m_isStopped là trạng thái từ frame trước
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
    // 4.3: Di chuyển bình thường (hoặc giảm tốc)
    else {
        position += (direction / distance) * moveAmount;
    }

    shape.setPosition(position);
    return true; 
}

// ===== GETTERS & DRAW =====
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