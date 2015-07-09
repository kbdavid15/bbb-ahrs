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
//#include "../include/regaddr.h"
#include "BlackLib/BlackSPI/BlackSPI.h"
#include "../include/HMC5883L.h"

using namespace std;

int main() {
	HMC::HMC5883L *mHMC = new HMC::HMC5883L();
	string devID = mHMC->getDeviceID();
	cout << "New Class ID: " << devID << endl;
//	mHMC->setConfigRegA();
//
//	ConfigRegA a(AvgSamples1, DataRate_3, Normal);
//	cout << sizeof(a) << endl;
	mHMC->setConfigRegA(HMC::AVG_SAMPLES_4 | HMC::DATA_RATE_15 | HMC::MEAS_MODE_NORM);
	printf("HMC Config Reg A: %x\n", mHMC->getConfigRegA());
//	mHMC->setConfigRegA(HMC::CRA_DEFAULT);
//	printf("HMC Default Config Reg A: %x\n", mHMC->getConfigRegA());
	mHMC->setModeRegister(HMC::ContinuousMeasurement);
	cout << "Mode Register: " << mHMC->getModeRegister() << endl;

	HMC::Data data = mHMC->getDataXYZ();
	cout << "X Data: " << data.x << endl;
	cout << "Y Data: " << data.y << endl;
	cout << "Z Data: " << data.z << endl;

	BlackLib::BlackSPI mySpi(BlackLib::SPI0_0, 8, BlackLib::SpiMode3, 2400000);
	mySpi.open( BlackLib::ReadWrite | BlackLib::NonBlock );
	uint8_t result = mySpi.transfer(0x00);
	printf("ADXL345 Device ID (Reg 0x00): %x\n", result);
	return 0;
}
