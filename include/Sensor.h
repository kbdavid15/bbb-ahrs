/*
 * Sensor.h
 *
 *  Created on: Jul 19, 2015
 *      Author: kbdavid15
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include <string>
#include <sstream>
//#include <queue>
#include <deque>
#include <DataPoint.h>

class Sensor {
private:
	static const uint8_t QUEUE_SIZE;
	std::deque<double> _xQ, _yQ, _zQ;
	double _xfLast, _yfLast, _zfLast;
	double _xTrapLast, _yTrapLast, _zTrapLast;
	DataPoint lastDataPoint;
	double lpf = 1;	// default value of 1 means no filter
public:
	DataPoint dataPoint;
	virtual ~Sensor();
	Sensor();
	/**
	 *
	 * @param Sample rate in microseconds
	 * @return
	 */
	double trapX(long);	// trapezoidal integral
	double trapZ(long);	// trapezoidal integral
	DataPoint getLPFData();
	virtual DataPoint getSensorData()=0;	// must be defined in implementation class

	void setLPF(double alpha) {
		this->lpf = alpha;
	}

	const std::deque<double>& getXQ() const {
		return _xQ;
	}

	const std::deque<double>& getYQ() const {
		return _yQ;
	}

	const std::deque<double>& getZQ() const {
		return _zQ;
	}

	const DataPoint& getLastDataPoint() const {
		return lastDataPoint;
	}

	void setLastDataPoint(const DataPoint& lastDataPoint) {
		this->lastDataPoint = lastDataPoint;
	}
};

#endif /* SENSOR_H_ */
