/*
 * BodyAccelMessage.cpp
 *
 *  Created on: Nov 21, 2015
 *      Author: kbdavid15
 */

#include "BodyAccelMessage.h"

BodyAccelMessage::BodyAccelMessage() {
	period_ms = 50;
	x_acceleration = 0;
	y_acceleration = 0;
	z_acceleration = 0;
}

BodyAccelMessage::~BodyAccelMessage() {
	// TODO Auto-generated destructor stub
}

can_frame BodyAccelMessage::getFrame() {
	struct can_frame frame;
	frame.can_dlc = 6;
	frame.can_id  = ARBID_BODY_ACCELERATIONS;
	frame.data[0] = x_acceleration >> 8;
	frame.data[1] = x_acceleration & 0xFF;
	frame.data[2] = y_acceleration >> 8;
	frame.data[3] = y_acceleration & 0xFF;
	frame.data[4] = z_acceleration >> 8;
	frame.data[5] = z_acceleration & 0xFF;
	return frame;
}
