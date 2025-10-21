#pragma once
#include <string>

class base_obj{
	private:
	    std::string name;
	    int id;
	public:
	    base_obj();
	    base_obj(std::string n, int i);
	    
	    virtual int get_id() const;
	    virtual std::string get_name() const;
};

