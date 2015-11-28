/*
 * HeadingPitchRollMessage.h
 *
 *  Created on: Nov 27, 2015
 *      Author: kbdavid15
 */

#ifndef CAN_UTILS_HEADINGPITCHROLLMESSAGE_H_
#define CAN_UTILS_HEADINGPITCHROLLMESSAGE_H_

#include "txmsg.h"

class HeadingPitchRollMessage: public txmsg {
private:
	uint16_t heading;
	int16_t pitch;
	int16_t roll;
public:
	HeadingPitchRollMessage();
	HeadingPitchRollMessage(long period);
	virtual ~HeadingPitchRollMessage();
	virtual void updateFrameData();
	void init(long period_ms);
	void updateFrame(float heading, float pitch, float roll);
};

#endif /* CAN_UTILS_HEADINGPITCHROLLMESSAGE_H_ */
