
#BBB-AHRS
A cape for [BeagleBone Black](http://beagleboard.org/BLACK), this contains libraries and code to create a 9 DOF Attitude and Heading Reference System. The board is designed to work with the following devices:

 - ADXL345, accelerometer from Analog Devices
 - L3G4200D, gyroscope from ST Microelectronics
 - HMC5883L, magnetometer from Honeywell
 - TJA1051, HS CAN transceiver from NXP

Device | BeagleBone Pins Used
------- |-------------
ADXL345 | SPI0
L3G4200D| SPI1
HMC5883L| I<sup>2</sup>C2
TJA1051 | DCAN1

##Getting Started
The BeagleBone pins are configured at startup using a Device Tree Overlay. To configure the BeagleBone to use the BBB-AHRS cape, compile the device tree overlay file [BBB-AHRS-00A0.dts](https://github.com/kbdavid15/bbb-ahrs/blob/master/setup/BBB-AHRS-00A0.dts). There is a helper bash script in the setup folder to get started.

To see exactly which pins are in use by the BeagleBone, look at the contents of the pingroups file:

    cat /sys/kernel/debug/pinctrl/44e10800.pinmux/pingroups


