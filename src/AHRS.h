/*
 * AHRS.h
 *
 *  Created on: Apr 10, 2016
 *      Author: kbdavid15
 */

#ifndef AHRS_H_
#define AHRS_H_

#include "sensor/ADXL345.h"
#include "sensor/L3G4200D.h"
#include "sensor/HMC5883L.h"
#include "can-utils/hscan.h"

using namespace sensor;

class AHRS {
private:
	void initAccel();
	void initGyro();
	void initCompass();
public:
	ADXL345 accel;
	L3G4200D gyro;
	HMC5883L compass;
	hscan can;

	void init();

	AHRS();
	virtual ~AHRS();
};

#endif /* AHRS_H_ */
