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
protected:
	struct bcm_message msg;
public:
	uint32_t  period_ms;
	txmsg();
	virtual ~txmsg();
	virtual can_frame getFrame() = 0;

	const struct bcm_message& getMsg() const {
		return msg;
	}

	void setMsg(const struct bcm_message& msg) {
		this->msg = msg;
	}
};





#endif /* TXMSG_H_ */
