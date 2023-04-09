#ifndef SRC_COMPUTERSYSTEM_H_
#define SRC_COMPUTERSYSTEM_H_

#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dispatch.h>
#include <vector>
#include <math.h>
#include "Plane.h"
#include "Structures.h"

class ComputerSystem {
	int rcvid;
	int server_coid;

public:
	pthread_t thread_id;
	std::vector<plane_info> planes;

	ComputerSystem();
	int listen();
	std::vector<violating_pair_ids> getCollision();
	int toDisplay(compsys_display_msg);
	int toCommunicationSystem(plane_msg);
	virtual ~ComputerSystem();
};

#endif /* SRC_COMPUTERSYSTEM_H_ */
