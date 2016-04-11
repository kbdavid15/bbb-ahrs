/*
 * HMC5883L.h
 *
 *  Created on: Jul 9, 2015
 *      Author: kbdavid15
 */

#ifndef HMC5883L_H_
#define HMC5883L_H_

#include "i2cDevice.h"
#include "Sensor.h"
#include <string>
#include <sstream>
#include <math.h>

namespace sensor {

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
/**
 *
 */
enum GainIdx : unsigned char {
	GAIN_0,//!< GAIN_0 - GAIN_1370
	GAIN_1,//!< GAIN_1 - GAIN_1090 - Default
	GAIN_2,//!< GAIN_2 - GAIN_820
	GAIN_3,//!< GAIN_3 - GAIN_660
	GAIN_4,//!< GAIN_4 - GAIN_440
	GAIN_5,//!< GAIN_5 - GAIN_390
	GAIN_6,//!< GAIN_6 - GAIN_330
	GAIN_7 //!< GAIN_7 - GAIN_230
};


enum AvgSamples {
	AvgSamples1 = 0,
	AvgSamples2,
	AvgSamples4,
	AvgSamples8
};
/**
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
+-----+-----+-----+------------------+*/
enum HMCDataRate {
	DataRate_0_75 = 0,
	DataRate_1_5,
	DataRate_3,
	DataRate_7_5,
	DataRate_15,
	DataRate_30,
	DataRate_75
};
enum MeasureMode {
	NormalMode = 0,
	PositiveBiasMode,
	NegativeBiasMode
};
struct ConfigRegA {
	bool 		reserved 	= false;
	AvgSamples 	avgSamples 	= AvgSamples1;
	HMCDataRate	dataRate 	= DataRate_15;
	MeasureMode mode 		= NormalMode;

	ConfigRegA() {}
	ConfigRegA(AvgSamples a, HMCDataRate d, MeasureMode m) {
		avgSamples = a;
		dataRate = d;
		mode = m;
	}
	ConfigRegA(uint8_t data) {
		reserved 	= (data & 0x80) >> 7;
		avgSamples 	= (AvgSamples)((data & 0x60) >> 5);
		dataRate 	= (HMCDataRate)((data & 0x1C) >> 2);
		mode 		= (MeasureMode)(data & 0x03);
	}
	uint8_t getData() {
		return ((reserved << 7) | (avgSamples << 5) |
				(dataRate << 2) | (mode));
	}
};
struct SelfTestCal {
	int16_t X_STP, Y_STP, Z_STP;
};
struct Status {
	bool DataLocked;
	bool DataReady;
};
enum OperatingMode : unsigned char {
	ContinuousMeasurement = 0,
	SingleMeasurement,
	IdleMode
};

class HMC5883L : public Sensor {
public:
	class Gain {
		//TODO: Convert to nested class instead of struct
	private:
		GainIdx	currentGain;
		GainIdx	prevGain;
		bool 	updateFlag;
		void init();
	public:

		unsigned char getConfigRegB();
		Gain(GainIdx idx);
		Gain();
		void updateIdx(GainIdx idx);

		GainIdx incrementIdx();

		GainIdx getGainIdx() const {
			return currentGain;
		}

		void setGainIdx(GainIdx idx) {
			currentGain = idx;
		}

		GainIdx getPrevGain() const {
			return prevGain;
		}

		bool isUpdateFlag() const {
			return updateFlag;
		}

		void setUpdateFlag(bool updateFlag) {
			this->updateFlag = updateFlag;
		}
	};
private:
	const static uint16_t LSB_PER_GAUSS[];
	i2cDevice device;
	ConfigRegA _CRA;
	Gain _gain;
public:
	HMC5883L();
	HMC5883L(GainIdx gain);
	~HMC5883L();

	std::string getDeviceID();
	void setConfigRegA(ConfigRegA reg);
	ConfigRegA getConfigRegA();
	Gain getConfigRegB();
	void setConfigRegB(GainIdx gain);
	OperatingMode getModeRegister();
	void setModeRegister(OperatingMode mode);
	Status getStatus();
	bool runSelfTest();
	void dumpAllRegisters(unsigned char * regData, unsigned char len);
	virtual DataPoint getSensorData();
	double getHeadingDeg();
};
};	/* namespace sensor */
#endif /* HMC5883L_H_ */
