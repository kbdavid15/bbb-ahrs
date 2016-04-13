/*
 * hscan.h
 *
 *  Created on: Nov 19, 2015
 *      Author: kbdavid15
 */

#ifndef MSG_CAN_UTILS_HSCAN_H_
#define MSG_CAN_UTILS_HSCAN_H_

#include <linux/can.h>
#include <net/if.h>

#include "linux/can/bcm.h"
#include "TxMsg.h"

class hscan {
private:
	int bcm_socket;
	struct sockaddr_can addr;
	struct ifreq ifr;

public:
	hscan();	// defaults to using the "can0" interface
	hscan(const char * interface);
	virtual ~hscan();
	void add_message(bcm_message msg);
	bcm_message add_message(canid_t addr, long period, unsigned char len, unsigned char * data);
	bcm_message add_message(can_frame frame, long period);
	void update_message(bcm_message);
	bool isUp();
	int set_if_flags(char *ifname, short flags);
	int set_if_up(char *ifname, short flags);
};

#endif /* MSG_CAN_UTILS_HSCAN_H_ */
