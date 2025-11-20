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
		
		int m_vehicleCount; // So luong xe dang di tren canh nay (dung de tinh tac duong)
		
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

		// Quan ly so luong xe tren duong
		void addVehicle();
		void removeVehicle();
		int getVehicleCount() const;
};