/*
 * HMC5883L.cpp
 *
 *  Created on: Jul 9, 2015
 *      Author: kbdavid15
 */

#include "../include/HMC5883L.h"
#include <iostream>

using namespace std;

const uint16_t HMC5883L::LSB_PER_GAUSS[] = { 1370, 1090, 820, 660, 440, 390, 330, 230 };

HMC5883L::HMC5883L():device(HMC_DEVICE_ADDRESS) {
	_gain = getConfigRegB();
	//_gain.updateIdx(GAIN_1);
//	if (!runSelfTest()) {
//		cout << "Error: self test failed" << endl;
//
//	} else {
//		cout << "Self test passed" << endl;
//	}
}
HMC5883L::HMC5883L(GainIdx gain):device(HMC_DEVICE_ADDRESS) {
	getConfigRegB();
	setConfigRegB(gain);
}
HMC5883L::~HMC5883L() {
	cout << "HMC obj deleted" << endl;
}

string HMC5883L::getDeviceID() {
	uint8_t recvBytes[3];
	device.readBytes(HMC_ID_REG_A, recvBytes, sizeof(recvBytes));
	stringstream ss;
	for (size_t i = 0; i < 3; i++)
		ss << device.i2c_read_buffer[i];
	return ss.str();
}

ConfigRegA HMC5883L::getConfigRegA() {
	ConfigRegA reg = (ConfigRegA)device.readByte(HMC_CONFIG_REG_A);
	return reg;
}

void HMC5883L::setConfigRegA(ConfigRegA reg) {
	device.writeByte(HMC_CONFIG_REG_A, reg.getData());
	_CRA = reg;
}
HMC5883L::Gain HMC5883L::getConfigRegB() {
	unsigned char regB = device.readByte(HMC_CONFIG_REG_B);
	GainIdx idx = (GainIdx)(regB >> GAIN_BIT_OFFSET);
	_gain.updateIdx(idx);
	return _gain;
}
void HMC5883L::setConfigRegB(GainIdx gain) {
	_gain.updateIdx(gain);
	device.writeByte(HMC_CONFIG_REG_B, (gain << GAIN_BIT_OFFSET));
}
OperatingMode HMC5883L::getModeRegister() {
	unsigned char mode = device.readByte(HMC_MODE_REG);
	switch (mode) {
	case 0x00:
		return ContinuousMeasurement;
	case 0x01:
		return SingleMeasurement;
	default:
		return IdleMode;
	}
}
void HMC5883L::setModeRegister(OperatingMode mode) {
	device.writeByte(HMC_MODE_REG, mode);
}

//Data HMC5883L::getDataReadyXYZ(uint16_t timeout) {
//	uint16_t counter = 0;
//	while (!getStatus().DataReady) {
//		if (counter >= timeout) {
//			cout << "Error: Timeout in getDataReadyXYZ()" << endl;
//			break;
//		}
//		counter++;
//		usleep(1000);
//	}
//
//	return getDataXYZ();
//}
Status HMC5883L::getStatus() {
	unsigned char statusReg = device.readByte(HMC_STATUS_REG);
	Status status;
	status.DataReady = statusReg & 0x01;
	status.DataLocked = statusReg & 0x02;
	return status;
}
//bool HMC5883L::runSelfTest() {
//	// positive self-test process using continuous measurement mode
//	setConfigRegA(AVG_SAMPLES_8 | DATA_RATE_75 | MEAS_MODE_POS);
//	setConfigRegB(GAIN_5);
//	setModeRegister(SingleMeasurement);
//	// wait until data ready
//	uint8_t counter = 0;
//	while (!getStatus().DataReady) {
//		counter++;
//		if (counter > 255) {
//			cout << "Error, data never ready. Self test aborted" << endl;
//			return false;
//		}
//	}
//	// read data ( if recently changed the gain do extra read to reset)
//	if (_gain.updateFlag)
//	{
//		getDataXYZ();
//		_gain.updateFlag = false;
//	}
//	while (true) {
//		Data d = getDataReadyXYZ();
//		int upperLimit = 575 * LSB_PER_GAUSS[_gain.gIdx] / 390;
//		int lowerLimit = 243 * LSB_PER_GAUSS[_gain.gIdx] / 390;
//		if (	((d.x <= upperLimit) && (d.x >= lowerLimit)) |
//				((d.y <= upperLimit) && (d.y >= lowerLimit)) |
//				((d.z <= upperLimit) && (d.z >= lowerLimit)) )	{
//			// test passed
//			setConfigRegA( CRA_DEFAULT | MEAS_MODE_NORM );
//			cout << "Passed @ Gain: " << _gain.gIdx << "\t" << d.toString() << endl;
//			return true;
//		} else {
//			if (_gain.gIdx < GAIN_7) {
//				cout << "Failed @ Gain: " << _gain.gIdx << "\t" << d.toString() << endl;
//				setConfigRegB(_gain.incrementIdx());
//			}
//			else {
//				setConfigRegA( CRA_DEFAULT | MEAS_MODE_NORM );
//				return false;
//			}
//		}
//	}
//}
void HMC5883L::dumpAllRegisters(unsigned char* regData, unsigned char len) {
	device.readBytes(HMC_CONFIG_REG_A, regData, len);
	for (uint8_t i = 0; i < len; i++) {
		printf("0x%X ", regData[i]);
	}
	cout << endl;
}
void HMC5883L::getSensorData() {
	uint8_t recvBytes[6];
	device.readBytes(HMC_DATA_2B_START_X, recvBytes, sizeof(recvBytes));
	setX((recvBytes[0] << 8) + (recvBytes[1]));
	setZ((recvBytes[2] << 8) + (recvBytes[3]));
	setY((recvBytes[4] << 8) + (recvBytes[5]));
	GainIdx dataGain;
	if (_gain.isUpdateFlag()) {
		// use previous gain
		dataGain = _gain.getPrevGain();
		_gain.setUpdateFlag(false);
	} else {
		dataGain = _gain.getIdx();
	}
	setFormatMultiplier(1.0/LSB_PER_GAUSS[dataGain]);
}
double HMC5883L::getHeadingDeg(){
	double deg = 0;
	if (getY() > 0) {
		deg =  90 - ((atan((double)getX()/getY()))*180/PI);
	} else if (getY() < 0) {
		deg =  270 - ((atan((double)getX()/getY()))*180/PI);
	} else if (getY() == 0 && getX() < 0) {
		deg = 180.0;
	} else if (getY() == 0 && getX() > 0) {
		deg = 0.0;
	}
	return deg;
}

unsigned char HMC5883L::Gain::getConfigRegB() {
	return (gIdx << GAIN_BIT_OFFSET);
}
HMC5883L::Gain::Gain(GainIdx idx) {
	gIdx = idx;
	prevGain = idx;
	updateFlag = true;
}
HMC5883L::Gain::Gain() {
	init();
}
void HMC5883L::Gain::updateIdx(GainIdx idx) {
	if (idx != gIdx) {
		prevGain = gIdx;
		gIdx = idx;
		updateFlag = true;
	} else {
		updateFlag = false;
	}
}
void HMC5883L::Gain::init() {
	gIdx = GAIN_1;	// default
	prevGain = GAIN_1;
	updateFlag = false;
}
GainIdx HMC5883L::Gain::incrementIdx() {
	unsigned char i = (unsigned char)gIdx;
	i++;
	gIdx = (GainIdx)i;
	return gIdx;
}
