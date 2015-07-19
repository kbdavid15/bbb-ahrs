/*
 * L3G4200D.cpp
 *
 *  Created on: Jul 9, 2015
 *      Author: kbdavid15
 */

#include "../include/L3G4200D.h"
#include <iostream>

using namespace std;

namespace L3G {

	L3G4200D::L3G4200D():spi(BlackLib::SPI1_0, 8, BlackLib::SpiMode3, 2400000) {
		spi.open( BlackLib::ReadWrite | BlackLib::NonBlock );
		_range = dps_250;
		_block_data_update = CR4_BLOCK_DATA_UPDATE;
		// set up config registers
		uint8_t sendBytes[2] = { CTRL_REG1 | BYTE_WRITE, 0x0F };
		uint8_t recvBytes[2];
		spi.transfer(sendBytes, recvBytes, sizeof(sendBytes));
		sendBytes[0] = CTRL_REG4 | BYTE_WRITE;
		sendBytes[1] = _range | _block_data_update;
		spi.transfer(sendBytes, recvBytes, sizeof(sendBytes));
	}

	L3G4200D::L3G4200D(CR4_MeasureRange range):spi(BlackLib::SPI1_0, 8, BlackLib::SpiMode3, 2400000) {
		spi.open( BlackLib::ReadWrite | BlackLib::NonBlock );
		_range = range;
		_block_data_update = CR4_BLOCK_DATA_UPDATE;
		// set up config registers
		uint8_t sendBytes[2] = { CTRL_REG1 | BYTE_WRITE, 0x0F };
		uint8_t recvBytes[2];
		spi.transfer(sendBytes, recvBytes, sizeof(sendBytes));
		sendBytes[0] = CTRL_REG4 | BYTE_WRITE;
		sendBytes[1] = _range | _block_data_update;
		spi.transfer(sendBytes, recvBytes, sizeof(sendBytes));
	}

	L3G4200D::~L3G4200D() {
		spi.close();
	}

	unsigned char L3G4200D::readByte(uint8_t address) {
		uint8_t wbuf[2] = { (uint8_t)(address | BYTE_READ), 0x00 };
		uint8_t rbuf[2];
		spi.transfer(wbuf, rbuf, sizeof(rbuf));
		return rbuf[1];
	}
	/**	Reads a number of bytes from the sensor
	 *
	 * @param address: Register address to start reading from
	 * @param readData: Return data array pointer
	 * @param len: Length of bytes to read, +1. This should also be the sizeof readData
	 */
	void L3G4200D::readBytes(uint8_t address, uint8_t* readData, uint8_t len) {
		uint8_t wbuf[len];
		wbuf[0] = { (uint8_t)(address | BYTE_READ | MULTI_BYTE) };
		spi.transfer(wbuf, readData, len);
	}
	void L3G4200D::writeByte(unsigned char address, unsigned char data) {
		uint8_t wbuf[2] = { address, data };
		uint8_t rbuf[2];
		spi.transfer(wbuf, rbuf, sizeof(wbuf));
	}
	void L3G4200D::writeBytes(unsigned char address, unsigned char *data, unsigned char len) {
		uint8_t wbuf[len + 1];
		wbuf[0] = address | MULTI_BYTE;
		for (uint8_t i = 1; i < (len + 1); i++) {
			wbuf[i] = data[i-1];
		}
		uint8_t rbuf[len + 1];
		spi.transfer(wbuf, rbuf, sizeof(wbuf));
	}

	unsigned char L3G4200D::getDeviceID() {
		uint8_t sendBytes[2] = { WHO_AM_I | BYTE_READ, 0x00 };
		uint8_t recBytes[2];
		spi.transfer(sendBytes, recBytes, 2);
		return recBytes[1];
	}
	Data L3G4200D::getXYZ() {
		uint8_t sendBytes[7] = { OUT_X_L | BYTE_READ | MULTI_BYTE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		uint8_t recvBytes[7];
		spi.transfer(sendBytes, recvBytes, sizeof(sendBytes));
		Data data;
		data.x = ((recvBytes[2] << 8) | recvBytes[1]);
		data.y = ((recvBytes[4] << 8) | recvBytes[3]);
		data.z = ((recvBytes[6] << 8) | recvBytes[5]);
		return data;
	}
	DPS L3G4200D::getDPS() {
		Data data = getXYZ();
		DPS d(data, _range);
		return d;
	}
	CR4_MeasureRange L3G4200D::getMeasurementRange() {
		uint8_t sendBytes[2] = { CTRL_REG4 | BYTE_READ, 0x00 };
		uint8_t recBytes[2];
		spi.transfer(sendBytes, recBytes, sizeof(sendBytes));
		return (CR4_MeasureRange)(recBytes[1] & 0b00110000);
	}
	unsigned char L3G4200D::getTemperature() {
		uint8_t sendBytes[2] = { OUT_TEMP | BYTE_READ, 0x00 };
		uint8_t recvBytes[2];
		spi.transfer(sendBytes, recvBytes, sizeof(sendBytes));
		return recvBytes[1];
	}
}
