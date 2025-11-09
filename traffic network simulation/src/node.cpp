#include "node.hpp"
#include <iostream>
using namespace std;
using namespace map_types;

// Ham dung ----------------
node::node() : name(""), id(0), pos(0, 0){}

node::node(string n, int i, double x_pos, double y_pos)
    : name(n), id(i), pos(x_pos, y_pos){}

//get set ------------------
string node::get_name() const{ return name; }
int node::get_id() const{ return id; }
coord node::get_coord() const { return pos;}
double node::get_x() const{ return pos.x; }
double node::get_y() const{ return pos.y; }

//phuong thuc------------------------
void node::display() const{
    cout << "Node [" << id << "] " << name
         << " (" << pos.x << ", " << pos.y << ")\n";
}

void node::display_name() const{
	cout << "[ID: " << id << "]" << name << endl;
}

//===================================================
//                    LOP KE THUA 
//===================================================

//=------------------- location ---------------------=
//ham dung ---------------------
location::location(string n, int i, double x_pos, double y_pos, int pop, lo_type t)
    : node(n, i, x_pos, y_pos), population(pop), type(t){}

//phuong thuc --------------------
void location::display() const{
	node::display();
	cout << " - Location type: " << to_string(type) << endl; 
    cout << " - Population: " << population << endl;
}


//=----------------- Junction ---------------------
//ham dung ---------------------
junction::junction(string n, int i, double x_pos, double y_pos, map_types::junction_type t)
 : node(n, i, x_pos, y_pos), type(t){}

//phuong thuc ---------------------
void junction::display() const{
 	 node::display();
 	 cout << " - Loai giao lo: " << to_string(type) << endl;
}

void junction::determine_type(int branch_cnt){
    if (type == junction_type::UNDEFINED){
        if (branch_cnt == 3){
            type = junction_type::T_JUNCTION;
        } else if (branch_cnt == 4){
            type = junction_type::CROSS_JUNCTION;
        } else if (branch_cnt > 4){
            type = junction_type::COMPLEX_JUNCTION; 
        }
    }
}

int junction::get_max_branches() const{
    switch (type){
        case junction_type::ROUNDABOUT:
            return 8;
        case junction_type::T_JUNCTION:
            return 3;
        case junction_type::CROSS_JUNCTION:
            return 4;
        case junction_type::COMPLEX_JUNCTION:
        case junction_type::UNDEFINED:
            return 100;
    }
    return 100;
}
