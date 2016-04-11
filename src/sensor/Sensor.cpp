/*
 * Sensor.cpp
 *
 *  Created on: Jul 19, 2015
 *      Author: kbdavid15
 */

#include "Sensor.h"

using namespace std;

const uint8_t Sensor::QUEUE_SIZE = 2;

Sensor::~Sensor() {
	// TODO Auto-generated destructor stub
}

Sensor::Sensor() {
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

double Sensor::trapZ(long uTs) {
	double Ts = uTs / 1000000.0;
	_zTrapLast = (_zTrapLast) + Ts * (lastDataPoint.getZf() + dataPoint.getZf());
	return _zTrapLast;
}

DataPoint Sensor::getLPFData() {
//	fXg = Xg * alpha + (fXg * (1.0 - alpha));
	DataPoint p = DataPoint::copyFormatFrom(this->dataPoint);
	p.setX(dataPoint.getX() * lpf + (lastDataPoint.getX() * (1.0 - lpf)));
	p.setY(dataPoint.getY() * lpf + (lastDataPoint.getY() * (1.0 - lpf)));
	p.setZ(dataPoint.getZ() * lpf + (lastDataPoint.getZ() * (1.0 - lpf)));
	return p;
}

DoublePoint* Sensor::SimpleLowPass(double alpha) {
	double xf = dataPoint.xf + alpha * (lastDataPoint.xf - dataPoint.xf);
	double yf = dataPoint.yf + alpha * (lastDataPoint.yf - dataPoint.yf);
	double zf = dataPoint.zf + alpha * (lastDataPoint.zf - dataPoint.zf);
	return new DoublePoint(xf,yf,zf);
}
