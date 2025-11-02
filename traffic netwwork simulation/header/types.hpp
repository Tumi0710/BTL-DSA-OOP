#pragma once
#include <string>
#include <stdexcept> 
 
 namespace map_types{
	enum class direction{
		east, //dong
		west, //tay
		south, //nam
		north //bac
	};
	
	// ====================== LOAI DIA DIEM ===================== 
	enum class lo_type{
 		TOURIST_ATTRACTION,
 		LANDMARK,
 		RESTAURANT,
 		OTHERS,
 		UNDEFINED //gia tri mac dinh xu ly loi 
	};
	
	//lo_type enum -> string 
	inline std::string to_string (lo_type t){
		switch (t){
			case lo_type::TOURIST_ATTRACTION:
				return "Diem tham quan";
			case lo_type::LANDMARK:
				return "Landmark";
			case lo_type::RESTAURANT:
				return "Quan an";
			case lo_type::OTHERS:
				return "Khac";
			case lo_type::UNDEFINED:
				return "Khong xac dinh";
		}
		return "UNKNOWN_TYPE"; // idk chac loi gi do
	}
	
	//string -> enum
	inline lo_type to_enum_lo_type(const std::string &str){
		if(str == "Diem tham quan" || str == "diem tham quan")
			return lo_type::TOURIST_ATTRACTION;
			
		else if(str == "Landmark" || str == "landmark")
			return lo_type::LANDMARK;
		
		else if(str == "Quan an" || str == "quan an")
			return lo_type::RESTAURANT;
		
		else if(str == "Khac" || str == "khac")
			return lo_type::OTHERS;
		
		else
			throw std::invalid_argument ("Loai dia diem k hop le: " + str);
	}
	
	//======================= LOAI GIAO LO ==========================
	enum class junction_type{
			ROUNDABOUT,		//vong xuyen (bung binh)
			T_JUNCTION, 	//giao lo 3 nahnh
			CROSS_JUNCTION, 	//Giao lo 4 nhanh
			COMPLEX_JUNCTION, 	//giao lo nhieu nhanh
			UNDEFINED		//loi
	};
	
	//enum -> string
	inline std::string to_string(junction_type t){
		switch (t){
			case junction_type::ROUNDABOUT:
            return "Vong Xuyen (Bung Binh)";
			case junction_type::T_JUNCTION:
				return "Giao lo chu T (Nga 3)";
			case junction_type::CROSS_JUNCTION:
				return "Giao lo chu thap (Nga 4)";
			case junction_type::COMPLEX_JUNCTION:
				return "Giao lo phuc hop";
			case junction_type::UNDEFINED:
				return "Khong xac dinh";
		}
		return "Unknow type";
	}
	
	inline junction_type to_enum_junction_type(const std::string &str){
		if(str == "Giao lo chu T" || str == "giao lo chu T"
			|| str == "Nga ba" || str == "nga ba")
			return junction_type::T_JUNCTION;
			
		else if(str == "Giao lo chu thap" || str == "giao lo chu thap"
			|| str == "Nga tu" || str == "nga tu")
			return junction_type::CROSS_JUNCTION;
			
		else if(str == "Giao lo phuc hop" || str == "giao lo phuc hop")
			return junction_type::COMPLEX_JUNCTION;
			
		else
			throw std::invalid_argument ("Loai giao lo k hop le: " + str);
	}
	
 }
