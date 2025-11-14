#pragma once
#include <string>
#include "coord.hpp"

class edge{
	private:
		std::string name;
		int id;
		int src, dest;
		double weight;
		bool is_directed; 
		
		int m_vehicleCount; // (Cần cho logic Google Maps)
		
	public:
		edge();
		edge(std::string n, int i, int s, int d, double w, bool dir);
		
		std::string get_name() const;
		int get_id() const;
		int get_src() const;
		int get_dest() const;
		bool get_direction() const;
		double get_weight() const;
		
		virtual void display() const;
		virtual ~edge() = default;

		// (Cần cho logic Google Maps)
		void addVehicle();
		void removeVehicle();
		int getVehicleCount() const;
};