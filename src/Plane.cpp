#include "Plane.h"
using namespace std;

// ----------------------------------- Constants -----------------------------------
vector<int> Plane::airspace;

// ----------------------------------- Class Methods  -----------------------------------

void* plane_start_routine(void *arg){
	Plane& plane = *(Plane*) arg;
	plane.updateLocation();
	return NULL;
}

int Plane::getAirspaceSize() {
    return airspace.size();
}


int Plane::updateLocation(){
	cTimer timer(1,0,1, 0); //updates location every one second
	name_attach_t *attach;
	planes_information plane_info;
	plane_msg plane_msg;
	char buffer[10];

	if((attach = name_attach(NULL, itoa(ID,buffer,10), 0)) == NULL){ //thread that wishes to receive messages creates a channel, thread that wishes to send messages connects to a channel
		printf("Plane failed to attach to channel\n\n");
		return EXIT_FAILURE;
	}

	while (arrivalPosX < 100000 && arrivalPosY < 100000 && arrivalPosZ < 25000 && arrivalPosZ > 15000) //when an airplane has entered the airspace, add it to the vector
	{
		 timer.wait_next_activation();

		if (find(airspace.begin(), airspace.end(), ID) != airspace.end()) { //airspace finding plane
			// cout << "plane not inside the airspace";
		}
		else {
			airspace.push_back(ID); //adds ID of the plane to the vector, not the entire plane itself, useful for display
		}

		this->arrivalPosX += arrivalVelX;
		this->arrivalPosY += arrivalVelY;
		this->arrivalPosZ += arrivalVelZ;

		plane_info = {ID, arrivalPosX, arrivalPosY, arrivalPosZ, arrivalVelX, arrivalVelY, arrivalVelZ}; //message info being sent to the client
		rcvid = MsgReceive(attach->chid, &plane_msg, sizeof(plane_msg), NULL);

		if (rcvid == -1) {/* Error condition, exit */
					break;
		}


		if (plane_msg.hdr.type == 0x00) {
			MsgReply(rcvid, EOK, &plane_info, sizeof(plane_info)); //sends a reply to the radar
		}

		else if (plane_msg.hdr.type == 0x01) {
			cout << "New command: " << plane_msg.ID << " |arrivalPosZ "<< plane_msg.arrivalPosZ << " |arrivalPosX "<< plane_msg.arrivalPosX << " |arrivalPosY "<< plane_msg.arrivalPosY << " |arrivalVelX " << plane_msg.arrivalVelX << " |arrivalVelY " << plane_msg.arrivalVelY << " |arrivalVelZ " << plane_msg.arrivalVelZ << endl;
			 setVelocity(plane_msg.arrivalVelX, plane_msg.arrivalVelY, plane_msg.arrivalVelZ);
			 setCoordinates(plane_msg.arrivalPosX, plane_msg.arrivalPosY, plane_msg.arrivalPosZ);
		}
	}


	airspace.erase(remove(airspace.begin(), airspace.end(), ID), airspace.end());
	name_detach(attach, 0); //detaching all threads
	pthread_exit(NULL);
	return EXIT_SUCCESS;
}


double distanceBetweenPlanes(const Plane& p1, const Plane& p2) { //distance between two planes function to send to computer sys
  double dx = p1.arrivalPosX - p2.arrivalPosX;
  double dy = p1.arrivalPosY - p2.arrivalPosY;
  double dz = p1.arrivalPosZ - p2.arrivalPosZ;
  return sqrt(dx*dx + dy*dy + dz*dz);
}


Plane::Plane(int ID, int time, int arrivalPosX, int arrivalPosY, int arrivalPosZ, int arrivalVelX, int arrivalVelY, int arrivalVelZ){
	//creating planes, as well as initializing a thread for each plane
	setPlane(ID, time, arrivalPosX, arrivalPosY, arrivalPosZ, arrivalVelX, arrivalVelY, arrivalVelZ);

	if(pthread_create(&thread_id,NULL,plane_start_routine,(void *) this)!=EOK){
		printf("Plane: failed to initialize.");
	}

}


void Plane::setPlane(int ID, int time, int arrivalPosX, int arrivalPosY, int arrivalPosZ, int arrivalVelX, int arrivalVelY, int arrivalVelZ){
	this->ID = ID;
	this->time = time;
	this->arrivalPosX = arrivalPosX;
	this->arrivalPosY = arrivalPosY;
	this->arrivalPosZ = arrivalPosZ;
	this->arrivalVelX = arrivalVelX;
	this->arrivalVelY = arrivalVelY;
	this->arrivalVelZ = arrivalVelZ;
}


void Plane::setCoordinates(int arrivalPosX, int arrivalPosY, int arrivalPosZ){
	this->arrivalPosX = arrivalPosX;
	this->arrivalPosY = arrivalPosY;
	this->arrivalPosZ = arrivalPosZ;
}


void Plane::changeVelocity(int newVelX, int newVelY, int newVelZ) { //useful for communication with the operator system
    this->arrivalVelX = newVelX;
    this->arrivalVelY = newVelY;
    this->arrivalVelZ = newVelZ;
}

void Plane::setVelocity(int arrivalVelX, int arrivaVelY, int arrivalVelZ){
	this->arrivalVelX = arrivalVelX;
	this->arrivalVelY = arrivalVelY;
	this->arrivalVelZ = arrivalVelZ;
}


Plane::~Plane()
{
}

