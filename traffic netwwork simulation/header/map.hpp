#pragma once
#include <vector>
#include <memory>
#include "node.hpp"
#include "edge.hpp" 

class map{
	private:
	    std::vector<std::unique_ptr<node>> nodes;
	    std::vector<std::unique_ptr<edge>> edges;
	    
	    node* get_node(int id) const;		//de dung trong add edge by id 
	    int cnt_branches(int id) const;		//de dung trong auto tinh loai giao lo (add edge by id)
	
	public:
	    map();
	
	    void add_node(std::unique_ptr<node> n);
	    void add_edge(std::unique_ptr<edge> e);
	    
	    //auto tinh w
	    void add_edge_by_id(std::string n, int i, int id_src, int id_dest, bool dir); 
	
	    void show_all() const;
};

