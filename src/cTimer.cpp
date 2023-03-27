#include "cTimer.h"



cTimer::cTimer(int offset, int period) {
	chid = ChannelCreate(0);
	coid = ConnectAttach(0,0,chid,0,0);
	if(coid == -1){
		std::cerr << "connection could not be initialized" << errno << "\n";
	}

	SIGEV_PULSE_INIT(&sig_event, coid, SIGEV_PULSE_PRIO_INHERIT, 1, 0);

	set_timer(5, 0);

//	if (set_timer(offset, period) < 0)
//	{
//		perror("Start periodic timer");
//	}

	// cycles_per_sec = SYSPAGE_ENTRY(qtime)->cycles_per_sec; //dont think we need this either
}

cTimer::~cTimer() {

}


//void cTimer::set_timer(int offset, int period){ believe we can delete this!!
//	timer_spec.it_value.tv_sec = offset;
//	timer_spec.it_value.tv_nsec = 0;
//	timer_spec.it_interval.tv_sec = period;
//	timer_spec.it_interval.tv_nsec = 0;
	//timer_settime(timer_id, 0, &timer_spec, NULL);
//}

void cTimer::set_timer(int offset, int period){
	int res;
	/* set timer parameters */
	timer_spec.it_value.tv_sec = offset; //I believe we only need offset and period here, the rest are specified in nano seconds
	//timer_spec.it_value.tv_nsec = 0; //
	timer_spec.it_interval.tv_sec = period;
	//timer_spec.it_interval.tv_nsec = 0;

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
	 //int MsgReceive( int chid, void * msg,    size_t size,  struct _msg_info * info );
} //receives a message from the client


//	int MsgReply( int rcvid,  long status,  const void* msg,   size_t size ); replies to the client








