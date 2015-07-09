/*
 * HMC5883L.h
 *
 *  Created on: Jul 9, 2015
 *      Author: kbdavid15
 */

#ifndef HMC5883L_H_
#define HMC5883L_H_

#include "../include/myi2c.h"
#include "../include/regaddr.h"
#include <string>
#include <sstream>

namespace HMC {
	// Config Register A Bitmasks
	const unsigned char AVG_SAMPLES_1	= 0b00000000;
	const unsigned char AVG_SAMPLES_2	= 0b00100000;
	const unsigned char AVG_SAMPLES_4	= 0b01000000;
	const unsigned char AVG_SAMPLES_8	= 0b01100000;
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

	// Config Register B Bitmasks
	const unsigned char GAIN_1370		= 0b00000000;
	const unsigned char GAIN_1090		= 0b00100000;
	const unsigned char GAIN_820		= 0b01000000;
	const unsigned char GAIN_660		= 0b01100000;
	const unsigned char GAIN_440		= 0b10000000;
	const unsigned char GAIN_390		= 0b10100000;
	const unsigned char GAIN_330		= 0b11000000;
	const unsigned char GAIN_230		= 0b11100000;
	const unsigned char CRB_DEFAULT		= 0x20;

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
	struct Data {
		int16_t x;
		int16_t y;
		int16_t z;
	};
	enum Status {
		DataLocked,
		DataReady
	};
	enum OperatingMode : unsigned char {
		ContinuousMeasurement,
		SingleMeasurement,
		Idle
	};

	class HMC5883L
	{
		i2cDevice *device;

	public:
		HMC5883L();
		~HMC5883L();

		std::string getDeviceID();
		unsigned char getConfigRegA();
		unsigned char getConfigRegB();
		void setConfigRegA(unsigned char flags);
		void setConfigRegA(ConfigRegA reg);
		void setConfigRegB(unsigned char flags);
		OperatingMode getModeRegister();
		void setModeRegister(OperatingMode mode);
		int16_t getDataX();
		int16_t getDataY();
		int16_t getDataZ();
		Data getDataXYZ();
		Status getStatus();
	};
}


#endif /* HMC5883L_H_ */
