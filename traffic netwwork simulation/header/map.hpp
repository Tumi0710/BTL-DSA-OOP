#pragma once
#include <vector>
#include <memory>
#include "node.hpp"

class map{
	private:
	    std::vector<std::unique_ptr<node>> nodes;
	
	public:
	    map();
	
	    void add_node(std::unique_ptr<node> n);
	
	    void show_all() const;
};

