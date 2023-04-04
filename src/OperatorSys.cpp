#include "OperatorSys.h"


// ----------------------------------- Constants -----------------------------------
#define COMPUTER_ATTACH_POINT "ComputerSystem"

// ----------------------------------- Class Methods -----------------------------------

 //flight level, speed, and position of aircraft #1

enum string_code {
    speedUpX,
    speedUpY,
    speedUpZ,
	unknownCode
};

string_code hashit (std::string const& inString) {
    if (inString == "speed up") return speedUpY;
    if (inString == "change flight level") return speedUpZ;
    if (inString == "change flight position") return speedUpX;
    else return unknownCode;
}


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

	string commands[5] = {"speed up", "change flight level", "change flight position", "speed up", "change flight level"};
	int amounts[5] = {100, 200, 300, 400, 500};
	int IDs[5] = {1, 2, 3, 4, 5};

	std::vector<Plane*> planes;


	while (1){
			timer.wait_next_activation();
			// get the populated airspace in order to manipulate the planes in the airspace
			airspace = Plane::airspace;

			if (airspace.empty()){
						printf("Airspace empty, no airplanes to change");
					}
					else {

		for (int i = 0; i < 5; i++) {
			switch (hashit(commands[i])) {

					// radar req, formulate a response and send
					case speedUpX: //likely only going to change the velocity in the x direction

						cout << "hellooooooo";

						for (const auto& plane : planes) {
							if (IDs[i] == plane->ID){

					            cout << "old velocity of plane 1: " << planes[i]->arrivalVelX;

								plane->arrivalVelX = amounts[i];

					            cout << "new velocity of plane 1: " << planes[i]->arrivalVelX;

							}
							else{
								cout << "plane ID not found, please try again";
							}
						}

					break;

					// respond to different types of commands
					case speedUpY: //only going to be changing the flight level in the Y direction
						for (const auto& plane : planes) {
							if (IDs[i] == plane->ID){
								plane->arrivalVelY = amounts[i];
							}
							else{
								cout << "plane ID not found, please try again";
							}
						}
					break;

					case speedUpZ: //going to change the position in the Z axis
						for (const auto& plane : planes) {
							if (IDs[i] == plane->ID){
								plane->arrivalVelZ = amounts[i];
							}
							else{
								cout << "plane ID not found, please try again";
							}
						}
					break;
					case unknownCode: //if input isn't what is expected, cout
						cout << "wrong code. please try again";
					break;
				}
		}
					}

			//need the operator system to be treated as a client, plane to be the server


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
				// printf("Radar: Data of Plane #%d: Coords(%d, %d, %d)\n\n", rmsg.ID, rmsg.posX, rmsg.posY, rmsg.posZ);

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


