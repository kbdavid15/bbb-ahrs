//============================================================================
// Name        : main.cpp
// Author      : Kyle David
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <math.h>
#include <signal.h>
#include <sys/time.h>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

#include "ADXL345.h"
#include "can-utils/hscan.h"
#include "can-utils/txmsg.h"
#include "DataPoint.h"
#include "HMC5883L.h"
#include "L3G4200D.h"
#include "Sensor.h"

using namespace std;

bool updateDataFlag = true;
const long SAMPLE_RATE_uS = 5000;

void timer_handler(int signum) {
	updateDataFlag = true;
}

int main() {
//	bcmserver server;
//	char * testperiod[] = { "can0", "A", "1", "0", "123", "1", "FE"};
//	server.messagecommand(7, testperiod);

	struct sigaction sa;
	struct itimerval timer;
	memset ( &sa, 0, sizeof ( sa ) );

	sa.sa_handler = &timer_handler;
	sigaction ( SIGALRM, &sa, NULL );

	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = SAMPLE_RATE_uS;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = SAMPLE_RATE_uS;

	setitimer ( ITIMER_REAL, &timer, NULL );

	// create device objects and initialize
	HMC5883L hmc;
	hmc.setModeRegister(ContinuousMeasurement);
	hmc.setConfigRegA((ConfigRegA){ AvgSamples1, DataRate_75, NormalMode });
//	hmc.setConfigRegB(GAIN_0);

	L3G4200D l3g;
	l3g.calculateOffset();

	ADXL345 adx;
	adx.startSelfTest();
	adx.resetOffset();
	//adx.calibrateOffset();	issue with z axis calibration (COMPUNDING CALIBRATIONS, NEED TO RESET BEFORE CALCULATING)

	DataFormat format;
	format.fullRes = 1;
	format.range = DataRange2g;
	format.justify = 0;
	adx.setDataFormat(format);	// value of 0x0B sets full resolution mode and range to +/- 16g
	adx.setPowerCtrl(0x08);		// value of 0x08 enables measurement mode
	//adx.setInterruptEnable(0x00);	// disables interrupts
	adx.setInterruptEnable(0x80);	// value of 0x80 enables DataReady bit
	PwrDataRate odr(false, ODR_200); // set data rate to 100Hz
	adx.setDataRate(odr);
	adx.setLPF(0.5);
	// wait 1.1ms + 1/ODR
	struct timespec waitTime = adx.getInitWaitTime();
	nanosleep(&waitTime, NULL);

	ofstream mFile;
	mFile.open("acceldata.csv", ios::out);
	mFile << "AccelX,AccelY,AccelZ,Pitch,Roll,IntAccelX,GyroX,GyroY,GyroZ,IntGyroZ,MagX,MagY,MagZ,Heading,Yaw" << endl;

	long counter = 0;

	// send can frame
	can mCan;
	unsigned char test[] = { 'h', 'e', 'l', 'l', 'o', ' ', ' ', ' ' };
//	mCan.sendframe(0x513, 8, test);
	txmsg devid(0x513, 8, test, 100);
	bcm_message can_device_id = mCan.add_message(devid);

	// main program loop
	while (true)
	{
		if (updateDataFlag)
		{
			DataPoint p = adx.getSensorData();
			p = adx.getLPFData();
			mFile << p.toFile(false, ',') << ",";
			double pitch = adx.getPitch();
			double roll = adx.getRoll();
			mFile << pitch * (180/PI) << ",";
			mFile << roll * (180/PI) << ",";
			mFile << adx.trapX(SAMPLE_RATE_uS) << ",";
//			cout << p.toString(false) << endl;

			p = l3g.getSensorData();
			cout << p.toString(false) << endl;
			mFile << p.toFile(false, ',') << ",";
			mFile << l3g.trapZ(SAMPLE_RATE_uS) << ",";

			p = hmc.getSensorData();
//			cout << p.toString(false) << endl;
			mFile << p.toFile(false, ',') << ",";
			mFile << hmc.getHeadingDeg() << ",";

			// calculate yaw rate
			double XH = (p.getXf() * cos(pitch)) +
						(p.getYf() * sin(pitch) * sin(roll)) +
						(p.getZf() * sin(pitch) * cos(roll));
			double YH = (p.getYf() * cos(roll)) + (p.getZf() * sin(roll));
			double yaw = atan(-YH/XH) * (180/PI);	// why not atan2?

			mFile << yaw << endl;

			counter++;

			updateDataFlag = false;
		}
		if (counter > 500) break;
		if (counter == 250) {
			can_device_id.frame.data[6] = 'w';
			mCan.update_message(can_device_id);
		}
	}
	mFile.close();

	return 0;
}
