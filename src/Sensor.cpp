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
	_x = 0;
	_y = 0;
	_z = 0;
	_xf = 0;
	_yf = 0;
	_zf = 0;
	_dataFormatMultiplier = 0;
}

int16_t Sensor::getX() {
	return _x;
}
int16_t Sensor::getY() {
	return _y;
}
int16_t Sensor::getZ() {
	return _z;
}
int16_t Sensor::getXf() {
	return _xf;
}
int16_t Sensor::getYf() {
	return _yf;
}
int16_t Sensor::getZf() {
	return _zf;
}
void Sensor::setX(int16_t x){
	_x = x;
}
void Sensor::setY(int16_t y){
	_y = y;
}
void Sensor::setZ(int16_t z){
	_z = z;
}
string Sensor::dataToString() {
	stringstream ss;
	ss << "X: " << _x << "\t";
	ss << "Y: " << _y << "\t";
	ss << "Z: " << _z;
	return ss.str();
}
string Sensor::dataToFile(bool raw, char formatSpecifier) {
	stringstream ss;
	if (raw) {
		ss << _x << formatSpecifier;
		ss << _y << formatSpecifier;
		ss << _z;
	} else {
		format();
		ss << _xf << formatSpecifier;
		ss << _yf << formatSpecifier;
		ss << _zf;
	}
	return ss.str();
}
string Sensor::dataToString(bool rawData) {

	stringstream ss;
	if (rawData) {
		ss << "X: " << _x << "\t";
		ss << "Y: " << _y << "\t";
		ss << "Z: " << _z;
	} else {
		format();
		ss << "Xf: " << _xf << "\t";
		ss << "Yf: " << _yf << "\t";
		ss << "Zf: " << _zf;
	}
	return ss.str();
}
void Sensor::format(double multiplier) {
	_dataFormatMultiplier = multiplier;
	_xf = _x * multiplier;
	_yf = _y * multiplier;
	_zf = _z * multiplier;
}

void Sensor::format() {
	_xf = _x * _dataFormatMultiplier;
	_yf = _y * _dataFormatMultiplier;
	_zf = _z * _dataFormatMultiplier;
}

void Sensor::setFormatMultiplier(double multiplier) {
	_dataFormatMultiplier = multiplier;
}
