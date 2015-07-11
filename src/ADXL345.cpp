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
		uint8_t sendBytes[2] = { DEVICE_ID | 0x80, 0x00 };
		uint8_t recBytes[2];
		spi.transfer(sendBytes, recBytes, 2);
		return recBytes[1];
	}
	unsigned char ADXL345::getThresholdTap() {
		uint8_t sendBytes[2] = { THRESH_TAP | 0x80, 0x00 };
		uint8_t recBytes[2];
		spi.transfer(sendBytes, recBytes, 2);
		return recBytes[1];
	}
	Data  ADXL345::getXYZ() {
		uint8_t sendBytes[7] = { DATAX0 | 0xC0, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02 };
		uint8_t recvBytes[7];
		spi.transfer(sendBytes, recvBytes, sizeof(recvBytes));
		Data data;
		data.x = ((recvBytes[2] << 8) + recvBytes[1]);
		data.y = ((recvBytes[4] << 8) + recvBytes[3]);
		data.z = ((recvBytes[6] << 8) + recvBytes[5]);
		return data;

	}
	void ADXL345::setDataFormat(unsigned char format) {
		// value of 0x0B sets full resolution mode and range to +/- 16g
		uint8_t sendBytes[2] = { DATA_FORMAT, format };
		uint8_t recvBytes[2];
		spi.transfer(sendBytes, recvBytes, sizeof(sendBytes));
	}
	void ADXL345::setPowerCtrl(unsigned char pwr) {
		// value of 0x08 enables measurement mode
		uint8_t sendBytes[2] = { POWER_CTL, pwr };
		uint8_t recvBytes[2];
		spi.transfer(sendBytes, recvBytes, sizeof(sendBytes));
	}
	void ADXL345::setInterruptEnable(unsigned char interrupt) {
		// value of 0x80 enables DataReady bit
		uint8_t sendBytes[2] = { INT_ENABLE, interrupt };
		uint8_t recvBytes[2];
		spi.transfer(sendBytes, recvBytes, sizeof(sendBytes));
	}
}
