/*
 * MsgManager.h
 *
 *  Created on: Apr 10, 2016
 *      Author: kbdavid15
 */

#ifndef MSGMANAGER_H_
#define MSGMANAGER_H_

#include "msg/can-utils/hscan.h"
#include "msg/MessageAngularRate.h"
#include "msg/MessageBodyAccel.h"
#include "msg/MessageHeadingPitchRoll.h"
#include "sensor/DataPoint.h"

namespace ahrs {

class MsgManager {
private:
	MessageBodyAccel body;
	MessageAngularRate ang_rate;
	MessageHeadingPitchRoll hprmsg;
public:
	hscan can;
	void updateAccel(DataPoint p);
	void updateAngularRate(DataPoint p);
	void updateHeading(float heading, float pitch, float roll);
	void beginMessages();
	MsgManager();
	virtual ~MsgManager();
};

} /* namespace ahrs */

#endif /* MSGMANAGER_H_ */
