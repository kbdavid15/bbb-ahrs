/*
 * MessageAngularRate.cpp
 *
 *  Created on: Nov 21, 2015
 *      Author: kbdavid15
 */

#include "MessageAngularRate.h"

MessageAngularRate::MessageAngularRate() : MessageAngularRate(50) {
}

MessageAngularRate::MessageAngularRate(long period) : TxMsg(period, ARBID_ANGULAR_RATE, 6) {
	x_angular_rate = 0;
	y_angular_rate = 0;
	z_angular_rate = 0;
	updateFrameData();
}

MessageAngularRate::~MessageAngularRate() {
	// TODO Auto-generated destructor stub
}

void MessageAngularRate::updateFrame(float x, float y, float z) {
	x_angular_rate = x / 0.01;
	y_angular_rate = y / 0.01;
	z_angular_rate = z / 0.01;
	updateFrameData();
}

void MessageAngularRate::updateFrameData() {
	msg.frame.data[0] = x_angular_rate >> 8;
	msg.frame.data[1] = x_angular_rate & 0xFF;
	msg.frame.data[2] = y_angular_rate >> 8;
	msg.frame.data[3] = y_angular_rate & 0xFF;
	msg.frame.data[4] = z_angular_rate >> 8;
	msg.frame.data[5] = z_angular_rate & 0xFF;
}
