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


	ATC atc;
	atc.readInput();

	Display display;
	CommunicationSystem commsys;
	ComputerSystem compsys;
	Radar radar;

	OperatorSys operatorsys(atc);


	for(int i=0; i < atc.planes.size(); i++){
		pthread_join(atc.planes.at(i)->thread_id,NULL);
	}



	return EXIT_SUCCESS;
}
