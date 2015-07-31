/*
 * DataPoint.h
 *
 *  Created on: Jul 29, 2015
 *      Author: kbdavid15
 */

#ifndef DATAPOINT_H_
#define DATAPOINT_H_

#include <stdint.h>
#include <string>


class DataPoint {
private:
	long x = 0, y = 0, z = 0;
	double  xf = 0, yf = 0, zf = 0;
	double dataFormatMult;
public:

	DataPoint operator -(const DataPoint& p);
	DataPoint operator +(const DataPoint& p);
	DataPoint operator /(const double);

	static DataPoint average(DataPoint* points, unsigned int length);
	static DataPoint copyFormatFrom(DataPoint point);

	std::string toString();
	std::string toString(bool rawData);
	std::string toFile(bool rawData, char formatSpecifier);

	int16_t getX() const {
		return x;
	}
	void setX(int16_t x) {
		this->x = x;
		this->xf = x * dataFormatMult;
	}
	double getXf() const {
		return xf;
	}
	int16_t getY() const {
		return y;
	}
	void setY(int16_t y) {
		this->y = y;
		this->yf = y * dataFormatMult;
	}
	double getYf() const {
		return yf;
	}
	int16_t getZ() const {
		return z;
	}
	void setZ(int16_t z) {
		this->z = z;
		this->zf = z * dataFormatMult;
	}
	double getZf() const {
		return zf;
	}

	void setFormatMultiplier(double dataFormatMult) {
		this->dataFormatMult = dataFormatMult;
	}
};

#endif /* DATAPOINT_H_ */
