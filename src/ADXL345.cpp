/*
 * ADXL345.cpp
 *
 *  Created on: Jul 9, 2015
 *      Author: kbdavid15
 */

#include "../include/ADXL345.h"

namespace ADX
{
	ADXL345::ADXL345():spi(BlackLib::SPI0_0, 8, BlackLib::SpiMode3, 2400000) {
		spi.open( BlackLib::ReadWrite | BlackLib::NonBlock );
	}
	ADXL345::~ADXL345() {
		spi.close();
	}
	unsigned char ADXL345::getDeviceID() {
		return spi.transfer(DEVICE_ID);
	}
}

