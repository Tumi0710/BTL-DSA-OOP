#include <SFML/Graphics.hpp>
#include "map.hpp" 
#include <iostream>
#include <map> 
#include <vector>
#include <cmath> // Cần cho atan2
#include <limits>
#include <list> 
#include <random> 
#include <sstream>
#include <iomanip>
#include <optional>
#include <cstdlib> 

#include "ViewUtils.hpp" 
#include "Vehicle.hpp" 

// Cần cho hằng số M_PI (dùng để tính góc)
#define _USE_MATH_DEFINES
#include <math.h>

using namespace map_types;

// --- Thống kê hệ thống ---
class Statistics {
private:
    std::unique_ptr<sf::Text> statsText;
    sf::Font font;
    int totalVehicles;
    int vehiclesCompleted;
    float averageTravelTime;
    double m_totalDistanceKm;
    double m_averageDistanceKm;
    
public:
    Statistics() : totalVehicles(0), vehiclesCompleted(0), averageTravelTime(0),
                   m_totalDistanceKm(0.0), m_averageDistanceKm(0.0) 
    {
        if (font.openFromFile("assets/ARIAL.TTF")) {
            statsText = std::make_unique<sf::Text>(font, "", 14);
            statsText->setFillColor(sf::Color::White);
            statsText->setPosition(sf::Vector2f(10.f, 10.f));
        } else {
            statsText = nullptr;
        }
    }
    void vehicleSpawned() { totalVehicles++; }
    void vehicleCompleted(float travelTime, double distanceKm) {
        vehiclesCompleted++;
        averageTravelTime = (averageTravelTime * (vehiclesCompleted - 1) + travelTime) / vehiclesCompleted;
        m_totalDistanceKm += distanceKm;
        m_averageDistanceKm = m_totalDistanceKm / vehiclesCompleted;
    }
    void update(float fps) {
        if (!statsText) return;
        std::stringstream ss;
        ss << "Xe: " << totalVehicles << " (" << vehiclesCompleted << " hoan thanh)\n"
           << "Thoi gian di chuyen TB: " << std::fixed << std::setprecision(1) << averageTravelTime << "s\n"
           << "Quang duong di TB: " << std::fixed << std::setprecision(2) << m_averageDistanceKm << " km\n"
           << "FPS: " << std::setprecision(0) << fps;
        statsText->setString(ss.str());
    }
    void draw(sf::RenderWindow& window) {
        if (statsText) {
            window.draw(*statsText);
        }
    }
};

// Biến toàn cục
int startNodeId = -1;
int endNodeId = -1; 
std::vector<int> currentPath; 
std::list<Vehicle> vehicleManager;
sf::Clock deltaClock; 
bool isPanning = false;
sf::Vector2i panStartPos;
sf::View view; 

Statistics stats;


// ===== HÀM loadMapData PHIÊN BẢN CUỐI (ĐÃ KẾT NỐI CÁC ĐỊA ĐIỂM LƠ LỬNG) =====
void loadMapData(map& q1) {
    std::cout << "Loading map data...\n";

    // --- CÁC ĐỊA ĐIỂM (LOCATION) ---
    q1.add_node(new location("Dinh doc lap", 10, 106.69556, 10.77722, 500, lo_type::TOURIST_ATTRACTION));
    q1.add_node(new location("Thao cam vien", 11, 106.70472, 10.78806, 1000, lo_type::TOURIST_ATTRACTION));
    q1.add_node(new location("Cong vien 23/9", 12, 106.69380, 10.76940, 1000, lo_type::TOURIST_ATTRACTION));
    q1.add_node(new location("Toa Bitexco", 13, 106.70444, 10.77167, 1500, lo_type::LANDMARK));
    q1.add_node(new location("Nha tho duc ba", 14, 106.69900, 10.77992, 300, lo_type::TOURIST_ATTRACTION));
    q1.add_node(new location("Cho Ben Thanh", 15, 106.69806, 10.77250, 750, lo_type::TOURIST_ATTRACTION));
    q1.add_node(new location("Pho Ong Cat Gia Truyen Since 1988", 16, 106.69309, 10.76851, 25, lo_type::RESTAURANT));
    q1.add_node(new location("Nori - Modern Izakaya", 17, 106.69690, 10.77403, 50, lo_type::RESTAURANT));
    q1.add_node(new location("Trung tam bao hanh Asus", 18, 106.68918, 10.77317, 75, lo_type::OTHERS));
    
    // --- CÁC NÚT GIAO (JUNCTION) ---
    q1.add_node(new junction("GL_CMT8_ND", 100, 106.70222, 10.78440, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_DDL1", 101, 106.69305, 10.77735, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_DDL2", 102, 106.69500, 10.77956, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_DDL3", 103, 106.69798, 10.77680, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_DDL4", 104, 106.69605, 10.77475, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_N6PD", 105, 106.69316, 10.77140, junction_type::ROUNDABOUT));
    q1.add_node(new junction("GL_CNTD8", 106, 106.69990, 10.77896, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_NTDB1", 107, 106.69809, 10.78288, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_NTDB2", 108, 106.70102, 10.78019, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_CBT", 109, 106.69712, 10.77399, junction_type::UNDEFINED)); // Gần Nori (17)
    q1.add_node(new junction("GL_BFT1", 110, 106.70433, 10.77087, junction_type::UNDEFINED)); // Gần Bitexco (13)
    q1.add_node(new junction("GL_CV1", 111, 106.69536, 10.76807, junction_type::UNDEFINED)); // Gần CV 23/9 (12) và Phở (16)
    q1.add_node(new junction("GL_CV2", 112, 106.69816, 10.77120, junction_type::UNDEFINED)); // Gần Chợ Bến Thành (15)
    q1.add_node(new junction("GL_CBT1", 113, 106.69995, 10.77324, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_NTDB2", 114, 106.70208, 10.77555, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_TVC", 115, 106.70788, 10.78442, junction_type::UNDEFINED)); // Gần Thảo Cầm Viên (11)
    q1.add_node(new junction("GL_PAS_ND", 116, 106.6967, 10.7788, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_PAS_LL", 117, 106.6990, 10.7725, junction_type::UNDEFINED)); 
    q1.add_node(new junction("GL_NGH_LL", 118, 106.7022, 10.7730, junction_type::UNDEFINED)); 
    q1.add_node(new junction("GL_NGH_HN", 119, 106.7038, 10.7705, junction_type::UNDEFINED)); // Gần Bitexco (13)
    q1.add_node(new junction("GL_NKKN_HN", 120, 106.6995, 10.7695, junction_type::UNDEFINED)); 

    // --- CÁC CẠNH (EDGES) ---
    q1.add_edge_by_id("Duong Huyen Tran Cong Chua", 101104, 101, 104, 0.40, 1); 
    q1.add_edge_by_id("Duong Nguyen Thi Minh Khai", 101102, 101, 102, 0.35, 0); 
    q1.add_edge_by_id("Duong Nguyen Du", 102103, 102, 103, 0.45, 1); 
    q1.add_edge_by_id("Duong Ly Tu Trong", 104109, 104, 109, 0.35, 1); 
    q1.add_edge_by_id("Duong Le Thanh Ton", 109112, 109, 112, 0.40, 1); 
    q1.add_edge_by_id("Duong Truong Dinh", 105111, 105, 111, 0.40, 0); 
    q1.add_edge_by_id("Duong Pham Hong Thai", 105109, 105, 109, 0.30, 0); 
    q1.add_edge_by_id("Duong Cong Xa Paris", 106108, 106, 108, 0.26, 0); 
    q1.add_edge_by_id("Duong Han Thuyen", 107106, 107, 106, 0.28, 1); 
    q1.add_edge_by_id("Duong Dinh Tien Hoang", 108115, 108, 115, 0.50, 0); 
    q1.add_edge_by_id("Duong Le Duan", 108102, 108, 102, 0.70, 0); 
    q1.add_edge_by_id("Duong Le Duan 2", 108100, 108, 100, 0.20, 0); 
    q1.add_edge_by_id("Duong Thach Lam", 115100, 115, 100, 0.65, 0); 
    q1.add_edge_by_id("Duong Nguyen Du (doan 2)", 103116, 103, 116, 0.18, 1); 
    q1.add_edge_by_id("Duong Pasteur", 116104, 116, 104, 0.45, 1); 
    q1.add_edge_by_id("Duong Pasteur (doan 2)", 104117, 104, 117, 0.30, 1); 
    q1.add_edge_by_id("Duong Le Loi (doan 1)", 117118, 117, 118, 0.35, 1); 
    q1.add_edge_by_id("Duong Le Loi (doan 2)", 112117, 112, 117, 0.10, 1); 
    q1.add_edge_by_id("Pho di bo Nguyen Hue", 118119, 118, 119, 0.40, 1); 
    q1.add_edge_by_id("Duong Ham Nghi", 120119, 120, 119, 0.50, 0); 
    q1.add_edge_by_id("Duong NKKN (doan 2)", 112120, 112, 120, 0.25, 1); 
    q1.add_edge_by_id("Ket noi Ham Nghi - CV 23/9", 120111, 120, 111, 0.50, 0); 
    
    // --- LỐI VÀO CÁC ĐỊA ĐIỂM ---
    q1.add_edge_by_id("Loi vao Dinh Doc Lap", 1010, 103, 10, 0.15, 0); 
    q1.add_edge_by_id("Loi vao Nha Tho Duc Ba", 14106, 14, 106, 0.05, 0); 
    q1.add_edge_by_id("Loi vao Thao Cam Vien", 11115, 11, 115, 0.10, 0); 
    q1.add_edge_by_id("Loi vao Bitexco", 13110, 13, 110, 0.05, 0); 
    q1.add_edge_by_id("Loi vao Cho Ben Thanh", 15112, 15, 112, 0.10, 0); 
    q1.add_edge_by_id("Loi vao Bitexco 2", 119110, 119, 110, 0.10, 0);
    q1.add_edge_by_id("Loi vao Nori Izakaya", 17109, 17, 109, 0.05, 0); // Nối Nori (17) -> GL_CBT (109)
    q1.add_edge_by_id("Loi vao CV 23/9", 12111, 12, 111, 0.05, 0); // Nối CV 23/9 (12) -> GL_CV1 (111)
    q1.add_edge_by_id("Loi vao Pho Ong Cat", 16111, 16, 111, 0.10, 0); // Nối Phở (16) -> GL_CV1 (111)
    
    std::cout << "Map data loaded!\n";

    // --- CẤU HÌNH ĐÈN GIAO THÔNG ---
	dynamic_cast<junction*>(q1.find_node_by_id(102))->setHasTrafficLight(true, 7.f, 2.f, 9.f, 0.0f);
	dynamic_cast<junction*>(q1.find_node_by_id(103))->setHasTrafficLight(true, 6.f, 2.f, 8.f, 0.33f);
	dynamic_cast<junction*>(q1.find_node_by_id(106))->setHasTrafficLight(true, 8.f, 2.f, 10.f, 0.66f);
    dynamic_cast<junction*>(q1.find_node_by_id(112))->setHasTrafficLight(true, 7.f, 2.f, 9.f, 0.15f);
    dynamic_cast<junction*>(q1.find_node_by_id(118))->setHasTrafficLight(true, 8.f, 2.f, 10.f, 0.45f);
    dynamic_cast<junction*>(q1.find_node_by_id(119))->setHasTrafficLight(true, 7.f, 2.f, 9.f, 0.75f);

    std::cout << "Smart traffic lights configured!\n";
}

// Sửa hàm này để nhận 'bounds'
int findClosestNode(sf::Vector2f worldMousePos, 
                    const std::map<int, node*>& nodeMap, 
                    const MapBounds& bounds) // <-- Dùng MapBounds
{
    float minDistance = std::numeric_limits<float>::infinity();
    int closestNodeId = -1;
    sf::Vector2f projectedPos;

    for (const auto& pair : nodeMap) {
        node* n = pair.second;
        projectedPos = project(n->get_coord(), bounds); // <-- Dùng MapBounds
        float dx = projectedPos.x - worldMousePos.x;
        float dy = projectedPos.y - worldMousePos.y;
        float distance = std::sqrt(dx*dx + dy*dy);
        if (distance < minDistance && distance < 30.0f) { // Tăng khoảng cách click
            minDistance = distance;
            closestNodeId = n->get_id();
        }
    }
    return closestNodeId;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "Traffic Simulation - Optimized");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("assets/ARIAL.TTF")) {
        std::cerr << "Loi: Khong tai duoc font 'assets/ARIAL.TTF'\n";
        system("pause"); 
        return -1; 
    }
    
    // Mẫu cho Tên Địa điểm (Location)
    sf::Text locationNameText(font, "", 12);
    locationNameText.setFillColor(sf::Color(220, 220, 220)); 

    // Mẫu cho Tên Nút giao (Junction)
    sf::Text junctionNameText(font, "", 9); // Font nhỏ hơn
    junctionNameText.setFillColor(sf::Color(150, 150, 150)); // Màu xám

    // Mẫu cho Tên Đường (Road) - Sẽ dùng trong vòng lặp render
    sf::Text roadNameText(font, "", 9);
    roadNameText.setFillColor(sf::Color(200, 200, 0, 150)); // Màu vàng, hơi mờ


    std::random_device rd;
    std::mt19937 gen(rd()); // Đã sửa lỗi 'G'
    std::uniform_int_distribution<> vehicleTypeDist(0, 2);

    map q1;
    loadMapData(q1);
    q1.build_adjList();
    std::cout << "Adjacency List built!\n";

    MapBounds bounds; 
    for (node* n : q1.getNodes()) {
        double x = n->get_x(), y = n->get_y();
        if (x < bounds.minX) bounds.minX = x; if (x > bounds.maxX) bounds.maxX = x;
        if (y < bounds.minY) bounds.minY = y; if (y > bounds.maxY) bounds.maxY = y;
    }
    
    std::map<int, node*> nodeMap;
    for (node* n : q1.getNodes()) {
        nodeMap[n->get_id()] = n;
    }
    
    deltaClock.restart();
    
    view.setSize(sf::Vector2f(static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT)));
    view.setCenter(sf::Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f));
    window.setView(view);

    // ===== "BAKE" TÊN ĐỊA ĐIỂM VÀ NÚT GIAO =====
    std::vector<sf::Text> locationNameLabels;
    std::vector<sf::Text> junctionNameLabels; // Vector riêng cho tên nút giao

    for (node* n : q1.getNodes()) {
        sf::Vector2f nodePos = project(n->get_coord(), bounds);
        sf::FloatRect textBounds;
        float textWidth, textHeight;

        junction* junc = dynamic_cast<junction*>(n);
        if (junc) { 
            // NẾU LÀ NÚT GIAO (JUNCTION)
            junctionNameText.setString(n->get_name());
            textBounds = junctionNameText.getLocalBounds();
            // Dùng cú pháp SFML 3.0
            textWidth = textBounds.position.x + textBounds.size.x;
            textHeight = textBounds.position.y + textBounds.size.y;
            junctionNameText.setOrigin(sf::Vector2f(textWidth / 2.f, textHeight / 2.f));
            junctionNameText.setPosition(sf::Vector2f(nodePos.x, nodePos.y + 10.f)); // Hơi dịch xuống
            junctionNameLabels.push_back(junctionNameText);
        } else { 
            // NẾU LÀ ĐỊA ĐIỂM (LOCATION)
            locationNameText.setString(n->get_name());
            textBounds = locationNameText.getLocalBounds();
            // Dùng cú pháp SFML 3.0
            textWidth = textBounds.position.x + textBounds.size.x;
            textHeight = textBounds.position.y + textBounds.size.y;
            locationNameText.setOrigin(sf::Vector2f(textWidth / 2.f, textHeight / 2.f));
            locationNameText.setPosition(sf::Vector2f(nodePos.x, nodePos.y + 15.f)); // Dịch xuống nhiều hơn
            locationNameLabels.push_back(locationNameText);
        }
    }

    // ===== GAME LOOP =====
    while (window.isOpen())
    {
        // === XỬ LÝ SỰ KIỆN ===
        while (std::optional<sf::Event> event = window.pollEvent())
        {
            if (event.has_value()) {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }
                else if (const auto* mouseWheel = event->getIf<sf::Event::MouseWheelScrolled>()) {
                    if (mouseWheel->delta > 0) view.zoom(0.9f); 
                    else if (mouseWheel->delta < 0) view.zoom(1.1f); 
                }
                else if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseButton->button == sf::Mouse::Button::Middle) {
                        isPanning = true; 
                        panStartPos = mouseButton->position;
                    }
                    else if (mouseButton->button == sf::Mouse::Button::Right) {
                        startNodeId = -1; endNodeId = -1; currentPath.clear();
                    }
                    else if (mouseButton->button == sf::Mouse::Button::Left) {
                        sf::Vector2f worldMousePos = window.mapPixelToCoords(mouseButton->position, view);
                        int clickedNodeId = findClosestNode(worldMousePos, nodeMap, bounds);
                        if (clickedNodeId != -1) {
                            if (startNodeId == -1) {
                                startNodeId = clickedNodeId; endNodeId = -1; currentPath.clear();
                            } else if (endNodeId == -1 && clickedNodeId != startNodeId) {
                                endNodeId = clickedNodeId;
                                currentPath = q1.dijkstra(startNodeId, endNodeId);
                                if (!currentPath.empty()) {
                                    int randType = vehicleTypeDist(gen);
                                    Vehicle::VehicleType type;
                                    if (randType == 0) type = Vehicle::VehicleType::Car;
                                    else if (randType == 1) type = Vehicle::VehicleType::Bus;
                                    else type = Vehicle::VehicleType::Truck;
                                    vehicleManager.emplace_back(type, currentPath, nodeMap, bounds, q1); 
                                    stats.vehicleSpawned();
                                }
                            } else {
                                startNodeId = clickedNodeId; endNodeId = -1; currentPath.clear();
                            }
                        }
                    }
                }
                else if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonReleased>()) {
                    if (mouseButton->button == sf::Mouse::Button::Middle) {
                        isPanning = false;
                    }
                }
                else if (const auto* mouseMove = event->getIf<sf::Event::MouseMoved>()) {
                    if (isPanning) {
                        sf::Vector2f currentPos = window.mapPixelToCoords(mouseMove->position, view);
                        sf::Vector2f startPos = window.mapPixelToCoords(panStartPos, view);
                        sf::Vector2f delta = startPos - currentPos;
                        view.move(delta); 
                        panStartPos = mouseMove->position;
                    }
                }
            }
        } 

        // === CẬP NHẬT LOGIC ===
        sf::Time dt = deltaClock.restart(); 
        float fps = 1.f / dt.asSeconds();
        stats.update(fps);
 	    for (node* n : q1.getNodes()) {
 	 	    junction* junc = dynamic_cast<junction*>(n);
 	 	    if (junc) { 
 	 	 	    junc->updateLight(dt);
 	 	    }
 	    }
        for (auto it = vehicleManager.begin(); it != vehicleManager.end();) {
            if (it->update(dt, nodeMap, bounds, q1, vehicleManager)) { 
                ++it;
            } else {
                if (it->isFinished()) {
                    stats.vehicleCompleted(it->getTravelTime(), it->getTotalDistanceKm());
                }
                it = vehicleManager.erase(it);
            }
        }

        // === VẼ ĐỒ HỌA ===
        window.clear(sf::Color(50, 50, 50)); 
        window.setView(view);

        // 1. Vẽ đường phố
        sf::VertexArray road(sf::PrimitiveType::TriangleStrip, 4); 
        float roadThickness = 3.f; 
        for (edge* edge : q1.getEdges()) { 
            node* srcNode = nodeMap[edge->get_src()];
            node* destNode = nodeMap[edge->get_dest()];
            if (srcNode && destNode) {
                sf::Vector2f p1 = project(srcNode->get_coord(), bounds); 
                sf::Vector2f p2 = project(destNode->get_coord(), bounds); 
                sf::Vector2f direction = p2 - p1;
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                if (length == 0) continue; 
                sf::Vector2f unitDirection = direction / length;
                sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);
                sf::Vector2f offset = (roadThickness / 2.f) * unitPerpendicular;
                
                int carCount = edge->getVehicleCount();
                sf::Color roadColor;
                if (carCount > 6) { 
                    roadColor = sf::Color(200, 30, 30, 220);
                } else if (carCount > 3) { 
                    roadColor = sf::Color(230, 160, 0, 200);
                } else if (carCount > 0) {
                    roadColor = sf::Color(150, 150, 50, 200);
                } else { 
                    roadColor = sf::Color(100, 100, 100, 180);
                }

                road[0].position = p1 - offset; 
                road[1].position = p1 + offset;
                road[2].position = p2 - offset; 
                road[3].position = p2 + offset;
                for (int i=0; i<4; ++i) road[i].color = roadColor;
                window.draw(road);
            }
        }

        // 2. VẼ TÊN ĐƯỜNG (CHỈ KHI ZOOM GẦN)
        float currentZoom = view.getSize().x;
        if (currentZoom < WINDOW_WIDTH * 0.5) // Chỉ hiển thị khi zoom vào 50% trở lên
        {
            for (edge* edge : q1.getEdges()) {
                node* srcNode = nodeMap[edge->get_src()];
                node* destNode = nodeMap[edge->get_dest()];
                if (srcNode && destNode) {
                    sf::Vector2f p1 = project(srcNode->get_coord(), bounds); 
                    sf::Vector2f p2 = project(destNode->get_coord(), bounds); 
                    
                    sf::Vector2f midPoint = (p1 + p2) / 2.f;
                    float angle = std::atan2(p2.y - p1.y, p2.x - p1.x) * 180.f / M_PI;
                    
                    if (angle > 90.f || angle < -90.f) {
                        angle += 180.f;
                    }
                    
                    roadNameText.setString(edge->get_name());

                    // SỬA LỖI 1 & 2: Dùng .size.x, .size.y, .position.x/y
                    sf::FloatRect textBounds = roadNameText.getLocalBounds();
                    float textWidth = textBounds.position.x + textBounds.size.x;
                    float textHeight = textBounds.position.y + textBounds.size.y;
                    
                    // ⭐ SỬA LỖI 3: Dùng sf::Vector2f() cho setOrigin (Cú pháp SFML 3.0) ⭐
                    roadNameText.setOrigin(sf::Vector2f(textWidth / 2.f, textHeight / 2.f + 5.f)); // +5.f offset
                    
                    roadNameText.setPosition(midPoint);

                    // SỬA LỖI 4: Dùng sf::degrees()
                    roadNameText.setRotation(sf::degrees(angle));
                    
                    window.draw(roadNameText);
                }
            }
        }

        // 3. Vẽ tên địa điểm (LOCATION)
        for (const auto& text : locationNameLabels) {
            window.draw(text);
        }

        // 4. VẼ TÊN NÚT GIAO (JUNCTION) (CHỈ KHI ZOOM GẦN)
        if (currentZoom < WINDOW_WIDTH * 0.8) // Zoom gần hơn một chút
        {
            for (const auto& text : junctionNameLabels) {
                window.draw(text);
            }
        }

        // 5. Vẽ nút (với đèn động)
        sf::CircleShape nodeCircle(5.f); 
        for (node* n : q1.getNodes()) {
            sf::Vector2f nodePos = project(n->get_coord(), bounds); 
            junction* junc = dynamic_cast<junction*>(n);
            
            if (junc) { 
                nodeCircle.setRadius(4.f);
                if (junc->hasTrafficLight()) {
                    junction::LightState state = junc->getLightState();
                    if (state == junction::LightState::Green)
                        nodeCircle.setFillColor(sf::Color(0, 255, 0, 230));
                    else if (state == junction::LightState::Yellow)
                        nodeCircle.setFillColor(sf::Color(255, 255, 0, 230));
                    else 
                        nodeCircle.setFillColor(sf::Color(255, 0, 0, 230));
                } else {
                    nodeCircle.setFillColor(sf::Color(200, 200, 200, 180)); 
                }
            } else { 
                nodeCircle.setFillColor(sf::Color(0, 200, 255, 220)); 
                nodeCircle.setRadius(6.f);
            }
            nodeCircle.setPosition(nodePos);
            nodeCircle.setOrigin(sf::Vector2f(nodeCircle.getRadius(), nodeCircle.getRadius())); 
            window.draw(nodeCircle);
        }

        // 6. Vẽ đường đi (Path)
        if (!currentPath.empty()) {
            sf::VertexArray pathLine(sf::PrimitiveType::LineStrip);
            for (int nodeId : currentPath) {
                sf::Vertex v;
                v.position = project(nodeMap[nodeId]->get_coord(), bounds); 
                v.color = sf::Color(255, 255, 0, 200);
                pathLine.append(v);
            }
            window.draw(pathLine);
        }

        // 7. Vẽ điểm chọn
        sf::CircleShape selectionCircle(10.f);
        selectionCircle.setOrigin(sf::Vector2f(10.f, 10.f));
        selectionCircle.setFillColor(sf::Color::Transparent);
        selectionCircle.setOutlineThickness(3.f);
        if (startNodeId != -1) {
            selectionCircle.setOutlineColor(sf::Color(0, 255, 0, 200));
            selectionCircle.setPosition(project(nodeMap[startNodeId]->get_coord(), bounds)); 
            window.draw(selectionCircle);
        }
        if (endNodeId != -1) {
            selectionCircle.setOutlineColor(sf::Color(255, 0, 0, 200));
            selectionCircle.setPosition(project(nodeMap[endNodeId]->get_coord(), bounds)); 
            window.draw(selectionCircle);
        }

        // 8. Vẽ xe
        for (auto& vehicle : vehicleManager) {
            vehicle.draw(window);
        }

        // Vẽ thống kê (trên View mặc định)
        window.setView(window.getDefaultView());
        stats.draw(window);
        
        window.display();
    }

    return 0;
}