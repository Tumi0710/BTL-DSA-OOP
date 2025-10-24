#include <iostream>
#include "map.hpp"
//#include "edge.hpp"
using namespace std; 

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char** argv){
	map m;

    m.add_node(unique_ptr<node>(new node("A", 1, 2, 3)));
	m.add_node(unique_ptr<node>(new location("B", 2, 4, 5, 20)));
	m.add_edge(unique_ptr<edge>(new edge("AB", 11, 1, 2, 22)));

    m.show_all();
	
//	cout << "hello world" << endl;
//	edge a("AAA", 1, 2, 3, 4.4);
//	cout << a.get_name() << endl;
//	cout << a.get_id() << endl;
	return 0;
}
