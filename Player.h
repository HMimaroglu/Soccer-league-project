//Struct of all player data that is needed
#ifndef _Player_h_
#define _Player_h_

#include <string>

struct Player {

	std::string first_name;
	std::string last_name;
	int year_of_birth;
	char registration_status;
	std::string category;
};


#endif