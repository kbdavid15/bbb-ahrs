/*
 * AHRS.cpp
 *
 *  Created on: Apr 10, 2016
 *      Author: kbdavid15
 */

#include "AHRS.h"

AHRS::AHRS() {

}

AHRS::~AHRS() {

}

void AHRS::initAccel() {
	//	accel.startSelfTest();
	accel.resetOffset();
	//accel.calibrateOffset();	issue with z axis calibration (COMPOUNDING CALIBRATIONS, NEED TO RESET BEFORE CALCULATING)

	DataFormat format;
	format.fullRes = 1;
	format.range = DataRange2g;
	format.justify = 0;
	accel.setDataFormat(format);	// value of 0x0B sets full resolution mode and range to +/- 16g
	accel.setPowerCtrl(0x08);		// value of 0x08 enables measurement mode
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
}
