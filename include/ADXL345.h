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

#define BYTE_READ		0x80
#define MULTI_BYTE	 	0x40
#define MULTI_BYTE_READ	(BYTE_READ | MULTI_BYTE)	// 0xC0
#define BYTE_WRITE		0x00


#define DATA_RATE_OFFSET	6

#define SELF_TEST_MIN_X_16	6
#define SELF_TEST_MAX_X_16	67
#define SELF_TEST_MIN_Y_16	-67
#define SELF_TEST_MAX_Y_16	-6
#define SELF_TEST_MIN_Z_16	10
#define SELF_TEST_MAX_Z_16	110

#include "../include/BlackLib/BlackSPI/BlackSPI.h"
#include <string>
#include <sstream>

namespace ADX
{




/*	+----------+---------+------------+
	| ODR (Hz) | BW (Hz) | Rate Code  |
	+----------+---------+------------+
	| 3200     | 1600    |  15   1111 |
	| 1600     | 800     |  14   1110 |
	| 800      | 400     |  13   1101 |
	| 400      | 200     |  12   1100 |
	| 200      | 100     |  11   1011 |
	| 100      | 50      |  10   1010 |
	| 50       | 25      |   9   1001 |
	| 25       | 12.5    |   8   1000 |
	| 12.5     | 6.25    |   7   0111 |
	| 6.25     | 3.125   |   6   0110 |
	+----------+---------+------------+	*/
	enum DataRate : uint8_t {
		ODR_3200 	= 0b1111,
		ODR_1600 	= 0b1110,
		ODR_800 	= 0b1101,
		ODR_400 	= 0b1100,
		ODR_200 	= 0b1011,
		ODR_100 	= 0b1010,
		ODR_50 		= 0b1001,
		ODR_25 		= 0b1000,
		ODR_12_5 	= 0b0111,
		ODR_6_25 	= 0b0110
	};
/*	+----+----+----+-----------+----+----+----+----+
	| D7 | D6 | D5 |     D4    | D3 | D2 | D1 | D0 |
	+----+----+----+-----------+----+----+----+----+
	|  0 |  0 |  0 | LOW_POWER |        Rate       |
	+----+----+----+-----------+-------------------+	*/
	struct PwrDataRate {
		bool lowPowerMode;
		DataRate dataRate;
		PwrDataRate(bool lowPowerMode, DataRate rate) {
			this->lowPowerMode = lowPowerMode;
			this->dataRate = rate;
		}
		PwrDataRate(uint8_t reg) {
			lowPowerMode = (reg >> 4) & 0x01;
			dataRate = (DataRate)(reg & 0x0F);
		}
		uint8_t getByteFormat() {
			uint8_t bt = dataRate;
			bt |= lowPowerMode ? 1 : 0;		// sets the low power mode bit to 1 if true
			return bt;
		}
	};


/*	+----+----+---------+
	| D1 | D0 | g Range |
	+----+----+---------+
	|  0 |  0 | ±2 g    |
	|  0 |  1 | ±4 g    |
	|  1 |  0 | ±8 g    |
	|  1 |  1 | ±16 g   |
	+----+----+---------+	*/
	enum DataRange : uint8_t {
		DataRange2g,
		DataRange4g,
		DataRange8g,
		DataRange16g
	};

/*	+-----------+-----+------------+----+----------+---------+----+----+
	| D7        | D6  | D5         | D4 | D3       | D2      | D1 | D0 |
	+-----------+-----+------------+----+----------+---------+----+----+
	| SELF_TEST | SPI | INT_INVERT | 0  | FULL_RES | Justify | Range   |
	+-----------+-----+------------+----+----------+---------+---------+
	FULL_RES: When this bit is set to a value of 1, the device is in full resolution
	mode, where the output resolution increases with the g range
	set by the range bits to maintain a 3.9 mg/LSB scale factor. When
	the FULL_RES bit is set to 0, the device is in 10-bit mode, and
	the range bits determine the maximum g range and scale factor.
*/
	struct DataFormat {
		uint8_t selfTest = 0;
		uint8_t spiMode  = 0;
		uint8_t intInvert= 0;
		uint8_t fullRes	 = 0;
		uint8_t justify  = 0;
		DataRange range	 = DataRange2g;

		DataFormat() { }
		DataFormat(bool useFullRes, DataRange range, uint8_t selfTest = 0, uint8_t spiMode = 0, uint8_t intInvert = 0, uint8_t justify = 0) {
			// assume all other inputs are 0
			this->selfTest = selfTest;
			this->spiMode = spiMode;
			this->intInvert = intInvert;
			this->fullRes = (useFullRes ? 1 : 0);
			this->range = range;
		}
		DataFormat(uint8_t data) {
			selfTest = (data & 0x80) >> 7;
			spiMode  = (data & 0x40) >> 6;
			intInvert= (data & 0x20) >> 5;
			fullRes  = (data & 0x08) >> 3;
			justify  = (data & 0x04) >> 2;
			range 	 = (DataRange)(data & 0x03);
		}
		uint8_t getData() {
			return ((selfTest << 7)  | (spiMode << 6) |
					(intInvert << 5) | (fullRes << 3) |
					(justify << 2)   | range);
		}
	};

	struct Data {
		int16_t x, y, z;
		double 	xg, yg, zg;
		bool 	fullResMode;

//		Data(x, y, z) {
//			this->x = x;
//			this->y = y;
//			this->z = z;
//		}
//		void setX(int16_t x) {
//			this->x = x;
//
//		}
		std::string toString() {
			std::stringstream ss;
			ss << "X: " << x << "\t";
			ss << "Y: " << y << "\t";
			ss << "Z: " << z;
			return ss.str();
		}
		std::string toString(bool raw, char formatSpecifier) {
			std::stringstream ss;
			if (raw) {
				ss << x << formatSpecifier;
				ss << y << formatSpecifier;
				ss << z;
			} else {
				ss << xg << formatSpecifier;
				ss << yg << formatSpecifier;
				ss << zg;
			}
			return ss.str();
		}
		std::string toString(bool rawData) {
			std::stringstream ss;
			if (rawData) {
				ss << "X: " << x << "\t";
				ss << "Y: " << y << "\t";
				ss << "Z: " << z;
			} else {
				ss << "Xg: " << xg << "\t";
				ss << "Yg: " << yg << "\t";
				ss << "Zg: " << zg;
			}

			return ss.str();
		}

		//Convert the accelerometer value to G's.
		//With 10 bits measuring over a +/-4g range we can find how to convert by using the equation:
		//Gs = Measurement Value * (G-range/(2^10)) or Gs = Measurement Value * (8/1024)
		void convertToG(bool fullResMode, DataRange range) {
			double scaleFactor;
			if (fullResMode) {
				scaleFactor = 3.9/1000;
			} else {
				switch (range) {
				case DataRange2g:
					scaleFactor = 4.0/1024;
					break;
				case DataRange4g:
					scaleFactor = 8.0/1024;
					break;
				case DataRange8g:
					scaleFactor = 16.0/1024;
					break;
				case DataRange16g:
					scaleFactor = 32.0/1024;
					break;
				}
			}
			xg = x * scaleFactor;
			yg = y * scaleFactor;
			zg = z * scaleFactor;
		}
		void convertToG(DataFormat format) {
			double scaleFactor;
			if (format.fullRes) {
				scaleFactor = 3.9/1000;
			} else {
				switch (format.range) {
				case DataRange2g:
					scaleFactor = 4.0/1024;
					break;
				case DataRange4g:
					scaleFactor = 8.0/1024;
					break;
				case DataRange8g:
					scaleFactor = 16.0/1024;
					break;
				case DataRange16g:
					scaleFactor = 32.0/1024;
					break;
				}
			}
			xg = x * scaleFactor;
			yg = y * scaleFactor;
			zg = z * scaleFactor;
		}

	};

	struct AvgData {
		double x, y, z;
		std::string toString() {
			std::stringstream ss;
			ss << "X: " << x << "\t";
			ss << "Y: " << y << "\t";
			ss << "Z: " << z;
			return ss.str();
		}
		/**
		 * Overloads the minus (-) operator to define subtraction
		 * of one instance of this object from another
		 * @param a: Subtrahend
		 * @return	The difference of this instance (minuend) and "a"
		 */
		AvgData operator -(const AvgData& a)  {
			AvgData newData;
			newData.x = x - a.x;
			newData.y = y - a.y;
			newData.z = z - a.z;
			return newData;
		}
	};

	enum PwrMode : unsigned char {
		MeasureOff = 0,
		MeasureOn  = 0x08
	};

	class ADXL345
	{
	private:

		BlackLib::BlackSPI spi;
		unsigned char readByte(unsigned char REG_ADDR);
		void readBytes(uint8_t REG_ADDR, uint8_t* readData, uint8_t len);
		void writeByte(unsigned char REG_ADDR, unsigned char data);
		void writeBytes(unsigned char REG_ADDR, unsigned char *data, unsigned char len);
		// output data rate in Hz
		double dataRate;
		const static double DATA_RATE_VAL[];

	public:
		ADXL345();
		~ADXL345();

		unsigned char getDeviceID();
		unsigned char getThresholdTap();
		void setThresholdTap(unsigned char thresh);
		void setAxisOffset(unsigned char *offset);
		void resetOffset();
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
		PwrDataRate getDataRate();
		void setDataRate(PwrDataRate);
		unsigned char getPowerCtrl();
		/**
		 *  +----+----+------+------------+---------+-------+--------+----+
			| D7 | D6 |  D5  |     D4     |   D3    |  D2   |   D1   | D0 |
			+----+----+------+------------+---------+-------+--------+----+
			|  0 |  0 | Link | AUTO_SLEEP | Measure | Sleep |    Wakeup   |
			+----+----+------+------------+---------+-------+--------+----+
		 *
		 * @param
		 */
		void setPowerCtrl(unsigned char);
		unsigned char getInterruptEnable();
		void setInterruptEnable(unsigned char);
		unsigned char getInterruptMapCtrl();
		void setInterruptMapCtrl(unsigned char);
		unsigned char getInterruptSource();
		DataFormat getDataFormat();
		void setDataFormat(DataFormat);
		int16_t getDataX();
		int16_t getDataY();
		int16_t getDataZ();
		Data getXYZ();
		unsigned char getFIFOCtrl();
		void setFIFOCtrl(unsigned char);
		unsigned char getFIFOStatus();
		void setFIFOStatus(unsigned char);
		bool startSelfTest();
		void calibrateOffset();
		AvgData averageDataPoints(uint8_t numPoints);
		struct timespec getInitWaitTime();
		long getWaitTime(bool);
		void waitInitTime();
		void waitTime(bool);
	};

}

#endif /* ADXL345_H_ */
