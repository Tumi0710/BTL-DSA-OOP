#include <iostream>
#include "map.hpp"
//#include "edge.hpp"
using namespace std;
using namespace map_types;

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char** argv){
	map q1;
	
	//cac dia diem (10-99)
	q1.add_node(unique_ptr<node>(new location("Dinh doc lap", 10, 106.69655, 10.78251, 500, lo_type::TOURIST_ATTRACTION)));
	q1.add_node(unique_ptr<node>(new location("Thao cam vien", 11, 106.70779, 10.78841, 1000, lo_type::TOURIST_ATTRACTION)));
	q1.add_node(unique_ptr<node>(new location("Cong vien 23/9", 12, 106.69234, 10.76876, 1000, lo_type::TOURIST_ATTRACTION)));
	q1.add_node(unique_ptr<node>(new location("Toa Bitexco", 13, 106.70437, 10.77172, 1500, lo_type::LANDMARK)));
	q1.add_node(unique_ptr<node>(new location("Nha tho duc ba", 14, 106.69900, 10.77992, 300, lo_type::TOURIST_ATTRACTION)));
	q1.add_node(unique_ptr<node>(new location("Cho Ben Thanh", 15, 106.69805, 10.77265, 750, lo_type::TOURIST_ATTRACTION)));
	q1.add_node(unique_ptr<node>(new location("Pho Ong Cat Gia Truyen Since 1988", 16, 106.69309, 10.76851, 25, lo_type::RESTAURANT)));
	q1.add_node(unique_ptr<node>(new location("Nori - Modern Izakaya", 17, 106.69690, 10.77403, 50, lo_type::RESTAURANT)));
	q1.add_node(unique_ptr<node>(new location("Trung tam bao hanh Asus", 18, 106.68918, 10.77317, 75, lo_type::OTHERS)));
	
	//cac giao lo (100-999)
	q1.add_node(unique_ptr<node>(new junction("GL_CMT8_ND", 100, 106.70222, 10.78440, junction_type::UNDEFINED)));
	q1.add_node(unique_ptr<node>(new junction("GL_DDL1", 101, 106.69305, 10.77735, junction_type::UNDEFINED)));
	q1.add_node(unique_ptr<node>(new junction("GL_DDL2", 102, 106.69500, 10.77956, junction_type::UNDEFINED)));
	q1.add_node(unique_ptr<node>(new junction("GL_DDL3", 103, 106.69798, 10.77680, junction_type::UNDEFINED)));
	q1.add_node(unique_ptr<node>(new junction("GL_DDL4", 104, 106.69605, 10.77475, junction_type::UNDEFINED)));
	q1.add_node(unique_ptr<node>(new junction("GL_N6PD", 105, 106.69316, 10.77140, junction_type::ROUNDABOUT)));
	q1.add_node(unique_ptr<node>(new junction("GL_CNTD8", 106, 106.69990, 10.77896, junction_type::UNDEFINED)));
	q1.add_node(unique_ptr<node>(new junction("GL_NTDB1", 107, 106.69809, 10.78288, junction_type::UNDEFINED)));
	q1.add_node(unique_ptr<node>(new junction("GL_NTDB2", 108, 106.70102, 10.78019, junction_type::UNDEFINED)));
	q1.add_node(unique_ptr<node>(new junction("GL_CBT", 109, 106.69712, 10.77399, junction_type::UNDEFINED)));
	q1.add_node(unique_ptr<node>(new junction("GL_BFT1", 110, 106.70433, 10.77087, junction_type::UNDEFINED)));
	q1.add_node(unique_ptr<node>(new junction("GL_CV1", 111, 106.69536, 10.76807, junction_type::UNDEFINED)));
	q1.add_node(unique_ptr<node>(new junction("GL_CV2", 112, 106.69816, 10.77120, junction_type::UNDEFINED)));
	q1.add_node(unique_ptr<node>(new junction("GL_CBT1", 113, 106.69995, 10.77324, junction_type::UNDEFINED)));
	q1.add_node(unique_ptr<node>(new junction("GL_NTDB2", 114, 106.70208, 10.77555, junction_type::UNDEFINED)));
	q1.add_node(unique_ptr<node>(new junction("GL_TVC", 115, 106.70788, 10.78442, junction_type::UNDEFINED)));
	
	//cac duong (id dau + id cuoi)
	q1.add_edge_by_id("Duong Huyen Tran Cong Chua", 101104, 101, 104, 0);
	q1.add_edge_by_id("Duong A", 10111, 101, 11, 0);
	q1.add_edge_by_id("Duong B", 10115, 101, 14, 0);
	
	q1.show_all();
	
	//============== THU NGHIEM TIM KIEM =======================
    std::string search_term;
    std::cout << "\n----------------------------------------\n";
    std::cout << "Nhap ten/chuoi con cua Node can tim: ";
    
    // Ð?c toàn b? dòng nh?p vào (bao g?m c? kho?ng tr?ng)
    std::getline(std::cin, search_term); 
    
    if (search_term.empty()) {
        std::cout << "Chuoi tim kiem khong duoc de trong.\n";
        return 0;
    }

    // Goi ham tim kiem
    std::vector<node*> found_nodes = q1.search_node_by_name(search_term);

    // In ket qua
    std::cout << "\nKet qua tim kiem cho \"" << search_term << "\" (" 
              << found_nodes.size() << " ket qua):\n";
              
    if (found_nodes.empty()) {
        std::cout << "=> Khong tim thay Node nao.\n";
    } else {
        std::cout << "========================================\n";
        for (node* n_ptr : found_nodes) {
            n_ptr->display_name();
        }
        std::cout << "========================================\n";
    }
    
    // ... (Ph?n còn l?i c?a main.cpp)
    
    
	return 0;
}
