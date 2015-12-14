/*
 * MessageHeadingPitchRoll.h
 *
 *  Created on: Nov 27, 2015
 *      Author: kbdavid15
 */

#ifndef MESSAGEHEADINGPITCHROLL_H_
#define MESSAGEHEADINGPITCHROLL_H_

#include "can-utils/TxMsg.h"

class MessageHeadingPitchRoll: public TxMsg {
private:
	uint16_t heading;
	int16_t pitch;
	int16_t roll;

	void updateFrameData();

public:
	MessageHeadingPitchRoll();
	MessageHeadingPitchRoll(long period);
	virtual ~MessageHeadingPitchRoll();

	void updateFrame(float heading, float pitch, float roll);
};

#endif /* MESSAGEHEADINGPITCHROLL_H_ */
