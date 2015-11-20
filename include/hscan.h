/*
 * mcan.h
 *
 *  Created on: Nov 19, 2015
 *      Author: kbdavid15
 */

#ifndef HSCAN_H_
#define HSCAN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "can-utils/lib.h"

class can {
public:
	int sendframe(canid_t addr, unsigned char len, unsigned char * data);
};



#endif /* HSCAN_H_ */
