/*
 * HMC5883L.h
 *
 *  Created on: Jul 9, 2015
 *      Author: kbdavid15
 */

#ifndef HMC5883L_H_
#define HMC5883L_H_

#include "../include/myi2c.h"
#include "../include/regaddr.h"
#include <string>
#include <sstream>

class HMC5883L
{
	i2cDevice *device;

public:
	HMC5883L();
	~HMC5883L();

	std::string getDeviceID();

};



#endif /* HMC5883L_H_ */
