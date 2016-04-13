/*
 * hscan.cpp
 *
 *  Created on: Nov 19, 2015
 *      Author: kbdavid15
 */

#include "../../msg/can-utils/hscan.h"

#include <linux/can/raw.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <net/if.h>
#include <errno.h>

hscan::hscan() : hscan("can0") {}

hscan::hscan(const char * interface) {
	// initialize the interface
	bcm_socket = socket(PF_CAN, SOCK_DGRAM, CAN_BCM);
	addr.can_family = AF_CAN;
	strcpy(ifr.ifr_name, interface);
	ioctl(bcm_socket, SIOCGIFINDEX, &ifr);
	addr.can_ifindex = ifr.ifr_ifindex;

	connect(bcm_socket, (struct sockaddr *)&addr, sizeof(addr));
}

hscan::~hscan() {

}

bool hscan::isUp() {
	return !!(ifr.ifr_flags & IFF_UP);
}

//void hscan::bringUp() {
//	ifr.ifr_flags |= IFF_UP;
//	ioctl(bcm_socket, SIOCSIFFLAGS, &ifr);
//}

int hscan::set_if_flags(char *ifname, short flags)
{
	 struct ifreq ifr;
	 int res = 0;

	 ifr.ifr_flags = flags;
	 strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

	 res = ioctl(bcm_socket, SIOCSIFFLAGS, &ifr);
	 if (res < 0) {
			 printf("Interface '%s': Error: SIOCSIFFLAGS failed: %s\n",
					 ifname, strerror(errno));
	 } else {
			 printf("Interface '%s': flags set to %04X.\n", ifname, flags);
	 }

	 return res;
}

int hscan::set_if_up(char *ifname, short flags)
{
	 return set_if_flags(ifname, flags | IFF_UP);
}

void hscan::add_message(bcm_message msg) {
	// set up the message
	msg.msg_head.flags = SETTIMER | STARTTIMER;
	write(bcm_socket, &msg, sizeof(msg));
}

bcm_message hscan::add_message(canid_t addr, long period, unsigned char len,
		unsigned char* data) {
	bcm_message msg;
	msg.msg_head.opcode = TX_SETUP;
	msg.msg_head.can_id = addr;
	msg.msg_head.flags = SETTIMER | STARTTIMER | TX_CP_CAN_ID;
	msg.msg_head.nframes = 1;
	msg.msg_head.count = 0;
	if (period > 1000) { // 1 second
			msg.msg_head.ival2.tv_sec = period / 1000;
			msg.msg_head.ival2.tv_usec = (period % 1000) * 1000;
	} else {
		msg.msg_head.ival2.tv_sec = 0;
		msg.msg_head.ival2.tv_usec = period * 1000;
	}
	msg.frame.can_dlc = len;
	for (int i = 0; i < len; i ++) {
		msg.frame.data[i]= data[i];
	}
	write(bcm_socket, &msg, sizeof(msg));
	return msg;
}

bcm_message hscan::add_message(can_frame frame, long period) {
	bcm_message msg;
	msg.msg_head.opcode = TX_SETUP;
	msg.msg_head.can_id = frame.can_id;
	msg.msg_head.flags = SETTIMER | STARTTIMER | TX_CP_CAN_ID;
	msg.msg_head.nframes = 1;
	msg.msg_head.count = 0;
	if (period > 1000) { // 1 second
		msg.msg_head.ival2.tv_sec = period / 1000;
		msg.msg_head.ival2.tv_usec = (period % 1000) * 1000;
	} else {
		msg.msg_head.ival2.tv_sec = 0;
		msg.msg_head.ival2.tv_usec = period * 1000;
	}
	msg.frame = frame;
	write(bcm_socket, &msg, sizeof(msg));
	return msg;
}

void hscan::update_message(bcm_message msg) {
	msg.msg_head.flags = 0;
	write(bcm_socket, &msg, sizeof(msg));
}
