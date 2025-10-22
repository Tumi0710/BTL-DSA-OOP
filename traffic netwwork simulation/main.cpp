#include <iostream>
#include "map.hpp"
using namespace std; 

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char** argv){
	map m;

    m.add_node(unique_ptr<node>(new node("A", 1, 2, 3)));
	m.add_node(unique_ptr<node>(new location("B", 2, 4, 5, 20)));

    m.show_all();
    return 0;
	
//	cout << "hello world" << endl;
//	node a("AAA", 1, 2, 3);
//	cout << a.get_id() << endl;
	return 0;
}
