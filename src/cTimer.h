#ifndef CTIMER_H_
#define CTIMER_H_

#include <stdio.h>
#include <iostream>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sync.h>
#include <sys/siginfo.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <sys/syspage.h>
#include <inttypes.h>
#include <stdint.h>

class cTimer {

	timer_t timer_id;
	struct sigevent sig_event; //creating a struct called sigevent which will be used as a notification system
	struct itimerspec timer_spec;

	char msg_buffer[100];

	uint64_t cycles_per_sec;
	sigset_t sig_set;
	struct timespec tv;

	int chid;
	int coid;
	int p_nsec = 0;
	int o_nsec = 0;


public:
	cTimer(uint32_t,uint32_t,uint32_t,uint32_t);
	virtual ~cTimer();
	void set_timer(uint32_t,uint32_t,uint32_t,uint32_t); //adding timer functions from tutorial code
	void wait_next_activation();
	//int startTimer(int offset, int period); believe we can delete this as well
};

#endif /* CTIMER_H_ */
