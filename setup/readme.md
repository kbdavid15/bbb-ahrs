# Device Tree Overlay Setup

The device tree overlays are modified from the ones used in [BlackLib](https://github.com/yigityuce/BlackLib). The DTS enables SPI, I<sup>2</sup>C, and CAN features of Beaglebone Black. 

### Enable cape on boot
Add the following line to uEnv.txt: <pre>cape_enable=capemgr.enable_partno=BBB-AHRS</pre>
For debian 4.1 kernel: http://www.thing-printer.com/cape-manager-is-back-baby/

### FROM HOST PC
- If you are doing these steps from your host pc, you must run "ahrs\_setup\_from_host" script. Before run this, you must give executable permission.
- You can do this with this command: "chmod +x ahrs\_setup\_from_host"
- "BBB-AHRS-00A0.dts" and "ahrs\_setup\_from_host" files must be located in the same directory as the script.
- After do that you must run the script with this command: "./ahrs\_setup\_from_host"
- When you run the script, it will ask you Beaglebone Black's root user's password two times. This is required for executing "scp" and "ssh" command.

### FROM BEAGLEBONE BLACK
- If you are doing these steps from your BBB, you must connect to BBB and
change your current directory to script's directory with "cd" command.
- "BBB-AHRS-00A0.dts" and "ahrs\_setup\_from_BBB" files must be located in this directory.
- Give the script executable permission with this command: "chmod +x ahrs\_setup\_from_BBB"
- Run the script: "./ahrs\_setup\_from_BBB"

# Device Tree on Debian Beaglebone Black
- First run the following script to install the device tree compiler
wget -c https://raw.github.com/RobertCNelson/tools/master/pkgs/dtc.sh 
chmod +x dtc.sh 
./dtc.sh

