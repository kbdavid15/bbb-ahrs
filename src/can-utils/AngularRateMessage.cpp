/*
 * AngularRateMessage.cpp
 *
 *  Created on: Nov 21, 2015
 *      Author: kbdavid15
 */

#include "AngularRateMessage.h"

AngularRateMessage::AngularRateMessage() {
	period_ms = 50;
	init(period_ms);
}

AngularRateMessage::AngularRateMessage(long period) {
	period_ms = period;
	init(period_ms);
}

void AngularRateMessage::init(long period) {
	x_angular_rate = 0;
	y_angular_rate = 0;
	z_angular_rate = 0;
	msg.msg_head.opcode = TX_SETUP;
	msg.msg_head.can_id = ARBID_ANGULAR_RATE;
	msg.msg_head.nframes = 1;
	msg.msg_head.count = 0;
	msg.msg_head.ival1.tv_sec = 0;
	msg.msg_head.ival1.tv_usec = 0;
	if (period > 1000) { // 1 second
		msg.msg_head.ival2.tv_sec = period / 1000;
		msg.msg_head.ival2.tv_usec = (period % 1000) * 1000;
	} else {
		msg.msg_head.ival2.tv_sec = 0;
		msg.msg_head.ival2.tv_usec = period * 1000;
	}
	msg.frame.can_dlc = 6;
	msg.frame.can_id  = ARBID_ANGULAR_RATE;
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
