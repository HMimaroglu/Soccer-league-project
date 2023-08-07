//Interacts with the user
#ifndef _Viewer_h_
#define _Viewer_h_

#include "Buffer.h"

const char clear_command[] = "cls"; // for Windows
//const char clear_command[] = "clear"; // for Linux and possibly Mac's

class Viewer {
public:
	void run();

private:
	Buffer b;
	std::string error_message;
	PlayerMap search_map_print;
	int count = 0;


	void display_main();
	bool display_search();
	void execute_command(char command, bool& done);
	void execute_search_command(char command, bool& done);
	void display_statistics();
	void display_add();
	bool search();
	void display_edit();
	const std::string file_name = "player_data.txt";

};

#endif