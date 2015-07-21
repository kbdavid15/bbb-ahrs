/*
 * L3G4200D.cpp
 *
 *  Created on: Jul 9, 2015
 *      Author: kbdavid15
 */

#include "../include/L3G4200D.h"
#include <iostream>

using namespace std;

	L3G4200D::L3G4200D():spi(BlackLib::SPI1_0, 8, BlackLib::SpiMode3, 2400000) {
		// set up config registers
		spi.writeByte(CTRL_REG1, 0x0F);
		setControlReg4((CR4) { true, ST_Disabled, BigEndian, Scale500, SpiMode4Wire});
	}
//
//	L3G4200D::L3G4200D(CR4_MeasureRange range):SpiData(BlackLib::SPI1_0, 8, BlackLib::SpiMode3, 2400000) {
//		_range = range;
//		_block_data_update = CR4_BLOCK_DATA_UPDATE;
//		// set up config registers
//		writeByte(CTRL_REG1, 0x0F);
//		writeByte(CTRL_REG4, _range | _block_data_update);
//	}

	L3G4200D::~L3G4200D() {
	}

	unsigned char L3G4200D::getDeviceID() {
		return spi.readByte(WHO_AM_I);
	}
//	void L3G4200D::getXYZ() {
//		uint8_t recvBytes[7];
//		spi.readBytes(OUT_X_L, recvBytes, sizeof(recvBytes));
//		if (_ControlReg4.ble == BigEndian) {
//			x = ((recvBytes[2] << 8) | recvBytes[1]);
//			y = ((recvBytes[4] << 8) | recvBytes[3]);
//			z = ((recvBytes[6] << 8) | recvBytes[5]);
//		} else {
//			x = ((recvBytes[1] << 8) | recvBytes[2]);
//			y = ((recvBytes[3] << 8) | recvBytes[4]);
//			z = ((recvBytes[5] << 8) | recvBytes[6]);
//		}
//	}

	void L3G4200D::getSensorData() {
		uint8_t recvBytes[7];
		spi.readBytes(OUT_X_L, recvBytes, sizeof(recvBytes));
		if (_ControlReg4.ble == BigEndian) {
			x = ((recvBytes[2] << 8) | recvBytes[1]);
			y = ((recvBytes[4] << 8) | recvBytes[3]);
			z = ((recvBytes[6] << 8) | recvBytes[5]);
		} else {
			x = ((recvBytes[1] << 8) | recvBytes[2]);
			y = ((recvBytes[3] << 8) | recvBytes[4]);
			z = ((recvBytes[5] << 8) | recvBytes[6]);
		}
	}


//	DPS L3G4200D::getDPS() {
////		Data data = getXYZ();
////		DPS d(data, _range);
////		return d;
//	}
	void L3G4200D::setControlReg4(CR4 reg) {
		spi.writeByte(CTRL_REG4, reg.getData());
		_ControlReg4 = reg;
	}
	CR4 L3G4200D::getMeasurementRange() {
		uint8_t regVal = spi.readByte(CTRL_REG4);
		_ControlReg4.update(regVal);
		return _ControlReg4;
	}
	unsigned char L3G4200D::getTemperature() {
		return spi.readByte(OUT_TEMP);
	}
