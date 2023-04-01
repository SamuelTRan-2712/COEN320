#ifndef STRUCTURES_H_
#define STRUCTURES_H_

typedef struct _pulse msg_header_t;


typedef struct _my_msg {
	msg_header_t hdr;
} msg;


typedef struct _plane_info {
	int ID, arrivalPosX, arrivalPosY, arrivalPosZ, arrivalVelX, arrivalVelY, arrivalVelZ;
} plane_info;


typedef struct _all_planes {
	msg_header_t hdr;
	std::vector<plane_info> allPlanes;
} all_planes;


typedef struct {
	int plane_ID_1;
	int plane_ID_2;
} violating_pair_ids;


typedef struct {
	all_planes planes;
	std::vector<violating_pair_ids> colliding_planes;
} compsys_display_msg;


#endif /* STRUCTURES_H_ */
