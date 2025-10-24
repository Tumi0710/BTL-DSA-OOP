#pragma once
#include <string>

class edge{
	protected:
		std::string name;
		int id;
		int src, dest;
		double weight;
	public:
		edge();
		edge(std::string n, int i, int s, int d, double w);
		
		std::string get_name() const;
		int get_id() const;
		int get_src() const;
		int get_dest() const;
		double get_weight() const;
		
		virtual void display() const;
		virtual ~edge() = default;
};
