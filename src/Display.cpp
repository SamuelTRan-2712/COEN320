#include "Display.h"

#define DISPLAY_ATTACH_POINT "Display"
const uint64_t timeout = 5000000;

using namespace std;

void* display_start_routine(void* arg) {
	Display& display = *(Display*) arg;
	display.runDisplay();
	return NULL;
}

Display::Display(){
	if(pthread_create(&thread_id,NULL,display_start_routine,(void *) this)!=EOK)
	{
		thread_id=NULL;
	}
}

int Display::runDisplay() {
	name_attach_t *attach;
	all_planes data;
	if((attach = name_attach(NULL, DISPLAY_ATTACH_POINT, 0)) == NULL){
		printf("Display failed to create channel\n\n");
		return EXIT_FAILURE;
	}

	while(1){
		rcvid = MsgReceive(attach->chid, &data, sizeof(data), NULL); // receive messages from radar
		if (rcvid == -1) {/* Error condition, exit */
			break;
		}

		if (rcvid == 0) {/* Pulse received */
			switch (data.hdr.code) {
				case _PULSE_CODE_DISCONNECT:
				/*
				 * A client disconnected all its connections (called
				 * name_close() for each name_open() of our name) or
				 * terminated
				 */
					ConnectDetach(data.hdr.scoid);
				    break;
				case _PULSE_CODE_UNBLOCK:
				/*
				 * REPLY blocked client wants to unblock (was hit by
				 * a signal or timed out).  It's up to you if you
				 * reply now or later.
				 */
					break;
				default:
				/*
				 * A pulse sent by one of your processes or a
				 * _PULSE_CODE_COIDDEATH or _PULSE_CODE_THREADDEATH
				 * from the kernel?
				 */
					break;
			}
		    continue;
		}

		/* name_open() sends a connect message, must EOK this */
		if (data.hdr.type == _IO_CONNECT ) {
			MsgReply( rcvid, EOK, NULL, 0 );
			continue;
		}

		/* Some other QNX IO message was received; reject it */
		if (data.hdr.type > _IO_BASE && data.hdr.type <= _IO_MAX ) {
			MsgError( rcvid, ENOSYS );
			continue;
		}
		// check for appropriate header and copy the data to planes
		if (data.hdr.type == 0x01){
			planes = data.allPlanes;
//			for(plane_info i: planes){
//				printf("plane id#%d; coords(%d,%d,%d)\n\n", i.ID, i.posX, i.posY, i.posZ);
//			}
		}
		MsgReply(rcvid, EOK, 0, 0);
		cTimer timer(5,5);
		timer.waitTimer();
		for(int i = 0; i <51; i++){
			for (int j = 0; j < 102; j++){
				bool isprinted = false;
				if (i == 0) {
					std::cout << "-";
				}
				else if (j == 0) {
					std::cout << "|";
				}
				else if (i == 50) {
					std::cout << "-";
				}
				else if (j == 101) {
					std::cout << "|";
				}

				else if (planes.size() != 0){
					for(plane_info x: planes){
						if(i == ((x.posX/2000)+1) && (j == ((x.posY/1000)+1))) {
							std::cout << x.ID;
							isprinted = true;
						}
					}
				}
				if (!isprinted && i != 0 && j != 0 && i != 50 && j != 101) {
					std::cout << " ";
				}
			}
			std::cout << "\n";
		}

		for(plane_info y: planes) {
			std::cout << "P" << y.ID << ": " << "\n";
			//std::cout << "ID: " << ID.at(s) << "\n";
			std::cout << "Position X: " << y.posX << "\n";
			std::cout << "Position Y: " << y.posY << "\n" << "\n";
		}
	}
	name_detach(attach, 0);
	return EXIT_SUCCESS;
}

Display::~Display() {

}
