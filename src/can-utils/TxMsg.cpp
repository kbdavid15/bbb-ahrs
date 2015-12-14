/*
 * txmsg.cpp
 *
 *  Created on: Nov 19, 2015
 *      Author: kbdavid15
 */

#include "TxMsg.h"


TxMsg::TxMsg() {
	//period_ms = 1000;
}
TxMsg::TxMsg(uint32_t period, canid_t arbid, uint8_t dlc) {
	msg.msg_head.opcode = TX_SETUP;
	msg.msg_head.can_id = arbid;
	msg.msg_head.nframes = 1;
	msg.msg_head.count = 0;
	msg.msg_head.ival1.tv_sec = 0;
	msg.msg_head.ival1.tv_usec = 0;
	// use second timer in addition to microsecond timer if period is too long
	if (period > 1000) {
		msg.msg_head.ival2.tv_sec = period / 1000;
		msg.msg_head.ival2.tv_usec = (period % 1000) * 1000;
	} else {
		msg.msg_head.ival2.tv_sec = 0;
		msg.msg_head.ival2.tv_usec = period * 1000;
	}
	msg.frame.can_dlc = dlc;
	msg.frame.can_id  = arbid;
}
TxMsg::~TxMsg() {

}
