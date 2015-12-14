/*
 * BodyAccelMessage.h
 *
 *  Created on: Nov 21, 2015
 *      Author: kbdavid15
 */

#ifndef CAN_UTILS_BODYACCELMESSAGE_H_
#define CAN_UTILS_BODYACCELMESSAGE_H_

#include "DataPoint.h"
#include "can-utils/TxMsg.h"

class BodyAccelMessage: virtual public TxMsg {
private:
	int16_t x_acceleration;
	int16_t y_acceleration;
	int16_t z_acceleration;
public:
	BodyAccelMessage();
	BodyAccelMessage(long period_ms);
	virtual ~BodyAccelMessage();
	virtual void updateFrameData();
	void init();
	void updateFrame(DataPoint p);
};

#endif /* CAN_UTILS_BODYACCELMESSAGE_H_ */
