/*
 * myi2c.h
 *
 *  Created on: Jul 2, 2015
 *      Author: kbdavid15
 */

#ifndef LIB_MYI2C_H_
#define LIB_MYI2C_H_

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>

#define MAX_BUF_SIZE 64

class i2cDevice {
	int g_i2cFile;

public:
	i2cDevice();	// constructor
	~i2cDevice();	// destructor

	unsigned char i2c_write_buffer[MAX_BUF_SIZE];
	unsigned char i2c_read_buffer[MAX_BUF_SIZE];

	void i2cOpen();
	void i2cClose();
	void seti2cAddress(unsigned char address);
	void sendByte(unsigned char DEVICE_ADDR, unsigned char REG_ADDR, unsigned char DATA);
	unsigned char readByte(unsigned char DEVICE_ADDR, unsigned char REG_ADDR);

	unsigned char readBytes(unsigned char DEVICE_ADDR, unsigned char REG_ADDR, size_t n);
};
#endif /* LIB_MYI2C_H_ */
