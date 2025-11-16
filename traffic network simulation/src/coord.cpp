#include "coord.hpp"
#include <cmath>
#include <iostream>

// Cần định nghĩa này TRƯỚC KHI #include <cmath> 
// (hoặc ở đầu tệp) để sử dụng hằng số M_PI
#define _USE_MATH_DEFINES 
#include <cmath> 

using namespace std;

coord::coord() : x(0), y(0) {}

coord::coord(double x_pos, double y_pos) : x(x_pos), y(y_pos) {}

// ⭐ HÀM TÍNH KHOẢNG CÁCH ĐÃ SỬA BẰNG CÔNG THỨC HAVERSINE ⭐
double coord::distance(const coord& a) const{
	// Công thức này tính khoảng cách "chim bay" (straight-line) 
    // giữa 2 điểm (kinh độ, vĩ độ) trên Trái Đất.
    
	const double R = 6371.0; // Bán kính Trái Đất (km)

    // Chuyển từ độ sang radian
    // (y là vĩ độ, x là kinh độ)
    double lat1_rad = y * M_PI / 180.0;
    double lon1_rad = x * M_PI / 180.0;
    double lat2_rad = a.y * M_PI / 180.0;
    double lon2_rad = a.x * M_PI / 180.0;

    // Tính delta (chênh lệch)
    double dLat = lat2_rad - lat1_rad;
    double dLon = lon2_rad - lon1_rad;

    // Áp dụng công thức Haversine
    double haversine = sin(dLat / 2.0) * sin(dLat / 2.0) +
                       cos(lat1_rad) * cos(lat2_rad) *
                       sin(dLon / 2.0) * sin(dLon / 2.0);
    
    double c = 2.0 * atan2(sqrt(haversine), sqrt(1.0 - haversine));
    
    // Trả về khoảng cách bằng Kilômét (KM)
    return R * c; 
}