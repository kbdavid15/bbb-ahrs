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
	int16_t _x, _y, _z;
	double 	_xf, _yf, _zf;
	double _dataFormatMultiplier;	// updated whenever the scale sensitivity is changed
public:
	// getters and setters
	int16_t getX();
	int16_t getY();
	int16_t getZ();
	int16_t getXf();
	int16_t getYf();
	int16_t getZf();
	void setX(int16_t);
	void setY(int16_t);
	void setZ(int16_t);
	void setFormatMultiplier(double multiplier);

	virtual ~Sensor();
	Sensor();
	Sensor(int16_t x, int16_t y, int16_t z);
	Sensor(int16_t x, int16_t y, int16_t z, double multiplier);
	std::string dataToString();
	std::string dataToString(bool rawData);
	std::string dataToFile(bool raw, char formatSpecifier);
	void format(double);
	void format();
	virtual void getSensorData()=0;	// defined in implementation class

};

#endif /* SENSOR_H_ */
