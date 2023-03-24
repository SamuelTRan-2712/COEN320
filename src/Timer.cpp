#include "Timer.h"

void Timer::wait_next_execution(void) {
	int dummy;
	/* suspend calling process until a signal is pending */
	sigwait(&sigst, &dummy);
}

int Timer::start_periodic_timer(uint64_t offset, int period) {
	timer_t timer;
	const int signal = SIGALRM;
	int res;
	channel_id = ChannelCreate(0);
	connection_id = ConnectAttach(0,0,channel_id,0,0);
	if(connection_id == -1){
		std::cerr << "Timer, Connect Attach error : " << errno << "\n";
	}

	SIGEV_PULSE_INIT(&sigev, connection_id, SIGEV_PULSE_PRIO_INHERIT, 1, 0);

	/* set timer parameters */
	timer_spec.it_value.tv_sec = offset;
	timer_spec.it_value.tv_nsec = 0;
	timer_spec.it_interval.tv_sec = period;
	timer_spec.it_interval.tv_nsec = 0;

	sigemptyset(&sigst); // initialize a signal set
	sigaddset(&sigst, signal); // add SIGALRM to the signal set
	sigprocmask(SIG_BLOCK, &sigst, NULL); //block the signal

	/* set the signal event a timer expiration */
	memset(&sigev, 0, sizeof(struct sigevent));
	sigev.sigev_notify = SIGEV_SIGNAL;
	sigev.sigev_signo = signal;

	/* create timer */
	res = timer_create(CLOCK_MONOTONIC, &sigev, &timer);

	if (res < 0) {
		perror("Timer Create");
		exit(-1);
	}

	/* activate the timer */
	return timer_settime(timer, 0, &timer_spec, NULL);
}

Timer::Timer(uint64_t offset, int period) {
	if (start_periodic_timer(offset, period) < 0)
	{
		perror("Start periodic timer");
	}
}

Timer::~Timer() {
	// TODO Auto-generated destructor stub
}

