#ifndef STRUCTURES_H_
#define STRUCTURES_H_

typedef struct _pulse msg_header_t;

typedef struct _my_msg {
	msg_header_t hdr;
} msg;

typedef struct _plane_info {
	int ID, posX, posY, posZ, velX, velY, velZ;
} plane_info;

typedef struct _all_planes {
	msg_header_t hdr;
	std::vector<plane_info> allPlanes;
} all_planes;

#endif /* STRUCTURES_H_ */
