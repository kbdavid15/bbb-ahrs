/*
 * Sensor.cpp
 *
 *  Created on: Jul 19, 2015
 *      Author: kbdavid15
 */

#include <Sensor.h>

using namespace std;

const uint8_t Sensor::QUEUE_SIZE = 2;

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
	_xTrapLast = 0;
	_yTrapLast = 0;
	_zTrapLast = 0;
	_xfLast = 0;
	_yfLast = 0;
	_zfLast = 0;
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
double Sensor::getXf() {
	return _xf;
}
double Sensor::getYf() {
	return _yf;
}
double Sensor::getZf() {
	return _zf;
}
void Sensor::setX(int16_t x) {
	_xfLast = _xf;
	_x = x;
	_xf = _x * _dataFormatMultiplier;
//	_xQ.push_back(x);
//	if (_xQ.size() > QUEUE_SIZE) {
//		_xQ.pop_front();
//	}
}
void Sensor::setY(int16_t y){
	_yfLast = _yf;
	_y = y;
	_yf = _y * _dataFormatMultiplier;
//	_yQ.push_back(y);
//	if (_yQ.size() > QUEUE_SIZE) {
//		_yQ.pop_front();
//	}
}
void Sensor::setZ(int16_t z){
	_zfLast = _zf;
	_z = z;
	_zf = _z * _dataFormatMultiplier;
//	_zQ.push_back(z);
//	if (_zQ.size() > QUEUE_SIZE) {
//		_zQ.pop_front();
//	}
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
//		format();
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
//		format();
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
double Sensor::trapX(long uTs) {
	double Ts = uTs / 1000000.0;
//	_xTrapLast = (_xTrapLast) + (1/(2*(double)fs)) * (_xLast + _x);
	_xTrapLast = (_xTrapLast) + (Ts/2.0) * (_xfLast + _xf);
	return _xTrapLast;
}
