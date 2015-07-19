//============================================================================
// Name        : main.cpp
// Author      : Kyle David
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>

#include "../include/myi2c.h"
//#includ e "../include/regaddr.h"
#include "../include/BlackLib/BlackSPI/BlackSPI.h"
#include "../include/BlackLib/BlackGPIO/BlackGPIO.h"
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

//	struct sigaction sa;
//	struct itimerval timer;
//	memset ( &sa, 0, sizeof ( sa ) );
//
//	sa.sa_handler = &timer_handler;
//	sigaction ( SIGALRM, &sa, NULL );
//
//	timer.it_value.tv_sec = 0;
//	timer.it_value.tv_usec = 100000;
//	timer.it_interval.tv_sec = 0;
//	timer.it_interval.tv_usec = 100000;
//
//	setitimer ( ITIMER_REAL, &timer, NULL );

	// create device objects and initialize
//	HMC::HMC5883L hmc;
//	hmc.setModeRegister(HMC::ContinuousMeasurement);
//	hmc.setConfigRegA(HMC::DATA_RATE_75 | HMC::MEAS_MODE_NORM);
//	hmc.setConfigRegB(HMC::GAIN_0);
//
//	L3G::L3G4200D l3g(L3G::dps_500);

	// set up GPIO interrupt
	BlackLib::BlackGPIO adxInt1(BlackLib::GPIO_60, BlackLib::input, BlackLib::SecureMode);

	ADX::ADXL345 adx;
	//adx.startSelfTest();
	//adx.resetOffset();
	adx.calibrateOffset();

	ADX::DataFormat format;
	format.fullRes = 1;
	format.range = ADX::DataRange4g;
	format.justify = 0;
	adx.setDataFormat(format);	// value of 0x0B sets full resolution mode and range to +/- 16g
	adx.setPowerCtrl(0x08);		// value of 0x08 enables measurement mode
	//adx.setInterruptEnable(0x00);	// disables interrupts
	adx.setInterruptEnable(0x80);	// value of 0x80 enables DataReady bit
	ADX::PwrDataRate odr(false, ADX::ODR_1600); // set data rate to 100Hz
	adx.setDataRate(odr);

	// wait 1.1ms + 1/ODR
	struct timespec waitTime = adx.getInitWaitTime();
	nanosleep(&waitTime, NULL);

	ofstream mFile;
	mFile.open("acceldata.csv", ios::out);
	mFile << "X,Y,Z" << endl;

	long counter = 0;

	// main program loop
	while (true)
	{
		if (adxInt1.isHigh()) {
			// check interrupt source

			ADX::Data d = adx.getXYZ();
			d.convertToG(format);
			//cout << counter << ": " << d.toString() << endl;
			mFile << d.toString(false, ',') << endl;
			cout << d.toString(false) << endl;
			counter++;
		}
		if (counter > 5000) break;



//		if (updateDataFlag)
//		{
////			if (hmc.getStatus().DataReady)
////			{
////				HMC::Data data = hmc.getDataXYZ();
////				cout << data.toString(false) << endl;
////				printf("Heading (deg): %f\n", data.getHeadingDeg());
////			}
////			else {
////				cout << "Data not ready" << endl;
////			}
//
//			ADX::Data d = adx.getXYZ();
//			cout << d.toString() << endl;
//
//			updateDataFlag = false;
//		}
	}

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
	mFile.close();
	return 0;
}
