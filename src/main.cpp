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
#include "../include/ADXL345.h"

using namespace std;

int main() {
	HMC::HMC5883L hmc;
	string devID = hmc.getDeviceID();
	cout << "New Class ID: " << devID << endl;

	hmc.setConfigRegA(HMC::AVG_SAMPLES_4 | HMC::DATA_RATE_15 | HMC::MEAS_MODE_NORM);
	printf("HMC Config Reg A: %x\n", hmc.getConfigRegA());
//	hmc.setConfigRegA(HMC::CRA_DEFAULT);
//	printf("HMC Default Config Reg A: %x\n", hmc.getConfigRegA());
//	hmc.setModeRegister(HMC::ContinuousMeasurement);
//	cout << "Mode Register: " << hmc.getModeRegister() << endl;

	cout << "Data Locked: " << hmc.getStatus().DataLocked << endl;
	cout << "Data Ready: " << hmc.getStatus().DataReady << endl;

	HMC::Data data = hmc.getDataXYZ();
	cout << "X: " << data.x << endl;
	cout << "Y: " << data.y << endl;
	cout << "Z: " << data.z << endl;

	BlackLib::BlackSPI mySpi(BlackLib::SPI1_0, 8, BlackLib::SpiDefault, 100000);
	mySpi.open( BlackLib::ReadWrite | BlackLib::NonBlock );
	uint8_t result = mySpi.transfer(0x0F);	// expected: 0b11010011
	printf("L3G4200D Device ID (0x0F): %x\n", result);

	ADX::ADXL345 adx;
	printf("\nDevice ID: %X\n", adx.getDeviceID());
	return 0;
}
