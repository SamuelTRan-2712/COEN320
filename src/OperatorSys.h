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
#include "Plane.h"
#include <map>
#include "ATC.h"



class OperatorSys {
	int server_coid;
	std::vector<int> airspace;
	std::vector<plane_info>allPlaneData;


public:
	ATC atc;
	pthread_t thread_id;
	OperatorSys(ATC atc);
	int toComputerSys(all_planes);
	virtual ~OperatorSys();
	void getCommands();
	void Initialize();

};

