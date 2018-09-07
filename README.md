# Thread-Programming-and-Device-driver-in-Zephyr-RTOS-for-distance-measurement-with-EEPROM
NAME: CSE 522 Assignment-3: Measurement in Zephyr RTOS
_______________________________________________________________________________________________________________________________________________________________________________________________________

AUTHORS:  Team-13

Nisarg Trivedi (1213314867)
Nagarjun Chinnari (1213287788)
_______________________________________________________________________________________________________________________________________________________________________________________________________

INCLUDED IN REPOSITORY:

-> HCSR_app (Program Source code)
-> README
-> Report

_______________________________________________________________________________________________________________________________________________________________________________________________________

ABOUT: 

we have developed a distance sensor channel for HC-SR04 and an I2C based EEPROM device in Zephyr RTOS on Galelio. When the board is booted up, two instances of
HCSR04 and one instance of EEPROMM are created. The user is required to enter shell module commands to select the sensor and then read and write data into EEPROM.
The commands are given in the assignment.
_______________________________________________________________________________________________________________________________________________________________________________________________________

SYSTEM REQUIREMENTS:

-> Linux machine for host
-> Zephyr 1.10.0 and SDK: Zephyr-0.9.2
-> 24FC256 CMOS EEPROM
-> Intel Galileo Gen2 board
-> 2 HCSR04 sensors
-> Connecting wires
-> CMake version 3.8.2 or higher is required
_______________________________________________________________________________________________________________________________________________________________________________________________________

SETUP:

-> Install Zephyr
-> Format SD Card as FAT
-> Create the  directories
   efi
   efi/boot
   kernel
-> after cloning find the binary at $ZEPHYR_BASE/boards/x86/galileo/support/grub/bin/grub.efi and copy it to $SDCARD/efi/boot and rename it to bootia32.efi.
-> Create a $SDCARD/efi/boot/grub.cfg file containing: 
   set default=0
   set timeout=10
   menuentry "Zephyr Kernel" {multiboot /kernel/zephyr.strip}

-> Make sure that cmake version is 3.8.2 or higher
-> Export ZEPHYR_GCC_VARIANT=zephyr
-> Export ZEPHYR_SDK_INSTALL_DIR=<sdk installation directory>
-> Export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
-> Write protection pin is IO0.
-> Connect GPIO pins on Galileo Gen 2, IO3 is trigger pin for hc-sr04 1, IO12 is echo pin for hc-sr04 1,IO0 is for EEPROM.
_______________________________________________________________________________________________________________________________________________________________________________________________________

COMPILATION:

-> type: cd $ZEPHYR_BASE/samples/measure_13
-> mkdir build
-> cd build
-> cmake -DBOARD=galileo ..
-> make

-> Copy zephyr.strip file to in $ZEPHYR_BASE/samples/measure_n/build/zephyr to $SDCARD/kernel
-> Put SD Card in board and reboot.
_______________________________________________________________________________________________________________________________________________________________________________________________________

