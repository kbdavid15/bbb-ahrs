/*
 * AHRS.cpp
 *
 *  Created on: Apr 10, 2016
 *      Author: kbdavid15
 */

#include "AHRS.h"

#include <cmath>
#include <ctime>
#include <fstream>
#include <string>
#include <sys/time.h>

#include "msg/can-utils/hscan.h"
#include "msg/can-utils/TxMsg.h"
#include "sensor/Sensor.h"
#include "sensor/DoublePoint.h"

extern "C" {
	#include "MadgwickAHRS/MadgwickAHRS.h"
}

const char * bbb_ahrs_id = "BBB-AHRS";

AHRS::AHRS() {
}
AHRS::~AHRS() {
}

void AHRS::initCAN() {
	msgManager.can.add_message(0x513, 5000, 8, (unsigned char *)bbb_ahrs_id);
	msgManager.beginMessages();
}

void AHRS::updateData() {
	accelData = accel.getSensorData();
	gyroData = gyro.getSensorData();
	compassData = compass.getSensorData();

	// update MadgwickAHRS
	MadgwickAHRSupdate(gyroData.getXf()* (PI/180), gyroData.getYf()* (PI/180), gyroData.getZf()* (PI/180),
			accelData.getXf(), accelData.getYf(), accelData.getZf(),
			compassData.getXf(), compassData.getYf(), compassData.getZf());

	float madHeading = atan2(2*q2*q3 - 2*q1*q4, 2*q1*q1 + 2*q2*q2 - 1);
	float madRoll = -asin(2*q2*q4 + 2*q1*q3);
	float madPitch = atan2(2*q3*q4 - 2*q1*q2, 2*q1*q1 + 2*q4*q4 - 1);	// unsure why calculation is off by 180 degrees

	msgManager.updateAccel(accelData);
	msgManager.updateAngularRate(gyroData);
	msgManager.updateHeading(madHeading*(180/PI), madPitch*(180/PI), madRoll*(180/PI));
}

void AHRS::initAccel() {
	//	accel.startSelfTest();
	accel.resetOffset();
	accel.calibrateOffset();	// issue with z axis calibration (COMPOUNDING CALIBRATIONS, NEED TO RESET BEFORE CALCULATING)

	DataFormat format;
	format.fullRes = 1;
	format.range = DataRange2g;
	format.justify = 0;
	accel.setDataFormat(format);	// value of 0x0B sets full resolution mode and range to +/- 16g
	accel.setPowerCtrl(0x08);		// enable measurement mode
	accel.setInterruptEnable(0x00);	// disables interrupts
	PwrDataRate odr(false, ODR_100); // set data rate to 100Hz
	accel.setDataRate(odr);
	accel.setLPF(0.5);

	// wait 1.1ms + 1/ODR
	struct timespec waitTime = accel.getInitWaitTime();
	nanosleep(&waitTime, NULL);
}

void AHRS::initGyro() {
	gyro.calculateOffset();
}

void AHRS::initCompass() {
	compass.setModeRegister(ContinuousMeasurement);
	compass.setConfigRegA((ConfigRegA){ AvgSamples1, DataRate_75, NormalMode });
//	compass.setConfigRegB(GAIN_0);
}

void AHRS::init() {
	initCompass();
	initGyro();
	initAccel();
	initCAN();
}

void AHRS::printToFile(std::ofstream& oFile) {
	if (oFile.is_open()) {
		oFile << accelData.toFile(false, ',') << ",";
		oFile << gyroData.toFile(false, ',') << ",";
		oFile << compassData.toFile(false, ',') << ",";
	}
}

void AHRS::printLineToFile(std::ofstream& oFile) {
	if (oFile.is_open()) {
		printToFile(oFile);
		oFile << std::endl;
	}
}
