/*
 * AngularRateMessage.h
 *
 *  Created on: Nov 21, 2015
 *      Author: kbdavid15
 */

#ifndef CAN_UTILS_ANGULARRATEMESSAGE_H_
#define CAN_UTILS_ANGULARRATEMESSAGE_H_

#include "../DataPoint.h"
#include "TxMsg.h"

class AngularRateMessage: virtual public TxMsg {
private:
	int16_t x_angular_rate;
	int16_t y_angular_rate;
	int16_t z_angular_rate;
public:
	AngularRateMessage();
	AngularRateMessage(long period);
	virtual ~AngularRateMessage();
	virtual void updateFrameData();
	void init();
	void updateFrame(DataPoint p);
};

#endif /* CAN_UTILS_ANGULARRATEMESSAGE_H_ */
