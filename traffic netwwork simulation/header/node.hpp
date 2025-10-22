#pragma once
#include <string>
#include <iostream>
using namespace std;

class node{
	protected:
	    string name;
	    int id;
	    double x, y;
	
	public:
	    node();
	    node(string n, int i, double x_pos, double y_pos);
	
	    string get_name() const;
	    int get_id() const;
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
	    location(string n, int i, double x_pos, double y_pos, int pop);
	    void display() const override;
};

