/*
 * MsgManager.cpp
 *
 *  Created on: Apr 10, 2016
 *      Author: kbdavid15
 */

#include "MsgManager.h"
#include <iostream>
namespace ahrs {

MsgManager::MsgManager() {
	if (!can.isUp()) {
		std::cout << "Interface is down" << std::endl;
		can.set_if_up("can0", 0);
	}
}

MsgManager::~MsgManager() {
	// TODO Auto-generated destructor stub
}

void MsgManager::beginMessages() {
	can.add_message(body.getMsg());
	can.add_message(ang_rate.getMsg());
	can.add_message(hprmsg.getMsg());
}

void MsgManager::updateAccel(DataPoint p) {
	body.updateFrame(p.getXf(), p.getYf(), p.getZf());
	can.update_message(body.getMsg());
}

void MsgManager::updateAngularRate(DataPoint p) {
	ang_rate.updateFrame(p.getXf(), p.getYf(), p.getZf());
	can.update_message(ang_rate.getMsg());
}

void MsgManager::updateHeading(float heading, float pitch, float roll) {
	hprmsg.updateFrame(heading, pitch, roll);
	can.update_message(hprmsg.getMsg());
}

} /* namespace ahrs */
