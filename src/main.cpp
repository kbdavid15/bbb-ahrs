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
#include "AHRS.h"
#include "ADXL345.h"
#include "can-utils/hscan.h"
#include "can-utils/TxMsg.h"
#include "DataPoint.h"
#include "HMC5883L.h"
#include "L3G4200D.h"
#include "MessageAngularRate.h"
#include "MessageBodyAccel.h"
#include "MessageHeadingPitchRoll.h"
#include "Sensor.h"

extern "C" {
	#include "MadgwickAHRS/MadgwickAHRS.h"
}

#define LOG_FILE = 1;

using namespace std;

bool updateDataFlag = true;
const long SAMPLE_RATE_uS = 10000;	// 10ms for 100Hz

void timer_handler(int signum) {
	updateDataFlag = true;
}

void setup_interrupt() {
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
}

int main() {
	setup_interrupt();

	AHRS ahrs;
	ahrs.init();

#ifdef LOG_FILE
	ofstream mFile;
	mFile.open("acceldata.csv", ios::out);
	mFile << "AccelX,AccelY,AccelZ,Pitch,Roll,IntAccelX,GyroX,GyroY,GyroZ,IntGyroZ,MagX,MagY,MagZ,Heading,Yaw,MadPitch,MadRoll,MadHeading" << endl;
#endif

	long counter = 0;

	// send can frame
	hscan mcan;
	const char * bbb_ahrs_id = "BBB-AHRS";
	mcan.add_message(0x513, 5000, 8, (unsigned char *)bbb_ahrs_id);
	MessageBodyAccel body;
	MessageAngularRate ang_rate(50);
	MessageHeadingPitchRoll hprmsg(50);
	mcan.add_message(body.getMsg());
	mcan.add_message(ang_rate.getMsg());
	mcan.add_message(hprmsg.getMsg());

	ahrs.accel.setLPF(0.05);
	ahrs.gyro.setLPF(0.005);

	// main program loop
	while (true)
	{
		if (updateDataFlag)
		{
			DataPoint accelp = ahrs.accel.getSensorData();
			DataPoint gyrop = ahrs.gyro.getSensorData();
			DataPoint magp = ahrs.compass.getSensorData();

//			DataPoint filt_accelp = adx.getLPFData();
//			DataPoint filt_gyrop = l3g.getLPFData();

			// update MadgwickAHRS
			MadgwickAHRSupdate(gyrop.getXf()* (PI/180), gyrop.getYf()* (PI/180), gyrop.getZf()* (PI/180),
					accelp.getXf(), accelp.getYf(), accelp.getZf(),
					magp.getXf(), magp.getYf(), magp.getZf());

			// update body acceleration can message
			body.updateFrame(accelp);
			mcan.update_message(body.getMsg());
			ang_rate.updateFrame(gyrop);
			mcan.update_message(ang_rate.getMsg());


//			double pitch = adx.getPitch();
//			double roll = adx.getRoll();
//			cout << p.toString(false) << endl;
//			cout << gyrop.toString(false) << endl;
//			cout << p.toString(false) << endl;

//			double heading = hmc.getHeadingDeg();

			// calculate yaw rate
//			double XH = (magp.getXf() * cos(pitch)) +
//						(magp.getYf() * sin(pitch) * sin(roll)) +
//						(magp.getZf() * sin(pitch) * cos(roll));
//			double YH = (magp.getYf() * cos(roll)) + (magp.getZf() * sin(roll));
//			double yaw = atan(-YH/XH) * (180/PI);	// why not atan2?

			float madHeading = atan2(2*q2*q3 - 2*q1*q4, 2*q1*q1 + 2*q2*q2 - 1);
			float madRoll = -asin(2*q2*q4 + 2*q1*q3);
			float madPitch = atan2(2*q3*q4 - 2*q1*q2, 2*q1*q1 + 2*q4*q4 - 1);	// unsure why calculation is off by 180 degrees

//			cout << "Pitch: " << madPitch << "\t";
//			cout << "Roll: " << madRoll << "\t";
//			cout << "Heading: " << madHeading << endl;

			hprmsg.updateFrame(madHeading*(180/PI), madPitch*(180/PI), madRoll*(180/PI));
			mcan.update_message(hprmsg.getMsg());

#ifdef LOG_FILE
//			mFile << pitch * (180/PI) << ",";
//			mFile << roll * (180/PI) << ",";
//			mFile << adx.trapX(SAMPLE_RATE_uS) << ",";
			mFile << accelp.toFile(false, ',') << ",";
			mFile << gyrop.toFile(false, ',') << ",";
//			mFile << magp.toFile(false, ',') << ",";
//			mFile << heading << ",";
//			mFile << yaw << ",";
//			mFile << madPitch << ",";
//			mFile << madRoll << ",";
//			mFile << madHeading << ",";
			mFile << endl;
#endif
			counter++;
			updateDataFlag = false;
		}
//		if (counter > 500) break;
	}

#ifdef LOG_FILE
	mFile.close();
#endif

	return 0;
}
