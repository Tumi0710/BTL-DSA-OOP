#pragma once
#include <vector>
#include <memory>
#include "node.hpp"
#include "edge.hpp" 
#include <map> 
#include <queue> 
#include <limits> 
#include <utility> 
#include <unordered_map>

struct Neighbor {
    int id;
    double weight;
};

class map{
	private:
	    std::vector<node*> nodes; 
	    std::vector<edge*> edges; 
	    
	    node* get_node(int id) const;		
	    int cnt_branches(int id) const;		
	    
	    std::map<int, std::vector<Neighbor>> adjList;

        using EdgePair = std::pair<int, int>;
        std::map<EdgePair, edge*> m_edgeMap;
        
        // Tối ưu hóa truy cập
        std::unordered_map<int, node*> m_nodeMap;
        std::unordered_map<int, std::vector<edge*>> m_edgesByNode;
        
	public:
	    map();
	    ~map(); 

	    void add_node(node* n); 
	    void add_edge(edge* e); 
	    
        // HÀM CŨ (5 tham số - Tự động tính khoảng cách "chim bay")
	    void add_edge_by_id(std::string n, int i, int id_src, int id_dest, bool dir); 
	
        // ⭐ HÀM MỚI (6 tham số - Nhập khoảng cách thủ công) ⭐
        void add_edge_by_id(std::string n, int i, int id_src, int id_dest, double manual_weight_km, bool dir);

	    void show_all() const;
	    
		node* find_node_by_id(int id) const; 
		std::vector<node*> search_node_by_name(const std::string& n);

        void build_adjList();
		std::vector<int> dijkstra(int startId, int endId); // GIỮ NGUYÊN DIJKSTRA

        const std::vector<node*>& getNodes() const;
	    const std::vector<edge*>& getEdges() const;
        
        edge* getEdge(int srcId, int destId);
        
        // Tối ưu hóa truy cập edges
        const std::vector<edge*>& get_edges_from_node(int nodeId) const;
};