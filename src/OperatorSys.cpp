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


	cTimer timer(20,0, 5, 0); //creating a polling server which will call this function every 5 seconds, asking if we want to change airplane commands

	msg msg;
	msg.hdr.type = 0x00;
	plane_info rmsg;
	all_planes data;
	data.hdr.type = 0x01;
	char buffer[10];

	string commands[5] = {"speed up", "change flight level", "change flight position", "speed up", "change flight level"};
	int amounts[5] = {100, 200, 300, 400, 500};
	int IDs[5] = {2, 3, 4, 5, 6};


	while (1){

			timer.wait_next_activation();
			// get the populated airspace in order to manipulate the planes in the airspace

			if (atc.planes.empty()){
				printf("Airspace empty, no airplanes to change");
				}
			else {

		for (int i = 0; i < 5; i++) {
			switch (hashit(commands[i])) {

					// radar req, formulate a response and send
					case speedUpX: //likely only going to change the velocity in the x direction

						for (const auto& plane : atc.planes) {
							if (IDs[i] == plane->ID){

					            cout << "plane ID: " << plane->ID << " old velocity of plane in X axis: " << atc.planes[i]->arrivalVelY << endl;

								plane->arrivalVelX = amounts[i];

					            cout << "plane ID: " << plane->ID << " new velocity of plane in X axis: " << atc.planes[i]->arrivalVelY << endl;
					            break;
							}

							else{
								// cout << "plane ID not found, please try again";
							}
						}

					break;

					// respond to different types of commands
					case speedUpY: //only going to be changing the flight level in the Y direction
						for (const auto& plane : atc.planes) {

							if (IDs[i] == plane->ID){

					            cout << "plane ID: " << plane->ID << " old velocity of plane in Y axis: " << atc.planes[i]->arrivalVelY << endl;


								plane->arrivalVelY = amounts[i];

					            cout << "plane ID: " << plane->ID << " new velocity of plane in Y axis: " << atc.planes[i]->arrivalVelY << endl;
					            break;
							}
							else{
								// cout << "plane ID: " << IDs[i] << " not found!!!";
							}
						}
					break;

					case speedUpZ: //going to change the position in the Z axis
						for (const auto& plane : atc.planes) {
							if (IDs[i] == plane->ID){

					            cout << "plane ID: " << plane->ID << " old velocity of plane in Z axis: " << atc.planes[i]->arrivalVelY << endl;

								plane->arrivalVelZ = amounts[i];

					            cout << "plane ID: " << plane->ID << " new velocity of plane in Z axis: " << atc.planes[i]->arrivalVelY << endl;

								break;
							}
							else{
								// cout << "plane ID not found, please try again";
							}
						}
					break;
					case unknownCode: //if input isn't what is expected, cout
						cout << "wrong code. please try again";
					break;
				}
			}
		}

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


OperatorSys::OperatorSys(ATC atc) {

		this->atc=atc;
		this->server_coid = 0;
		if(pthread_create(&thread_id,NULL,operator_system_start_routine,(void *) this)!=EOK)
		{
			printf("Operator System: Failed to start.\n\n");
		}
}

OperatorSys::~OperatorSys() {
	// TODO Auto-generated destructor stub
}


