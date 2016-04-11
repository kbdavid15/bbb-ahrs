/*
 * DoublePoint.h
 *
 *  Created on: Jan 5, 2016
 *      Author: kbdavid15
 */

#ifndef DOUBLEPOINT_H_
#define DOUBLEPOINT_H_

#include <string>

class DoublePoint {
public:
	double x = 0;
	double y = 0;
	double z = 0;
	DoublePoint();
	DoublePoint(double, double, double);
	virtual ~DoublePoint();

	std::string toFile(char formatSpecifier);
};

#endif /* DOUBLEPOINT_H_ */
