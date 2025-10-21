#include <iostream>
#include "node.hpp"

using namespace std;

int main(int argc, char** argv) {
	node a("AAA", 69);
	cout << a.get_name() << endl;
	cout << a.get_id() << endl;
}
