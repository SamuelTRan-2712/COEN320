#include "cTimer.h"

int cTimer::startTimer(int offset, int period){
	int res;
	/* set timer parameters */
	timer_spec.it_value.tv_sec = offset;
	timer_spec.it_value.tv_nsec = 0;
	timer_spec.it_interval.tv_sec = period;
	timer_spec.it_interval.tv_nsec = 0;

	/* create timer */
	res = timer_create(CLOCK_REALTIME, &sig_event, &timer_id);

	if (res < 0) {
		perror("Timer Create");
		exit(-1);
	}

	/* activate the timer */
	return timer_settime(timer_id, 0, &timer_spec, NULL);
}
void cTimer::setTimerSpec(int offset, int period){
	timer_spec.it_value.tv_sec = offset;
	timer_spec.it_value.tv_nsec = 0;
	timer_spec.it_interval.tv_sec = period;
	timer_spec.it_interval.tv_nsec = 0;
	timer_settime(timer_id, 0, &timer_spec, NULL);
}
void cTimer::wait_next_activation(){
	int rcvid; //waits for the next message to be received before clock interrupt?
	rcvid = MsgReceive(channel_id, &msg_buffer, sizeof(msg_buffer), NULL);
}


cTimer::cTimer(int offset, int period) {
	channel_id = ChannelCreate(0);
	connection_id = ConnectAttach(0,0,channel_id,0,0);
	if(connection_id == -1){
		std::cerr << "Timer, Connect Attach error : " << errno << "\n";
	}

	SIGEV_PULSE_INIT(&sig_event, connection_id, SIGEV_PULSE_PRIO_INHERIT, 1, 0);
	//debugcout << "TIMER pulse initiated" << endl;
	//

	if (startTimer(offset, period) < 0)
	{
		perror("Start periodic timer");
	}

	cycles_per_sec = SYSPAGE_ENTRY(qtime)->cycles_per_sec;
}


cTimer::~cTimer() {
	// TODO Auto-generated destructor stub
}
