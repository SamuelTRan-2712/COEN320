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
	if(pthread_create(&thread_id,NULL,display_start_routine,(void *) this)!=EOK) //error can be -1, ENOSYS, ERESTART, EOK, or the error code that you want to set for the client.

	{
		thread_id=NULL;
	}
}

int Display::runDisplay() {
	name_attach_t *attach;
	all_planes data;
	compsys_display_msg msg;

	if((attach = name_attach(NULL, DISPLAY_ATTACH_POINT, 0)) == NULL){
		printf("Display failed to create channel\n\n");
		return EXIT_FAILURE;
	}

	while(1){
		rcvid = MsgReceive(attach->chid, &msg, sizeof(msg), NULL); // receive messages from radar
		if (rcvid == -1) {/* Error condition, exit */
			break;
		}

		data = msg.planes;
//		printf("Hdrr %d\n\n", msg.planes.hdr.type);

		if (rcvid == 0) {/* Pulse received */
			switch (data.hdr.code) {
				case _PULSE_CODE_DISCONNECT:

					ConnectDetach(data.hdr.scoid);
				    break;
				case _PULSE_CODE_UNBLOCK:

					break;
				default:

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

		}
		MsgReply(rcvid, EOK, 0, 0);
		cTimer timer(5,0,5,0);
		timer.wait_next_activation();

		std::cout << "X-Y view" << endl;

		for(int i = 0; i < 20; i++){
			for (int j = 0; j < 50; j++){
				bool isprinted = false;
					if (i == 0 || i == 19) {
						std::cout << "-";
					}
					else if (j == 0 || j == 49) {
						std::cout << "|";
					}

					if (planes.size() != 0){
						for(plane_info x: planes){
							if((19-i) == ((x.arrivalPosX/20000)+1) && ((j) == ((x.arrivalPosY/10000)+1))) {
								std::cout << x.ID;
								isprinted = true;
							}
						}
					}
					if (!isprinted && i != 0 && j != 0 && i != 19 && j != 49) {
						std::cout << " ";
					}
				}
				std::cout << "\n";
		}

		std::cout << "X-Z view" << endl;
				for(int i = 0; i < 20; i++){
					for (int j = 0; j < 50; j++){
						bool isprinted = false;
						if (i == 0 || i == 19) {
							std::cout << "-";
						}
						else if (j == 0 || j == 49) {
							std::cout << "|";
						}

						if (planes.size() != 0){
							for(plane_info x: planes){
								if((19 - i) == ((x.arrivalPosX/2000)+1) && ((j+10) == ((x.arrivalPosZ/1000)+1))) {
									std::cout << x.ID;
									isprinted = true;
								}
							}
						}
						if (!isprinted && i != 0 && j != 0 && i != 19 && j != 49) {
							std::cout << " ";
						}
					}
					std::cout << "\n";
				}
		for(plane_info y: planes) {
			std::cout << "P" << y.ID << ": " << "\n";
			//std::cout << "ID: " << ID.at(s) << "\n";
			std::cout << "Position X: " << y.arrivalPosX << "\n";
			std::cout << "Position Y: " << y.arrivalPosY << "\n";
			std::cout << "Position Z: " << y.arrivalPosZ << "\n" << "\n";
			if (y.arrivalPosY >= 100000 || y.arrivalPosX >= 100000 || y.arrivalPosZ > 25000){
				std::cout << y.ID << " arrivalPosZ "<< y.arrivalPosZ << " arrivalPosX "<< y.arrivalPosX << " arrivalPosY "<< y.arrivalPosY << endl;
				std::cout << "Plane "<< y.ID << " has exited the monitor" << endl << endl;
			}
		}
	}
	name_detach(attach, 0);
	return EXIT_SUCCESS;
}

Display::~Display() {

}
