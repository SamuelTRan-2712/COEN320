#include "cTimer.h"


cTimer::cTimer(uint32_t period_sec, uint32_t period_msec, uint32_t offset_sec,uint32_t offset_msec ) {

	const int signal = SIGALRM;
	sigemptyset(&sig_set); // initialize a signal set
	sigaddset(&sig_set, signal); // add SIGALRM to the signal set
	sigprocmask(SIG_BLOCK, &sig_set, NULL); //block the signal

	/* set the signal event a timer expiration */
	memset(&sig_event, 0, sizeof(struct sigevent));
	sig_event.sigev_notify = SIGEV_SIGNAL;
	sig_event.sigev_signo = signal;

	chid = ChannelCreate(0);
	coid = ConnectAttach(0,0,chid,0,0);
	if(coid == -1){
		std::cerr << "connection could not be initialized" << errno << "\n";
	}

	SIGEV_PULSE_INIT(&sig_event, coid, SIGEV_PULSE_PRIO_INHERIT, 1, 0);

	if (timer_create(CLOCK_REALTIME, &sig_event, &timer_id) == -1){
			std::cerr << "Timer, Init error : " << errno << "\n";
		}

	set_timer(period_sec,1000000* period_msec,offset_sec,1000000* offset_msec);

}

cTimer::~cTimer() {

}

void cTimer::set_timer(uint32_t p_sec, uint32_t p_nsec, uint32_t o_sec, uint32_t o_nsec){
	int res;
	/* set timer parameters */
	timer_spec.it_value.tv_sec = o_sec; //I believe we only need offset and period here, the rest are specified in nano seconds
	timer_spec.it_value.tv_nsec = o_nsec; //
	timer_spec.it_interval.tv_sec = p_sec;
	timer_spec.it_interval.tv_nsec = p_nsec;

	/* create timer */
	res = timer_create(CLOCK_REALTIME, &sig_event, &timer_id); //creating a real time timer with its own ID,

	if (res < 0) {
		perror("creation of timer was unsuccesful, please try again");
		exit(-1);
	}

	/* activate the timer */
	timer_settime(timer_id, 0, &timer_spec, NULL);
}


void cTimer::wait_next_activation(){ //need to change arguments, see if we can change message buffer and size of
	 int rcvid = MsgReceive(chid, &msg_buffer, sizeof(msg_buffer), NULL); //once message has been received, the clock will wake up again

} //receives a message from the client










