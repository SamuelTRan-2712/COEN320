#ifndef RADAR_H_
#define RADAR_H_

#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dispatch.h>
#include <vector>
#include "cTimer.h"
#include "Structures.h"
#include "Plane.h"
#include <unistd.h>	// To be removed, for testing only

class Radar {
	int server_coid;
	std::vector<int> airspace;
	std::vector<planes_information>allPlaneData;
public:
	pthread_t thread_id;
	Radar();
	void getAirspace();
	int toComputerSys(compsys_msg);
	virtual ~Radar();
//	void removePlaneFromAirspace(int planeID);
//	void addPlaneToAirspace(int planeID);
};

#endif /* RADAR_H_ */
