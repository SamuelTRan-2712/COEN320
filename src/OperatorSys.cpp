#include "OperatorSys.h"


// ----------------------------------- Constants -----------------------------------
#define COMPUTER_ATTACH_POINT "ComputerSystem"

// ----------------------------------- Class Methods -----------------------------------

static enum StringValue { evNotDefined,
                          evStringValue1,
                          evStringValue2,
                          evStringValue3,
                          evEnd };

static std::map<std::string, StringValue> s_mapStringValues;

static char szInput[_MAX_PATH];

static void Initialize();


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
	string direction;
	int amount;

	while (1){
		// get the populated airspace in order to manipulate the planes in the airspace
		airspace = Plane::airspace;

		cout << "Please enter a string (end to terminate): ";
		    cout.flush();
		    cin.getline(szInput, _MAX_PATH);

		cout << "Send a command to a certain plane followed by its ID:  ";
		cin >> command >> direction >> amount >> ID;

		switch (s_mapStringValues[szInput]) {

				// radar req, formulate a response and send
				case evStringValue1 == "speed up": //likely only going to change the velocity in the x direction
					for (const auto& plane : planes) {
						if (ID == plane->ID){
							plane.XVel == amount;
						}
						else{
							cout << "plane ID not found, please try again";
						}
					}
				break;

				// responde to different types of commands
				case evStringValue2 == "change flight level": //only going to be changing the flight level in the Y direction
					for (const auto& plane : planes) {
						if (ID == plane->ID){
							plane.YVel == amount;
						}
						else{
							cout << "plane ID not found, please try again";
						}
					}
				break;

				case evStringValue3 == "change flight position": //going to change the position in the Z axis
					for (const auto& plane : planes) {
						if (ID == plane->ID){
							plane.YVel == amount;
						}
						else{
							cout << "plane ID not found, please try again";
						}
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

void Initialize()
{
  s_mapStringValues["First Value"] = evStringValue1;
  s_mapStringValues["Second Value"] = evStringValue2;
  s_mapStringValues["Third Value"] = evStringValue3;
  s_mapStringValues["end"] = evEnd;

  cout << "s_mapStringValues contains "
       << s_mapStringValues.size()
       << " entries." << endl;
}

OperatorSys::~OperatorSys() {
	// TODO Auto-generated destructor stub
}


