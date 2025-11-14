#pragma once
#include "coord.hpp"
#include <SFML/System/Vector2.hpp>

// --- Tọa độ màn hình và bản đồ ---
const unsigned int WINDOW_WIDTH = 1600;
const unsigned int WINDOW_HEIGHT = 1000;
const float PADDING = 50.f; 

// Struct để giữ giới hạn của bản đồ (kinh độ/vĩ độ)
struct MapBounds {
    double minX = 180.0;
    double maxX = -180.0;
    double minY = 90.0;
    double maxY = -90.0;
};

// --- HÀM CHUYỂN ĐỔI TỌA ĐỘ (Chuyển từ main.cpp ra đây) ---
// (inline để tránh lỗi "multiple definition")
inline sf::Vector2f project(const coord& pos, const MapBounds& bounds) {
    
    double spanX = bounds.maxX - bounds.minX;
    double spanY = bounds.maxY - bounds.minY;

    float x;
    float y;

    if (spanX == 0.0) {
        x = (WINDOW_WIDTH / 2.0f); 
    } else {
        x = (float)((pos.x - bounds.minX) / spanX);
        x = x * (WINDOW_WIDTH - PADDING * 2) + PADDING;
    }

    if (spanY == 0.0) {
        y = (WINDOW_HEIGHT / 2.0f); 
    } else {
        y = (float)((pos.y - bounds.minY) / spanY);
        y = (1.0f - y) * (WINDOW_HEIGHT - PADDING * 2) + PADDING;
    }

    return sf::Vector2f(x, y);
}