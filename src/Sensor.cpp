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
//	_x = 0;
//	_y = 0;
//	_z = 0;
//	_xf = 0;
//	_yf = 0;
//	_zf = 0;
//	_dataFormatMultiplier = 0;
	_xTrapLast = 0;
	_yTrapLast = 0;
	_zTrapLast = 0;
	_xfLast = 0;
	_yfLast = 0;
	_zfLast = 0;
}

double Sensor::trapX(long uTs) {
	double Ts = uTs / 1000000.0;
//	_xTrapLast = (_xTrapLast) + (1/(2*(double)fs)) * (_xLast + _x);
	_xTrapLast = (_xTrapLast) + (Ts/2.0) * (_xfLast + dataPoint.getXf());
	return _xTrapLast;
}