/*
 * MessageBodyAccel.h
 *
 *  Created on: Nov 21, 2015
 *      Author: kbdavid15
 */

#ifndef MSG_MESSAGEBODYACCEL_H_
#define MSG_MESSAGEBODYACCEL_H_

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

	void updateFrame(float x, float y, float z);
};

#endif /* MSG_MESSAGEBODYACCEL_H_ */
