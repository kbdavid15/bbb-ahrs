/*
 * MessageBodyAccel.h
 *
 *  Created on: Nov 21, 2015
 *      Author: kbdavid15
 */

#ifndef MESSAGEBODYACCEL_H_
#define MESSAGEBODYACCEL_H_

#include "sensor/DataPoint.h"
#include "can-utils/TxMsg.h"

class MessageBodyAccel: public TxMsg {
private:
	int16_t x_acceleration;
	int16_t y_acceleration;
	int16_t z_acceleration;
	void updateFrameData();
public:
	MessageBodyAccel();
	MessageBodyAccel(long period_ms);
	virtual ~MessageBodyAccel();

	void updateFrame(DataPoint p);
};

#endif /* MESSAGEBODYACCEL_H_ */
