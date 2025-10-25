#include "node.hpp"
#include <iostream>
using namespace std;

// Ham dung ----------------
node::node() : name(""), id(0), pos(0, 0){}

node::node(string n, int i, double x_pos, double y_pos)
    : name(n), id(i), pos(x_pos, y_pos){}

//get set ------------------
string node::get_name() const{ return name; }
int node::get_id() const{ return id; }
coord node::get_pos() const { return pos;}
double node::get_x() const{ return pos.x; }
double node::get_y() const{ return pos.y; }

//phuong thuc------------------------
void node::display() const{
    cout << "Node [" << id << "] " << name
         << " (" << pos.x << ", " << pos.y << ")\n";
}

//===================================================
//                  LOP KE THUA 
//===================================================

//=------------------- location ---------------------=
//ham dung ---------------------
location::location(string n, int i, double x_pos, double y_pos, int pop)
    : node(n, i, x_pos, y_pos), population(pop){}

//phuong thuc --------------------
void location::display() const{
	node::display();
    cout << " - Population: " << population << endl;
}

