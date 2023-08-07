//Buffer that stores all data and has the commands that the user can access
#ifndef _Buffer_h_
#define _Buffer_h_

#include <iostream>
#include <fstream>
#include <istream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>

#include "Player.h"

using PlayerMap = std::map<std::string, Player>;

class Buffer {
public:
	friend std::istream& operator>>(std::istream& in, Buffer& b);
	friend std::ostream& operator<<(std::ostream& out, const Player& pl);
	bool open();
	bool search(const std::string& first_name, const std::string& last_name, int year_of_birth, const char registration_status, const std::string& category, const std::string keyword);
	void add(const std::string& first_name, const std::string& last_name, int year_of_birth, const char& registration_status);
	bool edit(const std::string& last_name, const std::string& first_name, int year_of_birth, const char& registration_status, int count);
	bool write(std::string file, PlayerMap pm);
	bool print_main(std::string file_name);
	PlayerMap search_map() { return search_map_; }
	PlayerMap player_map() { return player_map_; }
	std::string season_year() { return season_year_; }
	void set_season_year(const std::string& season_year) { season_year_ = season_year; }
	void clear_maps();


private:
	Player p;
	std::string season_year_;
	PlayerMap player_map_;
	PlayerMap search_map_;
	const std::string file_name = "player_data.txt";
	std::string new_file;
};

inline void Buffer::clear_maps() {
	player_map_.clear();
	search_map_.clear();
}

#endif