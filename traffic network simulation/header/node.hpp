#pragma once
#include "coord.hpp"
#include "types.hpp" 
#include <string>

class node{
	private:
	    std::string name;
	    int id;
	    coord pos;
	
	public:
	    node();
	    node(std::string n, int i, double x_pos, double y_pos);
	
	    std::string get_name() const;
	    int get_id() const;
	    coord get_coord() const;
	    double get_x() const;
	    double get_y() const;
	
		void display_name() const;
	    virtual void display() const;
	    virtual ~node() = default;
};

// ================= LOP KE THUA ==================
//dia diem ----------------------
class location : public node{
	private:
	    int population;
	    map_types::lo_type type;
	
	public:
	    location(std::string n, int i, double x_pos, double y_pos, int pop, map_types::lo_type t);
	    void display() const override;
};

//giao lo --------------------
class junction : public node{
	private:
		 map_types::junction_type type;
	public:
//		junction();
		junction(std::string n, int i, double x_pos, double y_pos, map_types::junction_type t);
		void display() const override;
		
		//auto xac dinh loai giao lo 
		void determine_type(int branch_cnt);
		int get_max_branches() const; //gioi han de sau nay ng dung nhap
		
};

