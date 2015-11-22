/*
 * txmsg.h
 *
 *  Created on: Nov 19, 2015
 *      Author: kbdavid15
 */

#ifndef TXMSG_H_
#define TXMSG_H_

#define ARBID_VELOCITY				0x603;
#define ARBID_BODY_ACCELERATIONS	0x605;
#define ARBID_HEADING_PITCH_ROLL	0x607;
#define ARBID_ANGULAR_RATE			0x608;

#include <linux/can.h>
#include <stdint.h>
#include "linux/can/bcm.h"

struct bcm_message {
	struct bcm_msg_head msg_head;
	struct can_frame frame;
};

class txmsg {
private:
	struct bcm_message msg;
public:
	uint32_t  period_ms;
	txmsg();
	virtual ~txmsg();
//	txmsg(can_frame, uint32_t);
//	txmsg(canid_t addr1, unsigned char len, unsigned char * data, uint32_t period);
	virtual can_frame getFrame() = 0;
	bcm_message * get_bcm_msg() {
		return &msg;
	}
};





#endif /* TXMSG_H_ */
