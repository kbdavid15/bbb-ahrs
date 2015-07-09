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
	HMC5883L::HMC5883L() {
		device = new i2cDevice(HMC_DEVICE_ADDRESS);
	}
	HMC5883L::~HMC5883L() {
		cout << "HMC obj deleted" << endl;
	}

	string HMC5883L::getDeviceID() {
		device->readBytes(HMC_ID_REG_A, 3);

		stringstream ss;

		for (size_t i = 0; i < 3; i++)
		{
			ss << device->i2c_read_buffer[i];
		}

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
	unsigned char HMC5883L::getConfigRegB() {
		return device->readByte(HMC_CONFIG_REG_B);
	}
	void HMC5883L::setConfigRegB(unsigned char flags) {
		device->sendByte(HMC_CONFIG_REG_B, flags);
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
	int16_t HMC5883L::getDataX() {
		// first check if device is ready
		device->readBytes(HMC_DATA_2B_START_X, 2);
		return ((device->i2c_read_buffer[0] << 8) + (device->i2c_read_buffer[1]));
	}
	int16_t HMC5883L::getDataY() {
		// first check if device is ready
		device->readBytes(HMC_DATA_2B_START_Y, 2);
		return ((device->i2c_read_buffer[0] << 8) + (device->i2c_read_buffer[1]));
	}
	int16_t HMC5883L::getDataZ() {
		// first check if device is ready
		device->readBytes(HMC_DATA_2B_START_Z, 2);
		return ((device->i2c_read_buffer[0] << 8) + (device->i2c_read_buffer[1]));
	}
	Data HMC5883L::getDataXYZ() {
		device->readBytes(HMC_DATA_2B_START_X, 6);
		Data d;
		d.x = ((device->i2c_read_buffer[0] << 8) + (device->i2c_read_buffer[1]));
		d.y = ((device->i2c_read_buffer[2] << 8) + (device->i2c_read_buffer[3]));
		d.z = ((device->i2c_read_buffer[4] << 8) + (device->i2c_read_buffer[5]));
		return d;
	}

}
