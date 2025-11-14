#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <list> 
#include <memory>  // Thêm dòng này
#include "node.hpp"
#include "ViewUtils.hpp" 

class map;
class edge; 

class Vehicle {
public: 
    enum class VehicleType { Car, Bus, Truck };
private:
    sf::CircleShape shape; 
    std::vector<int> path; 
    int pathIndex;         

    sf::Vector2f position;  
    sf::Vector2f targetPos; 

    float m_maxSpeed;     
    float m_currentSpeed; 
    VehicleType m_type;   
    
    int m_currentNodeId;  
    int m_targetNodeId;   
    edge* m_currentEdge; 
    
    bool finished = false;  
    bool m_isStopped; 
    sf::Clock m_travelTimer;

    sf::Vector2f getNodePosition(int nodeId, const std::map<int, node*>& nodeMap, const MapBounds& bounds);

public:
    Vehicle(VehicleType type, const std::vector<int>& p, const std::map<int, node*>& nodeMap, const MapBounds& bounds, map& worldMap);
    
    bool update(sf::Time dt, 
                const std::map<int, node*>& nodeMap, 
                const MapBounds& bounds,
                map& worldMap,
                const std::list<Vehicle>& allVehicles);
    
    void draw(sf::RenderTarget& target); 
    bool isFinished() const;
    bool isStopped() const;
    sf::Vector2f getPosition() const;
    int getCurrentTargetNodeId() const;
    
    // Thêm hàm lấy thời gian di chuyển
    float getTravelTime() const { return m_travelTimer.getElapsedTime().asSeconds(); }
};