/*
 * L3G4200D.h
 *
 *  Created on: Jul 9, 2015
 *      Author: kbdavid15
 */

#ifndef L3G4200D_H_
#define L3G4200D_H_

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

#define BYTE_READ		0x80
#define BYTE_WRITE		0x00
#define MULTI_BYTE		0x40

#define DPS_CONV_250	0.00875
#define DPS_CONV_500	0.0175
#define DPS_CONV_2000	0.07

#define CR4_SCALE_BITS	4
#define CR4_STEST_BITS	1

#include "../include/BlackLib/BlackSPI/BlackSPI.h"
#include <string>
#include <sstream>
#include <SpiData.h>

using namespace std;
using namespace BlackLib;

	const unsigned char CR4_BLOCK_DATA_UPDATE = 0x80;

	enum CR4_MeasureRange : unsigned char {
		dps_250 = 0,
		dps_500 = 0b00010000,
		dps_2000= 0b00100000
	};
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
		SelfTest selfTestEnable  = ST_Disabled;
		Endian 	 ble			 = LittleEndian;	// LSB @ lower addresses
		Scale 	 scale			 = Scale250;
		SpiMode  mode			 = SpiMode4Wire;

		CR4() {}
		CR4(bool blockDataUpdate, SelfTest selfTestEnable, Endian e, Scale scale, SpiMode mode) {
			this->blockDataUpdate = blockDataUpdate;
			this->selfTestEnable = selfTestEnable;
			this->ble = e;
			this->scale = scale;
			this->mode = mode;
		}
		CR4(uint8_t data) {
			blockDataUpdate = (data & 0x80) != 0;
			ble  			= (Endian)((data & 0x40) >> 6);
			scale			= (Scale)((data & 0x30) >> 4);
			selfTestEnable  = (SelfTest)((data & 0x06) >> 1);
			mode			= (SpiMode)(data & 0x01);
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



	struct Data {
		int16_t x, y, z;

		std::string toString() {
			std::stringstream ss;
			ss << "X: " << x << "\t";
			ss << "Y: " << y << "\t";
			ss << "Z: " << z;
			return ss.str();
		}
	};
	struct DPS {
		// holds formatted data
		double dpx, dpy, dpz;
		DPS(Data d, CR4_MeasureRange range) {
			double format = 0;
			switch (range) {
			case dps_250:
				format = DPS_CONV_250;
				break;
			case dps_500:
				format = DPS_CONV_500;
				break;
			case dps_2000:
				format = DPS_CONV_2000;
				break;
			}
			dpx = d.x * format;
			dpy = d.y * format;
			dpz = d.z * format;
		}
		std::string toString() {
			std::stringstream ss;
			ss << "X: " << dpx << "\t";
			ss << "Y: " << dpy << "\t";
			ss << "Z: " << dpz;
			return ss.str();
		}
	};


	class L3G4200D : public SpiData {

	private:
		CR4 _ControlReg4;
		double dataFormatMultiplier;	// updated whenever the scale sensitivity is changed
	public:
		L3G4200D();
		~L3G4200D();
		unsigned char getDeviceID();
		void getXYZ();
//		DPS getDPS();
		CR4 getMeasurementRange();
		void setControlReg4(CR4);
		unsigned char getTemperature();

	};

#endif /* L3G4200D_H_ */
