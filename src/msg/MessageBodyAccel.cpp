/*
 * BodyAccelMessage.cpp
 *
 *  Created on: Nov 21, 2015
 *      Author: kbdavid15
 */

#include "MessageBodyAccel.h"

MessageBodyAccel::MessageBodyAccel() : MessageBodyAccel(50) {
}

MessageBodyAccel::MessageBodyAccel(long period) : TxMsg(period, ARBID_BODY_ACCELERATIONS, 6)  {
	x_acceleration = 0;
	y_acceleration = 0;
	z_acceleration = 0;
	updateFrameData();
}

MessageBodyAccel::~MessageBodyAccel() {
	// TODO Auto-generated destructor stub
}

void MessageBodyAccel::updateFrame(float x, float y, float z) {
	x_acceleration = x / 0.01;
	y_acceleration = y / 0.01;
	z_acceleration = z / 0.01;
	updateFrameData();
}

void MessageBodyAccel::updateFrameData() {
	msg.frame.data[0] = x_acceleration >> 8;
	msg.frame.data[1] = x_acceleration & 0xFF;
	msg.frame.data[2] = y_acceleration >> 8;
	msg.frame.data[3] = y_acceleration & 0xFF;
	msg.frame.data[4] = z_acceleration >> 8;
	msg.frame.data[5] = z_acceleration & 0xFF;
}
