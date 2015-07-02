//============================================================================
// Name        : main.cpp
// Author      : Kyle David
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include "../lib/regaddr.h"
#include "../lib/myi2c.h"

using namespace std;

int main() {
	i2cDevice *i2cptr = new i2cDevice();
	unsigned char value = i2cptr->readByte(HMC_DEVICE_ADDRESS, HMC_ID_REG_A);
	cout << "Value: " << value;
	return 0;
}
