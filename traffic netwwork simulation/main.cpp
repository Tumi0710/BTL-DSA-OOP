#include <iostream>
#include "map.hpp"
//#include "edge.hpp"
using namespace std;
using namespace map_types;

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char** argv){
	map q1;
	
	q1.add_node(unique_ptr<node>(new location("Dinh doc lap", 11, 106.69655, 10.78251, 500, lo_type::TOURIST_ATTRACTION)));
	q1.add_node(unique_ptr<node>(new location("Thao cam vien", 12, 106.70779, 10.78841, 1000, lo_type::TOURIST_ATTRACTION)));
	q1.add_node(unique_ptr<node>(new location("Cong vien 23/9", 13, 106.69234, 10.76876, 1000, lo_type::TOURIST_ATTRACTION)));
	q1.add_node(unique_ptr<node>(new location("Toa Bitexco", 14, 106.70437, 10.77172, 1500, lo_type::LANDMARK)));
	q1.add_node(unique_ptr<node>(new junction("Giao Lo A (Auto)", 15, 106.70237, 10.76173, junction_type::UNDEFINED)));
//	q1.add_node(unique_ptr<node>(new location("Nha tho duc ba", 5, )));
	
	q1.add_edge_by_id("Duong Le Duan", 1112, 11, 12, 0);
	q1.add_edge_by_id("Duong Nguyen Hue", 1512, 15, 12, 0);
	q1.add_edge_by_id("Duong ABC", 1513, 15, 13, 0);
	q1.add_edge_by_id("Duong ABCD", 1514, 15, 14, 1);
	
//    m.add_node(unique_ptr<node>(new node("A", 1, 2, 3)));
//	m.add_node(unique_ptr<node>(new location("B", 2, 4, 5, 20, lo_type::RESTAURANT)));
//	m.add_edge(unique_ptr<edge>(new edge("AB", 11, 1, 2, 22, 1)));
//	m.add_edge_by_id("Duong AB", 707, 7, 3, 0);
//	m.add_edge_by_id("Duong AB", 707, 1, 2, 0);

    q1.show_all();
    
	return 0;
}
