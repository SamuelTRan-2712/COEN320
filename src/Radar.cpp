#include "Radar.h"
using namespace std;


// ----------------------------------- Constants -----------------------------------
#define COMPUTER_ATTACH_POINT "ComputerSystem"


// ----------------------------------- Class Methods -----------------------------------
int Radar::toComputerSys(compsys_msg data){
	if ((server_coid = name_open(COMPUTER_ATTACH_POINT, 0)) == -1) {
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

//void Radar::removePlaneFromAirspace(int planeID) {
//    airspace.erase(std::remove(airspace.begin(), airspace.end(), planeID), airspace.end());
//}
//
//void Radar::addPlaneToAirspace(int planeID) {
//    airspace.push_back(planeID);
//}


void Radar::pingAirspace(){
	cTimer timer(1,0, 1, 0); //creating a timer of period 1 with an offset of 1. CHANGE IF WE WANT TO CHANGE THE AMOUNT OF TIME BETWEEN PINGS

	plane_msg msg;
	msg.hdr.type = 0x00;
	plane_info rmsg;
	compsys_msg data;
	data.hdr.type = 0x01;
	char buffer[10];


	while (1){
		// get the populated airspace
		airspace = Plane::airspace;


		timer.wait_next_activation();


		if (airspace.empty()){
			printf("Radar: Airspace empty\n\n");
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
				//printf("Radar: Data of Plane #%d: Coords(%d, %d, %d)\n\n", rmsg.ID, rmsg.posX, rmsg.posY, rmsg.posZ);

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


void* radar_start_routine(void *arg)
{
	Radar& radar = *(Radar*) arg;
	radar.pingAirspace();
	return NULL;
}


Radar::Radar()
{
	this->server_coid = 0;
	if(pthread_create(&thread_id,NULL,radar_start_routine,(void *) this)!=EOK)
	{
		printf("Radar: Failed to start.\n\n");
	}
}


Radar::~Radar()
{
}

