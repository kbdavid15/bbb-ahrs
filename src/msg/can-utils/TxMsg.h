/*
 * txmsg.h
 *
 *  Created on: Nov 19, 2015
 *      Author: kbdavid15
 */

#ifndef TXMSG_H_
#define TXMSG_H_

const int ARBID_VELOCITY 		   = 0x603;
const int ARBID_BODY_ACCELERATIONS = 0x605;
const int ARBID_HEADING_PITCH_ROLL = 0x607;
const int ARBID_ANGULAR_RATE	   = 0x608;

#include <linux/can.h>
#include <stdint.h>
#include "../../msg/can-utils/linux/can/bcm.h"

struct bcm_message {
	struct bcm_msg_head msg_head;
	struct can_frame frame;
};

class TxMsg {
protected:
	struct bcm_message msg;
public:
	TxMsg(uint32_t period, canid_t arbid, uint8_t dlc);
	virtual ~TxMsg();

	const struct bcm_message& getMsg() const {
		return msg;
	}

	void setMsg(const struct bcm_message& msg) {
		this->msg = msg;
	}
};

#endif /* TXMSG_H_ */
