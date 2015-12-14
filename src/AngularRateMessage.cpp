/*
 * AngularRateMessage.cpp
 *
 *  Created on: Nov 21, 2015
 *      Author: kbdavid15
 */

#include "AngularRateMessage.h"

AngularRateMessage::AngularRateMessage() : TxMsg(50, ARBID_ANGULAR_RATE, 6) {
	init();
}

AngularRateMessage::AngularRateMessage(long period) : TxMsg(period, ARBID_ANGULAR_RATE, 6) {
	init();
}

void AngularRateMessage::init() {
	x_angular_rate = 0;
	y_angular_rate = 0;
	z_angular_rate = 0;
	updateFrameData();
}

AngularRateMessage::~AngularRateMessage() {
	// TODO Auto-generated destructor stub
}

void AngularRateMessage::updateFrame(DataPoint p) {
	x_angular_rate = p.getXf() / 0.01;
	y_angular_rate = p.getYf() / 0.01;
	z_angular_rate = p.getZf() / 0.01;
	updateFrameData();
}

void AngularRateMessage::updateFrameData() {
	msg.frame.data[0] = x_angular_rate >> 8;
	msg.frame.data[1] = x_angular_rate & 0xFF;
	msg.frame.data[2] = y_angular_rate >> 8;
	msg.frame.data[3] = y_angular_rate & 0xFF;
	msg.frame.data[4] = z_angular_rate >> 8;
	msg.frame.data[5] = z_angular_rate & 0xFF;
}
