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
#include "../include/regaddr.h"
#include "BlackLib/BlackSPI/BlackSPI.h"
#include "../include/HMC5883L.h"

using namespace std;

int main() {
	HMC5883L *mHMC = new HMC5883L();
	string devID = mHMC->getDeviceID();
	cout << "New Class ID: " << devID << endl;

//	i2cDevice *i2cptr = new i2cDevice(HMC_DEVICE_ADDRESS);
//	unsigned char value = i2cptr->readByte(HMC_ID_REG_A);
//	cout << "Value: " << value << endl;
//
//	i2cptr->readBytes(HMC_ID_REG_A, 3);
//	for (int i = 0; i < 3; i++)
//	{
//		cout << i2cptr->i2c_read_buffer[i];
//	}
//	cout << endl;

	BlackLib::BlackSPI mySpi(BlackLib::SPI0_0, 8, BlackLib::SpiMode3, 2400000);
	mySpi.open( BlackLib::ReadWrite | BlackLib::NonBlock );
	uint8_t result = mySpi.transfer(0x00);
	printf("ADXL345 Device ID (Reg 0x00): %x\n", result);
	return 0;
}
