#include "Viewer.h"
using namespace std;

const string seperator(50, '-');

void Viewer::display_main() {
    cout << "[N]ew Season";
    cout << setw(19) << "[A]dd";
    cout << setw(17) << "[S]earch\n";
    cout << "[D]isplay Statistics";
    cout << setw(13) << "[P]rint";
    cout << setw(12) << "[Q]uit";
    cout << "\nCommand: ";
}//End of display_main

//execute command
void Viewer::execute_command(char command, bool& done) {
    char user_choice;
    bool search_done = true;
    string season_year;
    string file;
    switch (command) {
    case 'N':
    case 'n':
        char confirm;
        cout << "Confirm (y)es or (n)o: ";
        cin >> confirm;
        while ((tolower(confirm) != 'y') && (tolower(confirm) != 'n')) {
            cout << confirm;
            cout << "Confirm (y)es or (n)o: ";
        }

        if (tolower(confirm) == 'y') {
            b.clear_maps();
            search_map_print.clear();
            cout << "New season year: ";
            cin >> season_year;
            b.set_season_year(season_year);
            b.write(file_name, b.player_map());
        }
        break;
    case 'A':
    case 'a':
        display_add();
        b.write(file_name, b.player_map());
        break;
    case 'S':
    case 's':
        count = 0;
        cout << "What would you like to search for(press enter to leave that category blank)\n";
        if (!search()) {
            system(clear_command);
            cout << error_message << endl;
            search_done = false;
        }
        while (search_done) {
            display_search();
            cin >> user_choice;
            if (tolower(user_choice) == 's') {
                done = false;
                break;
            }
            execute_search_command(user_choice, search_done);
        }
        break;
    case 'D':
    case 'd':
        display_statistics();
        break;
    case 'P':
    case 'p':
        cout << "Print to file: ";
        cin >> file;
        if (!b.print_main(file)) error_message = "Could not open file";
        cout << error_message;
        error_message = "";
        break;
    case 'Q':
    case 'q':
        done = false;
        break;
    default:
        error_message = "Invalid Command";
        cout << error_message << endl;
        error_message = "";
        break;
    }//End of switch
}//End of execute_command

//Display Add
void Viewer::display_add() {
    string last, first;
    int year;
    char registration;
    cout << "Last Name: ";
    cin.ignore();
    getline(cin, last);
    cout << "First Name: ";
    getline(cin, first);
    int current_year;
    stringstream ss(b.season_year());
    ss >> current_year;
    cout << "Year of Birth: ";
    cin >> year;
    while ((current_year - year) >= 17) {
        cout << "Invalid entry.\nYear of Birth: ";
        cin >> year;
    }
    do {
        cout << "Registration Status[(p)aid or (n)ot paid]: ";
        cin.ignore();
        cin.get(registration);
    } while (registration != 'n' && registration != 'p');

    b.add(first, last, year, registration);
}//End of Display Add


//Display Stats
void Viewer::display_statistics() {
    // Define a list of age groups
    vector<string> age_groups = { "U17", "U14", "U12", "U10", "U8", "U6" };

    // Calculate total stats
    PlayerMap pm = b.player_map();
    int players = pm.size();
    int paid_players = count_if(pm.begin(), pm.end(), [](const auto& p) { return p.second.registration_status == 'p'; });
    int unpaid_players = players - paid_players;

    // Display total stats
    cout << "Categoty | Player count | Paid (Y/N)" << endl;
    cout << " Total" << setw(5) << " | " << players << setw(15 - to_string(players).length()) << " | " << paid_players << "/" << unpaid_players << endl;

    // Calculate stats for each age group
    for (const auto& age_group : age_groups) {
        b.search("", "", -1, 'p', age_group, "");
        PlayerMap paid_pm = b.search_map();
        int paid_players_age = paid_pm.size();
        b.search("", "", -1, 'n', age_group, "");
        PlayerMap unpaid_pm = b.search_map();
        int unpaid_players_age = unpaid_pm.size();
        int players_age = paid_players_age + unpaid_players_age;

        int adjust = 0;
        if (age_group == "U6" || age_group == "U8") {
            adjust = 1;
        }

        // Display stats for the age group
        cout << " " << age_group << setw(7 + adjust) << " | " << players_age << setw(15 - to_string(players_age).length()) << " | " << paid_players_age << "/" << unpaid_players_age << endl;
    }
}//End of Display Stats

//Display Search
bool Viewer::display_search() {

    system(clear_command);
    if (b.search_map().size() == 0) return false;
    search_map_print = b.search_map();


    cout << "Player: " << count + 1 << "/" << search_map_print.size() << endl;
    cout << seperator << endl;
    auto itr = search_map_print.begin();
    if (count != 0) advance(itr, count);


    if (search_map_print.size() > 0) {
        cout << itr->first << " || " << itr->second.year_of_birth << " || " << itr->second.category << " || " << itr->second.registration_status << endl;
    }
    cout << seperator << endl;

    cout << "\t Next\t Back\t Print\t Edit\t Stop\t Quit" <<
        "\nCommand: ";

    return true;

}//End of display_search

//Search Function
bool Viewer::search() {
    string last, first, category, keyword;
    int year;
    char registration;
    cout << "Last Name: ";
    cin.ignore();
    getline(cin, last);
    cout << "First Name: ";
    getline(cin, first);
    bool valid = false;
    do {
        string getyear;

        cout << "Year of Birth: ";
        getline(cin, getyear);
        if (getyear == "") {
            year = -1;
            valid = true;
        }
        else {
            stringstream ss(getyear);
            if (ss >> year) {
                valid = true;
            }
        }
    } while (!valid);
    cout << "Registration Status[(p)aid or (n)ot paid]: ";
    cin.get(registration);
    cout << "Category: ";
    if (registration == 'p' || registration == 'n') cin.ignore();
    getline(cin, category);
    cout << "Keyword: ";
    getline(cin, keyword);
    if (!b.search(first, last, year, registration, category, keyword)) {
        error_message = "Search Failed";
        return false;
    }
    return true;
}//End of Search

//Execute Search Command
void Viewer::execute_search_command(char command, bool& done) {

    string file;
    auto itr = search_map_print.begin();
    switch (command) {
    case 'N':
    case 'n':
        if (count < search_map_print.size() - 1) {
            ++count;
        }
        break;
    case 'B':
    case 'b':
        if (count > 0)
            --count;
        break;
    case 'P':
    case 'p':
        cout << "Print to file: ";
        cin >> file;
        if (!b.write(file, search_map_print)) {
            error_message = "File did not open!";
            done = false;
        }
        break;
    case 'E':
    case 'e':
        if (count != 0) advance(itr, count);
        search_map_print.clear();
        display_edit();
        break;
    case 'Q':
    case 'q':
        done = false;
        system(clear_command);
        break;
    default:
        error_message = "Invalid Command";
        cout << error_message << endl;
        error_message = "";
        break;
    }//End of Switch
}//End of execute_search_command

//Display Edit
void Viewer::display_edit() {
    system(clear_command);
    string lastname;
    string firstname;
    int year;
    char registration = '\0';

    cout << "Edit(Leave blank if you don't want to change)";
    cout << "\nLast Name: ";
    cin.ignore();
    getline(cin, lastname);
    cout << "First Name: ";
    getline(cin, firstname);
    bool valid = false;
    do {
        string getyear;

        cout << "Year of Birth: ";
        getline(cin, getyear);
        if (getyear == "") {
            year = -1;
            valid = true;
        }
        else {
            int current_year;
            stringstream ss(getyear);
            stringstream ss2(b.season_year());
            ss2 >> current_year;
            if ((ss >> year) && (year < current_year) && (year > current_year - 17)) {
                valid = true;
            }
        }
    } while (!valid);

    cout << "Registration Status[(p)aid or (n)ot paid]: ";
    cin.get(registration);

    b.edit(lastname, firstname, year, registration, count);
}//End of display edit

void Viewer::run() {
    bool done = true;
    char user_choice;
    if (!b.open()) {
        cout << file_name + " file not found" << endl;
        done = false;
    }
    while (done) {
        if (error_message != "") {
            display_main();
            error_message = "";
        }
        else display_main();
        cin >> user_choice;
        system(clear_command);
        execute_command(user_choice, done);
    }
    b.write(file_name, b.player_map());
}