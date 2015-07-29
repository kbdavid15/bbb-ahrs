/*
 * DataPoint.cpp
 *
 *  Created on: Jul 29, 2015
 *      Author: kbdavid15
 */

#include <DataPoint.h>


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

DataPoint DataPoint::average(DataPoint* points, unsigned int length) {
	DataPoint totalPoint;
	for (unsigned int i = 0; i < length; i++) {
		totalPoint = totalPoint + points[i];
	}
	return totalPoint;
}

DataPoint DataPoint::copyFormatFrom(DataPoint point) {
	DataPoint newPoint;
	newPoint.dataFormatMult = point.dataFormatMult;
	return newPoint;
}
