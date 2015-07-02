//============================================================================
// Name        : myi2c.cpp
// Author      : Kyle David
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include "../lib/myi2c.h"
#include <iostream>
using namespace std;

i2cDevice::i2cDevice() {
	this->i2cOpen();
}

i2cDevice::~i2cDevice() {
	this->i2cClose();
}

void i2cDevice::i2cOpen()
{
	cout << "beagle-i2c opening /dev/i2c-1... ";
	g_i2cFile = open("/dev/i2c-1", O_RDWR);
	if (g_i2cFile < 0) {
		perror("i2cOpen in i2cDevice::i2cOpen");
		exit(1);
	}
	else cout << "OK"<<endl;
}

void i2cDevice::i2cClose()
{
	close(g_i2cFile);
}

void i2cDevice::seti2cAddress(unsigned char address)
{
	cout << "beagle-i2c setting address 0x"<< hex <<(int)address <<"... ";
	if (ioctl(g_i2cFile, I2C_SLAVE, address) < 0) {
		perror("i2cSetAddress error in myI2C::i2cSetAddress");
		exit(1);
	}
	else cout << "OK" <<endl;
}

void i2cDevice::sendByte(unsigned char device_addr, unsigned char reg_addr, unsigned char data)
{
	seti2cAddress(device_addr);
	cout << "beagle-i2c writing 0x"<< hex << (int)data <<" to 0x"<<hex <<(int)device_addr << ", reg 0x" <<hex<<(int)reg_addr <<"... ";
	i2c_write_buffer[0] = reg_addr;
	i2c_write_buffer[1] = data;

	if(write(g_i2cFile, i2c_write_buffer, 2) != 2) {
		perror("Write Error in myI2C::Send_I2C_Byte");
	}
	else cout << "OK";
}

unsigned char i2cDevice::readByte(unsigned char DEVICE_ADDR,unsigned char Reg_ADDR){
	i2c_write_buffer[0] = Reg_ADDR;

	seti2cAddress(DEVICE_ADDR);
	if(write(g_i2cFile, i2c_write_buffer, 1) != 1) {
		perror("Write Error in myI2C::Read_I2C_Byte");
	}
	seti2cAddress(DEVICE_ADDR);
	if(read(g_i2cFile, i2c_read_buffer, 1) !=1){
		perror("Read Error myI2C::Read_I2C_Byte");
	}

	return i2c_read_buffer[0];
}












