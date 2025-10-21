#pragma once
#include "base_obj.hpp"
#include <string>

class node : public base_obj{
    public:
        node();
        node(std::string n, int i);
};
