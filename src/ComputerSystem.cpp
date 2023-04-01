#include "ComputerSystem.h"

// ----------------------------------- Constants -----------------------------------
#define COMPUTER_ATTACH_POINT "ComputerSystem"
#define DISPLAY_ATTACH_POINT "Display"

const uint64_t timeout = 5000000;


// ----------------------------------- Class Methods -----------------------------------
std::vector<violating_pair_ids> ComputerSystem::getCollision() {
	std::vector<violating_pair_ids> violating_planes;

	for (unsigned i = 0; i < planes.size(); i ++) {
		for (unsigned j = i + 1; j < planes.size(); j++) {
			if (abs(planes.at(i).arrivalPosX - planes.at(j).arrivalPosX) < 3000 ||
				abs(planes.at(i).arrivalPosY - planes.at(j).arrivalPosY) < 3000 ||
				abs(planes.at(i).arrivalPosZ - planes.at(j).arrivalPosZ) < 1000)
			{
					violating_pair_ids violating_pair_ids;
					violating_pair_ids.plane_ID_1 = planes.at(i).ID;
					violating_pair_ids.plane_ID_2 = planes.at(j).ID;
					violating_planes.push_back(violating_pair_ids);
			}
		}
	}
	return violating_planes;
}


int ComputerSystem::fromRadar() {
	name_attach_t *attach;
	all_planes data;
	compsys_display_msg msg;


	if((attach = name_attach(NULL, COMPUTER_ATTACH_POINT, 0)) == NULL) {
		printf("ComputerSys failed to create channel\n\n");
		return EXIT_FAILURE;
	}

	while (1) {
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
			MsgError( rcvid, ENOSYS ); // error can be -1, ENOSYS, ERESTART, EOK, or the error code that you want to set for the client.

		    continue;
		}

		// check for appropriate header and copy the data to planes
		if (data.hdr.type == 0x01) {
			planes = data.allPlanes;
			// for(plane_info i: planes){
			// 	printf("plane id#%d; coords(%d,%d,%d)\n\n", i.ID, i.posX, i.posY, i.posZ);
			// }

			// Construct data to send to display
			msg.planes = data;
			msg.colliding_planes = getCollision();
//			printf("Hdrrrrrr %d\n\n", msg.planes.hdr.type);

			toDisplay(msg);

		}
		MsgReply(rcvid, EOK, 0, 0);
	}
	name_detach(attach, 0); //destroying channel with server
	return EXIT_SUCCESS;
}


int ComputerSystem::toDisplay(compsys_display_msg msg){
	if ((server_coid = name_open(DISPLAY_ATTACH_POINT, 0)) == -1) {
		printf("CompSys failed connection to server %d\n\n");
		return EXIT_FAILURE;
	}

	if (MsgSend(server_coid, &msg, sizeof(msg), 0, 0) == -1) {
		printf("Failed to send message %d\n\n");
		return EXIT_FAILURE;
	}

	name_close(server_coid);
	return EXIT_SUCCESS;
}


void* compsys_start_routine(void *arg) {
	ComputerSystem& compsys = *(ComputerSystem*)arg;
	compsys.fromRadar();
	return NULL;
}


ComputerSystem::ComputerSystem() {
	this->server_coid = -1;
	this->rcvid = -1;
	if (pthread_create(&thread_id,NULL,compsys_start_routine,(void *) this) != EOK) {
		printf("ComputerSystem: Failed to start.\n\n");;
	}
}


ComputerSystem::~ComputerSystem() {

}
