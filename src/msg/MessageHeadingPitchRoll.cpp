/*
 * HeadingPitchRollMessage.cpp
 *
 *  Created on: Nov 27, 2015
 *      Author: kbdavid15
 */

#include "MessageHeadingPitchRoll.h"

MessageHeadingPitchRoll::MessageHeadingPitchRoll() : MessageHeadingPitchRoll(50) {
}

MessageHeadingPitchRoll::MessageHeadingPitchRoll(long period) : TxMsg(period, ARBID_HEADING_PITCH_ROLL, 6) {
	heading = 0;
	pitch = 0;
	roll = 0;
	updateFrameData();
}

MessageHeadingPitchRoll::~MessageHeadingPitchRoll() {
	// TODO Auto-generated destructor stub
}

void MessageHeadingPitchRoll::updateFrame(float heading, float pitch, float roll) {
	this->heading = heading / 0.01;
	this->pitch = pitch / 0.01;
	this->roll = roll / 0.01;
	updateFrameData();
}

void MessageHeadingPitchRoll::updateFrameData() {
	msg.frame.data[0] = heading >> 8;
	msg.frame.data[1] = heading & 0xFF;
	msg.frame.data[2] = pitch >> 8;
	msg.frame.data[3] = pitch & 0xFF;
	msg.frame.data[4] = roll >> 8;
	msg.frame.data[5] = roll & 0xFF;
}
