/*
 * ADXL345.cpp
 *
 *  Created on: Jul 9, 2015
 *      Author: kbdavid15
 */

#include "ADXL345.h"

#include <iostream>
#include <math.h>
#include "BlackLib/BlackSPI/BlackSPI.h"

namespace sensor {

using namespace std;

#ifndef PI
#define PI				3.14159265359
#endif
#define G_CONVERSION	0.101972

const double ADXL345::DATA_RATE_VAL[] = { 6.25, 12.5, 25, 50, 100, 200, 400, 800, 1600, 3200 };

ADXL345::ADXL345():spi(BlackLib::SPI0_0, 8, BlackLib::SpiMode3, 2400000) {
	dataRate = 100;	// default value
	formatMult = 1;
}
ADXL345::~ADXL345() {
	spi.close();
}

unsigned char ADXL345::getDeviceID() {
	return spi.readByte(DEVICE_ID);
}
unsigned char ADXL345::getThresholdTap() {
	return spi.readByte(THRESH_TAP);
}

void ADXL345::setDataFormat(DataFormat format) {
	spi.writeByte(DATA_FORMAT, format.getData());
	// set the scale format multiplier
	if (format.fullRes) {
		formatMult = 3.9/1000;
	} else {
		switch (format.range) {
		case DataRange2g:
			formatMult = 4.0/1024;
			break;
		case DataRange4g:
			formatMult = 8.0/1024;
			break;
		case DataRange8g:
			formatMult = 16.0/1024;
			break;
		case DataRange16g:
			formatMult = 32.0/1024;
			break;
		}
	}
	dataPoint.setFormatMultiplier(formatMult);
}
DataFormat ADXL345::getDataFormat() {
	uint8_t regData = spi.readByte(DATA_FORMAT);
	return DataFormat(regData);
}
void ADXL345::setPowerCtrl(unsigned char pwr) {
	spi.writeByte(POWER_CTL, pwr);
}
void ADXL345::setInterruptEnable(unsigned char interrupt) {
	spi.writeByte(INT_ENABLE, interrupt);
}
PwrDataRate ADXL345::getDataRate() {
	uint8_t regVal = spi.readByte(BW_RATE);
	PwrDataRate pdr(regVal);
	return pdr;
}
void ADXL345::setDataRate(PwrDataRate pdr) {
	spi.writeByte(BW_RATE, pdr.getByteFormat());
	dataRate = DATA_RATE_VAL[pdr.dataRate - DATA_RATE_OFFSET];
}
bool ADXL345::startSelfTest() {
	DataFormat data;
	data.fullRes = 0;
	data.range = DataRange16g;
	data.selfTest = 0;
	setDataFormat(data);
	setDataRate((PwrDataRate) { false, ODR_800 });
	setPowerCtrl(MeasureOn);
	waitTime(true);	// waits 11.1 ms for data rate of 100Hz
	AvgData avgNominal = averageDataPoints(100);
	data.selfTest = 1;
	setDataFormat(data);
	waitTime(true);	// waits 11.1 ms for data rate of 100Hz
	AvgData avgSelfTest = averageDataPoints(100);

	AvgData diff = avgSelfTest-avgNominal;
	cout << diff.toString() << endl;

	// compare against self test limits
	if (diff.x > SELF_TEST_MAX_X_16 || diff.x < SELF_TEST_MIN_X_16 ||
		diff.y > SELF_TEST_MAX_Y_16 || diff.y < SELF_TEST_MIN_Y_16 ||
		diff.z > SELF_TEST_MAX_Z_16 || diff.z < SELF_TEST_MIN_Z_16) {
		cerr << "Self test failed" << endl;
		return false;
	} else {
		cout << "Self test passed" << endl;
		return true;
	}
}
void ADXL345::calibrateOffset() {
	DataFormat format(true, DataRange16g);
	setDataFormat(format);
	setPowerCtrl(MeasureOn);
	setInterruptEnable(0x80);
	setDataRate((PwrDataRate) { false, ODR_100 });
	waitTime(true);	// wait for ODR settling

	AvgData avgd = averageDataPoints(100);

	int8_t sxCal = -(avgd.x / 4);
	int8_t syCal = -(avgd.y / 4);
	int8_t szCal = -((-avgd.z - 256) / 4);	// app note assumes that sensor is in Z=+1g orientation (it's in -1g)

	uint8_t calib[3];
	calib[0] = static_cast<unsigned char>(sxCal);
	calib[1] = static_cast<unsigned char>(syCal);
	calib[2] = static_cast<unsigned char>(szCal);
	setAxisOffset(calib);
}
AvgData ADXL345::averageDataPoints(uint8_t numPoints) {
	long total_x = 0, total_y = 0, total_z = 0;
	long sleepTimeL= getWaitTime(false); 	// gets time to wait between reads depending on the output data rate
	timespec sleepTimeSpec = { 0, sleepTimeL };

	for (uint8_t i = 0; i < numPoints; i++) {
		getSensorData();
		total_x += dataPoint.getX();
		total_y += dataPoint.getY();
		total_z += dataPoint.getZ();
		nanosleep(&sleepTimeSpec, NULL);
	}
	AvgData avg;
	avg.x = total_x / (double) numPoints;
	avg.y = total_y / (double) numPoints;
	avg.z = total_z / (double) numPoints;
	return avg;
}
/**
 * Calculate the amount of time needed to wait after writing to configuration registers
 *
 * @return	Amount of time to wait in nanoseconds
 */
timespec ADXL345::getInitWaitTime() {
	double time = 1100000 + (1 / dataRate) * 1000000000;
	long timeL = (long)(time+0.5);
	struct timespec waitTime = { 0, timeL };
	return waitTime;
}
void ADXL345::waitTime(bool initOffset) {
	double time = (1 / dataRate) * 1000000000;
	if (initOffset) {
		time += 1100000;
	}
	long timeL = (long)(time+0.5);
	timespec waitTime = { 0, timeL };
	nanosleep(&waitTime, NULL);
}
long ADXL345::getWaitTime(bool initOffset) {
	double time = (1 / dataRate) * 1000000000;
	if (initOffset) {
		time += 1100000;
	}
	return (long)(time+0.5);
}
void ADXL345::setAxisOffset(unsigned char *offset) {
	spi.writeBytes(OFFSET_X, offset, 3);
}
void ADXL345::resetOffset() {
	uint8_t offset[] = { 0, 0, 0};
	spi.writeBytes(OFFSET_X, offset, 3);
}
DataPoint ADXL345::getSensorData() {
	setLastDataPoint(dataPoint);
	uint8_t recvBytes[7];
	spi.readBytes(DATAX0, recvBytes, sizeof(recvBytes));
	int16_t x, y, z;
	x = (int)(recvBytes[2] << 8) | (int)recvBytes[1];
	y = (int)(recvBytes[4] << 8) | (int)recvBytes[3];
	z = -((int)(recvBytes[6] << 8) | (int)recvBytes[5]);
	return dataPoint = DataPoint(x, y, z, formatMult);
}
// radians
double ADXL345::getPitch() {
	double x = dataPoint.getXf();
	double y = dataPoint.getYf();
	double z = dataPoint.getZf();
	return atan2(x,sqrt(y*y + z*z));
}
// radians
double ADXL345::getRoll() {
//	double x = dataPoint.getXf();
	double y = dataPoint.getYf();
	double z = dataPoint.getZf();
	return atan2(y, y*y + z*z);
}
};	/* namespace sensor */
