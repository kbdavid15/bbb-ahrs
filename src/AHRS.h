/*
 * AHRS.h
 *
 *  Created on: Apr 10, 2016
 *      Author: kbdavid15
 */

#ifndef AHRS_H_
#define AHRS_H_

#include <iostream>

#include "MsgManager.h"
#include "sensor/ADXL345.h"
#include "sensor/DataPoint.h"
#include "sensor/HMC5883L.h"
#include "sensor/L3G4200D.h"

using namespace sensor;

class AHRS {
private:
	void initAccel();
	void initGyro();
	void initCompass();
	void initCAN();
public:
	ADXL345 accel;
	L3G4200D gyro;
	HMC5883L compass;
	ahrs::MsgManager msgManager;
	DataPoint accelData;
	DataPoint gyroData;
	DataPoint compassData;

	void init();
	void updateData();
	void printToFile(std::ofstream& file);
	void printLineToFile(std::ofstream& file);

	AHRS();
	virtual ~AHRS();
};

#endif /* AHRS_H_ */
