#include "Plane.h"
using namespace std;

vector<int> Plane::airspace;
const uint64_t timeout = 8000000;

// Plane start routine that calls updateLocation()
void* plane_start_routine(void *arg){
	Plane& plane = *(Plane*) arg;
	plane.updateLocation();
	return NULL;
}

// Updates location every second and listens to ping from radar
int Plane::updateLocation(){
	cTimer timer(time,1);
	name_attach_t *attach;
	msg msg;
	plane_info plane_info;
	char buffer[10];
	if((attach = name_attach(NULL, itoa(ID,buffer,10), 0)) == NULL){
		printf("Plane failed to create channel\n\n");
		return EXIT_FAILURE;
	}
	// loop until plane exits the monitored airspace
	while (posX < 100000 && posY < 100000 && posZ < 25000) // remember to put the actual values
	{
		timer.wait_next_activation();
		// add plane id to airspace vector
		if (find(airspace.begin(), airspace.end(), ID) != airspace.end()){
			//printf("ID present in airspace %d\n\n", ID);
		}
		else {
			airspace.push_back(ID);
			//printf("ID written to airspace %d\n\n", ID);
		}
		// update location
		this->posX += velX;
		this->posY += velY;
		this->posZ += velZ;

		// listen for messages from radar and send reply
		plane_info = {ID, posX, posY, posZ, velX, velY, velZ};
		rcvid = MsgReceive(attach->chid, &msg, sizeof(msg), NULL);
		if (msg.hdr.type == 0x00){
			MsgReply(rcvid, EOK, &plane_info, sizeof(plane_info));
		}
	}
	// remove plane id from the airspace and kill thread
	airspace.erase(remove(airspace.begin(), airspace.end(), ID), airspace.end());
	name_detach(attach, 0);
	pthread_exit(NULL);
	return EXIT_SUCCESS;
}

Plane::Plane(int time, int ID, int posX, int posY, int posZ, int velX, int velY, int velZ){
	// set plane arguments and create thread
	setPlane(time, ID, posX, posY, posZ, velX, velY, velZ);

	if(pthread_create(&thread_id,NULL,plane_start_routine,(void *) this)!=EOK){
		thread_id=NULL;
	}
}

void Plane::setPlane(int time, int ID, int posX, int posY, int posZ, int velX, int velY, int velZ){
	this->time = time;
	this->ID = ID;
	this->posX = posX;
	this->posY = posY;
	this->posZ = posZ;
	this->velX = velX;
	this->velY = velY;
	this->velZ = velZ;
}

void Plane::setCoordinates(int posX, int posY, int posZ){
	this->posX = posX;
	this->posY = posY;
	this->posZ = posZ;
}

void Plane::setVelocity(int velX, int velY, int velZ){
	this->velX = velX;
	this->velY = velY;
	this->velZ = velZ;
}

void Plane::sendInfo(){
	// send extra information to radar on operator request
}

Plane::~Plane() {
	pthread_mutex_destroy( &mutex );
	thread_id = NULL;
}

