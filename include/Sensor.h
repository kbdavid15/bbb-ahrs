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

class Sensor {
private:
	double _dataFormatMultiplier;	// updated whenever the scale sensitivity is changed
public:
	virtual ~Sensor();
	Sensor();
	Sensor(int16_t x, int16_t y, int16_t z);
	Sensor(int16_t x, int16_t y, int16_t z, double multiplier);
	int16_t x, y, z;
	double 	xf, yf, zf;
	std::string dataToString();
	std::string dataToString(bool rawData);
	std::string dataToFile(bool raw, char formatSpecifier);
	void format(double);
	void format();
	virtual void getSensorData()=0;	// defined in implementation class
	void setFormatMultiplier(double multiplier);
};

#endif /* SENSOR_H_ */
