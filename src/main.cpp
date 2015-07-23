//============================================================================
// Name        : main.cpp
// Author      : Kyle David
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <ADXL345.h>
#include <HMC5883L.h>
#include <L3G4200D.h>
#include <signal.h>
#include <sys/time.h>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

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
	timer.it_value.tv_usec = 5000;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 5000;

	setitimer ( ITIMER_REAL, &timer, NULL );

	// create device objects and initialize
	HMC5883L hmc;
	hmc.setModeRegister(ContinuousMeasurement);
	hmc.setConfigRegA((ConfigRegA){ AvgSamples1, DataRate_75, NormalMode });
//	hmc.setConfigRegB(GAIN_0);

	L3G4200D l3g;

	ADXL345 adx;
	adx.startSelfTest();
	//adx.resetOffset();
	adx.calibrateOffset();

	DataFormat format;
	format.fullRes = 1;
	format.range = DataRange4g;
	format.justify = 0;
	adx.setDataFormat(format);	// value of 0x0B sets full resolution mode and range to +/- 16g
	adx.setPowerCtrl(0x08);		// value of 0x08 enables measurement mode
	//adx.setInterruptEnable(0x00);	// disables interrupts
	adx.setInterruptEnable(0x80);	// value of 0x80 enables DataReady bit
	PwrDataRate odr(false, ODR_200); // set data rate to 100Hz
	adx.setDataRate(odr);

	// wait 1.1ms + 1/ODR
	struct timespec waitTime = adx.getInitWaitTime();
	nanosleep(&waitTime, NULL);

	ofstream mFile;
	mFile.open("acceldata.csv", ios::out);
	mFile << "AccelX,AccelY,AccelZ,GyroX,GyroY,GyroZ,MagX,MagY,MagZ" << endl;

	long counter = 0;

	// main program loop
	while (true)
	{
		if (updateDataFlag)
		{
			adx.getSensorData();
			mFile << adx.dataToFile(false, ',') << ",";
//			cout << adx.dataToString(false) << endl;

			l3g.getSensorData();
//			cout << l3g.dataToString(false) << endl;
			mFile << l3g.dataToFile(false, ',') << ",";


			//HMC::Data data = hmc.getDataXYZ();
			hmc.getSensorData();
			cout << hmc.dataToString(false) << endl;
//			cout << data.toString(false) << endl;
			mFile << hmc.dataToFile(false, ',') << endl;
//			printf("Heading: %f\n", data.getHeadingDeg());

			counter++;

			updateDataFlag = false;
		}
		if (counter > 500) break;
	}
	mFile.close();
	return 0;
}
