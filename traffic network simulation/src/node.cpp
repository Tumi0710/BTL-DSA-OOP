#include "node.hpp"
#include <iostream>
#include <algorithm>
using namespace std;
using namespace map_types;

// ============= LOP NODE CO BAN =============
node::node() : name(""), id(0), pos(0, 0){}
node::node(string n, int i, double x_pos, double y_pos)
    : name(n), id(i), pos(x_pos, y_pos){}
string node::get_name() const{ return name; }
int node::get_id() const{ return id; }
coord node::get_coord() const { return pos;}
double node::get_x() const{ return pos.x; }
double node::get_y() const{ return pos.y; }
void node::display() const{
    cout << "Node [" << id << "] " << name
         << " (" << pos.x << ", " << pos.y << ")\n";
}
void node::display_name() const{
	cout << "[ID: " << id << "]" << name << endl;
}

// ============= LOP LOCATION (DIA DIEM) =============
location::location(string n, int i, double x_pos, double y_pos, int pop, lo_type t)
    : node(n, i, x_pos, y_pos), population(pop), type(t){}
void location::display() const{
	node::display();
	cout << " - Location type: " << to_string(type) << endl; 
    cout << " - Population: " << population << endl;
}

// ============= LOP JUNCTION (GIAO LO) =============
junction::junction(string n, int i, double x_pos, double y_pos, map_types::junction_type t)
 : node(n, i, x_pos, y_pos), 
   type(t),
   m_hasTrafficLight(false), 
   m_lightState(LightState::Green), 
   m_lightTimer(sf::Time::Zero),
   m_waitingVehicles(0),
   m_cycleTime(sf::seconds(18.f)),
   m_phaseOffset(0.f),
   m_currentPhase(0)
{
    setHasTrafficLight(false);
}

void junction::display() const{
 	 node::display();
 	 cout << " - Loai giao lo: " << to_string(type) << endl;
}

void junction::determine_type(int branch_cnt){
    if (type == junction_type::UNDEFINED){
        if (branch_cnt == 3){ type = junction_type::T_JUNCTION; }
        else if (branch_cnt == 4){ type = junction_type::CROSS_JUNCTION; }
        else if (branch_cnt > 4){ type = junction_type::COMPLEX_JUNCTION; }
    }
}

int junction::get_max_branches() const{
    switch (type){
        case junction_type::ROUNDABOUT: return 8;
        case junction_type::T_JUNCTION: return 3;
        case junction_type::CROSS_JUNCTION: return 4;
        case junction_type::COMPLEX_JUNCTION:
        case junction_type::UNDEFINED: return 100;
    }
    return 100;
}

// Thiet lap den tin hieu va do lech pha (de tao lan song xanh)
void junction::setHasTrafficLight(bool hasLight, float greenSecs, float yellowSecs, float redSecs, float phaseOffset) {
    m_hasTrafficLight = hasLight;
    m_greenDuration = sf::seconds(greenSecs);
    m_yellowDuration = sf::seconds(yellowSecs);
    m_redDuration = sf::seconds(redSecs);
    m_cycleTime = m_greenDuration + m_yellowDuration + m_redDuration;
    m_phaseOffset = phaseOffset;
    
    if (hasLight && phaseOffset > 0.f) {
        float offsetTime = phaseOffset * m_cycleTime.asSeconds();
        m_lightTimer = sf::seconds(offsetTime);
        
        if (offsetTime < greenSecs) {
            m_lightState = LightState::Green;
        } else if (offsetTime < greenSecs + yellowSecs) {
            m_lightState = LightState::Yellow;
        } else {
            m_lightState = LightState::Red;
        }
    }
}

bool junction::hasTrafficLight() const {
    return m_hasTrafficLight;
}

junction::LightState junction::getLightState() const {
    if (!m_hasTrafficLight) return LightState::Green;
    return m_lightState;
}

// Cap nhat trang thai den theo thoi gian
void junction::updateLight(sf::Time dt) {
    if (!m_hasTrafficLight) return; 
    
    m_lightTimer += dt;
    
    float totalTime = m_lightTimer.asSeconds();
    float greenTime = m_greenDuration.asSeconds();
    float yellowTime = m_yellowDuration.asSeconds();
    float cycleTime = m_cycleTime.asSeconds();
    
    if (totalTime >= cycleTime) {
        m_lightTimer -= m_cycleTime;
        
        // Tu dong dieu chinh thoi gian den neu tac duong
        adjustTimingBasedOnTraffic(); 
        
        cycleTime = m_cycleTime.asSeconds(); 
        
        totalTime = m_lightTimer.asSeconds();
    }
    
    if (totalTime < greenTime) {
        m_lightState = LightState::Green;
    } else if (totalTime < greenTime + yellowTime) {
        m_lightState = LightState::Yellow;
    } else {
        m_lightState = LightState::Red;
    }
}

void junction::addWaitingVehicle() {
    m_waitingVehicles++;
}

void junction::removeWaitingVehicle() {
    if (m_waitingVehicles > 0) {
        m_waitingVehicles--;
    }
}

int junction::getWaitingVehicles() const {
    return m_waitingVehicles;
}

bool junction::canEnter() const {
    if (!m_hasTrafficLight) return true;
    
    if (m_lightState == LightState::Green) return true;
    if (m_lightState == LightState::Yellow) {
        // Neu den vang sap het thi khong duoc vuot
        float yellowRemaining = m_greenDuration.asSeconds() + m_yellowDuration.asSeconds() - m_lightTimer.asSeconds();
        return yellowRemaining > 0.5f;
    }
    return false;
}

// Logic dieu chinh thoi gian den dua tren so luong xe cho
void junction::adjustTimingBasedOnTraffic() {
    if (!m_hasTrafficLight) return;
    
    if (m_waitingVehicles > 5) {
        m_greenDuration = sf::seconds(10.f);
        m_redDuration = sf::seconds(8.f);
    } else if (m_waitingVehicles > 3) {
        m_greenDuration = sf::seconds(8.f);
        m_redDuration = sf::seconds(9.f);
    } else {
        m_greenDuration = sf::seconds(7.f);
        m_redDuration = sf::seconds(9.f);
    }
    
    m_cycleTime = m_greenDuration + m_yellowDuration + m_redDuration;
}

// Cac ham ho tro logic den thong minh phuc tap hon
void junction::setSmartTrafficLight(bool enable, const std::vector<float>& phaseDurations) {
    m_hasTrafficLight = enable;
    if (enable && !phaseDurations.empty()) {
        m_trafficPhases.clear();
        for (float duration : phaseDurations) {
            m_trafficPhases.push_back({sf::seconds(duration), {}});
        }
        m_currentPhase = 0;
        m_lightTimer = sf::Time::Zero;
    }
}

void junction::addAllowedDirection(int phase, int fromNode, int toNode) {
    if (phase < m_trafficPhases.size()) {
        int directionKey = fromNode * 1000 + toNode;
        m_trafficPhases[phase].allowedDirections.push_back(directionKey);
    }
}

bool junction::canVehicleProceed(int fromNode, int toNode) const {
    if (!m_hasTrafficLight) return true;
    
    int directionKey = fromNode * 1000 + toNode;
    auto& currentPhase = m_trafficPhases[m_currentPhase];
    
    return std::find(currentPhase.allowedDirections.begin(),
                    currentPhase.allowedDirections.end(),
                    directionKey) != currentPhase.allowedDirections.end();
}

void junction::updateSmartLight(sf::Time dt) {
    if (!m_hasTrafficLight || m_trafficPhases.empty()) return;
    
    m_lightTimer += dt;
    if (m_lightTimer >= m_trafficPhases[m_currentPhase].duration) {
        m_lightTimer = sf::Time::Zero;
        m_currentPhase = (m_currentPhase + 1) % m_trafficPhases.size();
        adjustPhaseTiming();
    }
}

void junction::adjustPhaseTiming() {
    int totalWaiting = m_waitingVehicles;
    if (totalWaiting > 10) {
        for (auto& phase : m_trafficPhases) {
            phase.duration = sf::seconds(std::min(20.f, phase.duration.asSeconds() * 1.2f));
        }
    } else if (totalWaiting < 3) {
        for (auto& phase : m_trafficPhases) {
            phase.duration = sf::seconds(std::max(5.f, phase.duration.asSeconds() * 0.8f));
        }
    }
}