/*
 * HeadingPitchRollMessage.cpp
 *
 *  Created on: Nov 27, 2015
 *      Author: kbdavid15
 */

#include "HeadingPitchRollMessage.h"

HeadingPitchRollMessage::HeadingPitchRollMessage() {
	period_ms = 50;
	init(period_ms);
}

HeadingPitchRollMessage::HeadingPitchRollMessage(long period) {
	period_ms = period;
	init(period_ms);
}

HeadingPitchRollMessage::~HeadingPitchRollMessage() {
	// TODO Auto-generated destructor stub
}

void HeadingPitchRollMessage::init(long period) {
	heading = 0;
	pitch = 0;
	roll = 0;
	msg.msg_head.opcode = TX_SETUP;
	msg.msg_head.can_id = ARBID_HEADING_PITCH_ROLL;
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
	msg.frame.can_id  = ARBID_HEADING_PITCH_ROLL;
	updateFrameData();
}

void HeadingPitchRollMessage::updateFrame(float heading, float pitch, float roll) {
	this->heading = heading / 0.01;
	this->pitch = pitch / 0.01;
	this->roll = roll / 0.01;
	updateFrameData();
}

void HeadingPitchRollMessage::updateFrameData() {
	msg.frame.data[0] = heading >> 8;
	msg.frame.data[1] = heading & 0xFF;
	msg.frame.data[2] = pitch >> 8;
	msg.frame.data[3] = pitch & 0xFF;
	msg.frame.data[4] = roll >> 8;
	msg.frame.data[5] = roll & 0xFF;
}
