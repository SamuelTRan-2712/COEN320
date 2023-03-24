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
using namespace std;

class Plane {
	pthread_mutex_t mutex;
	friend void * start_routine(void* arg); // thread start routine
	int ID, time, posX, posY, posZ, velX, velY, velZ;
	int rcvid;
	void *ptr;
public:
	pthread_t thread_id;
	static vector<int> airspace;

	void * threadTask(void * );
	Plane(int time, int ID, int posX, int posY, int posZ, int velX, int velY, int velZ);

	void setPlane(int time, int ID, int posX, int posY, int posZ, int velX, int velY, int velZ);
	void setCoordinates(int posX, int posY, int posZ);
	void setVelocity(int velX, int velY, int velZ);
	int updateLocation(); // update location every second
	void sendInfo(); // sends extra data when requested, server?

	virtual ~Plane();
};

#endif /* PLANE_H_ */
