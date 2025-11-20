#include "map.hpp"
#include "coord.hpp"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <queue>

map::map(){}

map::~map() {
    for (node* n : nodes) {
        delete n;
    }
    for (edge* e : edges) {
        delete e;
    }
}

void map::add_node(node* n){ 
    nodes.push_back(n); 
    m_nodeMap[n->get_id()] = n;
}

void map::add_edge(edge* e){ 
	edges.push_back(e); 
	
	int src = e->get_src();
    int dest = e->get_dest();
    
    m_edgesByNode[src].push_back(e);
    if (!e->get_direction()) {
        m_edgesByNode[dest].push_back(e);
    }
    
    m_edgeMap[{src, dest}] = e;
    if (!e->get_direction()) {
        m_edgeMap[{dest, src}] = e;
    }
}

node* map::get_node(int id) const {
    auto it = m_nodeMap.find(id);
    return it != m_nodeMap.end() ? it->second : nullptr;
}

int map::cnt_branches(int id) const {
    auto it = m_edgesByNode.find(id);
    if (it != m_edgesByNode.end()) {
        return it->second.size();
    }
    return 0;
}

void map::show_all() const{
    std::cout << "\n Danh sach cac node:\n";
    for (node* n : nodes){ 
        n->display(); 
    }
    
    std::cout <<"\n-----------------------------------------------------------\n";
    
    std::cout << "\n Danh sach cac edge:\n";
    for (edge* e : edges){ 
        e->display(); 
    }
}

// Ham them canh va tu dong tinh khoang cach dua tren toa do
void map::add_edge_by_id(std::string n, int i, int id_src, int id_dest, bool dir){
	node* node_src = get_node(id_src);
	node* node_dest = get_node(id_dest);
	
	if( node_src == NULL || node_dest == NULL){
        std::cout << "Loi: Khong the them Edge " << n << " vi thieu Node sau: ";
        if (node_src == NULL){
            std::cout << "Node nguon ID " << id_src;
            if (node_dest == NULL) std::cout << " va ";
        }
        if (node_dest == NULL){
            std::cout << "Node dich ID " << id_dest;
        }
        std::cout << ".\n";
		return;
	}
	
	double distance = node_src->get_coord().distance(node_dest->get_coord());
	
    edge* e = new edge(n, i, id_src, id_dest, distance, dir); 
	add_edge(e);

	junction* junction_src = dynamic_cast<junction*>(node_src);		
    if (junction_src != NULL){
        int branches = cnt_branches(id_src);		
        junction_src->determine_type(branches);		
    }
    junction* junction_dest = dynamic_cast<junction*>(node_dest);
    if (junction_dest != NULL){
        int branches = cnt_branches(id_dest);
        junction_dest->determine_type(branches);
    }
}

// Ham them canh nhung cho phep nhap tay khoang cach (de chinh xac hon)
void map::add_edge_by_id(std::string n, int i, int id_src, int id_dest, double manual_weight_km, bool dir){
	node* node_src = get_node(id_src);
	node* node_dest = get_node(id_dest);
	
	if( node_src == NULL || node_dest == NULL){
        std::cout << "Loi: Khong the them Edge " << n << " vi thieu Node sau: ";
        if (node_src == NULL){
            std::cout << "Node nguon ID " << id_src;
            if (node_dest == NULL) std::cout << " va ";
        }
        if (node_dest == NULL){
            std::cout << "Node dich ID " << id_dest;
        }
        std::cout << ".\n";
		return;
	}
	
    edge* e = new edge(n, i, id_src, id_dest, manual_weight_km, dir); 
	add_edge(e);

	junction* junction_src = dynamic_cast<junction*>(node_src);		
    if (junction_src != NULL){
        int branches = cnt_branches(id_src);		
        junction_src->determine_type(branches);		
    }
    junction* junction_dest = dynamic_cast<junction*>(node_dest);
    if (junction_dest != NULL){
        int branches = cnt_branches(id_dest);
        junction_dest->determine_type(branches);
    }
}


std::string to_lower(const std::string& str){ 
	std::string lower_str = str; 
	for (int i = 0; i < str.length(); i++){
		lower_str[i] = tolower(str[i]);		
	}
	return lower_str;
}

std::vector<node*> map::search_node_by_name(const std::string& partial_n){ 
	std::vector<node*> rslt; 
	std::string lower_partial = to_lower(partial_n); 
	
    if (lower_partial.empty()) return rslt;

	for (node* n : nodes) { 
		std::string lower_full_n = to_lower(n->get_name()); 
		
        if (lower_full_n.find(lower_partial) != std::string::npos) {
            junction* junc_ptr = dynamic_cast<junction*>(n); 
            if (junc_ptr == NULL) { 
                rslt.push_back(n); 
            }
        }
	}
	return rslt;
}

node* map::find_node_by_id(int id) const {
    auto it = m_nodeMap.find(id);
    return it != m_nodeMap.end() ? it->second : nullptr;
}

const std::vector<node*>& map::getNodes() const {
    return nodes;
}

const std::vector<edge*>& map::getEdges() const {
    return edges;
}

using P = std::pair<double, int>; 
struct ComparePair {
    bool operator()(const P& a, const P& b) {
        return a.first > b.first;
    }
};

void map::build_adjList(){
	std::cout << "Building Adjacency List...\n"; 
    for (node* n : nodes) {
        adjList[n->get_id()] = std::vector<Neighbor>(); 
    }
    for (edge* e : edges) {
        adjList[e->get_src()].push_back({e->get_dest(), e->get_weight()});
        if (!e->get_direction()) {
            adjList[e->get_dest()].push_back({e->get_src(), e->get_weight()});
        }
    }
    std::cout << "Adjacency List built!\n"; 
}

std::vector<int> map::dijkstra(int startId, int endId) { 
    std::map<int, double> dist; 
    std::map<int, int> prev;   
    std::priority_queue<P, std::vector<P>, ComparePair> pq; 
    for (const auto& pair : adjList) {
        dist[pair.first] = std::numeric_limits<double>::infinity(); 
        prev[pair.first] = -1; 
    }
    dist[startId] = 0;
    pq.push({0.0, startId}); 
    
    while (!pq.empty()) {
        double d = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        
        if (d > dist[u]) { continue; }
        if (u == endId) { break; }
        
        // Duyet qua cac canh ke de tinh toan duong di
        for (const auto& neighbor : adjList.at(u)) {
            int v = neighbor.id;
            double staticWeight = neighbor.weight; // Khoảng cách thuc te

            // Tinh toan he so tac duong dua tren so luong xe dang di
            double trafficPenalty = 1.0; 
            edge* currentEdge = getEdge(u, v);
            
            if (currentEdge != nullptr) {
                int carCount = currentEdge->getVehicleCount();
                // Cang nhieu xe, trong so cang cao (kho di hon)
                trafficPenalty = 1.0 + (carCount * 0.5); 
            }
            
            double dynamicWeight = staticWeight * trafficPenalty;

            double newDist = dist[u] + dynamicWeight; 
            
            if (newDist < dist[v]) {
                dist[v] = newDist;
                prev[v] = u; 
                pq.push({newDist, v}); 
            }
        }
    }
    
    std::vector<int> path; 
    int curr = endId;
    if (prev[curr] == -1) { return path; }
    while (curr != -1) {
        path.push_back(curr);
        curr = prev[curr];
    }
    std::reverse(path.begin(), path.end()); 
    return path;
}

edge* map::getEdge(int srcId, int destId) {
    auto it = m_edgeMap.find({srcId, destId});
    if (it != m_edgeMap.end()) {
        return it->second;
    }
    return nullptr;
}

const std::vector<edge*>& map::get_edges_from_node(int nodeId) const {
    static std::vector<edge*> empty;
    auto it = m_edgesByNode.find(nodeId);
    return it != m_edgesByNode.end() ? it->second : empty;
}