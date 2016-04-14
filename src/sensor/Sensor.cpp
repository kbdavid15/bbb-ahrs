/*
 * Sensor.cpp
 *
 *  Created on: Jul 19, 2015
 *      Author: kbdavid15
 */

#include "Sensor.h"
#include <iostream>
using namespace std;

const uint8_t Sensor::QUEUE_SIZE = 2;

Sensor::~Sensor() {
	// TODO Auto-generated destructor stub
}

Sensor::Sensor() {
	_xfLast = 0;
	_yfLast = 0;
	_zfLast = 0;
	trap = DoublePoint(0,0,0);
}

void Sensor::trapezoidal() {
	double delta = (dataPoint.sensorReadTime - lastDataPoint.sensorReadTime) / (double)1000000.0;
	trap.x += (delta * lastDataPoint.getXf() + 0.5 * delta * (dataPoint.getXf() - lastDataPoint.getXf()));
	trap.y += (delta * lastDataPoint.getYf() + 0.5 * delta * (dataPoint.getYf() - lastDataPoint.getYf()));
	trap.z += (delta * lastDataPoint.getZf() + 0.5 * delta * (dataPoint.getZf() - lastDataPoint.getZf()));
	cout << trap.toFile(',') << endl;
}

DataPoint Sensor::getLPFData() {
//	fXg = Xg * alpha + (fXg * (1.0 - alpha));
	DataPoint p = DataPoint::copyFormatFrom(this->dataPoint);
	p.setX(dataPoint.getX() * lpf + (lastDataPoint.getX() * (1.0 - lpf)));
	p.setY(dataPoint.getY() * lpf + (lastDataPoint.getY() * (1.0 - lpf)));
	p.setZ(dataPoint.getZ() * lpf + (lastDataPoint.getZ() * (1.0 - lpf)));
	return p;
}

DoublePoint Sensor::SimpleLowPass(double alpha) {
	double xf = dataPoint.xf + alpha * (lastDataPoint.xf - dataPoint.xf);
	double yf = dataPoint.yf + alpha * (lastDataPoint.yf - dataPoint.yf);
	double zf = dataPoint.zf + alpha * (lastDataPoint.zf - dataPoint.zf);
	return DoublePoint(xf,yf,zf);
}
