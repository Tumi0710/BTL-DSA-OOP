#include "node.hpp"
using namespace std;

//------------------- node -------------------
node::node() : name(""), id(0), x(0), y(0){}
node::node(string n, int i, double x_pos, double y_pos)
    : name(n), id(i), x(x_pos), y(y_pos){}

string node::get_name() const{ return name; }
int node::get_id() const{ return id; }
double node::get_x() const{ return x; }
double node::get_y() const{ return y; }

void node::display() const{
    cout << "Node [" << id << "] " << name
         << " (" << x << ", " << y << ")\n";
}

// =================== LOP KE THUA ================== 

//---------------- location ----------------
location::location(string n, int i, double x_pos, double y_pos, int pop)
    : node(n, i, x_pos, y_pos), population(pop){}

void location::display() const{
    cout << "City Node [" << id << "] " << name
         << " (" << x << ", " << y << ") - Population: " << population << endl;
}

