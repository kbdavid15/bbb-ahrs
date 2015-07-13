/*
 * HMC5883L.cpp
 *
 *  Created on: Jul 9, 2015
 *      Author: kbdavid15
 */

#include "../include/HMC5883L.h"
#include <iostream>

using namespace std;

namespace HMC {

	const uint16_t HMC5883L::LSB_PER_GAUSS[] = { 1370, 1090, 820, 660, 440, 390, 330, 230 };

	HMC5883L::HMC5883L() {
		device = new i2cDevice(HMC_DEVICE_ADDRESS);
		_gain.gIdx = GAIN_1;
		_gain.updateFlag = false;
		_gain.prevGain = GAIN_1;
		if (!runSelfTest()) {
			cout << "Error: self test failed" << endl;
		}
	}
	HMC5883L::HMC5883L(GainIdx gain) {
		device = new i2cDevice(HMC_DEVICE_ADDRESS);
		_gain.init();
		_gain.updateIdx(gain);
		setConfigRegB(_gain.gIdx);
	}
	HMC5883L::~HMC5883L() {
		cout << "HMC obj deleted" << endl;
	}

	string HMC5883L::getDeviceID() {
		device->readBytes(HMC_ID_REG_A, 3);
		stringstream ss;
		for (size_t i = 0; i < 3; i++)
			ss << device->i2c_read_buffer[i];
		return ss.str();
	}

	unsigned char HMC5883L::getConfigRegA() {
		return device->readByte(HMC_CONFIG_REG_A);
	}
	void HMC5883L::setConfigRegA(unsigned char flags) {
		device->sendByte(HMC_CONFIG_REG_A, flags);
	}
	void HMC5883L::setConfigRegA(ConfigRegA reg) {
		//device->sendByte(HMC_CONFIG_REG_A, (char*)&reg);
	}
	Gain HMC5883L::getConfigRegB() {
		unsigned char regB = device->readByte(HMC_CONFIG_REG_B);
		GainIdx idx = (GainIdx)(regB >> GAIN_BIT_OFFSET);
		_gain.updateIdx(idx);
		return _gain;
	}
	void HMC5883L::setConfigRegB(GainIdx gain) {
		_gain.updateIdx(gain);
		device->sendByte(HMC_CONFIG_REG_B, (gain << GAIN_BIT_OFFSET));
	}
	OperatingMode HMC5883L::getModeRegister() {
		unsigned char mode = device->readByte(HMC_MODE_REG);
		switch (mode) {
		case 0x00:
			return ContinuousMeasurement;
		case 0x01:
			return SingleMeasurement;
		default:
			return Idle;
		}
	}
	void HMC5883L::setModeRegister(OperatingMode mode) {
		device->sendByte(HMC_MODE_REG, mode);
	}
//	int16_t HMC5883L::getDataX() {
//		// first check if device is ready
//		device->readBytes(HMC_DATA_2B_START_X, 2);
//		return ((device->i2c_read_buffer[0] << 8) + (device->i2c_read_buffer[1]));
//	}
//	int16_t HMC5883L::getDataY() {
//		// first check if device is ready
//		device->readBytes(HMC_DATA_2B_START_Y, 2);
//		return ((device->i2c_read_buffer[0] << 8) + (device->i2c_read_buffer[1]));
//	}
//	int16_t HMC5883L::getDataZ() {
//		// first check if device is ready
//		device->readBytes(HMC_DATA_2B_START_Z, 2);
//		return ((device->i2c_read_buffer[0] << 8) + (device->i2c_read_buffer[1]));
//	}
	Data HMC5883L::getDataXYZ() {
		device->readBytes(HMC_DATA_2B_START_X, 6);
		Data d;
		d.x = ((device->i2c_read_buffer[0] << 8) + (device->i2c_read_buffer[1]));
		d.z = ((device->i2c_read_buffer[2] << 8) + (device->i2c_read_buffer[3]));
		d.y = ((device->i2c_read_buffer[4] << 8) + (device->i2c_read_buffer[5]));
		return d;
	}
	Status HMC5883L::getStatus() {
		unsigned char statusReg = device->readByte(HMC_STATUS_REG);
		Status status;
		status.DataReady = statusReg & 0x01;
		status.DataLocked = statusReg & 0x02;
		return status;
	}
	bool HMC5883L::runSelfTest() {
		// positive self-test process using continuous measurement mode
		setConfigRegA(AVG_SAMPLES_8 | DATA_RATE_15 | MEAS_MODE_POS);
		setConfigRegB(GAIN_5);
		setModeRegister(ContinuousMeasurement);
		// wait until data ready
		uint8_t counter = 0;
		while (!getStatus().DataReady) {
			counter++;
			if (counter > 255) {
				cout << "Error, data never ready. Self test aborted" << endl;
				return false;
			}
		}
		// read data ( if recently changed the gain do extra read to reset)
		if (_gain.updateFlag)
		{
			getDataXYZ();
			_gain.updateFlag = false;
		}
		while (true) {
			Data d = getDataXYZ();
			int upperLimit = 575 * LSB_PER_GAUSS[_gain.gIdx] / 390;
			int lowerLimit = 243 * LSB_PER_GAUSS[_gain.gIdx] / 390;
			if (	((d.x <= upperLimit) && (d.x >= lowerLimit)) |
					((d.y <= upperLimit) && (d.y >= lowerLimit)) |
					((d.z <= upperLimit) && (d.z >= lowerLimit)) )	{
				// test passed
				setConfigRegA( CRA_DEFAULT | MEAS_MODE_NORM );
				return true;
			} else {
				if (_gain.gIdx < GAIN_7) {
					setConfigRegB(_gain.incrementIdx());
				}
				else {
					setConfigRegA( CRA_DEFAULT | MEAS_MODE_NORM );
					return false;
				}
			}
		}
	}
}
