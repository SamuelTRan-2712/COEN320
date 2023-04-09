#include "ComputerSystem.h"

// ----------------------------------- Constants -----------------------------------
#define COMPUTER_ATTACH_POINT "ComputerSystem"
#define DISPLAY_ATTACH_POINT "Display"
#define COMMUNICATION_ATTACH_POINT "CommunicationSystem"

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


int ComputerSystem::listen() { //computer system is the server. needs to create a channel, receive a message, and reply to the client
	name_attach_t *attach;
	compsys_msg data;
	compsys_display_msg msg;
	plane_msg plane_msg;
	plane_msg.hdr.type = 0x01;

	if((attach = name_attach(NULL, COMPUTER_ATTACH_POINT, 0)) == NULL) { //create a channel
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
			MsgReply( rcvid, EOK, NULL, 0 ); //reply to the client
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

			// Construct data to send to display
			all_planes all_planes;
			all_planes.hdr.type = data.hdr.type;
			all_planes.allPlanes = data.allPlanes;
			msg.planes = all_planes;
			msg.colliding_planes = getCollision();

			toDisplay(msg);

		}

		else if (data.hdr.type == 0x02) { //need to change this to show recieval of messages

			cout << "message from operator system has been received\n";
			plane_msg.ID = data.ID;
			plane_msg.arrivalPosX = data.arrivalPosX;
			plane_msg.arrivalPosY = data.arrivalPosY;
			plane_msg.arrivalPosZ = data.arrivalPosZ;
			plane_msg.arrivalVelX = data.arrivalVelX;
			plane_msg.arrivalVelY = data.arrivalVelY;
			plane_msg.arrivalVelZ = data.arrivalVelZ;
			toCommunicationSystem(plane_msg);

		}

		MsgReply(rcvid, EOK, 0, 0);
	}
	name_detach(attach, 0); //destroying channel with client
	return EXIT_SUCCESS;
}


int ComputerSystem::toDisplay(compsys_display_msg msg){
	if ((server_coid = name_open(DISPLAY_ATTACH_POINT, 0)) == -1) { //opening the channel to connect to the server
		printf("CompSys failed connection to server %d\n\n");
		return EXIT_FAILURE;
	}

	if (MsgSend(server_coid, &msg, sizeof(msg), 0, 0) == -1) { //sending message to the server

		printf("Failed to send message %d\n\n");
		return EXIT_FAILURE;
	}

	name_close(server_coid); //closing connection to the server
	return EXIT_SUCCESS;
}


int ComputerSystem::toCommunicationSystem(plane_msg plane_msg){
	if ((server_coid = name_open(COMMUNICATION_ATTACH_POINT, 0)) == -1) { //opening the channel to connect to the server
		printf("CompSys failed connection to CommSys %d\n\n");
		return EXIT_FAILURE;
	}

	if (MsgSend(server_coid, &plane_msg, sizeof(plane_msg), 0, 0) == -1) { //sending message to the server

		printf("Failed to send message %d\n\n");
		return EXIT_FAILURE;
	}

	name_close(server_coid); //closing connection to the server
	return EXIT_SUCCESS;
}


void* compsys_start_routine(void *arg) {
	ComputerSystem& compsys = *(ComputerSystem*)arg;
	compsys.listen();
	return NULL;
}


ComputerSystem::ComputerSystem() {
	this->server_coid = -1;
	this->rcvid = -1;
	if (pthread_create(&thread_id,NULL,compsys_start_routine,(void *) this) != EOK) {
		printf("ComputerSystem: Failed to start.\n\n");
	}
}


ComputerSystem::~ComputerSystem() {

}
