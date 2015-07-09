//============================================================================
// Name        : main.cpp
// Author      : Kyle David
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>

#include "../include/myi2c.h"
#include "../include/SPI.h"
#include "../include/regaddr.h"
#include "BlackLib/BlackSPI/BlackSPI.h"

using namespace std;

int main() {
	i2cDevice *i2cptr = new i2cDevice(HMC_DEVICE_ADDRESS);
	unsigned char value = i2cptr->readByte(HMC_DEVICE_ADDRESS, HMC_ID_REG_A);
	cout << "Value: " << value << endl;

	i2cptr->readBytes(HMC_DEVICE_ADDRESS, HMC_ID_REG_A, 3);
	for (int i = 0; i < 3; i++)
	{
		cout << i2cptr->i2c_read_buffer[i];
	}
	cout << endl;

	// create new spi device on SPI0 (ADXL)
//	SPI *spi = new SPI();
//	spi->open(2, 1);
//	spi->setMode(0);
//	spi->setSpeed(10000);
//	spi->setBitsPerWord(8);
//	spi->setClockPhase(1);
//	spi->setClockPolarity(1);
//	uint8_t writebuf[1] = { 0x00 };
//	uint8_t readbuf[1];
//
//	spi->xfer1(writebuf, readbuf, 1);
//	printf("SPI read this result: %x\n", readbuf[0]);

	BlackLib::BlackSPI mySpi(BlackLib::SPI0_0, 8, BlackLib::SpiMode3, 2400000);
	mySpi.open( BlackLib::ReadWrite | BlackLib::NonBlock );
	uint8_t result = mySpi.transfer(0x00);
	printf("ADXL345 Device ID (Reg 0x00): %x\n", result);
	return 0;
}
