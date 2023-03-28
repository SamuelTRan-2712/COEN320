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

class Radar {
	int server_coid;
	std::vector<int> airspace;
	std::vector<plane_info>allPlaneData;
public:
	pthread_t thread_id;
	Radar();
	void pingAirspace();
	int toComputerSys(all_planes);
	virtual ~Radar();
};

#endif /* RADAR_H_ */
