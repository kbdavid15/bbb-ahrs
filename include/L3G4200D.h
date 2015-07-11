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


#include "../src/BlackLib/BlackSPI/BlackSPI.h"
#include <string>
#include <sstream>

namespace L3G {

	const unsigned char CR4_BLOCK_DATA_UPDATE = 0x80;

	enum CR4_MeasureRange : unsigned char {
		dps_250 = 0,
		dps_500 = 0b00010000,
		dps_2000= 0b00100000
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
				format = dps_2000;
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


	class L3G4200D {

		BlackLib::BlackSPI spi;

	private:
		CR4_MeasureRange _range;
		unsigned char _block_data_update;	// 0x00 for continuous update, 0x80 for update after data read

	public:
		L3G4200D();
		L3G4200D(CR4_MeasureRange);
		~L3G4200D();
		unsigned char getDeviceID();
		Data getXYZ();
		DPS getDPS();
		CR4_MeasureRange getMeasurementRange();
		void setMeasurementRange(CR4_MeasureRange);



	};

}

#endif /* L3G4200D_H_ */
