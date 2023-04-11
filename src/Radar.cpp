#include "Radar.h"
using namespace std;


// ----------------------------------- Constants -----------------------------------
#define COMPUTER_ATTACH_POINT "ComputerSystem"

#define ATTACH_POINT "my_channel"


// ----------------------------------- Class Methods -----------------------------------
int Radar::toComputerSys(all_planes data){
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


void Radar::pingAirspace(){
	cTimer timer(1,0, 1, 0); //creating a timer of period 1 with an offset of 1. CHANGE IF WE WANT TO CHANGE THE AMOUNT OF TIME BETWEEN PINGS



	msg pos_msg;
	pos_msg.hdr.type = 0x00;
	plane_info rmsg;
	all_planes data;
	data.hdr.type = 0x01;
	char attach_points[20];
	name_attach_t *attach;

	while (1){

		timer.wait_next_activation();
		cout << "Timer waited 1s" << endl;
		// get the populated airspace
		airspace = Plane::airspace;


		if (airspace.empty()){
			printf("Radar: Airspace empty\n\n");
		}
		else {
			for (int i : airspace){

//				/* Create a local name (/dev/name/local/...) */
//				   if ((attach = name_attach(NULL, ATTACH_POINT, 0)) == NULL) {
//					   printf("Radar: Failed connection to server %d\n\n", i);

//					   break;
//				   }

				// go through the airspace and ping each plane
				if ((server_coid = name_open(itoa(i,attach_points,20), 0)) == -1){
					printf("Radar: Failed connection to server %d\n\n", i);
					break;
				}

				if (MsgSend(server_coid, &pos_msg, sizeof(pos_msg), &rmsg, sizeof(rmsg)) == -1){
					printf("Radar: Failed to send message %d\n\n", i);
					break;
				}

				name_close(server_coid);
				printf("Radar: Data of Plane #%d: Coords(%d, %d, %d)\n\n", rmsg.ID, rmsg.arrivalPosX, rmsg.arrivalPosY, rmsg.arrivalPosZ);

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

