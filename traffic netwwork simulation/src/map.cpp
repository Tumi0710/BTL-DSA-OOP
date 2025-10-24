#include "map.hpp"
#include <iostream>
using namespace std;

map::map(){}

void map::add_node(unique_ptr<node> n){
    nodes.push_back(move(n));
}

void map::add_edge(unique_ptr<edge> e){
	edges.push_back(move(e));
}

void map::show_all() const{
    cout << "Danh sach cac node:\n";
    for (const auto& n : nodes){
        n->display();
    }
    
    cout << "Danh sach cac edge:\n";
    for (const auto& e : edges){
        e->display();
    }
}

