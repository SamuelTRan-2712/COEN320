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

	struct sigevent sig_event;
	struct itimerspec timer_spec;
	timer_t timer_id;

	char msg_buffer[100];

	uint64_t cycles_per_sec;
	uint64_t tick_cycles, tock_cycles;

	int channel_id;
	int connection_id;

public:
	cTimer(int offset, int period);

	void setTimerSpec(int, int); //adding timer functions from tutorial code
	void wait_next_activation();
	int startTimer(int offset, int period);
	virtual ~cTimer();
};

#endif /* CTIMER_H_ */
