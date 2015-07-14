/*
 * HMC5883L.h
 *
 *  Created on: Jul 9, 2015
 *      Author: kbdavid15
 */

#ifndef HMC5883L_H_
#define HMC5883L_H_

/* HMC5883L - 3-Axis Digital Compass IC
 * Connected via I2C
 * Register Definitions
+---------+----------------------------+------------+
| Address |            Name            |   Access   |
+---------+----------------------------+------------+
|      00 | Configuration Register A   | Read/Write |
|      01 | Configuration Register B   | Read/Write |
|      02 | Mode Register              | Read/Write |
|      03 | Data Output X MSB Register | Read       |
|      04 | Data Output X LSB Register | Read       |
|      05 | Data Output Z MSB Register | Read       |
|      06 | Data Output Z LSB Register | Read       |
|      07 | Data Output Y MSB Register | Read       |
|      08 | Data Output Y LSB Register | Read       |
|      09 | Status Register            | Read       |
|      10 | Identification Register A  | Read       |
|      11 | Identification Register B  | Read       |
|      12 | Identification Register C  | Read       |
+---------+----------------------------+------------+
 */
#define HMC_DEVICE_ADDRESS	0x1E	// 7 bit address
#define HMC_READ_ADDRESS 	0x3D	// (0x1E << 1) | 0x01
#define HMC_WRITE_ADDRESS 	0x3C	// (0x1E << 1)
#define HMC_CONFIG_REG_A	0x00
#define HMC_CONFIG_REG_B	0x01
#define HMC_MODE_REG		0x02
#define HMC_DATA_2B_START_X	0x03
#define HMC_DATA_2B_START_Y	0x05
#define HMC_DATA_2B_START_Z	0x07
#define HMC_STATUS_REG		0x09
#define HMC_ID_REG_A		0x0A
#define HMC_ID_REG_B		0x0B
#define HMC_ID_REG_C		0x0C

#define GAIN_BIT_OFFSET		5		// shift gain value left 5 bits for register value

#define PI 					3.14159265

#include "../include/myi2c.h"
#include <string>
#include <sstream>
#include <math.h>

namespace HMC {
	// Config Register A Bitmasks
	const unsigned char AVG_SAMPLES_1	= 0b00000000;
	const unsigned char AVG_SAMPLES_2	= 0b00100000;
	const unsigned char AVG_SAMPLES_4	= 0b01000000;
	const unsigned char AVG_SAMPLES_8	= 0b01100000;
	/*
	+-----+-----+-----+------------------+
	| DO2 | DO1 | DO0 | Output Rate (Hz) |
	+-----+-----+-----+------------------+
	|   0 |   0 |   0 | 0.75             |
	|   0 |   0 |   1 | 1.5              |
	|   0 |   1 |   0 | 3                |
	|   0 |   1 |   1 | 7.5              |
	|   1 |   0 |   0 | 15 (Default)     |
	|   1 |   0 |   1 | 30               |
	|   1 |   1 |   0 | 75               |
	|   1 |   1 |   1 | Reserved         |
	+-----+-----+-----+------------------+
	 */
	const unsigned char DATA_RATE_0_75	= 0b00000000;
	const unsigned char DATA_RATE_1_5	= 0b00000100;
	const unsigned char DATA_RATE_3		= 0b00001000;
	const unsigned char DATA_RATE_7_5	= 0b00001100;
	const unsigned char DATA_RATE_15	= 0b00010000;
	const unsigned char DATA_RATE_30	= 0b00010100;
	const unsigned char DATA_RATE_75	= 0b00011000;
	const unsigned char MEAS_MODE_NORM	= 0b00000000;
	const unsigned char MEAS_MODE_POS	= 0b00000001;
	const unsigned char MEAS_MODE_NEG	= 0b00000010;
	const unsigned char CRA_DEFAULT		= 0x10;

	/*
	+-----+-----+-----+-------------------+-------------------+---------------------+-----------------------------+
	| GN2 | GN1 | GN0 | Recommended Range | Gain (LSb/ Gauss) | Resolution (mG/LSb) |        Output Range         |
	+-----+-----+-----+-------------------+-------------------+---------------------+-----------------------------+
	|   0 |   0 |   0 | ± 0.88 Ga         | 1370              | 0.73                | 0xF800–0x07FF (-2048–2047 ) |
	|   0 |   0 |   1 | ± 1.3 Ga          | 1090 (default)    | 0.92                | 0xF800–0x07FF (-2048–2047 ) |
	|   0 |   1 |   0 | ± 1.9 Ga          | 820               | 1.22                | 0xF800–0x07FF (-2048–2047 ) |
	|   0 |   1 |   1 | ± 2.5 Ga          | 660               | 1.52                | 0xF800–0x07FF (-2048–2047 ) |
	|   1 |   0 |   0 | ± 4.0 Ga          | 440               | 2.27                | 0xF800–0x07FF (-2048–2047 ) |
	|   1 |   0 |   1 | ± 4.7 Ga          | 390               | 2.56                | 0xF800–0x07FF (-2048–2047 ) |
	|   1 |   1 |   0 | ± 5.6 Ga          | 330               | 3.03                | 0xF800–0x07FF (-2048–2047 ) |
	|   1 |   1 |   1 | ± 8.1 Ga          | 230               | 4.35                | 0xF800–0x07FF (-2048–2047 ) |
	+-----+-----+-----+-------------------+-------------------+---------------------+-----------------------------+
	*/
	// Config Register B Bitmasks
//	enum GainValue : unsigned char {
//		GAIN_1370		= 0b00000000,
//		GAIN_1090		= 0b00100000,
//		GAIN_820		= 0b01000000,
//		GAIN_660		= 0b01100000,
//		GAIN_440		= 0b10000000,
//		GAIN_390		= 0b10100000,
//		GAIN_330		= 0b11000000,
//		GAIN_230		= 0b11100000,
//		GAIN_DEFAULT	= 0x20
//	};
	enum GainIdx : unsigned char {
		GAIN_0,
		GAIN_1,
		GAIN_2,
		GAIN_3,
		GAIN_4,
		GAIN_5,
		GAIN_6,
		GAIN_7
	};
	struct Gain {
		GainIdx	gIdx;
		GainIdx	prevGain;
		bool 	updateFlag;

		unsigned char getConfigRegB() {
			return (gIdx << GAIN_BIT_OFFSET);
		}
		Gain(GainIdx idx) {
			gIdx = idx;
			prevGain = idx;
			updateFlag = true;
		}
		Gain() {
			init();
		}
		void updateIdx(GainIdx idx) {
			if (idx != gIdx) {
				prevGain = gIdx;
				gIdx = idx;
				updateFlag = true;
			} else {
				updateFlag = false;
			}
		}
		void init() {
			gIdx = GAIN_1;	// default
			prevGain = GAIN_1;
			updateFlag = false;
		}
		GainIdx incrementIdx() {
			unsigned char i = (unsigned char)gIdx;
			i++;
			gIdx = (GainIdx)i;
			return gIdx;
		}
	};

	enum AvgSamples {
		AvgSamples1,
		AvgSamples2,
		AvgSamples4,
		AvgSamples8
	};

	enum DataRate {
		DataRate_0_75,
		DataRate_1_5,
		DataRate_3,
		DataRate_7_5,
		DataRate_15,
		DataRate_30,
		DataRate_75
	};
	enum MeasureMode {
		Normal,
		Positive,
		Negative
	};
	struct ConfigRegA {
		uint8_t reserved		:1;
		AvgSamples avgSamples	:2;
		DataRate dataRate		:3;
		MeasureMode mode 		:2;

		ConfigRegA(AvgSamples a, DataRate d, MeasureMode m) {
			reserved = 0;
			avgSamples = a;
			dataRate = d;
			mode = m;
		}
	};
	struct SelfTestCal {
		int16_t X_STP, Y_STP, Z_STP;
	};
	struct Data {
		int16_t x, y, z;
		double fx, fy, fz;	// format (gauss)

		Data(int16_t x, int16_t y, int16_t z, uint16_t factor) {
			this->x = x;
			this->y = y;
			this->z = z;
			calculateGauss(factor);
		}

		std::string toString() {
			std::stringstream ss;
			ss << "X: " << x << "\t";
			ss << "Y: " << y << "\t";
			ss << "Z: " << z;
			return ss.str();
		}
		std::string toString(bool optionRaw) {
			if (optionRaw) {
				return this->toString();
			} else {
				std::stringstream ss;
				ss << "X: " << fx << "\t";
				ss << "Y: " << fy << "\t";
				ss << "Z: " << fz;
				return ss.str();
			}
		}
		double getHeadingDeg() {
			double deg = 0;
			if (y > 0) {
				deg =  90 - ((atan((double)x/y))*180/PI);
			} else if (y < 0) {
				deg =  270 - ((atan((double)x/y))*180/PI);
			} else if (y == 0 && x < 0) {
				deg = 180.0;
			} else if (y == 0 && x > 0) {
				deg = 0.0;
			}
			return deg;
		}
		void calculateGauss(uint16_t factor) {
			fx = x / (double) factor;
			fy = y / (double) factor;
			fz = z / (double) factor;
		}
	};
	struct Status {
		bool DataLocked;
		bool DataReady;
	};
	enum OperatingMode : unsigned char {
		ContinuousMeasurement,
		SingleMeasurement,
		Idle
	};

	class HMC5883L	{
		const static uint16_t LSB_PER_GAUSS[];
	private:

		i2cDevice *device;
		Gain _gain;
//		GainValue _gainValue;
//		GainValue _previousGain = GAIN_DEFAULT;
		bool _gainChangeFlag;
	public:
		HMC5883L();
		HMC5883L(GainIdx gain);
		~HMC5883L();

		std::string getDeviceID();
		unsigned char getConfigRegA();
		Gain getConfigRegB();
		void setConfigRegA(unsigned char flags);
		void setConfigRegA(ConfigRegA reg);
		void setConfigRegB(GainIdx gain);
		OperatingMode getModeRegister();
		void setModeRegister(OperatingMode mode);
		int16_t getDataX();
		int16_t getDataY();
		int16_t getDataZ();
		Data getDataXYZ();
		Data getDataReadyXYZ(uint16_t timeout = 100);	//blocking function, returns data when it's ready
		Status getStatus();
		bool runSelfTest();
		unsigned char * dumpAllRegisters();
	};
}


#endif /* HMC5883L_H_ */
