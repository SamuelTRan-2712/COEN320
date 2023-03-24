#include "ComputerSystem.h"

#define COMPUTER_ATTACH_POINT "ComputerSystem"
#define DISPLAY_ATTACH_POINT "Display"

const uint64_t timeout = 5000000;

void* compsys_start_routine(void *arg){
	ComputerSystem& compsys = *(ComputerSystem*)arg;
	compsys.fromRadar();
	return NULL;
}

int ComputerSystem::toDisplay(all_planes planes){
	if ((server_coid = name_open(DISPLAY_ATTACH_POINT, 0)) == -1){
		//printf("CompSys failed connection to server %d\n\n");
		return EXIT_FAILURE;
	}
	if (MsgSend(server_coid, &planes, sizeof(planes), 0, 0) == -1){
		//printf("Failed to send message %d\n\n");
		return EXIT_FAILURE;
	}
	name_close(server_coid);
	return EXIT_SUCCESS;
}

int ComputerSystem::fromRadar(){
	name_attach_t *attach;
	all_planes data;

	if((attach = name_attach(NULL, COMPUTER_ATTACH_POINT, 0)) == NULL){
		printf("ComputerSys failed to create channel\n\n");
		return EXIT_FAILURE;
	}
	while (1){
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
		toDisplay(data);

	}
	name_detach(attach, 0);
	return EXIT_SUCCESS;
}

int point_2_point_dist(int x1, int y1, int x2, int y2) {
	int d;
	d = sqrt( ((x2 - x1)^2) + (y1 - y2)^2 );
	return d;
}

void ComputerSystem::calcCollision() {
	int xyResult = 0;
	bool xyCollision = false;
	bool zCollision = false;

	for (int i = 0; i < 180; i ++) {
		for(int j = 0; j < planes.size(); j++){
			for(int s = 0; s < planes.size(); s++){
				xyResult = point_2_point_dist(planes.at(j).posX, planes.at(j).posY,
								   planes.at(s).posX, planes.at(s).posY);
				if (xyResult <= 3000) {
					xyCollision = true;
				}
				if (abs(planes.at(j).posZ - planes.at(s).posZ)) {
					 zCollision = true;
				}
			}
		}
		for(int h = 0; h < planes.size(); h++){
			planes.at(h).posX += planes.at(h).velX;
			planes.at(h).posY += planes.at(h).velY;
			planes.at(h).posZ += planes.at(h).velZ;
		}
	}
}
ComputerSystem::ComputerSystem() {
	if(pthread_create(&thread_id,NULL,compsys_start_routine,(void *) this)!=EOK){
		thread_id=NULL;
	}
}

ComputerSystem::~ComputerSystem() {

}
