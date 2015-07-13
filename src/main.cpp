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
//#includ e "../include/regaddr.h"
#include "../include/BlackLib/BlackSPI/BlackSPI.h"
#include "../include/HMC5883L.h"
#include "../include/ADXL345.h"
#include "../include/L3G4200D.h"
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

using namespace std;

bool updateDataFlag = true;

void timer_handler(int signum) {
	updateDataFlag = true;
}

int main() {

	struct sigaction sa;
	struct itimerval timer;
	memset ( &sa, 0, sizeof ( sa ) );

	sa.sa_handler = &timer_handler;
	sigaction ( SIGALRM, &sa, NULL );

	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 10000;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 10000;

	setitimer ( ITIMER_REAL, &timer, NULL );

	// create device objects and initialize
	HMC::HMC5883L hmc;
	hmc.setModeRegister(HMC::ContinuousMeasurement);
	//hmc.setConfigRegA(HMC::AVG_SAMPLES_4 | HMC::DATA_RATE_15 | HMC::MEAS_MODE_NORM);

	L3G::L3G4200D l3g(L3G::dps_500);

	ADX::ADXL345 adx;
	adx.setDataFormat(0x08);
	adx.setPowerCtrl(0x08);
	adx.setInterruptEnable(0x80);


	// main progr am loop
	while (true)
	{
		if (updateDataFlag)
		{
			if (hmc.getStatus().DataReady)
			{
				HMC::Data data = hmc.getDataXYZ();
				cout << "X: " << data.x << "\t";
				cout << "Y: " << data.y << "\t";
				cout << "Z: " << data.z << endl;
			}
			else {
				cout << "Data not ready" << endl;
			}

//			ADX::Data d = adx.getXYZ();
//			cout << d.toString() << endl;


			updateDataFlag = false;
		}
	}

//	cout << "Data Locked: " << hmc.getStatus().DataLocked << endl;
//	cout << "Data Ready: " << hmc.getStatus().DataReady << endl;
//	HMC::Data data = hmc.getDataXYZ();
//	cout << "X: " << data.x << endl;
//	cout << "Y: " << data.y << endl;
//	cout << "Z: " << data.z << endl;
//
//
//	printf("L3G4200D ID (0x0F): %x\n", l3g.getDeviceID());
//	printf("L3G4200D Range: %x\n", l3g.getMeasurementRange());
//	printf("Temperature: %d\n", l3g.getTemperature());
//	for (int i = 0; i < 200; i ++) {
//		printf("Temperature: %d\n", l3g.getTemperature());
//		L3G::DPS d = l3g.getDPS();
//		cout << d.toString() << endl;
//		usleep(100 * 1000);
//	}
//
//
//	for (int i = 0; i < 10; i ++) {
//		ADX::Data d = adx.getXYZ();
//		cout << d.toString() << endl;
//		usleep(100 * 1000);
//	}
	return 0;
}
