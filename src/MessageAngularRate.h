/*
 * MessageAngularRate.h
 *
 *  Created on: Nov 21, 2015
 *      Author: kbdavid15
 */

#ifndef MESSAGEANGULARRATE_H_
#define MESSAGEANGULARRATE_H_

#include "DataPoint.h"
#include "can-utils/TxMsg.h"

class MessageAngularRate: virtual public TxMsg {
private:
	int16_t x_angular_rate;
	int16_t y_angular_rate;
	int16_t z_angular_rate;
	void updateFrameData();
public:
	MessageAngularRate();
	MessageAngularRate(long period);
	virtual ~MessageAngularRate();

	void updateFrame(DataPoint p);
};

#endif /* MESSAGEANGULARRATE_H_ */
