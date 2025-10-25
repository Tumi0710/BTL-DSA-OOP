#pragma once
#include "coord.hpp"
#include <string>

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
	    coord get_pos() const;
	    double get_x() const;
	    double get_y() const;
	
	    virtual void display() const;
	    virtual ~node() = default;
};

// ================= LOP KE THUA ================== 
class location : public node{
	private:
	    int population;
	
	public:
	    location(std::string n, int i, double x_pos, double y_pos, int pop);
	    void display() const override;
};

