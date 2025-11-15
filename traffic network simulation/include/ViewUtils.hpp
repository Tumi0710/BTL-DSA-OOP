#pragma once
#include "coord.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

// ===== CÁC BIẾN KÍCH THƯỚC CỬA SỔ NẰM Ở ĐÂY =====
const unsigned int WINDOW_WIDTH = 1600;
const unsigned int WINDOW_HEIGHT = 900;
// ============================================

/*
Quay lại sử dụng MapBounds để tính toán
*/
struct MapBounds {
    double minX = 181.0, maxX = -181.0, minY = 91.0, maxY = -91.0;
};

/*
Chỉ sử dụng hàm project() với MapBounds
*/
inline sf::Vector2f project(const coord& c, const MapBounds& bounds) 
{
    float x = static_cast<float>((c.x - bounds.minX) / (bounds.maxX - bounds.minX)) * WINDOW_WIDTH;
    float y = (1.f - static_cast<float>((c.y - bounds.minY) / (bounds.maxY - bounds.minY))) * WINDOW_HEIGHT;
    return sf::Vector2f(x, y);
}

// ===== ĐÃ XÓA MapCalibration và hàm project(cal) =====