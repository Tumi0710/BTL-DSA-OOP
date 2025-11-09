#include "map.hpp"
#include "coord.hpp"
#include <iostream>
#include <algorithm>
#include <cctype>
using namespace std;


//ham dung -------------------
map::map(){}

void map::add_node(unique_ptr<node> n){
    nodes.push_back(move(n));
}

void map::add_edge(unique_ptr<edge> e){
	edges.push_back(move(e));
}


//ham phuong thuc -----------------------
//----- private ----
node* map::get_node(int id) const {
	for (const auto& n : nodes){
		if(n->get_id() == id)	return n.get();
	}
	return NULL;
}

int map::cnt_branches(int id) const {
    int cnt = 0;
    for (const auto& e : edges){
    	//dem neu node do la nguon hoac dich cua edge
        if (e->get_src() == id || e->get_dest() == id){
            cnt++;
        }
    }
    return cnt;
}


// ----- public -------
void map::show_all() const{
    cout << "\n Danh sach cac node:\n";
    for (const auto& n : nodes){
        n->display();
    }
    
    cout <<"\n-----------------------------------------------------------\n";
    
    cout << "\n Danh sach cac edge:\n";
    for (const auto& e : edges){
        e->display();
    }
}

//them edge theo id
void map::add_edge_by_id(string n, int i, int id_src, int id_dest, bool dir){
	node* node_src = get_node(id_src);
	node* node_dest = get_node(id_dest);
	
	if( node_src == NULL || node_dest == NULL){
        cout << "Loi: Khong the them Edge " << n << " vi thieu Node sau: ";
        if (node_src == NULL){
            cout << "Node nguon ID " << id_src;
            if (node_dest == NULL) cout << " va ";
        }
        if (node_dest == NULL){
            cout << "Node dich ID " << id_dest;
        }
        cout << ".\n";
		return;
	}
	
	double distance = node_src->get_coord().distance(node_dest->get_coord());
	//dung unique ptr nen new edge auto huy
	unique_ptr<edge> e(new edge(n, i, id_src, id_dest, distance, dir));
	
	edges.push_back(move(e));
	
	
	//check source node (node nguon) 
	junction* junction_src = dynamic_cast<junction*>(node_src);		//check xem co phai node junc k 
    if (junction_src != NULL){
        int branches = cnt_branches(id_src);		//dem so branch 
        junction_src->determine_type(branches);		//auto update junc type 
    }

    // check dest node (node dich) 
    junction* junction_dest = dynamic_cast<junction*>(node_dest);
    if (junction_dest != NULL){
        int branches = cnt_branches(id_dest);
        junction_dest->determine_type(branches);
    }
}

// ====================== TIM KIEM ===================
// chuyen doi chuoi sang k viet hoa (ho tro trong tim kiem)
string to_lower(const string& str){
	string lower_str = str;
	for (int i = 0; i < str.length(); i++){
		lower_str[i] = tolower(str[i]);		//tolower co dau vao la int (ma ascii cua ky tu)
	}
	return lower_str;
}

//tim kiem node theo ten (partial_n = ten nguoi dung dang nhap)
vector<node*> map::search_node_by_name(const std::string& partial_n){
	//partial_n = 
	vector<node*> rslt; //result;
	
	string lower_partial = to_lower(partial_n);
	
	//tim alll node (tim kiem tuyen tinh) 
	for (const auto& n : nodes) {
		//lay ten day du cua node roi chuyen ve viet thuong
		string lower_full_n = to_lower(n->get_name());
		
		//tim kiem chuoi con
		bool found = false;
		for (size_t i = 0; i <= lower_full_n.length() - lower_partial.length(); i++){
			bool mismatch = false;
			//check node co khop ki tu voi chuoi ng dung dang nhap k
			for(size_t j = 0; j < lower_partial.length(); j++){
				if(lower_full_n[i+j] != lower_partial[j]){
					mismatch = true;
					break;
				}
			}
			if(!mismatch){
				found = true;
				break;
			}
		}
		if(found){
            
            junction* junc_ptr = dynamic_cast<junction*>(n.get());
            if (junc_ptr == NULL) { 
                rslt.push_back(n.get()); 
            }
        }
	}
	return rslt;
}


