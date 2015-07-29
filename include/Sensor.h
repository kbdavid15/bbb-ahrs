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
//	int16_t _x, _y, _z;
//	double 	_xf, _yf, _zf;
//	double _dataFormatMultiplier;	// updated whenever the scale sensitivity is changed
public:
	DataPoint dataPoint;
	// getters and setters
	virtual ~Sensor();
	Sensor();
	/**
	 *
	 * @param Sample rate in microseconds
	 * @return
	 */
	double trapX(long);	// trapezoidal integral
	virtual DataPoint getSensorData()=0;	// must be defined in implementation class

	const std::deque<double>& getXQ() const {
		return _xQ;
	}

	const std::deque<double>& getYQ() const {
		return _yQ;
	}

	const std::deque<double>& getZQ() const {
		return _zQ;
	}
};

#endif /* SENSOR_H_ */
