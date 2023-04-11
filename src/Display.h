#ifndef DISPLAY_H_
#define DISPLAY_H_
#include <iostream>
#include <pthread.h>
#include <vector>
#include <stdlib.h>
#include "ComputerSystem.h"
#include "cTimer.h"
#include "Structures.h"



class Display {
	friend void * display_start_routine(void* arg);
	void *ptr;
	int rcvid;

public:
	pthread_t thread_id;
	std::vector<planes_information> planes;
	void * threadTask(void * );
	Display();
	int runDisplay();
	virtual ~Display();
};

#endif /* DISPLAY_H_ */
