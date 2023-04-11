#include "Radar.h"
using namespace std;

// ----------------------------------- Constants -----------------------------------
#define COMPUTER_SYSTEM_ATTACH_POINT "ComputerSystem"

//#define ATTACH_POINT "my_channel"

// ----------------------------------- Class Methods -----------------------------------
int Radar::toComputerSys(compsys_msg data){
	if ((server_coid = name_open(COMPUTER_SYSTEM_ATTACH_POINT, 0)) == -1) {
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


void Radar::getAirspace(){
	cTimer timer(1,0, 1, 0);

	msg pos_msg;
	pos_msg.hdr.type = 0x00;

	planes_information plane_msg;
	compsys_msg data;
	data.hdr.type = 0x01;
	char attach_points[20];
	name_attach_t *attach;

	while (1){

		timer.wait_next_activation();
		cout << "Timer waited 1s" << endl;
		airspace = Plane::airspace;



		timer.wait_next_activation();

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

				if ((server_coid = name_open(itoa(i,attach_points,20), 0)) == -1){
					printf("Radar: Error occurred while attaching to channel %d\n\n", i);
					break;
				}

				if (MsgSend(server_coid, &pos_msg, sizeof(pos_msg), &plane_msg, sizeof(plane_msg)) == -1){
					printf("Radar: Error occurred while sending message %d\n\n", i);
					break;
				}

				name_close(server_coid);
				printf("Radar: Data of Plane #%d: Coords(%d, %d, %d)\n\n", plane_msg.ID, plane_msg.arrivalPosX, plane_msg.arrivalPosY, plane_msg.arrivalPosZ);
				allPlaneData.push_back(plane_msg);
			}

			data.allPlanes = allPlaneData;
			toComputerSys(data);
			allPlaneData.clear();
		}
	}
}


void* radar_start_routine(void *arg)
{
	Radar& radar = *(Radar*) arg;
	radar.getAirspace();
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

