#include "Radar.h"
#include "Plane.h"
using namespace std;


// ----------------------------------- Constants -----------------------------------
#define COMPUTER_ATTACH_POINT "ComputerSystem"


// ----------------------------------- Class Methods -----------------------------------
int Radar::toComputerSys(all_planes data){
	if ((server_coid = name_open(COMPUTER_ATTACH_POINT, 0)) == -1) {
		printf("Failed connection to server %d\n\n");
		return EXIT_FAILURE;
	}
	if (MsgSend(server_coid, &data, sizeof(data), 0, 0) == -1) {
		printf("Failed to send message %d\n\n");
		return EXIT_FAILURE;
	}
	name_close(server_coid);
	return EXIT_SUCCESS;
}


void Radar::pingAirspace(){
	cTimer timer(1,1);
	while (1){
//		TODO: set timer to ping planes every X seconds
		//timer.waitTimer();

		msg msg;
		plane_info rmsg;
		all_planes data;
		char buffer[10];
		msg.hdr.type = 0x00;
		airspace = Plane::airspace; // get the populated airspace

		if (airspace.empty()){
			printf("Airspace empty\n\n");
		}
		else {
			for (int i : airspace){
				// go through the airspace and ping each plane
				if ((server_coid = name_open(itoa(i,buffer,10), 0)) == -1){
					printf("Failed connection to server %d\n\n", i);
					break;
				}
				if (MsgSend(server_coid, &msg, sizeof(msg), &rmsg, sizeof(rmsg)) == -1){
					printf("Failed to send message %d\n\n", i);
					break;
				}
				name_close(server_coid);
				printf("Data of Plane #%d: Coords(%d, %d, %d)\n\n", rmsg.ID, rmsg.posX, rmsg.posY, rmsg.posZ);

				// add plane data to vector destined to computer system
				allPlaneData.push_back(rmsg);
			}

			// send data of all planes to computer system
			// reset allPlaneData
			data.hdr.type = 0x01;
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
	if(pthread_create(&thread_id,NULL,radar_start_routine,(void *) this)!=EOK)
	{
		thread_id = 0;
	}
}


Radar::~Radar()
{
}

