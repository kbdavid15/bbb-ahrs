/*
 * Sensor.h
 *
 *  Created on: Jul 19, 2015
 *      Author: kbdavid15
 */

#ifndef SENSORDATA_H_
#define SENSORDATA_H_

#include <string>
#include <sstream>

class SensorData {
private:

public:
	virtual ~SensorData();
	SensorData();
	SensorData(int16_t x, int16_t y, int16_t z);
	SensorData(int16_t x, int16_t y, int16_t z, double multiplier);
	int16_t x, y, z;
	double 	xf, yf, zf;
	std::string dataToString();
	std::string dataToString(bool rawData);
	std::string dataToFile(bool raw, char formatSpecifier);
	void format(double);
	virtual void getSensorData();	// defined in implementation class
};

#endif /* SENSORDATA_H_ */
