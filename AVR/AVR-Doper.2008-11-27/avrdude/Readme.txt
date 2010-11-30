This file contains instructions how to patch avrdude to work with
AVR-Doper's HID communication mode.


===========================
Do I Need to Patch Avrdude?
===========================

Before you start, make sure that your version of avrdude has not already
been patched. If you have a binary, run the command

    avrdude -c stk500v2 -P avrdoper -p atmega8

If it answers "Device not found" or "initialization failed" or if it simply
connects, then you have a patched version. If the error message is "No such
file or directory", then it's not patched. All versions since avrdude 5.3
support AVR-Doper in HID mode.

If you have the source code, look for the file ser_avrdoper.c. If it is
included, you have a patched version.


=======================================
How to Patch the Source Code of Avrdude
=======================================

If your version has not already been patched, get the source code of avrdude
(e.g. from http://savannah.nongnu.org/cvs/?group=avrdude) and perform the
following steps:

(1) Add the files ser_avrdoper.c and ser_avrdoper.h to the source directory.

(2) Edit Makefile.am, search for ser_posix.c and add a similar line for
ser_avrdoper.c.

(3) Edit stk500v2.c and search for "usbdevs.h". Immediately after

    #include "usbdevs.h"
add
    #include "ser_avrdoper.h"

Then search for "stk500v2_open" (first occurrence), advance to the line

    pgmtype = PGMTYPE_UNKNOWN;

and after it add the followint code to the function:

    if(strcasecmp(port, "avrdoper") == 0){
#if defined(HAVE_LIBUSB) || defined(WIN32NATIVE)
        serdev = &avrdoper_serdev;
        pgmtype = PGMTYPE_STK500;
#else
        fprintf(stderr, "avrdude was compiled without usb support.\n");
        return -1;
#endif
    }

(4) Re-run autoconf. This is done by running

    ./bootstrap

This step is required in order to propagate the changes in Makefile.am into
Makefile.in. If you can't get autoconf to work, you can attempt the more
complicated edit in Makefile.in instead.

Avrdude is now patched and you can proceed with building it.


===========================
Building Avrdude on Windows
===========================

Avrdude, at least with this patch, requires MinGW in order to compile. MinGW
comes with the headers and libraries of the Windows Driver Development Kit
(DDK). Go to the source directory and type

    ./configure

You may want to add a --prefix= parameter or other options to configure, see
"./configure --help" for more information.

Unfortunately, configure does not get all guesses right. In particular, it
always finds libusb, regardless whether you have it or not. If you don't
have libusb installed (very likely), edit the file "ac_cfg.h", search for
"HAVE_LIBUSB" and delete (or comment out) the line.

Furthermore, USB HID support needs more libraries on Windows than just -lusb.
Edit the file "Makefile", search for "-lusb" (should be found twice) and
replace all occurrences of "-lusb" with "-lhid -lusb -lsetupapi".

That's it. Now you can run "make" to build the code and "make install" to
install it.


====================================
Building Avrdude on Unix or Mac OS X
====================================

Contrary to Windows, you need libusb on Unix and Mac OS X. Make sure it is
installed. You can check whether it is installed with the command

    libusb-config --version

If the command exists, libusb is installed. Then change to the source
directory of avrdude and type

    ./configure

On Mac OS X, you must also specify additional libraries:

    ./configure LDFLAGS="-framework IOKit -framework CoreFoundation"

or if libusb is at a non-standard location, you may even need more options:

    ./configure CPPFLAGS=-I/usr/local/include \
    LDFLAGS="-L/usr/local/lib -framework IOKit -framework CoreFoundation"

In either case you may want to pass additional parameters such as --prefix=
or whatever. See "./configure --help" for details. Make sure that configure
has found libusb by searching the output for "usb".

After configure, simply type "make" to build the code.


(c) 2006 by OBJECTIVE DEVELOPMENT Software GmbH.
http://www.obdev.at/
