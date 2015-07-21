/*
 * Sensor.cpp
 *
 *  Created on: Jul 19, 2015
 *      Author: kbdavid15
 */

#include <Sensor.h>

using namespace std;

Sensor::~Sensor() {
	// TODO Auto-generated destructor stub
}

Sensor::Sensor() {
	x = 0;
	y = 0;
	z = 0;
	xf = 0;
	yf = 0;
	zf = 0;
	_dataFormatMultiplier = 0;
}
Sensor::Sensor(int16_t x, int16_t y, int16_t z) {
	this->x = x;
	this->y = y;
	this->z = z;
	xf = 0;
	yf = 0;
	zf = 0;
	_dataFormatMultiplier = 0;
}
Sensor::Sensor(int16_t x, int16_t y, int16_t z, double multiplier) {
	this->x = x;
	this->y = y;
	this->z = z;
	format(multiplier);
}

string Sensor::dataToString() {
	std::stringstream ss;
	ss << "X: " << x << "\t";
	ss << "Y: " << y << "\t";
	ss << "Z: " << z;
	return ss.str();
}
string Sensor::dataToFile(bool raw, char formatSpecifier) {
	std::stringstream ss;
	if (raw) {
		ss << x << formatSpecifier;
		ss << y << formatSpecifier;
		ss << z;
	} else {
		format();
		ss << xf << formatSpecifier;
		ss << yf << formatSpecifier;
		ss << zf;
	}
	return ss.str();
}
string Sensor::dataToString(bool rawData) {

	std::stringstream ss;
	if (rawData) {
		ss << "X: " << x << "\t";
		ss << "Y: " << y << "\t";
		ss << "Z: " << z;
	} else {
		format();
		ss << "Xf: " << xf << "\t";
		ss << "Yf: " << yf << "\t";
		ss << "Zf: " << zf;
	}
	return ss.str();
}
void Sensor::format(double multiplier) {
	_dataFormatMultiplier = multiplier;
	xf = x * multiplier;
	yf = y * multiplier;
	zf = z * multiplier;
}

void Sensor::format() {
	xf = x * _dataFormatMultiplier;
	yf = y * _dataFormatMultiplier;
	zf = z * _dataFormatMultiplier;
}

void Sensor::setFormatMultiplier(double multiplier) {
	_dataFormatMultiplier = multiplier;
}
