; Malt ein Rechteck
; cat Rechteck.gcode >/dev/ttyUSB1
;M300S12
;G4 P110 (wait 110ms)
;G1 X20 Y0 F600
;G1 X20 Y20
;G1 X-20 Y20
;G1 X-20 Y-20
;G1 X0 Y-20
;G1 X20 Y-20
;G1 X20 Y0
;M300S55
;G4 P110 (wait 110ms)
; 
G0 X0 Y0
M300S12
G4 P110 (wait 110ms)
G1 X1 Y0
G1 X1.5 Y0
G1 X2 Y0
G1 X2.5 Y0
G1 X3 Y0
G1 X3.5 Y0
G1 X4 Y0
G1 X4.5 Y0
G1 X5 Y0
G1 X5.5 Y0
G1 X6 Y0
G1 X6.5 Y0
G1 X7 Y0
G1 X7.5 Y0
G1 X8 Y0
G1 X8.5 Y0
G1 X9 Y0
G1 X9.5 Y0
G1 X10 Y0
G1 X10.5 Y0
G1 X11 Y0
G1 X11.5 Y0
G1 X12 Y0
G1 X12.5 Y0
G1 X13 Y0
G1 X13.5 Y0
G1 X14 Y0
G1 X14.5 Y0
G1 X15 Y0
G1 X15.5 Y0
G1 X16 Y0
G1 X16.5 Y0
G1 X17 Y0
G1 X17.5 Y0
G1 X18 Y0
G1 X18.5 Y0
G1 X19 Y0
G1 X19.5 Y0
G1 X20 Y0
G1 X20.5 Y0
G1 X21 Y0
G1 X21.5 Y0
G1 X22 Y0
G1 X22.5 Y0
G1 X23 Y0
;
G1 X0 Y0
M300S65
G4 P110 (wait 110ms)
G0 X0 Y0
M18
