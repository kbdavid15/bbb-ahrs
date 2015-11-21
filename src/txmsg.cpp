/*
 * txmsg.cpp
 *
 *  Created on: Nov 19, 2015
 *      Author: kbdavid15
 */

#include "txmsg.h"


txmsg::txmsg() {
	arbid = 0;
	period_ms = 1000;
	is_tx_on = false;
}

txmsg::txmsg(can_frame frame, uint32_t period) {
	this->frame = frame;
	this->period_ms = period;
	this->is_tx_on = false;
	this->arbid = frame.can_id;
}

txmsg::txmsg(canid_t addr1, unsigned char len, unsigned char* data, uint32_t period) {
	frame.can_id = addr1;
	frame.can_dlc = len;
	for (int i = 0; i < len; i ++) {
		frame.data[i]= data[i];
	}
	arbid = frame.can_id;
	is_tx_on = true;
	period_ms = period;
}
