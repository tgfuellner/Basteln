// Ip wird in Main.c geaendert
http://192.168.0.93:8080/


cd default
make
avrdude -c avrispv2 -P usb -p m168 -U flash:w:uWebSrv.hex

// Brownout 2.7V und Externer Quarz
sudo avrdude -c avrispv2 -P usb -p m168 -U lfuse:w:0xE0:m
sudo avrdude -c avrispv2 -P usb -p m168 -U hfuse:w:0xDD:m

find . -name '*.c' -or -name '*.h' | xargs ctags

