#pragma once

//coord = coordinate = toa do
struct coord{
	//x = kinh do, y = vi do 
	double x, y;
	
	coord();
	coord(double x_pos, double y_pos);
	
	double distance(const coord& a) const; 
};
