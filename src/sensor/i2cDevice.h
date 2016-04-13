/*
 * myi2c.h
 *
 *  Created on: Jul 2, 2015
 *      Author: kbdavid15
 */

#ifndef LIB_MYI2C_H_
#define LIB_MYI2C_H_


#define MAX_BUF_SIZE 64

class i2cDevice {
	int 			g_i2cFile;
	unsigned char 	_address;

public:
	unsigned char 	i2c_write_buffer[MAX_BUF_SIZE];
	unsigned char 	i2c_read_buffer[MAX_BUF_SIZE];

	i2cDevice();
	i2cDevice(unsigned char DEVICE_ADDR);
	~i2cDevice();

	void 			i2cOpen();
	void 			i2cClose();
	void 			seti2cAddress(unsigned char address);
	unsigned char 	readByte(unsigned char REG_ADDR);
	void 			readBytes(unsigned char REG_ADDR, unsigned char* readData, unsigned char len);
	void 			writeByte(unsigned char REG_ADDR, unsigned char data);
	void 			writeBytes(unsigned char REG_ADDR, unsigned char *data, unsigned char len);
};
#endif /* LIB_MYI2C_H_ */
