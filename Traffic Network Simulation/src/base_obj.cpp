#include "base_obj.hpp"

using namespace std;

base_obj::base_obj() : name(""), id(0) {}

base_obj::base_obj(string n, int i){
	name = n;
	id = i;
}

int base_obj::get_id() const {
    return id;
}

std::string base_obj::get_name() const{
    return name;
}

