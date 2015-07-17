/*
 * ADXL345.cpp
 *
 *  Created on: Jul 9, 2015
 *      Author: kbdavid15
 */

#include "../include/ADXL345.h"
#include <iostream>
#include <unistd.h>

using namespace std;

namespace ADX
{
	ADXL345::ADXL345():spi(BlackLib::SPI0_0, 8, BlackLib::SpiMode3, 2400000) {
		spi.open( BlackLib::ReadWrite | BlackLib::NonBlock );
	}
	ADXL345::~ADXL345() {
		spi.close();
	}

	unsigned char ADXL345::readByte(uint8_t address) {
		uint8_t wbuf[2] = { address | BYTE_READ, 0x00 };
		uint8_t rbuf[2];
		spi.transfer(wbuf, rbuf, sizeof(rbuf));
		return rbuf[1];
	}
	unsigned char * ADXL345::readBytes(uint8_t address, uint8_t len) {
		uint8_t wbuf[len];
		wbuf[0] = { address | MULTI_BYTE_READ };
		uint8_t rbuf[len];
		spi.transfer(wbuf, rbuf, len);
		return rbuf;
	}
	void ADXL345::writeByte(unsigned char address, unsigned char data) {
		uint8_t wbuf[2] = { address, data };
		uint8_t rbuf[2];
		spi.transfer(wbuf, rbuf, sizeof(wbuf));
	}

	unsigned char ADXL345::getDeviceID() {
		return readByte(DEVICE_ID);
	}
	unsigned char ADXL345::getThresholdTap() {
		return readByte(THRESH_TAP);
	}
	Data  ADXL345::getXYZ() {
		uint8_t *recvBytes = readBytes(DATAX0, 7);
		Data data;
		data.x = ((recvBytes[2] << 8) + recvBytes[1]);
		data.y = ((recvBytes[4] << 8) + recvBytes[3]);
		data.z = ((recvBytes[6] << 8) + recvBytes[5]);
		return data;

	}
	void ADXL345::setDataFormat(DataFormat format) {
		writeByte(DATA_FORMAT, format.getData());
	}
	DataFormat ADXL345::getDataFormat() {
		uint8_t regData = readByte(DATA_FORMAT);
		return DataFormat(regData);
	}
	void ADXL345::setPowerCtrl(unsigned char pwr) {
		writeByte(POWER_CTL, pwr);
	}
	void ADXL345::setInterruptEnable(unsigned char interrupt) {
		writeByte(INT_ENABLE, interrupt);
	}
	PwrDataRate ADXL345::getDataRate() {
		uint8_t regVal = readByte(BW_RATE);
		PwrDataRate pdr(regVal);
		return pdr;
	}
	void ADXL345::setDataRate(PwrDataRate pdr) {
		writeByte(BW_RATE, pdr.getByteFormat());
	}
	bool ADXL345::startSelfTest() {
		DataFormat data;
		data.fullRes = 1;
		data.range = DataRange16g;
		data.selfTest = 1;
	}
	void ADXL345::calibrateOffset() {

	}
	AvgData ADXL345::averageDataPoints(uint8_t numPoints) {
		long total_x = 0, total_y = 0, total_z = 0;
		for (uint8_t i = 0; i < numPoints; i++) {
			Data d = getXYZ();
			total_x += d.x;
			total_y += d.y;
			total_z += d.z;


		}

	}
}
