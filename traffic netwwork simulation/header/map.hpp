#pragma once
#include <vector>
#include <memory>
#include "node.hpp"
#include "edge.hpp" 

class map{
	private:
	    std::vector<std::unique_ptr<node>> nodes;
	    std::vector<std::unique_ptr<edge>> edges;
	
	public:
	    map();
	
	    void add_node(std::unique_ptr<node> n);
	    void add_edge(std::unique_ptr<edge> e);
	
	    void show_all() const;
};

