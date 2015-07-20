/*
 * Sensor.cpp
 *
 *  Created on: Jul 19, 2015
 *      Author: kbdavid15
 */

#include <SensorData.h>

using namespace std;

SensorData::~SensorData() {
	// TODO Auto-generated destructor stub
}

SensorData::SensorData() {
	x = 0;
	y = 0;
	z = 0;
	xf = 0;
	yf = 0;
	zf = 0;
}
SensorData::SensorData(int16_t x, int16_t y, int16_t z) {
	this->x = x;
	this->y = y;
	this->z = z;
	xf = 0;
	yf = 0;
	zf = 0;
}
SensorData::SensorData(int16_t x, int16_t y, int16_t z, double multiplier) {
	this->x = x;
	this->y = y;
	this->z = z;
	format(multiplier);
}

string SensorData::dataToString() {
	std::stringstream ss;
	ss << "X: " << x << "\t";
	ss << "Y: " << y << "\t";
	ss << "Z: " << z;
	return ss.str();
}
string SensorData::dataToFile(bool raw, char formatSpecifier) {
	std::stringstream ss;
	if (raw) {
		ss << x << formatSpecifier;
		ss << y << formatSpecifier;
		ss << z;
	} else {
		ss << xf << formatSpecifier;
		ss << yf << formatSpecifier;
		ss << zf;
	}
	return ss.str();
}
string SensorData::dataToString(bool rawData) {
	std::stringstream ss;
	if (rawData) {
		ss << "X: " << x << "\t";
		ss << "Y: " << y << "\t";
		ss << "Z: " << z;
	} else {
		ss << "Xf: " << xf << "\t";
		ss << "Yf: " << yf << "\t";
		ss << "Zf: " << zf;
	}
	return ss.str();
}
void SensorData::format(double multiplier) {
	xf = x * multiplier;
	yf = y * multiplier;
	zf = z * multiplier;
}

