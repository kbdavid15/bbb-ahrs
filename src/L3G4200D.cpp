/*
 * L3G4200D.cpp
 *
 *  Created on: Jul 9, 2015
 *      Author: kbdavid15
 */

#include "L3G4200D.h"

#include <iostream>

using namespace std;

const double L3G4200D::DPS_CONV_VAL[] = { 0.00875, 0.0175, 0.07 };
const double L3G4200D::DATA_RATE_VAL[] = { 100, 200, 400, 800};

L3G4200D::L3G4200D():spi(BlackLib::SPI1_0, 8, BlackLib::SpiMode3, 2400000) {
	// set up control registers
	setControlReg4((CR4) { true, BigEndian, ST_Disabled, Scale250, SpiMode4Wire});
	setControlReg1((CR1) { ODR_100Hz, BW_1, true, true, true, true });
}

L3G4200D::~L3G4200D() {
	spi.close();
}

unsigned char L3G4200D::getDeviceID() {
	return spi.readByte(WHO_AM_I);
}

DataPoint L3G4200D::getSensorData() {
	setLastDataPoint(dataPoint);	// save last data point before updating
	uint8_t recvBytes[7];
	spi.readBytes(OUT_X_L, recvBytes, sizeof(recvBytes));
	if (_ControlReg4.ble == BigEndian) {
		dataPoint.setX(((recvBytes[2] << 8) | recvBytes[1]) - _xOffset);
		dataPoint.setY(((recvBytes[4] << 8) | recvBytes[3]) - _yOffset);
		dataPoint.setZ(((recvBytes[6] << 8) | recvBytes[5]) - _zOffset);
	} else {
		dataPoint.setX(((recvBytes[1] << 8) | recvBytes[2]) - _xOffset);
		dataPoint.setY(((recvBytes[3] << 8) | recvBytes[4]) - _yOffset);
		dataPoint.setZ(((recvBytes[5] << 8) | recvBytes[6]) - _zOffset);
	}
	return dataPoint;
}

void L3G4200D::setControlReg4(CR4 reg) {
	spi.writeByte(CTRL_REG4, reg.getData());
	_ControlReg4 = reg;
	// set the gyro format multiplier based on range
	dataPoint.setFormatMultiplier(DPS_CONV_VAL[_ControlReg4.scale]);
}
CR4 L3G4200D::getMeasurementRange() {
	uint8_t regVal = spi.readByte(CTRL_REG4);
	_ControlReg4.update(regVal);
	return _ControlReg4;
}
void L3G4200D::setControlReg1(CR1 reg) {
	spi.writeByte(CTRL_REG1, reg.getData());
	_ControlReg1 = reg;
}
CR1 L3G4200D::getControlReg1() {
	uint8_t regVal = spi.readByte(CTRL_REG1);
	_ControlReg1.update(regVal);
	return _ControlReg1;
}
unsigned char L3G4200D::getTemperature() {
	return spi.readByte(OUT_TEMP);
}
void L3G4200D::calculateOffset() {
	// take 100 data points and average them
	long sleepTime = getWaitTime(); 	// gets time to wait between reads depending on the output data rate
	timespec sleepTimeSpec = { 0, sleepTime };
	int numPoints = 100;
	DataPoint points[numPoints];
	for (int i = 0; i < numPoints; i++) {
		points[i] = getSensorData();
		nanosleep(&sleepTimeSpec, NULL);
	}
	DataPoint avg = DataPoint::average(points, numPoints);
	this->_xOffset = avg.getX();
	this->_yOffset = avg.getY();
	this->_zOffset = avg.getZ();
}
long L3G4200D::getWaitTime() {
	double time = (1 / DATA_RATE_VAL[_ControlReg1.rate]) * 1000000000;
	return (long)(time+0.5);
}
