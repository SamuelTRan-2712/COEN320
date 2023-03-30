#include "OperatorSys.h"


// ----------------------------------- Constants -----------------------------------
#define COMPUTER_ATTACH_POINT "ComputerSystem"

// ----------------------------------- Class Methods -----------------------------------


int OperatorSys::toComputerSys(all_planes data) {
	if ((server_coid = name_open(COMPUTER_ATTACH_POINT, 0)) == -1) { //connects to the computer attach point
			printf("Radar: Failed connection to server %d\n\n");
			return EXIT_FAILURE;
		}
		if (MsgSend(server_coid, &data, sizeof(data), 0, 0) == -1) {
			printf("Radar: Failed to send message %d\n\n");
			return EXIT_FAILURE;
		}
		name_close(server_coid);
		return EXIT_SUCCESS;
	}


void OperatorSys::getCommands(){
	cTimer timer(5,0, 5, 0); //creating a polling server which will call this function every 5 seconds, asking if we want to change airplane commands

	msg msg;
	msg.hdr.type = 0x00;
	plane_info rmsg;
	all_planes data;
	data.hdr.type = 0x01;
	char buffer[10];

	string command;
	string ID;

	while (1){
		// get the populated airspace in order to manipulate the planes in the airspace
		airspace = Plane::airspace;

		cout << "Send a command to a certain plane followed by its ID:  ";
		cin >> command >> direction >> amount >> ID;

		switch (command) {

				// radar req, formulate a response and send
				case command == "speed up": //likely only going to change the velocity in the x direction
					if (msg.hdr.subtype == MsgSubtype::REQ) {
						// turn request msg into response msg with plane info and reply
						msg.hdr.subtype = MsgSubtype::REPLY;
						msg.info = plane.info; // send this plane info
						MsgReply(rcvid, EOK, &msg, sizeof(msg));
					}
					break;

				// respond to request for plane info from operator
				// DATA PATH: CONSOLE -> CPU -> COMMS -> PLANE,
				// then PLANE -> COMMS -> CPU -> DISPLAY
				case MsgType::INFO:
					MsgReply(rcvid, EOK, 0, 0);
					// make reply
					msg.hdr.subtype = MsgSubtype::REPLY;
					msg.info = plane.info;
					// open channel to comms
					int coid;
					if ((coid = name_open(COMMS_CHANNEL, 0)) == -1) {
						cout << "ERROR: CREATING CLIENT TO COMMS" << endl;
						break;
					}
					// send info
					MsgSend(coid, &msg, sizeof(msg), 0, 0);
					// close channel
					name_close(coid);
					break;

				// responde to different types of commands
				case command == "change flight level": //only going to be changing the flight level in the Y direction
					MsgReply(rcvid, EOK, 0, 0); // send the eok because it was blocked
					// message type command send by the the radar to make the plane change speed
					if (msg.hdr.subtype == MsgSubtype::CHANGE_SPEED) {
						double percent = (msg.doubleValue - plane.v) / plane.v;
						plane.info.dx *= (1 + percent);
						plane.info.dy *= (1 + percent);
						plane.v = msg.doubleValue;
					}
					// message type command send by the the radar to make the plane change altitude
					else if (msg.hdr.subtype == MsgSubtype::CHANGE_ALTITUDE) {
						plane.changeAltFlag = true;
						plane.finalAlt = msg.info.z;
						plane.info.dz = msg.info.z > plane.info.z ? +50 : -50;
					}
					// message type command send by the the radar to make the plane position
					else if (msg.hdr.subtype == MsgSubtype::CHANGE_POSITION) {
						// using trig to determine resultant directions using vector
						const double &dx = plane.info.dx, &dy = plane.info.dy;
						double angle = atan(dy / dx);
						if (dx < 0) angle += PI;
						angle += msg.doubleValue * PI / 180;
						plane.info.dx = plane.v * cos(angle);
						plane.info.dy = plane.v * sin(angle);
					}

				case command == "change flight position": //going to change the position in the Z axis
					MsgReply(rcvid, EOK, 0, 0); // send the eok because it was blocked
					// message type command send by the the radar to make the plane change speed
					if (msg.hdr.subtype == MsgSubtype::CHANGE_SPEED) {
						double percent = (msg.doubleValue - plane.v) / plane.v;
						plane.info.dx *= (1 + percent);
						plane.info.dy *= (1 + percent);
						plane.v = msg.doubleValue;
					}
					// message type command send by the the radar to make the plane change altitude
					else if (msg.hdr.subtype == MsgSubtype::CHANGE_ALTITUDE) {
						plane.changeAltFlag = true;
						plane.finalAlt = msg.info.z;
						plane.info.dz = msg.info.z > plane.info.z ? +50 : -50;
					}
					// message type command send by the the radar to make the plane position
					else if (msg.hdr.subtype == MsgSubtype::CHANGE_POSITION) {
						// using trig to determine resultant directions using vector
						const double &dx = plane.info.dx, &dy = plane.info.dy;
						double angle = atan(dy / dx);
						if (dx < 0) angle += PI;
						angle += msg.doubleValue * PI / 180;
						plane.info.dx = plane.v * cos(angle);
						plane.info.dy = plane.v * sin(angle);
					}
					break;



		//timer.waitTimer();

		if (airspace.empty()){
			printf("Airspace empty, no airplanes to change");
		}
		else {
			for (int i : airspace){


				// go through the airspace and ping each plane
				if ((server_coid = name_open(itoa(i,buffer,10), 0)) == -1){
					printf("Radar: Failed connection to server %d\n\n", i);
					break;
				}
				if (MsgSend(server_coid, &msg, sizeof(msg), &rmsg, sizeof(rmsg)) == -1){
					printf("Radar: Failed to send message %d\n\n", i);
					break;
				}
				name_close(server_coid);
				printf("Radar: Data of Plane #%d: Coords(%d, %d, %d)\n\n", rmsg.ID, rmsg.posX, rmsg.posY, rmsg.posZ);

				// add plane data to vector destined to computer system
				allPlaneData.push_back(rmsg);
			}

			// send data of all planes to computer system
			// reset allPlaneData
			data.allPlanes = allPlaneData;
			toComputerSys(data);
			allPlaneData.clear();
		}
	}
}


void* operator_system_start_routine(void *arg)
{
	OperatorSys& operatorSys = *(OperatorSys*) arg;
	operatorSys.getCommands();
	return NULL;
}

OperatorSys::OperatorSys() {
		this->server_coid = 0;
		if(pthread_create(&thread_id,NULL,operator_system_start_routine,(void *) this)!=EOK)
		{
			printf("Operator System: Failed to start.\n\n");
		}
}



OperatorSys::~OperatorSys() {
	// TODO Auto-generated destructor stub
}


