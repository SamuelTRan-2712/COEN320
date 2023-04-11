#ifndef STRUCTURES_H_
#define STRUCTURES_H_

typedef struct _pulse msg_header_t;


typedef struct _my_msg {
	msg_header_t hdr;
} msg;


typedef struct _planes_information {
	int ID, arrivalPosX, arrivalPosY, arrivalPosZ, arrivalVelX, arrivalVelY, arrivalVelZ;
} planes_information;


typedef struct {
	msg_header_t hdr;
	std::vector<planes_information> allPlanes;
	int ID, arrivalPosX, arrivalPosY, arrivalPosZ, arrivalVelX, arrivalVelY, arrivalVelZ;
} compsys_msg;

typedef struct _new_planes { //trying to use this from operator system to edit planes data
	msg_header_t hdr;
	std::vector<planes_information> allPlanes;
} new_planes;


typedef struct {
	int plane_ID_1;
	int plane_ID_2;
} violating_pair_ids;

typedef struct _all_planes {
	msg_header_t hdr;
	std::vector<planes_information> allPlanes;
} all_planes;

typedef struct {
	all_planes planes;
	std::vector<violating_pair_ids> colliding_planes;
} compsys_display_msg;


typedef struct {
	msg_header_t hdr;
	int ID, arrivalPosX, arrivalPosY, arrivalPosZ, arrivalVelX, arrivalVelY, arrivalVelZ;
} plane_msg;


#endif /* STRUCTURES_H_ */
