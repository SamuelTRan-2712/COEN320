#ifndef COMMUNICATION_SYSTEM_H_
#define COMMUNICATION_SYSTEM_H_

#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dispatch.h>
#include <vector>
#include <math.h>
#include "cTimer.h"
#include "Structures.h"
#include "Plane.h"
#include <unistd.h>	// To be removed, for testing only

class CommunicationSystem {
	int server_coid;
	int rcvid;

public:
	pthread_t thread_id;
	CommunicationSystem();
	int toPlane(plane_msg&);
	int fromCompSys();
	virtual ~CommunicationSystem();
};

#endif /* COMMUNICATION_SYSTEM_H_ */
