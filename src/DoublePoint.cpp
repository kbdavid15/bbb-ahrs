/*
 * DoublePoint.cpp
 *
 *  Created on: Jan 5, 2016
 *      Author: kbdavid15
 */

#include "DoublePoint.h"
#include <sstream>

DoublePoint::DoublePoint() {
	x = 0;
	y = 0;
	z = 0;
}

DoublePoint::DoublePoint(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}
DoublePoint::~DoublePoint() {
	// TODO Auto-generated destructor stub
}

std::string DoublePoint::toFile(char formatSpecifier) {
	std::stringstream ss;
	ss << x << formatSpecifier;
	ss << y << formatSpecifier;
	ss << z;
}
