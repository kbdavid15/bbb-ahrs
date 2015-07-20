/*
 * SpiData.h
 *
 *  Created on: Jul 19, 2015
 *      Author: kbdavid15
 */

#ifndef SPIDATA_H_
#define SPIDATA_H_

#define BYTE_READ		0x80
#define MULTI_BYTE	 	0x40
#define MULTI_BYTE_READ	(BYTE_READ | MULTI_BYTE)	// 0xC0
#define BYTE_WRITE		0x00

#include <SensorData.h>
#include <BlackLib/BlackSPI/BlackSPI.h>

using namespace BlackLib;

class SpiData: public SensorData {
private:
	BlackLib::BlackSPI spi;
public:
	unsigned char readByte(unsigned char REG_ADDR);
	void readBytes(uint8_t REG_ADDR, uint8_t* readData, uint8_t len);
	void writeByte(unsigned char REG_ADDR, unsigned char data);
	void writeBytes(unsigned char REG_ADDR, unsigned char *data, unsigned char len);
	SpiData(spiName spi, uint8_t spiBitsPerWord, uint8_t spiMode, uint32_t spiSpeed);
	virtual ~SpiData();
};

#endif /* SPIDATA_H_ */
