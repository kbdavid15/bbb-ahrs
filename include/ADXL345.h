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

#include "../include/BlackLib/BlackSPI/BlackSPI.h"
#include <string>
#include <sstream>

namespace ADX
{
	struct Data {
		int16_t x;
		int16_t y;
		int16_t z;

		std::string toString() {
			std::stringstream ss;
			ss << "X: " << x << "\t";
			ss << "Y: " << y << "\t";
			ss << "Z: " << z;
			return ss.str();
		}
	};

	class ADXL345
	{
		BlackLib::BlackSPI spi;

	public:
		ADXL345();
		~ADXL345();

		unsigned char getDeviceID();
		unsigned char getThresholdTap();
		void setThresholdTap(unsigned char thresh);
		unsigned char getAxisOffsetX();
		void setAxisOffsetX(unsigned char offset);
		unsigned char getAxisOffsetY();
		void setAxisOffsetY(unsigned char offset);
		unsigned char getAxisOffsetZ();
		void setAxisOffsetZ(unsigned char offset);
		unsigned char getTapDuration();
		void setTapDuration(unsigned char dur);
		unsigned char getTapLatency();
		void setTapLatency(unsigned char latency);
		unsigned char getTapWindow();
		void setTapWindow(unsigned char window);
		unsigned char getActivityThreshold();
		void setActivityThreshold(unsigned char thresh);
		unsigned char getInactivityThreshold();
		void setInactivityThreshold(unsigned char thresh);
		unsigned char getInactivityTime();
		void setInactivityTime(unsigned char);
		unsigned char getAxisEnableControl();
		void setAxisEnableControl(unsigned char);
		unsigned char getFreeFallThreshold();
		void setFreeFallThreshold(unsigned char thresh);

		unsigned char getFreeFallTime();
		void setFreeFallTime(unsigned char);
		unsigned char getAxisControlTap();
		void setAxisControlTap(unsigned char);
		unsigned char getTapSource();
		unsigned char getDataRatePwrModeCtrl();
		void setDataRatePwrModeCtrl(unsigned char);
		unsigned char getPowerCtrl();
		void setPowerCtrl(unsigned char);
		unsigned char getInterruptEnable();
		void setInterruptEnable(unsigned char);
		unsigned char getInterruptMapCtrl();
		void setInterruptMapCtrl(unsigned char);
		unsigned char getInterruptSource();
		unsigned char getDataFormat();
		void setDataFormat(unsigned char);
		int16_t getDataX();
		int16_t getDataY();
		int16_t getDataZ();
		Data getXYZ();
		unsigned char getFIFOCtrl();
		void setFIFOCtrl(unsigned char);
		unsigned char getFIFOStatus();
		void setFIFOStatus(unsigned char);



























	};

}

#endif /* ADXL345_H_ */
