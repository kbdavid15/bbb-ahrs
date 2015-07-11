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
#include "../include/L3G4200D.h"
#include <unistd.h>

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

	L3G::L3G4200D l3g(L3G::dps_500);
	printf("L3G4200D ID (0x0F): %x\n", l3g.getDeviceID());
	printf("L3G4200D Range: %x\n", l3g.getMeasurementRange());
	for (int i = 0; i < 100; i ++) {
		L3G::DPS d = l3g.getDPS();
		cout << d.toString() << endl;
		usleep(50 * 1000);
	}

	ADX::ADXL345 adx;
	adx.setDataFormat(0x08);
	adx.setPowerCtrl(0x08);
	adx.setInterruptEnable(0x80);
	for (int i = 0; i < 10; i ++) {
		ADX::Data d = adx.getXYZ();
		cout << d.toString() << endl;
		usleep(100 * 1000);
	}

	printf("\nADXL Device ID: %X\n", adx.getDeviceID());
	printf("Thresh Tap: %X\n", adx.getThresholdTap());
	return 0;
}
