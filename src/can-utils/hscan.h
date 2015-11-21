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

struct bcm_message {
	struct bcm_msg_head msg_head;
	struct can_frame frame;
};


class can {
private:
	int sock;
	struct sockaddr_can addr;
	struct ifreq ifr;

public:
	can();	// defaults to using the "can0" interface
	can(const char * interface);
	int sendframe(canid_t addr, unsigned char len, unsigned char * data);
	bcm_message add_message(txmsg msg);
	void update_message(bcm_message);
};



#endif /* CAN_UTILS_HSCAN_H_ */
