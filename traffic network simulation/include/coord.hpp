#pragma once

// Cau truc luu toa do dia ly
struct coord{
	double x, y; // x: Kinh do, y: Vi do
	
	coord();
	coord(double x_pos, double y_pos);
	
	// Tinh khoang cach giua 2 diem (don vi km)
	double distance(const coord& a) const; 
};