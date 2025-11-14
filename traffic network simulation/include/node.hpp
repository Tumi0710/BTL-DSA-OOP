#pragma once
#include "coord.hpp"
#include "types.hpp" 
#include <string>
#include <SFML/System/Time.hpp>
#include <vector>
#include <map>

class node{
	private:
	    std::string name;
	    int id;
	    coord pos;
	
	public:
	    node();
	    node(std::string n, int i, double x_pos, double y_pos);
	
	    std::string get_name() const;
	    int get_id() const;
	    coord get_coord() const;
	    double get_x() const;
	    double get_y() const;
		void display_name() const;
	    virtual void display() const;
	    virtual ~node() = default; 
};

class location : public node{
	private:
	    int population;
	    map_types::lo_type type;
	
	public:
	    location(std::string n, int i, double x_pos, double y_pos, int pop, map_types::lo_type t);
	    void display() const override;
};

class junction : public node{
	public: 
		enum class LightState { Green, Yellow, Red };
		
	private:
		 map_types::junction_type type;
		 bool m_hasTrafficLight;
		 LightState m_lightState; 
		 sf::Time m_lightTimer; 
		 sf::Time m_greenDuration;
		 sf::Time m_yellowDuration;
		 sf::Time m_redDuration;
		 
		 // Quản lý xe chờ
		 int m_waitingVehicles;
		 sf::Time m_cycleTime;
		 float m_phaseOffset;

		 // Hệ thống đèn thông minh
		 struct TrafficPhase {
		     sf::Time duration;
		     std::vector<int> allowedDirections;
		 };
		 std::vector<TrafficPhase> m_trafficPhases;
		 int m_currentPhase;
		 std::map<int, int> m_laneWaitCount;

	public:
		junction(std::string n, int i, double x_pos, double y_pos, map_types::junction_type t);
		void display() const override;
		void determine_type(int branch_cnt);
		int get_max_branches() const; 
		
		// Hệ thống đèn cơ bản
		void setHasTrafficLight(bool hasLight, 
		                       float greenSecs = 7.f, 
		                       float yellowSecs = 2.f, 
		                       float redSecs = 9.f,
		                       float phaseOffset = 0.f);
		
		bool hasTrafficLight() const;
		LightState getLightState() const; 
		void updateLight(sf::Time dt); 
		
		// Quản lý hàng đợi
		void addWaitingVehicle();
		void removeWaitingVehicle();
		int getWaitingVehicles() const;
		bool canEnter() const;

		// Điều chỉnh động
		void adjustTimingBasedOnTraffic();

		// Hệ thống đèn thông minh
		void setSmartTrafficLight(bool enable, const std::vector<float>& phaseDurations = {});
		void addAllowedDirection(int phase, int fromNode, int toNode);
		bool canVehicleProceed(int fromNode, int toNode) const;
		void updateSmartLight(sf::Time dt);
		void adjustPhaseTiming();
};