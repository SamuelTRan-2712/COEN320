#include <pthread.h>
#include <stdio.h>
#include "Plane.h"
#include "Radar.h"
#include "ComputerSystem.h"
#include "Display.h"
#include "ATC.h"
#include <unistd.h>

int main (int argc, char* argv[]) {

	if (chdir("/COEN320Project/src/input.txt") != 0) {
		std::cerr << "Error: Failed to change directory" << std::endl;
		return 1;
	}

	ATC atc;
	atc.readInput();

	Display display;
	ComputerSystem compsys;
	Radar radar;


	for(int i=0; i < atc.planes.size(); i++){
		pthread_join(atc.planes.at(i)->thread_id,NULL);
	}


	return EXIT_SUCCESS;
}
