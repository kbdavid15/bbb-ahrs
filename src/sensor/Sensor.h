/*
 * Sensor.h
 *
 *  Created on: Jul 19, 2015
 *      Author: kbdavid15
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include "DataPoint.h"
#include "DoublePoint.h"

class Sensor {
private:
	double _xfLast, _yfLast, _zfLast;
	DataPoint lastDataPoint;
	uint64_t lastSampleTime = 0;
	double lpf = 1;	// default value of 1 means no filter
public:
	DoublePoint trap;
	DataPoint dataPoint;
	virtual ~Sensor();
	Sensor();
	void trapezoidal();
	DataPoint getLPFData();
	DoublePoint SimpleLowPass(double alpha);
	virtual DataPoint getSensorData()=0;	// must be defined in implementation class

	void setLPF(double alpha) {
		this->lpf = alpha;
	}

	const DataPoint& getLastDataPoint() const {
		return lastDataPoint;
	}

	void setLastDataPoint(const DataPoint& lastDataPoint) {
		this->lastDataPoint = lastDataPoint;
	}
};

#endif /* SENSOR_H_ */
