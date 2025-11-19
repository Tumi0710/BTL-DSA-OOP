#include <SFML/Graphics.hpp>
#include "map.hpp" 
#include <iostream>
#include <map> 
#include <vector>
#include <cmath> 
#include <limits>
#include <list> 
#include <random> 
#include <sstream>
#include <iomanip>
#include <optional>
#include <cstdlib> 
#include <string> 
#include <cctype> 

#include "ViewUtils.hpp" 
#include "Vehicle.hpp" 

#define _USE_MATH_DEFINES
#include <math.h>

using namespace map_types;

// --- GIAO DIỆN & THỐNG KÊ ---
class Statistics {
private:
    sf::Font font;
    std::unique_ptr<sf::Text> statsText;
    sf::RectangleShape bgBox;
    
    int totalVehicles = 0;
    int vehiclesCompleted = 0;
    float averageTravelTime = 0;
    double m_totalDistanceKm = 0;
    double m_averageDistanceKm = 0;
    
public:
    Statistics() {
        if (font.openFromFile("assets/ARIAL.TTF")) {
            statsText = std::make_unique<sf::Text>(font, "", 16);
            statsText->setFillColor(sf::Color::White);
            statsText->setPosition(sf::Vector2f(20.f, 20.f));
            
            bgBox.setFillColor(sf::Color(0, 0, 0, 200)); 
            bgBox.setPosition(sf::Vector2f(10.f, 10.f));
            bgBox.setOutlineThickness(2.f);
            bgBox.setOutlineColor(sf::Color(200, 200, 200));
        }
    }
    
    void vehicleSpawned() { totalVehicles++; }
    
    void vehicleCompleted(float travelTime, double distanceKm) {
        vehiclesCompleted++;
        averageTravelTime = (averageTravelTime * (vehiclesCompleted - 1) + travelTime) / vehiclesCompleted;
        m_totalDistanceKm += distanceKm;
        m_averageDistanceKm = m_totalDistanceKm / vehiclesCompleted;
    }
    
    void update(float fps, int startNode, int endNode, const std::map<int, node*>& nodeMap) {
        if (!statsText) return;
        std::stringstream ss;
        
        std::string startName = "---";
        std::string endName = "---";

        if (startNode != -1 && nodeMap.count(startNode)) {
            startName = nodeMap.at(startNode)->get_name();
        }
        if (endNode != -1 && nodeMap.count(endNode)) {
            endName = nodeMap.at(endNode)->get_name();
        }
        
        ss << "=== BAN DO GIAO THONG Q1 ===\n"
           << "FPS: " << static_cast<int>(fps) << "\n\n" 
           << "--- TRANG THAI CHON ---\n"
           << "Diem DI  : " << startName << "\n"
           << "Diem DEN : " << endName << "\n\n" 
           << "--- THONG KE ---\n"
           << "Xe dang chay: " << (totalVehicles - vehiclesCompleted) << "\n"
           << "Xe hoan thanh: " << vehiclesCompleted << "\n"
           << "Quang duong TB: " << std::fixed << std::setprecision(2) << m_averageDistanceKm << " km\n\n"
           << "--- DIEU KHIEN ---\n"
           << "[F]       : Tim duong bang Ten\n"
           << "[Chuot]   : Chon diem truc tiep\n"
           << "[R]       : Reset Camera";

        statsText->setString(ss.str());
        
        sf::FloatRect bounds = statsText->getLocalBounds();
        bgBox.setSize(sf::Vector2f(bounds.size.x + 30.f, bounds.size.y + 30.f));
    }
    
    void draw(sf::RenderWindow& window) {
        if (statsText) {
            window.draw(bgBox);
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

// Biến hỗ trợ kéo thả chuột trái
bool isPanning = false;
sf::Vector2i panStartPos;       // Vị trí chuột (Pixel)
sf::Vector2i originalClickPos;  // Vị trí chuột gốc (Pixel)

sf::View view; 

Statistics stats;

bool isInteger(const std::string & s) {
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;
   char * p;
   strtol(s.c_str(), &p, 10);
   return (*p == 0);
}

int getNodeFromInput(map& worldMap, const std::string& prompt) {
    std::string inputName;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, inputName);

        if (inputName.empty()) continue;

        if (inputName == "?") {
            std::cout << "\n--- DANH SACH DIA DIEM ---\n";
            for (node* n : worldMap.getNodes()) {
                if (dynamic_cast<location*>(n)) {
                    std::cout << "ID " << n->get_id() << ": " << n->get_name() << "\n";
                }
            }
            std::cout << "--------------------------\n";
            continue;
        }

        if (isInteger(inputName)) {
            int id = std::stoi(inputName);
            node* n = worldMap.find_node_by_id(id);
            if (n) {
                std::cout << " >> Da chon theo ID: " << n->get_name() << "\n";
                return id;
            } else {
                std::cout << " >> Khong tim thay ID nay!\n";
                continue;
            }
        }

        std::vector<node*> results = worldMap.search_node_by_name(inputName);
        if (results.empty()) {
            std::cout << " >> Khong tim thay. (Nhap '?' de xem danh sach)\n";
        } else if (results.size() == 1) {
            std::cout << " >> Da chon: " << results[0]->get_name() << "\n";
            return results[0]->get_id();
        } else {
            std::cout << " >> Tim thay nhieu ket qua:\n";
            for (size_t i = 0; i < results.size(); ++i) 
                std::cout << "    [" << i + 1 << "] " << results[i]->get_name() << "\n";
            std::cout << " >> Chon so (0 de huy): ";
            int choice;
            if (std::cin >> choice) {
                std::cin.ignore(); 
                if (choice > 0 && choice <= results.size()) return results[choice - 1]->get_id();
                else if (choice == 0) return -1; 
            } else {
                std::cin.clear(); std::cin.ignore(10000, '\n');
            }
        }
    }
}

void handleConsoleInput(sf::RenderWindow& window, map& worldMap, const std::map<int, node*>& nodeMap, const MapBounds& bounds) {
    window.setActive(false); 
    std::cout << "\n========================================\n";
    std::cout << "       TIM DUONG (Go '?' de xem List)       \n";
    std::cout << "========================================\n";
    
    std::cin.clear();
    int sId = getNodeFromInput(worldMap, "Nhap ten/ID DIEM DI: ");
    if (sId != -1) {
        int eId = getNodeFromInput(worldMap, "Nhap ten/ID DIEM DEN: ");
        if (eId != -1 && sId != eId) {
            startNodeId = sId; endNodeId = eId;
            currentPath.clear(); 
            currentPath = worldMap.dijkstra(startNodeId, endNodeId);
            if (!currentPath.empty()) {
                std::cout << " >> THANH CONG: Xe dang xuat phat!\n";
                vehicleManager.emplace_back(Vehicle::VehicleType::Car, currentPath, nodeMap, bounds, worldMap);
                stats.vehicleSpawned();
            } else std::cout << " >> LOI: Khong co duong di giua 2 diem nay!\n";
        } else if (sId == eId) {
            std::cout << " >> LOI: Diem di va den trung nhau!\n";
        }
    }
    std::cout << "----------------------------------------\n";
    std::cout << " >> Quay lai cua so do hoa...\n\n";
    window.setActive(true);
    window.requestFocus(); 
    deltaClock.restart();  
}

// ===== HÀM loadMapData (ĐẦY ĐỦ) =====
void loadMapData(map& q1) {
    std::cout << "Loading map data...\n";
    // Locations
    q1.add_node(new location("Dinh doc lap", 10, 106.69556, 10.77722, 500, lo_type::TOURIST_ATTRACTION));
    q1.add_node(new location("Thao cam vien", 11, 106.70472, 10.78806, 1000, lo_type::TOURIST_ATTRACTION));
    q1.add_node(new location("Cong vien 23/9", 12, 106.69380, 10.76940, 1000, lo_type::TOURIST_ATTRACTION));
    q1.add_node(new location("Toa Bitexco", 13, 106.70444, 10.77167, 1500, lo_type::LANDMARK));
    q1.add_node(new location("Nha tho duc ba", 14, 106.69900, 10.77992, 300, lo_type::TOURIST_ATTRACTION));
    q1.add_node(new location("Cho Ben Thanh", 15, 106.69806, 10.77250, 750, lo_type::TOURIST_ATTRACTION));
    q1.add_node(new location("Pho Ong Cat Gia Truyen Since 1988", 16, 106.69309, 10.76851, 25, lo_type::RESTAURANT));
    q1.add_node(new location("Nori - Modern Izakaya", 17, 106.69690, 10.77403, 50, lo_type::RESTAURANT));
    q1.add_node(new location("Trung tam bao hanh Asus", 18, 106.68918, 10.77317, 75, lo_type::OTHERS));
    q1.add_node(new location("Buu dien trung tam", 19, 106.6999, 10.7800, 400, lo_type::TOURIST_ATTRACTION));
    q1.add_node(new location("Diamond Plaza", 20, 106.6995, 10.7815, 600, lo_type::LANDMARK));
    
    // Junctions
    q1.add_node(new junction("GL_CMT8_ND", 100, 106.70222, 10.78440, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_DDL1", 101, 106.69305, 10.77735, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_DDL2", 102, 106.69500, 10.77956, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_DDL3", 103, 106.69798, 10.77680, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_DDL4", 104, 106.69605, 10.77475, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_N6PD", 105, 106.69316, 10.77140, junction_type::ROUNDABOUT));
    q1.add_node(new junction("GL_CNTD8", 106, 106.69990, 10.77896, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_NTDB1", 107, 106.69809, 10.78288, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_NTDB2", 108, 106.70102, 10.78019, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_CBT", 109, 106.69712, 10.77399, junction_type::UNDEFINED)); 
    q1.add_node(new junction("GL_BFT1", 110, 106.70433, 10.77087, junction_type::UNDEFINED)); 
    q1.add_node(new junction("GL_CV1", 111, 106.69536, 10.76807, junction_type::UNDEFINED)); 
    q1.add_node(new junction("GL_CV2", 112, 106.69816, 10.77120, junction_type::UNDEFINED)); 
    q1.add_node(new junction("GL_CBT1", 113, 106.69995, 10.77324, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_NTDB2", 114, 106.70208, 10.77555, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_TVC", 115, 106.70788, 10.78442, junction_type::UNDEFINED)); 
    q1.add_node(new junction("GL_PAS_ND", 116, 106.6967, 10.7788, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_PAS_LL", 117, 106.6990, 10.7725, junction_type::UNDEFINED)); 
    q1.add_node(new junction("GL_NGH_LL", 118, 106.7022, 10.7730, junction_type::UNDEFINED)); 
    q1.add_node(new junction("GL_NGH_HN", 119, 106.7038, 10.7705, junction_type::UNDEFINED)); 
    q1.add_node(new junction("GL_NKKN_HN", 120, 106.6995, 10.7695, junction_type::UNDEFINED)); 
    q1.add_node(new junction("GL_ADR_NKKN", 121, 106.6952, 10.7808, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_ADR_PNT", 122, 106.6982, 10.7811, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_PNT_NTMK", 123, 106.6998, 10.7836, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_HBT_ND", 124, 106.6975, 10.7815, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_HBT_LD", 125, 106.6985, 10.7825, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_DK_LTT", 126, 106.7015, 10.7765, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_DK_ND", 127, 106.6980, 10.7785, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_HBT_LTT", 128, 106.7025, 10.7805, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_CMT8_NTMK", 129, 106.6895, 10.7740, junction_type::UNDEFINED)); 
    q1.add_node(new junction("GL_TD_NTMK", 130, 106.6910, 10.7755, junction_type::UNDEFINED)); 
    q1.add_node(new junction("GL_TD_ND", 131, 106.6935, 10.7730, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_THD_NCT", 132, 106.6945, 10.7675, junction_type::UNDEFINED)); 
    q1.add_node(new junction("GL_TDT_NHC", 133, 106.7075, 10.7810, junction_type::UNDEFINED)); 
    q1.add_node(new junction("GL_TDT_DK", 134, 106.7060, 10.7750, junction_type::UNDEFINED)); 
    q1.add_node(new junction("GL_CALMETTE", 135, 106.7000, 10.7660, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_NGUYEN_TRAI", 136, 106.6910, 10.7690, junction_type::UNDEFINED));
    q1.add_node(new junction("GL_DBP_NBK", 137, 106.6970, 10.7870, junction_type::UNDEFINED));

    // Edges
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
    q1.add_edge_by_id("Duong NKKN (doan 3)", 102121, 102, 121, 0.16, 0); 
    q1.add_edge_by_id("Duong A. de Rhodes", 121122, 121, 122, 0.35, 1); 
    q1.add_edge_by_id("Duong PNT (doan 1)", 123122, 123, 122, 0.28, 1); 
    q1.add_edge_by_id("Duong PNT (doan 2)", 122108, 122, 108, 0.16, 1); 
    q1.add_edge_by_id("Duong NTMK (doan 2)", 102123, 102, 123, 0.55, 0); 
    q1.add_edge_by_id("Duong NTMK (doan 3)", 123100, 123, 100, 0.25, 0); 
    q1.add_edge_by_id("Duong Nguyen Du (doan 3)", 121124, 121, 124, 0.20, 1);
    q1.add_edge_by_id("Duong Hai Ba Trung (doan 1)", 124125, 124, 125, 0.15, 0);
    q1.add_edge_by_id("Duong Hai Ba Trung (doan 2)", 125128, 125, 128, 0.45, 0);
    q1.add_edge_by_id("Duong Dong Khoi (doan 1)", 106127, 106, 127, 0.15, 1);
    q1.add_edge_by_id("Duong Dong Khoi (doan 2)", 127126, 127, 126, 0.40, 1);
    q1.add_edge_by_id("Duong Ly Tu Trong (doan 2)", 104126, 104, 126, 0.35, 1);
    q1.add_edge_by_id("Duong Ly Tu Trong (doan 3)", 126128, 126, 128, 0.30, 1);
    q1.add_edge_by_id("Duong Le Duan (doan 3)", 108125, 108, 125, 0.20, 0);
    q1.add_edge_by_id("Duong CMT8 (doan 1)", 12918, 129, 18, 0.10, 0); 
    q1.add_edge_by_id("Duong CMT8 (doan 2)", 129100, 129, 100, 0.80, 0); 
    q1.add_edge_by_id("Duong NTMK (doan 4)", 129130, 129, 130, 0.30, 1); 
    q1.add_edge_by_id("Duong Truong Dinh (doan 2)", 130131, 130, 131, 0.20, 1); 
    q1.add_edge_by_id("Duong Truong Dinh (doan 3)", 131105, 131, 105, 0.20, 1); 
    q1.add_edge_by_id("Duong NTMK (doan 5)", 130101, 130, 101, 0.30, 1); 
    q1.add_edge_by_id("Duong Tran Hung Dao", 132111, 132, 111, 0.30, 0); 
    q1.add_edge_by_id("Loi vao Pho (moi)", 16132, 16, 132, 0.10, 0); 
    q1.add_edge_by_id("Duong TDT (doan 1)", 115133, 115, 133, 0.40, 0); 
    q1.add_edge_by_id("Duong TDT (doan 2)", 133134, 133, 134, 0.50, 0); 
    q1.add_edge_by_id("Duong TDT (doan 3)", 134119, 134, 119, 0.40, 0); 
    q1.add_edge_by_id("Duong Ben Chuong Duong", 119135, 119, 135, 0.60, 0); 
    q1.add_edge_by_id("Loi vao CV 23/9 (phu)", 135111, 135, 111, 0.30, 0); 
    q1.add_edge_by_id("Duong Nguyen Trai", 111136, 111, 136, 0.40, 0); 
    q1.add_edge_by_id("Duong Cong Quynh", 136129, 136, 129, 0.50, 0); 
    q1.add_edge_by_id("Duong DBP (doan 1)", 129137, 129, 137, 0.80, 1); 
    q1.add_edge_by_id("Duong DBP (doan 2)", 13711, 137, 11, 0.30, 1); 

    // Entry points
    q1.add_edge_by_id("Loi vao Dinh Doc Lap", 1010, 103, 10, 0.15, 0); 
    q1.add_edge_by_id("Loi vao Nha Tho Duc Ba", 14106, 14, 106, 0.05, 0); 
    q1.add_edge_by_id("Loi vao Thao Cam Vien", 11115, 11, 115, 0.10, 0); 
    q1.add_edge_by_id("Loi vao Bitexco", 13110, 13, 110, 0.05, 0); 
    q1.add_edge_by_id("Loi vao Cho Ben Thanh", 15112, 15, 112, 0.10, 0); 
    q1.add_edge_by_id("Loi vao Bitexco 2", 119110, 119, 110, 0.10, 0);
    q1.add_edge_by_id("Loi vao Nori Izakaya", 17109, 17, 109, 0.05, 0); 
    q1.add_edge_by_id("Loi vao CV 23/9", 12111, 12, 111, 0.05, 0); 
    q1.add_edge_by_id("Loi vao Buu Dien", 19106, 19, 106, 0.02, 0);
    q1.add_edge_by_id("Loi vao Diamond Plaza", 20108, 20, 108, 0.05, 0);

    // --- CẤU HÌNH ĐÈN GIAO THÔNG ---
	dynamic_cast<junction*>(q1.find_node_by_id(102))->setHasTrafficLight(true, 7.f, 2.f, 9.f, 0.0f);
	dynamic_cast<junction*>(q1.find_node_by_id(103))->setHasTrafficLight(true, 6.f, 2.f, 8.f, 0.33f);
	dynamic_cast<junction*>(q1.find_node_by_id(106))->setHasTrafficLight(true, 8.f, 2.f, 10.f, 0.66f);
    dynamic_cast<junction*>(q1.find_node_by_id(112))->setHasTrafficLight(true, 7.f, 2.f, 9.f, 0.15f);
    dynamic_cast<junction*>(q1.find_node_by_id(118))->setHasTrafficLight(true, 8.f, 2.f, 10.f, 0.45f);
    dynamic_cast<junction*>(q1.find_node_by_id(119))->setHasTrafficLight(true, 7.f, 2.f, 9.f, 0.75f);
    dynamic_cast<junction*>(q1.find_node_by_id(121))->setHasTrafficLight(true, 7.f, 2.f, 9.f, 0.25f);
    dynamic_cast<junction*>(q1.find_node_by_id(122))->setHasTrafficLight(true, 8.f, 2.f, 10.f, 0.55f);
    dynamic_cast<junction*>(q1.find_node_by_id(123))->setHasTrafficLight(true, 7.f, 2.f, 9.f, 0.85f);
    dynamic_cast<junction*>(q1.find_node_by_id(125))->setHasTrafficLight(true, 7.f, 2.f, 9.f, 0.10f);
    dynamic_cast<junction*>(q1.find_node_by_id(128))->setHasTrafficLight(true, 8.f, 2.f, 10.f, 0.60f);
    dynamic_cast<junction*>(q1.find_node_by_id(129))->setHasTrafficLight(true, 8.f, 2.f, 10.f, 0.20f); 
    dynamic_cast<junction*>(q1.find_node_by_id(130))->setHasTrafficLight(true, 7.f, 2.f, 9.f, 0.50f);
    
    std::cout << "Smart traffic lights configured!\n";
}

int findClosestNode(sf::Vector2f worldMousePos, 
                    const std::map<int, node*>& nodeMap, 
                    const MapBounds& bounds) 
{
    float minDistance = std::numeric_limits<float>::infinity();
    int closestNodeId = -1;
    sf::Vector2f projectedPos;

    for (const auto& pair : nodeMap) {
        node* n = pair.second;
        projectedPos = project(n->get_coord(), bounds); 
        float dx = projectedPos.x - worldMousePos.x;
        float dy = projectedPos.y - worldMousePos.y;
        float distance = std::sqrt(dx*dx + dy*dy);
        if (distance < minDistance && distance < 30.0f) { 
            minDistance = distance;
            closestNodeId = n->get_id();
        }
    }
    return closestNodeId;
}

int main()
{
    // ⭐ MÀU NỀN XÁM (50, 50, 50) ⭐
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "Traffic Simulation - Optimized");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("assets/ARIAL.TTF")) {
        std::cerr << "Loi: Khong tai duoc font 'assets/ARIAL.TTF'\n";
        system("pause"); 
        return -1; 
    }
    
    sf::Text locationNameText(font, "", 12);
    locationNameText.setFillColor(sf::Color(220, 220, 220)); 

    // ⭐ KHÔNG HIỂN THỊ TÊN GIAO LỘ (JUNCTION) NHƯ YÊU CẦU ⭐
    // sf::Text junctionNameText(font, "", 9); 
    // junctionNameText.setFillColor(sf::Color(150, 150, 150)); 

    sf::Text roadNameText(font, "", 9);
    roadNameText.setFillColor(sf::Color(200, 200, 0, 150)); 

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

    std::vector<sf::Text> locationNameLabels;
    // std::vector<sf::Text> junctionNameLabels; // BỎ VECTOR NÀY

    for (node* n : q1.getNodes()) {
        sf::Vector2f nodePos = project(n->get_coord(), bounds);
        sf::FloatRect textBounds;
        float textWidth, textHeight;

        junction* junc = dynamic_cast<junction*>(n);
        if (junc) { 
            // ⭐ BỎ QUA VIỆC TẠO LABEL CHO JUNCTION
        } else { 
            // Sử dụng sf::String::fromUtf8 để hiển thị tên Tiếng Việt đúng
            std::string name = n->get_name();
            locationNameText.setString(sf::String::fromUtf8(name.begin(), name.end()));
            
            textBounds = locationNameText.getLocalBounds();
            textWidth = textBounds.position.x + textBounds.size.x;
            textHeight = textBounds.position.y + textBounds.size.y;
            locationNameText.setOrigin(sf::Vector2f(textWidth / 2.f, textHeight / 2.f));
            locationNameText.setPosition(sf::Vector2f(nodePos.x, nodePos.y + 15.f)); 
            locationNameLabels.push_back(locationNameText);
        }
    }

    // Biến cho chức năng Kéo/Thả
    bool isPanning = false;
    sf::Vector2i panStartPos;
    sf::Vector2i originalClickPos;

    while (window.isOpen())
    {
        while (std::optional<sf::Event> event = window.pollEvent())
        {
            if (event.has_value()) {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }
                else if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPress->code == sf::Keyboard::Key::F) {
                        handleConsoleInput(window, q1, nodeMap, bounds);
                    }
                    else if (keyPress->code == sf::Keyboard::Key::R) {
                         view.setCenter(sf::Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f));
                         view.setSize(sf::Vector2f(static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT)));
                    }
                }
                else if (const auto* mouseWheel = event->getIf<sf::Event::MouseWheelScrolled>()) {
                    if (mouseWheel->delta > 0) view.zoom(0.9f); 
                    else if (mouseWheel->delta < 0) view.zoom(1.1f); 
                }
                // MOUSE PRESSED
                else if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseButton->button == sf::Mouse::Button::Left) {
                        isPanning = true; 
                        panStartPos = mouseButton->position;
                        originalClickPos = mouseButton->position;
                    }
                    else if (mouseButton->button == sf::Mouse::Button::Right) {
                        startNodeId = -1; endNodeId = -1; currentPath.clear();
                    }
                }
                // MOUSE RELEASED (Check for Click vs Drag)
                else if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonReleased>()) {
                    if (mouseButton->button == sf::Mouse::Button::Left) {
                        isPanning = false;
                        int dx = mouseButton->position.x - originalClickPos.x;
                        int dy = mouseButton->position.y - originalClickPos.y;
                        
                        // If drag distance < 5px, consider it a CLICK
                        if (dx*dx + dy*dy < 25) {
                            sf::Vector2f worldMousePos = window.mapPixelToCoords(mouseButton->position, view);
                            int clickedNodeId = findClosestNode(worldMousePos, nodeMap, bounds);
                            if (clickedNodeId != -1) {
                                if (startNodeId == -1) {
                                    startNodeId = clickedNodeId; endNodeId = -1; currentPath.clear();
                                } else if (endNodeId == -1 && clickedNodeId != startNodeId) {
                                    endNodeId = clickedNodeId;
                                    currentPath = q1.dijkstra(startNodeId, endNodeId);
                                    if (!currentPath.empty()) {
                                        Vehicle::VehicleType type = Vehicle::VehicleType::Car;
                                        vehicleManager.emplace_back(type, currentPath, nodeMap, bounds, q1); 
                                        stats.vehicleSpawned();
                                    }
                                } else {
                                    startNodeId = clickedNodeId; endNodeId = -1; currentPath.clear();
                                }
                            }
                        }
                    }
                }
                // MOUSE MOVED (Pan map)
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

        sf::Time dt = deltaClock.restart(); 
        float fps = 1.f / dt.asSeconds();
        
        stats.update(fps, startNodeId, endNodeId, nodeMap);

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

        // ⭐ NỀN MÀU XÁM (50, 50, 50) ⭐
        window.clear(sf::Color(50, 50, 50)); 
        window.setView(view);

        sf::VertexArray road(sf::PrimitiveType::TriangleStrip, 4); 
        float roadThickness = 5.f; 
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
                    // ⭐ MÀU ĐƯỜNG SÁNG HƠN (130, 130, 130) ĐỂ DỄ NHÌN TRÊN NỀN XÁM ⭐
                    roadColor = sf::Color(130, 130, 130, 200); 
                }

                road[0].position = p1 - offset; 
                road[1].position = p1 + offset;
                road[2].position = p2 - offset; 
                road[3].position = p2 + offset;
                for (int i=0; i<4; ++i) road[i].color = roadColor;
                window.draw(road);
            }
        }

        float currentZoom = view.getSize().x;
        if (currentZoom < WINDOW_WIDTH * 0.5) 
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
                    
                    // Chuyển đổi tên đường sang UTF-8 để hiển thị
                    std::string name = edge->get_name();
                    roadNameText.setString(sf::String::fromUtf8(name.begin(), name.end()));

                    sf::FloatRect textBounds = roadNameText.getLocalBounds();
                    float textWidth = textBounds.position.x + textBounds.size.x;
                    float textHeight = textBounds.position.y + textBounds.size.y;
                    
                    roadNameText.setOrigin(sf::Vector2f(textWidth / 2.f, textHeight / 2.f + 5.f)); 
                    
                    roadNameText.setPosition(midPoint);

                    roadNameText.setRotation(sf::degrees(angle));
                    
                    window.draw(roadNameText);
                }
            }
        }

        for (const auto& text : locationNameLabels) {
            window.draw(text);
        }

        // ⭐ KHÔNG VẼ TÊN GIAO LỘ NỮA ⭐
        // if (currentZoom < WINDOW_WIDTH * 0.8) { for (const auto& text : junctionNameLabels) ... }

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

        for (auto& vehicle : vehicleManager) {
            vehicle.draw(window);
        }

        window.setView(window.getDefaultView());
        stats.draw(window);
        
        window.display();
    }

    return 0;
}