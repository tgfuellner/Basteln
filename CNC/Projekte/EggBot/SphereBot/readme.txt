v0.1

This is the firmware for an EggBot-style SphereBot.
The firmware directly interprets GCode send over the serial port.

There will be more information on the SphereBot at http://pleasantsoftware.com/developer/3d (in the near future...)

!!!!!!!!
This sketch needs the following non-standard libraries (install them in the Arduino library directory):
SoftwareServo: http://www.arduino.cc/playground/ComponentLib/Servo
TimerOne: http://www.arduino.cc/playground/Code/Timer1
!!!!!!!!

Copyright 2011 by Eberhard Rensch <http://pleasantsoftware.com/developer/3d>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>

Part of this code is based on/inspired by the Helium Frog Delta Robot Firmware
by Martin Price <http://www.HeliumFrog.com>

To create tags File: ctags --langmap="C++:+.pde" S*


For Testings:
Absolut modus:
    G90
Icremental modus:
    G91

Servo:
    M300S0
    M300S90

Stepper off
    M18

Y = egg rotation Stepper:
    G0X0Y40

X = pen Stepper:
    G0X40Y0
