#ifndef TIMER_H_
#define TIMER_H_

#include <sys/time.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sync.h>
#include <sys/siginfo.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <sys/syspage.h>
#include <inttypes.h>

#define ONE_THOUSAND	1000
#define ONE_MILLION		1000000

class Timer {
	int channel_id;
	int connection_id;

	sigset_t sigst;
	struct itimerspec timer_spec;
	struct sigevent sigev;
	timer_t timer_id;
	uint64_t offset;
	int period;
public:
	void wait_next_execution(void);
	int start_periodic_timer(uint64_t offset, int period);
	Timer(uint64_t offset, int period);
	virtual ~Timer();
};

#endif /* TIMER_H_ */
