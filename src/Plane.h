#ifndef PLANE_H_
#define PLANE_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <sys/dispatch.h>
#include <sys/neutrino.h>
#include <vector>
#include <string>
#include <algorithm>
#include "cTimer.h"
#include "Structures.h"
#include <unistd.h>	// To be removed, for testing only
using namespace std;


class Plane {
	int rcvid;

public:
	pthread_t thread_id;	//creating a thread for each plane
	static vector<int> airspace;

	Plane(int ID, int time, int arrivalPosX, int arrivalPosY, int arrivalPosZ, int arrivalVelX, int arrivalVelY, int arrivalVelZ);
	int ID, time, arrivalPosX, arrivalPosY, arrivalPosZ, arrivalVelX, arrivalVelY, arrivalVelZ;
	void setPlane(int ID, int time, int arrivalPosX, int arrivalPosY, int arrivalPosZ, int arrivalVelX, int arrivalVelY, int arrivalVelZ);
	void setCoordinates(int arrivalPosX, int arrivalPosY, int arrivalPosZ);
	void setVelocity(int arrivalVelX, int arrivalVelY, int arrivalVelZ);
	int updateLocation(); // update location every second

	virtual ~Plane();
};

#endif /* PLANE_H_ */
