/*
 * can.cpp
 *
 *  Created on: Nov 19, 2015
 *      Author: kbdavid15
 */


#include <hscan.h>

//can::can() {
//
//}

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
