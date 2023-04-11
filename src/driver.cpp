#include <pthread.h>
#include <stdio.h>
#include "Plane.h"
#include "Radar.h"
#include "ComputerSystem.h"
#include "Display.h"
#include "ATC.h"
#include "OperatorSys.h"
#include "CommunicationSystem.h"
#include <unistd.h>
#include <fstream>
#include <string>

using namespace std;


int main (int argc, char* argv[]) {

	// Initialize planes from input file
	ATC atc;
	atc.readInput();

	// Initialize all components
	Display display;
	CommunicationSystem commsys;
	ComputerSystem compsys;
	Radar radar;
	OperatorSys operatorsys(atc);

	// Close threads
	for(int i=0; i < atc.planes.size(); i++){
		pthread_join(atc.planes.at(i)->thread_id,NULL);
	}



	return EXIT_SUCCESS;
}
