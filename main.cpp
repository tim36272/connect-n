#include <iostream>
#include <fstream>
#include "types.h"
#include "utility.h"
#include "Connect.h"

int main(int argc, char* argv[]) {
	//setup default environment variables
	helper settings;
	settings.N = 4;
	settings.cols = 4;
	settings.rows = 4;
	settings.depth = 4;
	settings.use_ab = false;
	settings.ai_first = false;

	if(!utility::GetFlags(argc,argv,&settings)) {
		return 0;
	}

	//make a board
	Connect board(settings);

	//main program loop
	while(board.run()) {
		//show the board
		board.Display();
	}
	return 0;
}
