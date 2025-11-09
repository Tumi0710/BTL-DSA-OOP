#include "coord.hpp"
#include <cmath>
#include <iostream>
using namespace std;

coord::coord() : x(0), y(0) {}

coord::coord(double x_pos, double y_pos) : x(x_pos), y(y_pos) {}

double coord::distance(const coord& a) const{
	double dx = a.x - x;
	double dy = a.y - y;  
	return sqrt(pow(dx, 2) + pow(dy, 2));
} 
