#pragma once
#include "coord.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

// Kich thuoc cua so chuong trinh
const unsigned int WINDOW_WIDTH = 1600;
const unsigned int WINDOW_HEIGHT = 900;

// Gioi han khu vuc ban do de tinh ty le ve
struct MapBounds {
    double minX = 181.0, maxX = -181.0, minY = 91.0, maxY = -91.0;
};

// Chuyen doi toa do dia ly (Kinh do/Vi do) sang toa do man hinh (Pixel)
inline sf::Vector2f project(const coord& c, const MapBounds& bounds) 
{
    float x = static_cast<float>((c.x - bounds.minX) / (bounds.maxX - bounds.minX)) * WINDOW_WIDTH;
    float y = (1.f - static_cast<float>((c.y - bounds.minY) / (bounds.maxY - bounds.minY))) * WINDOW_HEIGHT;
    return sf::Vector2f(x, y);
}