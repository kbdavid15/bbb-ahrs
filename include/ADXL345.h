/*
 * ADXL345.h
 *
 *  Created on: Jul 9, 2015
 *      Author: kbdavid15
 */

#ifndef ADXL345_H_
#define ADXL345_H_

/* ADXL345 - Digital Accelerometer
 * Connected to SPI0_0
 * Register Definitions
 */
#define DEVICE_ID		0x00
#define THRESH_TAP		0x1D
#define OFFSET_X		0x1E
#define OFFSET_Y		0x1F
#define OFFSET_Z		0x20
#define TAP_DURATION	0x21
#define TAP_LATENCY		0x22
#define TAP_WINDOW		0x23
#define THRESH_ACT		0x24
#define THRESH_INACT	0x25
#define TIME_INACT		0x26
#define ACT_INACT_CTL	0x27
#define THRESH_FF		0x28
#define TIME_FF			0x29
#define TAP_AXES		0x2A
#define ACT_TAP_STATUS	0x2B
#define BW_RATE			0x2C
#define POWER_CTL		0x2D
#define INT_ENABLE		0x2E
#define INT_MAP			0x2F
#define INT_SOURCE		0x30
#define DATA_FORMAT		0x31
#define DATAX0			0x32
#define DATAX1			0x33
#define DATAY0			0x34
#define DATAY1			0x35
#define DATAZ0			0x36
#define DATAZ1			0x37
#define FIFO_CTL		0x38
#define FIFO_STATUS		0x39

#include "../src/BlackLib/BlackSPI/BlackSPI.h"

namespace ADX
{
	class ADXL345
	{
		BlackLib::BlackSPI spi;

	public:
		ADXL345();
		~ADXL345();

		unsigned char getDeviceID();
		unsigned char getThresholdTap();
		void setThresholdTap(unsigned char thresh);
	};

}

#endif /* ADXL345_H_ */
