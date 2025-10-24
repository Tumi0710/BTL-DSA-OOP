#include "edge.hpp"
#include <iostream>

using namespace std;

// ham dung --------------------------------
edge::edge() : name(""), id(0), src(0), dest(0), weight(0){}

edge::edge(string n, int i, int s, int d, double w)
	: name(n), id(i), src(s), dest(d), weight(w){}


//get set ----------------------------------
string edge::get_name() const{ return name; } 
int edge::get_id() const{ return id; }
int edge::get_src() const{ return src; }
int edge::get_dest() const{ return dest; }
double edge::get_weight() const { return weight; }

//phuong thucs ------------------------------
void edge::display() const{
	cout << "Edge [" << id << "] " << name
         << " (" << src << ", " << dest << ")\n";
}
