/*
 * hscan.h
 *
 *  Created on: Nov 19, 2015
 *      Author: kbdavid15
 */

#ifndef CAN_UTILS_HSCAN_H_
#define CAN_UTILS_HSCAN_H_

#include <linux/can.h>
#include "linux/can/bcm.h"
#include <net/if.h>
#include <string>
#include <vector>

#include "txmsg.h"

struct device_id {
	canid_t arbid = 0x513;
	unsigned char text[8];
};

class can {
private:
	int bcm_socket;
	struct sockaddr_can addr;
	struct ifreq ifr;

public:
	can();	// defaults to using the "can0" interface
	can(const char * interface);
	int sendframe(canid_t addr, unsigned char len, unsigned char * data);
	bcm_message add_message(txmsg *msg);
	bcm_message add_message(can_frame msg);
	bcm_message add_message(canid_t addr, long period, unsigned char len, unsigned char * data);
	bcm_message add_message(can_frame frame, long period);
	void update_message(bcm_message);
	void update_message(txmsg *msg);
};

#endif /* CAN_UTILS_HSCAN_H_ */
