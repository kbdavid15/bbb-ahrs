//============================================================================
// Name        : main.cpp
// Author      : Kyle David
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <signal.h>
#include <sys/time.h>
#include <cstring>
#include <ctime>
#include <string>
#include <iostream>

#include "AHRS.h"

#define LOG_FILE = 1;

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

std::string getCurrentDateTime() {
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time (&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer,80,"%Y-%m-%d_%H-%M-%S",timeinfo);
	return std::string(buffer);
}

int main() {
	setup_interrupt();
	AHRS ahrs;
	ahrs.init();

#ifdef LOG_FILE
	ofstream mFile;
	std::string filename = "data/data_" + getCurrentDateTime() + ".csv";

	mFile.open(filename.c_str(), ios::out);
	mFile << "AccelX,AccelY,AccelZ,GyroX,GyroY,GyroZ,MagX,MagY,MagZ,IntGyroX,IntGyroY,IntGyroZ" << endl;
//	mFile << "AccelX,AccelY,AccelZ,Pitch,Roll,IntAccelX,GyroX,GyroY,GyroZ,IntGyroZ,MagX,MagY,MagZ,Heading,Yaw,MadPitch,MadRoll,MadHeading" << endl;
#endif

	long counter = 0;

//	ahrs.accel.setLPF(0.05);
//	ahrs.gyro.setLPF(0.005);

	// main program loop
	while (true)
	{
		if (updateDataFlag)
		{
			ahrs.updateData();
#ifdef LOG_FILE
			ahrs.printToFile(mFile);
			ahrs.gyro.trapezoidal();
//			ahrs.accel.trapezoidal();
			mFile << ahrs.gyro.trap.toFile(',') << endl;
#endif

//			DataPoint filt_accelp = adx.getLPFData();
//			DataPoint filt_gyrop = l3g.getLPFData();

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

#ifdef LOG_FILE
//			mFile << pitch * (180/PI) << ",";
//			mFile << roll * (180/PI) << ",";
//			mFile << adx.trapX(SAMPLE_RATE_uS) << ",";
//			mFile << heading << ",";
//			mFile << yaw << ",";
//			mFile << madPitch << ",";
//			mFile << madRoll << ",";
//			mFile << madHeading << ",";
//			mFile << endl;
#endif
			counter++;
			updateDataFlag = false;
		}
		if (counter > 1000) break;
	}

#ifdef LOG_FILE
	mFile.close();
#endif
	cout << "Exiting..." << endl;
	return 0;
}
