/*
 * OperatorSys.h
 *
 *  Created on: Mar. 28, 2023
 *      Author: hughmckenzie
 */

#ifndef SRC_OPERATORSYS_H_
#define SRC_OPERATORSYS_H_


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <sys/dispatch.h>
#include <sys/neutrino.h>
#include <vector>
#include <string>
#include <algorithm>
#include "cTimer.h"
#include "Structures.h"
#include <unistd.h>	// To be removed, for testing only

class OperatorSys {
public:
	OperatorSys();
	int toComputerSys();
	virtual ~OperatorSys();
};

#endif /* SRC_OPERATORSYS_H_ */
