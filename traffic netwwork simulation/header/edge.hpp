#pragma once
#include <string>

class edge{
	private:
		std::string name;
		int id;
		int src, dest;
		double weight;
		bool is_directed; //co huong 
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
};
