/*
 * L3G4200D.h
 *
 *  Created on: Jul 9, 2015
 *      Author: kbdavid15
 */

#ifndef L3G4200D_H_
#define L3G4200D_H_

#include "MySpi.h"
#include <string>
#include <sstream>
#include "Sensor.h"

namespace sensor {

/* L3G4200D - 3-Axis Digital Gyroscope
 * Connected to SPI1_0
 * Register Definitions
 */
#define WHO_AM_I		0x0F
#define CTRL_REG1 		0x20
#define CTRL_REG2 		0x21
#define CTRL_REG3 		0x22
#define CTRL_REG4 		0x23
#define CTRL_REG5		0x24
#define REFERENCE 		0x25
#define OUT_TEMP 		0x26
#define STATUS_REG 		0x27
#define OUT_X_L			0x28
#define OUT_X_H 		0x29
#define OUT_Y_L 		0x2A
#define OUT_Y_H 		0x2B
#define OUT_Z_L 		0x2C
#define OUT_Z_H 		0x2D
#define FIFO_CTRL_REG	0x2E
#define FIFO_SRC_REG 	0x2F
#define INT1_CFG		0x30
#define INT1_SRC 		0x31
#define INT1_TSH_XH 	0x32
#define INT1_TSH_XL 	0x33
#define INT1_TSH_YH 	0x34
#define INT1_TSH_YL 	0x35
#define INT1_TSH_ZH 	0x36
#define INT1_TSH_ZL 	0x37
#define INT1_DURATION	0x38



using namespace std;
using namespace BlackLib;

	enum Scale : unsigned char {
		Scale250 = 0,
		Scale500,
		Scale2000
	};
	enum Endian : unsigned char {
		BigEndian, LittleEndian
	};
	enum SpiMode : unsigned char {
		SpiMode4Wire = 0,
		SpiMode3Wire
	};
	enum SelfTest : unsigned char {
		ST_Disabled = 0,
		ST_Enabled_Positive = 1,
		ST_Enabled_Negative = 3
	};

	struct CR4 {
		bool 	 blockDataUpdate = false;	// default: Continuous update
		Endian 	 ble			 = BigEndian;	// LSB @ lower addresses
		SelfTest selfTestEnable  = ST_Disabled;
		Scale 	 scale			 = Scale250;
		SpiMode  mode			 = SpiMode4Wire;

		CR4() {}
		CR4(bool blockDataUpdate, Endian e, SelfTest selfTestEnable, Scale scale, SpiMode mode) {
			this->blockDataUpdate = blockDataUpdate;
			this->ble = e;
			this->selfTestEnable = selfTestEnable;
			this->scale = scale;
			this->mode = mode;
		}
		CR4(uint8_t data) {
			update(data);
		}
		uint8_t getData() {
			return ((blockDataUpdate << 7) | (ble << 6) |
					(scale << 4) | (selfTestEnable << 1) | mode);
		}
		void update(uint8_t data) {
			blockDataUpdate = (data & 0x80) != 0;
			ble  			= (Endian)((data & 0x40) >> 6);
			scale			= (Scale)((data & 0x30) >> 4);
			selfTestEnable  = (SelfTest)((data & 0x06) >> 1);
			mode			= (SpiMode)(data & 0x01);
		}
	};

	enum OutputDataRate : unsigned char {
		ODR_100Hz = 0,
		ODR_200Hz,
		ODR_400Hz,
		ODR_800Hz
	};
	/// Cutoff frequency is dependent on the output data rate
	enum Bandwidth : unsigned char {
		BW_1 = 0,
		BW_2,
		BW_3,
		BW_4
	};
	struct CR1 {
		OutputDataRate 	rate	  = ODR_100Hz;
		Bandwidth		bandwidth = BW_1;
		bool			powerOn	  = false;
		bool			zEnable   = true;
		bool			yEnable   = true;
		bool			xEnable   = true;

		CR1() {}
		CR1(OutputDataRate rate, Bandwidth bandwidth, bool powerOn, bool zEnable, bool yEnable, bool xEnable) {
			this->rate = rate;
			this->bandwidth = bandwidth;
			this->powerOn = powerOn;
			this->zEnable = zEnable;
			this->yEnable = yEnable;
			this->xEnable = xEnable;
		}
		CR1(uint8_t data) {
			update(data);
		}
		uint8_t getData() {
			return ((rate << 6)    | (bandwidth << 4) | (powerOn << 3) |
					(zEnable << 2) | (yEnable << 1)   | xEnable);
		}
		void update(uint8_t data) {
			rate = (OutputDataRate)((data & 0xC0) >> 6);
			bandwidth = (Bandwidth)((data & 0x30) >> 4);
			powerOn	= ((data & 0x08) >> 3);
			zEnable = (data & 0x04) >> 2;
			yEnable	= (data & 0x02) >> 1;
			xEnable = data & 0x01;
		}
	};

	class L3G4200D : public Sensor {

	private:
		MySpi spi;
		CR1 _ControlReg1;
		CR4 _ControlReg4;
		const static double DPS_CONV_VAL[];
		const static double DATA_RATE_VAL[];
		int16_t _xOffset = 0;
		int16_t _yOffset = 0;
		int16_t _zOffset = 0;
	public:
		L3G4200D();
		~L3G4200D();
		unsigned char getDeviceID();
		CR4 getMeasurementRange();
		void setControlReg4(CR4);
		CR1 getControlReg1();
		void setControlReg1(CR1);
		unsigned char getTemperature();
		virtual DataPoint getSensorData();
		void calculateOffset();
		long getWaitTime();
	};
};	/* namespace sensor */
#endif /* L3G4200D_H_ */
