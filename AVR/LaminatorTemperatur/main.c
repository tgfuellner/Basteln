/*

  Laminator-Temperatur-Regelung V2.0
  Copyright 2006 Thomas Pfeifer
  http://thomaspfeifer.net/laminator_temperatur_regelung.htm


  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


  Target: AVR-Attiny13
  Compiler: avr-gcc

  changes:
  16.01.06 TP - detect Sensor break/short
  18.05.09 Thomas - Port to Attiny13

  PB0 = LED red (heating)
  PB1 = Triac
  PB2 = Tx SoftUart
  PB5 = LED green (Ready, temperature is ok)

  PB3 = ADC Input Rolle
  PB4 = ADC Input Alu

*/

#include <avr/io.h>
//#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

// Kleinere Werte bedeuten höhere Temperatur
#define SOLLROLLE 480 // gibt die Solltemperatur (ADC-Wert) an 371-424 := 180 Celsius
#define MINALU 440    // Temperatur am Alu soll nicht höher werden

void Debug(char *Ptr)
{
   #define DEBUG_DDR 	DDRB
   #define DEBUG_PORT	PORTB
   #define DEBUG_BIT	PB2

//Wartezeit in us 
// #define MINDELAY		208   	// fuer 4800 bps
#define MINDELAY		104   	// fuer 9600 bps
// #define MINDELAY		26 		// fuer 38400 bps
// #define MINDELAY 	417		// fuer 2400 bps ACHTUNG nicht mit _delay_us machtbar!!
								// zumindest nicht als einzelner aufruf!
								
   unsigned char i;

   DEBUG_DDR |= _BV(DEBUG_BIT);					// Port als Ausgang
   DEBUG_PORT |= _BV(DEBUG_BIT);				// Port mit HIGH vorbelegen

   while(*Ptr != 0)
   {
      DEBUG_PORT &= ~_BV(DEBUG_BIT);
      _delay_us(MINDELAY);				       // Start Bit

      for (i=0;i<8;i++)                      // Alle 8 Bits
      {
         if (*Ptr & (0x01<<i))
         {
            DEBUG_PORT |= _BV(DEBUG_BIT);
            _delay_us(MINDELAY);
         }
         else
         {
            DEBUG_PORT &= ~_BV(DEBUG_BIT);
			_delay_us(MINDELAY);         }
      }
      DEBUG_PORT |= _BV(DEBUG_BIT);				// Stop Bit
	  _delay_us(MINDELAY);
      Ptr++;
   }
}


void adcinit(void) {
  // Activate ADC with Prescaler 64 --> F_CPU/64
  //ADCSRA = _BV(ADEN) | _BV(ADPS2);
  ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1);
  ADMUX=  0;
}

int getadc(uint8_t channel) {
  // Select pin ADC<channel> using MUX
  ADMUX = channel | _BV(REFS0);

  //Start conversion
  ADCSRA |= _BV(ADSC);

  // wait until converstion completed
  while (ADCSRA & _BV(ADSC) ) {}

    // get converted value
  return(ADCW);
}

void waitms(int ms) {
  int i;
  for (i=0;i<ms;i++) _delay_ms(1);
}

////////////////////////////////////////////////////////

int main (void) {
  int rolle;
  int alu;
  char buffer[5];
  char blink = 0;
  int firstWait =  80; // Der Tempsensor am Alu ist träge. Überschwingen nach einschalten vermeiden.
                       // Pause beim Heizen von firstWait*MEASURE_INTEVALL
                       //
  const char MEASURE_INTERVALL=150; // ms

  adcinit();

  DDRB  = 0b00100011;  // Out = PB1 Triac,  Out = PB0 LED red, Out = PB5 LED green
  PORTB = 0b00000011;  // Start with heating, green LED is off: not ready

  while (1==1) {
    waitms(MEASURE_INTERVALL);

    rolle=getadc(3);     // Use ADC3 (PB3)

    Debug("Sekunde ADC Rolle = ");
    Debug(itoa(rolle, buffer, 10));

    alu=getadc(2);     // Use ADC2 (PB4)
    Debug(", ADC Alu = ");
    Debug(itoa(alu, buffer, 10));

    Debug("\r\n");


    if (rolle<100 || rolle>1000
        || alu<100 || alu>1000) { // detect Sensor contakt break/short
      PORTB = blink;  //off no heating; show problem
      if (!blink) blink=1; else blink=0;
      continue;
    }
    if (rolle < SOLLROLLE) {
      PORTB = 0b00100000;  //off no heating; Temp ok, ready
      continue;
    }
    if (alu-60 < MINALU && firstWait > 0) {
      firstWait--;
      PORTB = 0b00000000;  //off no heating; Alu zu heiss, Rolle muss sich noch erwärmen
      continue;
    }
    if (alu < MINALU) {
      PORTB = 0b00000000;  //off no heating; Alu zu heiss, Rolle muss sich noch erwärmen
      continue;
    }
    if (rolle-10 < SOLLROLLE) {
      PORTB = 0b00100011;  //on heat, Temperatur passt
      continue;
    }
            
    PORTB = 0b00000011;  //on heat

  }

}
