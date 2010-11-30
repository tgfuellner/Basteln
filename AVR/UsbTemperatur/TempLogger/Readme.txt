BUILDING AND INSTALLING
=======================
This project can be built on Unix (Linux, FreeBSD or Mac OS X) or Windows.

Building on Windows:
You need WinAVR to compile the firmware. A package can be downloaded from:

    WinAVR: http://winavr.sourceforge.net/

To build the firmware with WinAVR, change into the "firmware" directory,
check whether you need to edit the "Makefile" (e.g. change the ISP upload
tool) and type "make" to compile the source code. Before you upload the code
to the device with "make flash", you should set the fuses with "make fuse".

Building on Unix (Linux, FreeBSD and Mac):
You need the GNU toolchain and avr-libc to compile the firmware. See

    http://www.nongnu.org/avr-libc/user-manual/install_tools.html

for instructions on how to install avr-gcc and avr-libc.

To build the firmware, change to the "firmware" directory, edit "Makefile"
to use the programmer of your choice and type "make" to compile the source
code. Before you upload the code to the device with "make flash", you
should set the fuses with "make fuse".



ABOUT THE LICENSE
=================
It is our intention to make our USB driver and this demo application
available to everyone. Moreover, we want to make a broad range of USB
projects and ideas for USB devices available to the general public. We
therefore want that all projects built with our USB driver are published
under an Open Source license. Our license for the USB driver and demo code is
the GNU General Public License Version 2 (GPL2). See the file "License.txt"
for details.

If you don't want to publish your source code under the GPL2, you can simply
pay money for AVR-USB. As an additional benefit you get USB PIDs for free,
licensed exclusively to you. See the file "CommercialLicense.txt" for details.


MORE INFORMATION
================
For more information about Objective Development's firmware-only USB driver
for Atmel's AVR microcontrollers please visit the URL

    http://www.obdev.at/products/avrusb/

A technical documentation of the driver's interface can be found in the
file "firmware/usbdrv/usbdrv.h".


--
(c) 2007 by OBJECTIVE DEVELOPMENT Software GmbH.
http://www.obdev.at/
