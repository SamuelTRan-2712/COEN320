#include "ComputerSystem.h"

// ----------------------------------- Constants -----------------------------------
#define COMPUTER_ATTACH_POINT "ComputerSystem"
#define DISPLAY_ATTACH_POINT "Display"
#define COMMUNICATION_ATTACH_POINT "CommunicationSystem"

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
		if (rcvid == -1) {
			break;
		}
		if (rcvid == 0) {
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

		if (data.hdr.type == _IO_CONNECT ) {
			MsgReply( rcvid, EOK, NULL, 0 );
		    continue;
		}

		if (data.hdr.type > _IO_BASE && data.hdr.type <= _IO_MAX ) {
			MsgError( rcvid, ENOSYS );
		    continue;
		}

		if (data.hdr.type == 0x01) {
			planes = data.allPlanes;

			// Construct data to send to display
			all_planes all_planes;
			all_planes.hdr.type = data.hdr.type;
			all_planes.allPlanes = data.allPlanes;
			msg.planes = all_planes;
			msg.colliding_planes = getCollision();

			// write to file
			for (const auto& plane : all_planes.allPlanes) {

				// log to file
				std::string var = "Plane " + std::to_string(plane.ID) + " - " + std::to_string(plane.arrivalPosX) + " - " + std::to_string(plane.arrivalPosY) + " - " + std::to_string(plane.arrivalPosZ) + " - " + std::to_string(plane.arrivalVelX) + " - " + std::to_string(plane.arrivalVelY) + " - " + std::to_string(plane.arrivalVelZ) + "\n";

				const int length = var.length();

				// declaring character array (+1 for null terminator)
				char* char_array = new char[length + 1];

				// copying the contents of the
				// string to char array
				strcpy(char_array, var.c_str());

				writeToFile(char_array, length);

			}

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


int ComputerSystem::writeToFile(char* buffer, int size) {
	int  size_written;

	/* write the text              */
	size_written = write( this->fd, buffer, size );

	/* test for error              */
	if( size_written != size ) {
		perror( "Error writing history" );
		return EXIT_FAILURE;
	}

	/* close the file              */

	return EXIT_SUCCESS;
}


ComputerSystem::ComputerSystem() {
	this->fd = creat( "/data/home/qnxuser/history.txt", S_IRUSR | S_IWUSR | S_IXUSR );
	this->server_coid = -1;
	this->rcvid = -1;
	if (pthread_create(&thread_id,NULL,compsys_start_routine,(void *) this) != EOK) {
		printf("ComputerSystem: Failed to start.\n\n");
	}
}


ComputerSystem::~ComputerSystem() {
	close( fd );
}
