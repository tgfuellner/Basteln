; Malt eine Schraehge Linie
; cat file.gcode >/dev/ttyUSB1
M300S12
M18
G1 X20 Y20 F500
G1 X0 Y40
G1 X20 Y60
G1 X0 Y80
G1 X20 Y100
G1 X0 Y120
G1 X20 Y140
M300S55
G1X0
M18

