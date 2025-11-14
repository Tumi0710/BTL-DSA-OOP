#include "edge.hpp"
#include <iostream>
#include <iomanip> 

using namespace std;

// ham dung --------------------------------
edge::edge() : name(""), id(0), src(0), dest(0), weight(0), is_directed(false), m_vehicleCount(0) {} // <-- Khởi tạo

edge::edge(string n, int i, int s, int d, double w, bool dir)
	: name(n), id(i), src(s), dest(d), weight(w), is_directed(dir), m_vehicleCount(0) {} // <-- Khởi tạo


//get set ----------------------------------
string edge::get_name() const{ return name; } 
int edge::get_id() const{ return id; }
int edge::get_src() const{ return src; }
int edge::get_dest() const{ return dest; }
bool edge::get_direction() const {return is_directed; }
double edge::get_weight() const { return weight; }

//phuong thucs ------------------------------
void edge::display() const{
	cout << "Edge [" << id << "] " << name
         << " (" << src << ", " << dest << ")\n";
         
    if (is_directed){
    	cout << " - Duong 1 chieu" << endl;
	} else cout <<" - Duong 2 chieu" << endl;
	cout << " - Do dai: " << setprecision(2) << weight << " km" << endl;
}

// --- ⭐ NÂNG CẤP "GOOGLE MAPS" ---
void edge::addVehicle() {
    m_vehicleCount++;
}

void edge::removeVehicle() {
    if (m_vehicleCount > 0) {
        m_vehicleCount--;
    }
}

int edge::getVehicleCount() const {
    return m_vehicleCount;
}
// -------------------------------