#include "CommunicationSystem.h"
using namespace std;


// ----------------------------------- Constants -----------------------------------
#define COMPUTER_ATTACH_POINT "ComputerSystem"
#define COMMUNICATION_SYSTEM_ATTACH_POINT "CommunicationSystem"


// ----------------------------------- Class Methods -----------------------------------

// Helper function to help send command to planes
int CommunicationSystem::toPlane(plane_msg& plane_msg) {
	char buffer[10];
	char* plane_server = itoa(plane_msg.ID,buffer,10);
	plane_msg.hdr.type = 0x01;

	if ((server_coid = name_open(plane_server, 0)) == -1) {
		printf("CommunicationSys: Failed connection to server %d\n\n");
		return EXIT_FAILURE;
	}
	if (MsgSend(server_coid, &plane_msg, sizeof(plane_msg), 0, 0) == -1) {
		printf("CommunicationSys: Failed to send message %d\n\n");
		return EXIT_FAILURE;
	}
	name_close(server_coid);
	return EXIT_SUCCESS;
}


int CommunicationSystem::fromCompSys() {
	name_attach_t *attach;
	plane_msg data;


	if((attach = name_attach(NULL, COMMUNICATION_SYSTEM_ATTACH_POINT, 0)) == NULL) {
		printf("CommunicationSys failed to create channel\n\n");
		return EXIT_FAILURE;
	}

	while (1) {
		rcvid = MsgReceive(attach->chid, &data, sizeof(data), NULL); // receive messages from CompSys
		if (rcvid == -1) {/* Error condition, exit */
			break;
		}

		if (rcvid == 0) {/* Pulse received */
		    switch (data.hdr.code) {
		    	case _PULSE_CODE_DISCONNECT:
		    		ConnectDetach(data.hdr.scoid);
		    		break;
		    	case _PULSE_CODE_UNBLOCK:
		    		break;
		    	default:
		    		break;
		    }
		    continue;
		}

		/* name_open() sends a connect message, must EOK this */
		if (data.hdr.type == _IO_CONNECT ) {
			MsgReply( rcvid, EOK, NULL, 0 );
		    continue;
		}

		/* Some other QNX IO message was received; reject it */
		if (data.hdr.type > _IO_BASE && data.hdr.type <= _IO_MAX ) {
			MsgError( rcvid, ENOSYS ); // error can be -1, ENOSYS, ERESTART, EOK, or the error code that you want to set for the client.

		    continue;
		}

		// check for appropriate header and copy the data to planes
		if (data.hdr.type == 0x01) {
			toPlane(data);
		}
		MsgReply(rcvid, EOK, 0, 0);
	}
	name_detach(attach, 0); //destroying channel with server
	return EXIT_SUCCESS;
}


void* commSys_start_routine(void *arg)
{
	CommunicationSystem& comm_sys = *(CommunicationSystem*) arg;
	comm_sys.fromCompSys();
	return NULL;
}


CommunicationSystem::CommunicationSystem()
{
	this->server_coid = -1;
	if (pthread_create(&thread_id,NULL,commSys_start_routine,(void *) this) != EOK)
	{
		printf("CommSys: Failed to start.\n\n");
	}
}


CommunicationSystem::~CommunicationSystem()
{
}

