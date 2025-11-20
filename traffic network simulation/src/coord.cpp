#include "coord.hpp"
#include <cmath>
#include <iostream>

// Dinh nghia de su dung M_PI
#define _USE_MATH_DEFINES 
#include <cmath> 

using namespace std;

coord::coord() : x(0), y(0) {}

coord::coord(double x_pos, double y_pos) : x(x_pos), y(y_pos) {}

double coord::distance(const coord& a) const{
	// Su dung cong thuc Haversine de tinh khoang cach duong chim bay tren mat cau
	const double R = 6371.0; // Ban kinh Trai Dat (km)

    // Doi sang radian
    double lat1_rad = y * M_PI / 180.0;
    double lon1_rad = x * M_PI / 180.0;
    double lat2_rad = a.y * M_PI / 180.0;
    double lon2_rad = a.x * M_PI / 180.0;

    double dLat = lat2_rad - lat1_rad;
    double dLon = lon2_rad - lon1_rad;

    double haversine = sin(dLat / 2.0) * sin(dLat / 2.0) +
                       cos(lat1_rad) * cos(lat2_rad) *
                       sin(dLon / 2.0) * sin(dLon / 2.0);
    
    double c = 2.0 * atan2(sqrt(haversine), sqrt(1.0 - haversine));
    
    return R * c; 
}