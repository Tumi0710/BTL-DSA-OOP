#include "map.hpp"
#include <iostream>
using namespace std;

map::map(){}

void map::add_node(std::unique_ptr<node> n){
    nodes.push_back(std::move(n));
}

void map::show_all() const{
    cout << "Danh sach cac node:\n";
    for (const auto& n : nodes){
        n->display();
    }
}

