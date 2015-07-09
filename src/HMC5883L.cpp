/*
 * HMC5883L.cpp
 *
 *  Created on: Jul 9, 2015
 *      Author: kbdavid15
 */

#include "../include/HMC5883L.h"
#include <iostream>

using namespace std;

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
