/*
 * BodyAccelMessage.h
 *
 *  Created on: Nov 21, 2015
 *      Author: kbdavid15
 */

#ifndef CAN_UTILS_BODYACCELMESSAGE_H_
#define CAN_UTILS_BODYACCELMESSAGE_H_

#include "txmsg.h"

class BodyAccelMessage: public txmsg {
public:
	int16_t x_acceleration;
	int16_t y_acceleration;
	int16_t z_acceleration;
	BodyAccelMessage();
	virtual ~BodyAccelMessage();
	virtual can_frame getFrame();
};

#endif /* CAN_UTILS_BODYACCELMESSAGE_H_ */
