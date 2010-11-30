# avrdoper.py - Copyright (c) 2007 Kai Morich (www.kai-morich.de)
#
# a pyserial like interface to the AVR-doper internal UART
# implements a subset of pyserial( http://pyserial.sourceforge.net)
# requires pyusb (http://pyusb.sourceforge.net) which uses libusb(-win32)
#
# This file is distributed under the MIT License:
# 
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use,
# copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following
# conditions:
# 
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
# OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.

# Note:
# the AVR-doper internal UART is slower than a native UART.
# writing data at 19200 baud was 325 vs. 1350 bytes/sec.

import time
import usb 

class SerialException(Exception):
    pass

class Serial:
    __VENDOR_RQ_WRITE=1
    __VENDOR_RQ_READ=2

    def __init__(self, timeout=None, poll=.02):
        """AVR-Doper internal UART. port is immediately opened"""
        # timeout like pyserial. port is fixed to 19200,8,N,1.
        # all other pyserial parameters are not applicable
        # poll is sleep interval in timeout handling
        self.timeout=timeout
        self.poll=poll
        self.handle=None
        self.open()

    def open(self):
        if self.handle:
            return
        for bus in usb.busses():
            for dev in bus.devices:
                if dev.idVendor==0x16c0 and dev.idProduct in (0x05e1,0x05df):
                    handle=dev.open()
                    if handle.getString(dev.iManufacturer, 256)=='obdev.at' and \
                       handle.getString(dev.iProduct, 256)=='AVR-Doper':
                        self.handle=handle
        if not self.handle:
            raise SerialException('AVR-Doper not found')

    def close(self):
        if self.handle:
            del self.handle
            self.handle=None

    def write(self, buffer):
        for c in buffer:
            self.handle.controlMsg(usb.TYPE_VENDOR|usb.RECIP_DEVICE|usb.ENDPOINT_OUT, 
                                       self.__VENDOR_RQ_WRITE, None, ord(c), 0)
        
    def read(self, size=1):
        if self.timeout: t0=time.time()    
        res=''
        while 1:
            buffer = self.handle.controlMsg(usb.TYPE_VENDOR|usb.RECIP_DEVICE|usb.ENDPOINT_IN, 
                                            self.__VENDOR_RQ_READ, size-len(res), 0, 0)
            for b in buffer:
                res+=chr(b<0 and 256+b or b)
            if len(res)==size or self.timeout==0 or (self.timeout!=None and time.time()-t0>self.timeout):
                break # calculate own timeout because controlMsg ignores usb timeout
            if not buffer:
                time.sleep(self.poll)
        return res

    def readline(self):
        line=''
        while 1:
            c=self.read(1)
            if not c: break
            line+=c
            if c=='\n': break
        return line

    def flushInput(self):
        """read and discard AVR-Doper serial buffer"""
        while self.handle.controlMsg(usb.TYPE_VENDOR|usb.RECIP_DEVICE|usb.ENDPOINT_IN,
                                     self.__VENDOR_RQ_READ, 1, 0, 0):
            pass

    def flushOutput(self):
        """there is no accessible output buffer"""
        pass

def test():
    """whatever you want to send/recieve"""
    s=Serial(timeout=0.1)
    for i in range(5):
        s.write('u\n')
        print '<',s.readline(),

if __name__ == '__main__':
    test()
