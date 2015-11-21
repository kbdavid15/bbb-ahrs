/*
 * MySpi.h
 *
 *  Created on: Jul 21, 2015
 *      Author: kbdavid15
 */

#ifndef MYSPI_H_
#define MYSPI_H_

#define BYTE_READ		0x80
#define MULTI_BYTE	 	0x40
#define MULTI_BYTE_READ	(BYTE_READ | MULTI_BYTE)	// 0xC0
#define BYTE_WRITE		0x00

#include "../src/BlackLib/BlackSPI/BlackSPI.h"

using namespace BlackLib;

class MySpi: public BlackLib::BlackSPI {
public:
	MySpi(spiName spi, uint8_t spiBitsPerWord, uint8_t spiMode, uint32_t spiSpeed);
	MySpi(spiName spi);
	virtual ~MySpi();
    unsigned char readByte(unsigned char REG_ADDR);
	void readBytes(uint8_t REG_ADDR, uint8_t* readData, uint8_t len);
	void writeByte(unsigned char REG_ADDR, unsigned char data);
	void writeBytes(unsigned char REG_ADDR, unsigned char *data, unsigned char len);
};

#endif /* MYSPI_H_ */
