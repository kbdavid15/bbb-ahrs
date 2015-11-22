/*
 * hscan.cpp
 *
 *  Created on: Nov 19, 2015
 *      Author: kbdavid15
 */

#include "hscan.h"

#include <linux/can/raw.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>

int can::sendframe(canid_t addr1, unsigned char len, unsigned char * data) {
	int s; /* can raw socket */
	int required_mtu = CAN_MTU;
	struct sockaddr_can addr;
	struct canfd_frame frame;
	struct ifreq ifr;

	frame.can_id = addr1;
	frame.len = len;
	for (int i = 0; i < len; i ++) {
		frame.data[i]= data[i];
	}

	/* open socket */
	if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("socket");
		return 1;
	}

	strncpy(ifr.ifr_name, "can0", IFNAMSIZ - 1);
	ifr.ifr_name[IFNAMSIZ - 1] = '\0';
	ifr.ifr_ifindex = if_nametoindex(ifr.ifr_name);
	if (!ifr.ifr_ifindex) {
		perror("if_nametoindex");
		return 1;
	}

	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	/* disable default receive filter on this RAW socket */
	/* This is obsolete as we do not read from the socket at all, but for */
	/* this reason we can remove the receive list in the Kernel to save a */
	/* little (really a very little!) CPU usage.                          */
	setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);

	if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("bind");
		return 1;
	}

	/* send frame */
	if (write(s, &frame, required_mtu) != required_mtu) {
		perror("write");
		return 1;
	}

	close(s);

	return 0;
}

can::can() : can("can0") {}

can::can(const char * interface) {
	// initialize the interface
	bcm_socket = socket(PF_CAN, SOCK_DGRAM, CAN_BCM);
	addr.can_family = AF_CAN;
	strcpy(ifr.ifr_name, interface);
	ioctl(bcm_socket, SIOCGIFINDEX, &ifr);
	addr.can_ifindex = ifr.ifr_ifindex;

	connect(bcm_socket, (struct sockaddr *)&addr, sizeof(addr));
}

bcm_message can::add_message(txmsg * message) {
	// set up the message
	bcm_message * msg = message->get_bcm_msg();
	msg->frame = message->getFrame();
	msg->msg_head.opcode = TX_SETUP;
	msg->msg_head.can_id = msg->frame.can_id;
	msg->msg_head.flags = SETTIMER | STARTTIMER | TX_CP_CAN_ID;
	msg->msg_head.nframes = 1;
	msg->msg_head.count = 0;
	msg->msg_head.ival1.tv_sec = 0;
	msg->msg_head.ival1.tv_usec = 0;
	msg->msg_head.ival2.tv_sec = 0;
	msg->msg_head.ival2.tv_usec = message->period_ms * 1000;

	write(bcm_socket, &msg, sizeof(msg));
	return * msg;
}

bcm_message can::add_message(canid_t addr, long period, unsigned char len,
		unsigned char* data) {
	bcm_message msg;
	msg.msg_head.opcode = TX_SETUP;
	msg.msg_head.can_id = addr;
	msg.msg_head.flags = SETTIMER | STARTTIMER | TX_CP_CAN_ID;
	msg.msg_head.nframes = 1;
	msg.msg_head.count = 0;
	msg.msg_head.ival1.tv_sec = 0;
	msg.msg_head.ival1.tv_usec = 0;
	msg.msg_head.ival2.tv_sec = 0;
	msg.msg_head.ival2.tv_usec = period * 1000;
	msg.frame.can_dlc = len;

	for (int i = 0; i < len; i ++) {
		msg.frame.data[i]= data[i];
	}

	write(bcm_socket, &msg, sizeof(msg));
	return msg;
}

bcm_message can::add_message(can_frame frame, long period) {
	bcm_message msg;
	msg.msg_head.opcode = TX_SETUP;
	msg.msg_head.can_id = frame.can_id;
	msg.msg_head.flags = SETTIMER | STARTTIMER | TX_CP_CAN_ID;
	msg.msg_head.nframes = 1;
	msg.msg_head.count = 0;
	msg.msg_head.ival1.tv_sec = 0;
	msg.msg_head.ival1.tv_usec = 0;
	msg.msg_head.ival2.tv_sec = 0;
	msg.msg_head.ival2.tv_usec = period * 1000;
	msg.frame = frame;
	write(bcm_socket, &msg, sizeof(msg));
	return msg;
}

void can::update_message(bcm_message msg) {
	msg.msg_head.flags &= ~(SETTIMER | STARTTIMER);
	write(bcm_socket, &msg, sizeof(msg));
}

void can::update_message(txmsg * msg) {
	msg->get_bcm_msg()->msg_head.flags = 0;
	write(bcm_socket, &msg, sizeof(msg));
}
