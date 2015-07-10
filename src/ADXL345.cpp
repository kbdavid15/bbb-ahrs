/*
 * ADXL345.cpp
 *
 *  Created on: Jul 9, 2015
 *      Author: kbdavid15
 */

#include "../include/ADXL345.h"
#include <iostream>

using namespace std;

namespace ADX
{
	ADXL345::ADXL345():spi(BlackLib::SPI0_0, 8, BlackLib::SpiMode3, 2400000) {
		spi.open( BlackLib::ReadWrite | BlackLib::NonBlock );
	}
	ADXL345::~ADXL345() {
		spi.close();
	}
	unsigned char ADXL345::getDeviceID() {
		uint8_t sendBytes[2] = { DEVICE_ID | 0x80, 0x01 };
		uint8_t recBytes[2];
		spi.transfer(sendBytes, recBytes, 2);
		return recBytes[1];
		//return spi.transfer(DEVICE_ID);
	}
	unsigned char ADXL345::getThresholdTap() {
		uint8_t tmp = THRESH_TAP;
		tmp |= 0x80;
		printf("tmp: %x\n", tmp);
		return spi.transfer(tmp);
	}
	Data  ADXL345::getXYZ() {
		uint8_t sendBytes[6] = { DATAX0 | 0xC0, 0x02, 0x02, 0x02, 0x02, 0x02 };
		uint8_t recvBytes[6];
		spi.transfer(sendBytes, recvBytes, sizeof(recvBytes));
		for (uint8_t i = 0; i < 6; i++) {
			printf("0x%x ", recvBytes[i]);
		}
		cout << "" <<  endl;
		Data data;
		data.x = 0;
		data.y = 0;
		data.z = 1;
	}
}
