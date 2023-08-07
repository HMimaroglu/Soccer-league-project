#include "Buffer.h"
using namespace std;


istream& operator>>(istream& in, Buffer& b) {
    getline(in, b.p.last_name);
    getline(in, b.p.first_name);
    in >> b.p.year_of_birth >> b.p.registration_status >> b.p.category;
    return in;
}

ostream& operator<<(ostream& out, const Player& pl) {
    out << pl.last_name << endl;
    out << pl.first_name << endl;
    out << pl.year_of_birth << " " << pl.registration_status << " " << pl.category << endl;
    return out;
}

// Open Function
bool Buffer::open() {
    player_map_.clear();
    std::ifstream in(file_name);
    if (!in) {
        return false;
    }

    getline(in, season_year_);
    while (in >> *this) {
        in.ignore();
        string full_name = p.last_name + " " + p.first_name;
        player_map_.insert(std::pair<std::string, Player>(full_name, p));
    }
    in.close();

    return true;
}//End of Open

bool Buffer::search(const std::string& first_name, const std::string& last_name, int year_of_birth, const char registration_status, const std::string& category, const std::string keyword) {
    // 1. last name (string)
    // 2. keyword (string)
    // 3. first name (string)
    // 4. year of birth (int)
    // 5. registration status (int)
    // 6. category (string)

    if (last_name != "") {
        for (auto itr = player_map_.lower_bound(last_name); itr != player_map_.end(); ++itr) {
            search_map_.insert({ itr->first, itr->second });
        }
    }
    else { search_map_ = player_map_; }//End of Last Name Search

    /*if (last_name != "") {
        for (auto itr = search_map_.begin(); itr != search_map_.end();) {
            if (itr->second.last_name != last_name) {
                itr = search_map_.erase(itr);
            }
            else ++itr;
        }
    }*/ //This part runs in linear but can be added if you want it to only print out that last name and not every other

    // Searching 

    if (keyword != "") {
        for (auto itr = search_map_.begin(); itr != search_map_.end();) {
            if (itr->first.find(keyword) == string::npos) {
                itr = search_map_.erase(itr);
            }
            else ++itr;
        }
    }

    //Search for first name
    if (first_name != "") {
        for (auto itr = search_map_.begin(); itr != search_map_.end();) {
            if (itr->second.first_name != first_name) {
                itr = search_map_.erase(itr);
            }
            else ++itr;
        }
    }

    //Search for year of birth
    if (year_of_birth != -1) {
        for (auto itr = search_map_.begin(); itr != search_map_.end();) {
            if (itr->second.year_of_birth != year_of_birth) {
                itr = search_map_.erase(itr);
            }
            else ++itr;
        }
    }

    //Search for registration status
    if ((tolower(registration_status) == 'p') || (tolower(registration_status) == 'n')) {
        for (auto itr = search_map_.begin(); itr != search_map_.end();) {
            if (itr->second.registration_status != registration_status) {
                itr = search_map_.erase(itr);
            }
            else ++itr;
        }
    }

    //Search for category
    if (category != "") {
        for (auto itr = search_map_.begin(); itr != search_map_.end();) {
            if (itr->second.category != category) {
                itr = search_map_.erase(itr);
            }
            else ++itr;
        }
    }

    int size = search_map_.size();
    if (search_map_.size() > 0) { return true; }
    return false;
}//End of Search

// Add Player Function
void Buffer::add(const string& first_name, const string& last_name, int year_of_birth, const char& registration_status) {
    //Player p;
    p.first_name = first_name;
    p.last_name = last_name;
    p.year_of_birth = year_of_birth;
    p.registration_status = registration_status;

    int current_year;
    stringstream ss(season_year_);
    ss >> current_year;

    int age = current_year - year_of_birth;

    if ((age > 14) && (age < 17)) {
        p.category = "U17";
    }
    else if (age > 12) {
        p.category = "U14";
    }
    else if (age > 10) {
        p.category = "U12";
    }
    else if (age > 8) {
        p.category = "U10";
    }
    else if (age > 6) {
        p.category = "U8";
    }
    else {
        p.category = "U6";
    }

    player_map_.insert({ last_name + " " + first_name, p });

}//End of Add


//Edit Function
bool Buffer::edit(const string& last_name, const string& first_name, int year_of_birth, const char& registration_status, int count) {
    auto itr = search_map_.begin();

    advance(itr, count);
    Player p = itr->second;

    string temp_ln = p.last_name;
    string temp_fn = p.first_name;
    int temp_yob = p.year_of_birth;
    char temp_rs = p.registration_status;

    string key = temp_ln + " " + temp_fn;
    auto it = player_map_.find(key);


    if (last_name != "") {
        temp_ln = last_name;
    }
    if (first_name != "") {
        temp_fn = first_name;
    }
    if (year_of_birth != -1) {
        temp_yob = year_of_birth;
    }
    if ((registration_status == 'p') || (registration_status) == 'n') {
        temp_rs = registration_status;
    }

    player_map_.erase(it);
    search_map_.erase(itr);
    add(temp_fn, temp_ln, temp_yob, temp_rs);

    return true;
}//End of Edit

//Print Main Function
bool Buffer::print_main(string file) {
    ofstream out(file);
    if (!out) return false;

    string cat;
    out << season_year_ << endl;
    for (int i = 0; i <= 5; i++) {
        if (i == 0) cat = "U6";
        if (i == 1) cat = "U8";
        if (i == 2) cat = "U10";
        if (i == 3) cat = "U12";
        if (i == 4) cat = "U14";
        if (i == 5) cat = "U17";

        for (const auto& p : player_map_) {
            if (p.second.category == cat) {
                out << p.second;
            }
        }
    }
    out.close();
    return true;
}//End of print_main

//Write Function
bool Buffer::write(string file, PlayerMap pm) {
    ofstream out(file);
    if (!out) return false;

    out << season_year_ << endl;
    for (const auto& p : pm) {
        out << p.second;
    }

    out.close();
    return true;
}//End of Write