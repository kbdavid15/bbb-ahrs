/*
 * DataPoint.cpp
 *
 *  Created on: Jul 29, 2015
 *      Author: kbdavid15
 */

#include "DataPoint.h"
#include <sys/time.h>
#include <sstream>

using namespace std;

uint64_t DataPoint::getMicros() {
	struct timeval tv;
	gettimeofday(&tv,NULL);
	auto time = tv.tv_sec*(uint64_t)1000000+tv.tv_usec;
	return time;
}

DataPoint::DataPoint(int16_t x, int16_t y, int16_t z, double format) {
	dataFormatMult = format;
	setX(x);
	setY(y);
	setZ(z);
	sensorReadTime = getMicros();
}

DataPoint::DataPoint() {
//	sensorReadTime = 0;
//	dataFormatMult = 0;
}

DataPoint::~DataPoint() { }

DataPoint DataPoint::operator -(const DataPoint& p)  {
	DataPoint newData = copyFormatFrom(p);
	newData.setX(x - p.x);
	newData.setY(y - p.y);
	newData.setZ(z - p.z);
	return newData;
}

DataPoint DataPoint::operator +(const DataPoint& p)  {
	DataPoint newData = copyFormatFrom(p);
	newData.setX(x + p.x);
	newData.setY(y + p.y);
	newData.setZ(z + p.z);
	return newData;
}
// TODO: Limitation of integer division
DataPoint DataPoint::operator /(const double num)  {
	DataPoint newData = copyFormatFrom(*this);
	newData.setX(x / num);
	newData.setY(y / num);
	newData.setZ(z / num);
	return newData;
}
DataPoint DataPoint::operator *(const double num)  {
	DataPoint newData = copyFormatFrom(*this);
	newData.setX(x * num);
	newData.setY(y * num);
	newData.setZ(z * num);
	return newData;
}
DataPoint DataPoint::average(DataPoint* points, unsigned int length) {
	DataPoint totalPoint = copyFormatFrom(points[0]);
	for (unsigned int i = 0; i < length; i++) {
		totalPoint = totalPoint + points[i];
	}
	return totalPoint/length;
}

DataPoint DataPoint::copyFormatFrom(DataPoint point) {
	DataPoint newPoint;
	newPoint.dataFormatMult = point.dataFormatMult;
	return newPoint;
}
string DataPoint::toString() {
	stringstream ss;
	ss << "X: " << x << "\t";
	ss << "Y: " << y << "\t";
	ss << "Z: " << z;
	return ss.str();
}
string DataPoint::toFile(bool rawData, char formatSpecifier) {
	stringstream ss;
	if (rawData) {
		ss << x << formatSpecifier;
		ss << y << formatSpecifier;
		ss << z;
	} else {
		ss << xf << formatSpecifier;
		ss << yf << formatSpecifier;
		ss << zf;
	}
	return ss.str();
}
string DataPoint::toString(bool rawData) {
	stringstream ss;
	if (rawData) {
		ss << "X: " << x << "\t";
		ss << "Y: " << y << "\t";
		ss << "Z: " << z;
	} else {
		ss << "Xf: " << xf << "\t";
		ss << "Yf: " << yf << "\t";
		ss << "Zf: " << zf;
	}
	return ss.str();
}
DataPoint DataPoint::filter(DataPoint& lastPoint) {
	DataPoint p = copyFormatFrom(*this);
}
