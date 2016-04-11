/*
 * MySpi.cpp
 *
 *  Created on: Jul 21, 2015
 *      Author: kbdavid15
 */

#include "MySpi.h"


MySpi::MySpi(spiName spi, uint8_t spiBitsPerWord, uint8_t spiMode, uint32_t spiSpeed):BlackLib::BlackSPI(spi, spiBitsPerWord, spiMode, spiSpeed) {
	open( BlackLib::ReadWrite | BlackLib::NonBlock );
}
MySpi::MySpi(spiName spi):BlackLib::BlackSPI(spi) {

}

MySpi::~MySpi() {
	// TODO Auto-generated destructor stub
}

unsigned char MySpi::readByte(uint8_t address) {
	uint8_t wbuf[2] = { (uint8_t)(address | BYTE_READ), 0x00 };
	uint8_t rbuf[2];
	transfer(wbuf, rbuf, sizeof(rbuf));
	return rbuf[1];
}
/**	Reads a number of bytes from the sensor
 *
 * @param address: Register address to start reading from
 * @param readData: Return data array pointer
 * @param len: Length of bytes to read, +1. This should also be the sizeof readData
 */
void MySpi::readBytes(uint8_t address, uint8_t* readData, uint8_t len) {
	uint8_t wbuf[len];
	wbuf[0] = { (uint8_t)(address | BYTE_READ | MULTI_BYTE) };
	transfer(wbuf, readData, len);
}
void MySpi::writeByte(unsigned char address, unsigned char data) {
	uint8_t wbuf[2] = { address, data };
	uint8_t rbuf[2];
	transfer(wbuf, rbuf, sizeof(wbuf));
}
void MySpi::writeBytes(unsigned char address, unsigned char *data, unsigned char len) {
	uint8_t wbuf[len + 1];
	wbuf[0] = address | MULTI_BYTE;
	for (uint8_t i = 1; i < (len + 1); i++) {
		wbuf[i] = data[i-1];
	}
	uint8_t rbuf[len + 1];
	transfer(wbuf, rbuf, sizeof(wbuf));
}
