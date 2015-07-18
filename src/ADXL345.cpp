/*
 * ADXL345.cpp
 *
 *  Created on: Jul 9, 2015
 *      Author: kbdavid15
 */

#include "../include/ADXL345.h"
#include <iostream>
#include <unistd.h>

using namespace std;

namespace ADX
{
	const double ADXL345::DATA_RATE_VAL[] = { 6.25, 12.5, 25, 50, 100, 200, 400, 800, 1600, 3200 };

	ADXL345::ADXL345():spi(BlackLib::SPI0_0, 8, BlackLib::SpiMode3, 2400000) {
		spi.open( BlackLib::ReadWrite | BlackLib::NonBlock );

	}
	ADXL345::~ADXL345() {
		spi.close();
	}

	unsigned char ADXL345::readByte(uint8_t address) {
		uint8_t wbuf[2] = { address | BYTE_READ, 0x00 };
		uint8_t rbuf[2];
		spi.transfer(wbuf, rbuf, sizeof(rbuf));
		return rbuf[1];
	}
	unsigned char * ADXL345::readBytes(uint8_t address, uint8_t len) {
		uint8_t wbuf[len];
		wbuf[0] = { address | MULTI_BYTE_READ };
		uint8_t rbuf[len];
		spi.transfer(wbuf, rbuf, len);
		return rbuf;
	}
	void ADXL345::writeByte(unsigned char address, unsigned char data) {
		uint8_t wbuf[2] = { address, data };
		uint8_t rbuf[2];
		spi.transfer(wbuf, rbuf, sizeof(wbuf));
	}
	void ADXL345::writeBytes(unsigned char address, unsigned char *data, unsigned char len) {
		uint8_t wbuf[len + 1];
		wbuf[0] = address | MULTI_BYTE;
		for (uint8_t i = 1; i < (len + 1); i++) {
			wbuf[i] = data[i];
		}
		uint8_t rbuf[len + 1];
		spi.transfer(wbuf, rbuf, sizeof(wbuf));
	}

	unsigned char ADXL345::getDeviceID() {
		return readByte(DEVICE_ID);
	}
	unsigned char ADXL345::getThresholdTap() {
		return readByte(THRESH_TAP);
	}
	Data  ADXL345::getXYZ() {
		uint8_t *recvBytes = readBytes(DATAX0, 7);
		Data data;
		data.x = ((int)(recvBytes[2] << 8) | (int)recvBytes[1]);
		data.y = ((int)(recvBytes[4] << 8) | (int)recvBytes[3]);
		data.z = ((int)(recvBytes[6] << 8) | (int)recvBytes[5]);
		return data;

	}
	void ADXL345::setDataFormat(DataFormat format) {
		writeByte(DATA_FORMAT, format.getData());
	}
	DataFormat ADXL345::getDataFormat() {
		uint8_t regData = readByte(DATA_FORMAT);
		return DataFormat(regData);
	}
	void ADXL345::setPowerCtrl(unsigned char pwr) {
		writeByte(POWER_CTL, pwr);
	}
	void ADXL345::setInterruptEnable(unsigned char interrupt) {
		writeByte(INT_ENABLE, interrupt);
	}
	PwrDataRate ADXL345::getDataRate() {
		uint8_t regVal = readByte(BW_RATE);
		PwrDataRate pdr(regVal);
		return pdr;
	}
	void ADXL345::setDataRate(PwrDataRate pdr) {
		writeByte(BW_RATE, pdr.getByteFormat());
		dataRate = DATA_RATE_VAL[pdr.dataRate - DATA_RATE_OFFSET];
	}
	bool ADXL345::startSelfTest() {
		//setPowerCtrl(MeasureOff);
		DataFormat data;
		data.fullRes = 1;
		data.range = DataRange16g;
		data.selfTest = 0;
		setDataFormat(data);
		setDataRate((PwrDataRate) { false, ODR_800 });
		setPowerCtrl(MeasureOn);
		waitTime(true);	// waits 11.1 ms for data rate of 100Hz
		AvgData avgNominal = averageDataPoints(100);
		data.selfTest = 1;
		setDataFormat(data);
		waitTime(true);	// waits 11.1 ms for data rate of 100Hz
		AvgData avgSelfTest = averageDataPoints(100);
		cout << avgNominal.toString() << endl;
		cout << avgSelfTest.toString() << endl;
		AvgData diff = avgSelfTest-avgNominal;
		cout << diff.toString() << endl;
	}
	void ADXL345::calibrateOffset() {
		DataFormat format(true, DataRange16g);
		setDataFormat(format);
		setPowerCtrl(MeasureOn);
		setInterruptEnable(0x80);
		setDataRate((PwrDataRate) { false, ODR_800 });
		waitTime(true);	// wait for ODR settling

		AvgData avgd = averageDataPoints(100);

		int8_t sxCal = -(avgd.x / 4);
		int8_t syCal = -(avgd.y / 4);
		int8_t szCal = -((avgd.z - 256) / 4);

		uint8_t calib[3];
		calib[0] = static_cast<unsigned char>(sxCal);
		calib[1] = static_cast<unsigned char>(syCal);
		calib[2] = static_cast<unsigned char>(szCal);
		setAxisOffset(calib);
	}
	AvgData ADXL345::averageDataPoints(uint8_t numPoints) {
		long total_x = 0, total_y = 0, total_z = 0;
		long sleepTimeL= getWaitTime(false); 	// gets time to wait between reads depending on the output data rate
		timespec sleepTimeSpec = { 0, sleepTimeL };

		for (uint8_t i = 0; i < numPoints; i++) {
			Data d = getXYZ();
			total_x += d.x;
			total_y += d.y;
			total_z += d.z;
			nanosleep(&sleepTimeSpec, NULL);
		}
		AvgData avg;
		avg.x = total_x / (double) numPoints;
		avg.y = total_y / (double) numPoints;
		avg.z = total_z / (double) numPoints;
		return avg;
	}
	/**
	 * Calculate the amount of time needed to wait after writing to configuration registers
	 *
	 * @return	Amount of time to wait in nanoseconds
	 */
	timespec ADXL345::getInitWaitTime() {
		double time = 1100000 + (1 / dataRate) * 1000000000;
		long timeL = (long)(time+0.5);
		struct timespec waitTime = { 0, timeL };
		return waitTime;
	}
	void ADXL345::waitInitTime() {
		double time = 1100000 + (1 / dataRate) * 1000000000;
		long timeL = (long)(time+0.5);
		timespec waitTime = { 0, timeL };
		nanosleep(&waitTime, NULL);
	}
	void ADXL345::waitTime(bool initOffset) {
		double time = (1 / dataRate) * 1000000000;
		if (initOffset) {
			time += 1100000;
		}
		long timeL = (long)(time+0.5);
		timespec waitTime = { 0, timeL };
		nanosleep(&waitTime, NULL);
	}
	long ADXL345::getWaitTime(bool initOffset) {
		double time = (1 / dataRate) * 1000000000;
		if (initOffset) {
			time += 1100000;
		}
		return (long)(time+0.5);
	}
	void ADXL345::setAxisOffset(unsigned char *offset) {
		writeBytes(OFFSET_X, offset, 3);
	}
	void ADXL345::resetOffset() {
		uint8_t offset[] = { 0, 0, 0};
		writeBytes(OFFSET_X, offset, 3);
	}
}
