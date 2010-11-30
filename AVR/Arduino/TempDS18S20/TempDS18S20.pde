#include <OneWire.h>

//1-wire
OneWire  ds(12);  // on pin 8
#define BADTEMP -1000
//define unique sensor serial code
//call search_devices() to determine codes
byte inside[8]  = {0x10, 0x87, 0x1A, 0xCA, 0x01, 0x08, 0x00, 0xAA};
byte outside[8] = {0x10, 0xEB, 0x46, 0xCA, 0x01, 0x08, 0x00, 0xA0};

void setup(void) {
 
  // start serial port
  Serial.begin(9600);
  digitalWrite(13, HIGH);   // sets the LED on
}

void search_devices()
{
   byte addr[8];
   int i=0;
 
  if ( !ds.search(addr)) {
      Serial.print("No more addresses.\n");
      ds.reset_search();
      return;
  }
  Serial.print("R=");
  for( i = 0; i < 8; i++) {
     Serial.print(addr[i], HEX);
     Serial.print(" ");
  }
  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.print("CRC is not valid!\n");
      return;
  }
 
  if ( addr[0] != 0x10) {
      Serial.print("Device is not a DS18S20 family device.\n");
      return;
  }
}

float get_temp(byte* addr)
{
  byte present = 0;
  byte i;
  byte data[12];
 
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);         // start conversion, with parasite power on at the end
 
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
 
  present = ds.reset();
  ds.select(addr);   
  ds.write(0xBE);         // Read Scratchpad
//  Serial.print("P=");
//  Serial.print(present,HEX);
//  Serial.print(" ");
  for ( i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
//   Serial.print(data[i], HEX);
//   Serial.print(" ");
  }
//  if (lastcrc8 != 0x00)
//     return BADTEMP;
  int temp;
  float ftemp;
  temp = data[0];      // load all 8 bits of the LSB
   
  if (data[1] > 0x80){  // sign bit set, temp is negative
    temp = !temp + 1; //two's complement adjustment
    temp = temp * -1; //flip value negative.
  }

//get hi-rez data
   int cpc;
   int cr = data[6];
   cpc = data[7];
//   Serial.println(cr, HEX);
//   Serial.println(cpc, HEX);
   
   if (cpc == 0)
      return BADTEMP;

   temp = temp >> 1;  // Truncate by dropping bit zero for hi-rez forumua
   ftemp = temp - (float)0.25 + (cpc - cr)/(float)cpc;
//end hi-rez data
   // ftemp = ((ftemp * 9) / 5.0) + 32; //C -> F
   return ftemp;
}


// ########### L O O P ###########
void loop(void) {

   // search_devices(); //enable this line to get sensor device id. comment out when finished
   
   float temp;
   temp = get_temp(inside);
   if (temp>27) {digitalWrite(13, HIGH);} else {digitalWrite(13, LOW);}
   
   Serial.print("Temp inside = ");
   Serial.print(temp*10, DEC); //cant print floats so multiply by 10. ie: 72.1 now = 721 for display purposes

   temp = get_temp(outside);
   
   Serial.print(", outside = ");
   Serial.println(temp*10, DEC); //cant print floats so multiply by 10. ie: 72.1 now = 721 for display purposes

   delay(1000*5);  //5 sec loop
}

