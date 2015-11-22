/*
 * BodyAccelMessage.cpp
 *
 *  Created on: Nov 21, 2015
 *      Author: kbdavid15
 */

#include "BodyAccelMessage.h"

BodyAccelMessage::BodyAccelMessage() {
	period_ms = 50;
	init(period_ms);
}

BodyAccelMessage::BodyAccelMessage(long period) {
	period_ms = period;
	init(period_ms);
}

void BodyAccelMessage::init(long period) {
	x_acceleration = 0;
	y_acceleration = 0;
	z_acceleration = 0;
	msg.msg_head.opcode = TX_SETUP;
	msg.msg_head.can_id = ARBID_BODY_ACCELERATIONS;
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
	msg.frame = getFrame();
}

BodyAccelMessage::~BodyAccelMessage() {
	// TODO Auto-generated destructor stub
}

void BodyAccelMessage::updateFrame(DataPoint p) {
	x_acceleration = p.getXf() / 0.01;
	y_acceleration = p.getYf() / 0.01;
	z_acceleration = p.getZf() / 0.01;
	msg.frame.data[0] = x_acceleration >> 8;
	msg.frame.data[1] = x_acceleration & 0xFF;
	msg.frame.data[2] = y_acceleration >> 8;
	msg.frame.data[3] = y_acceleration & 0xFF;
	msg.frame.data[4] = z_acceleration >> 8;
	msg.frame.data[5] = z_acceleration & 0xFF;
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
