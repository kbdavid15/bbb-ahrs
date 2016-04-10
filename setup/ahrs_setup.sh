#! /bin/bash
#compile the device tree overlay and copy to the firmware folder
dtc -O dtb -o BBB-AHRS-00A1.dtbo -b 0 -@ BBB-AHRS-00A1.dts
cp -r BBB-AHRS-00A1.dtbo /lib/firmware
rm -rf BBB-AHRS*
