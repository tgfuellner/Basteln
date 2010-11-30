#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdlib.h>
 
#define LED_DIR     DDRC
#define LED_PORT    PORTC
 
#ifndef OCR1A
#define OCR1A OCR1  // 2313 support
#endif
 
#ifndef WGM12
#define WGM12 CTC1  // 2313 support
#endif

#define BAUD 9600L


/******************* End Config **************************/


 
#define DEBOUNCE    256L        // debounce clock (256Hz = 4msec)
 
 
unsigned char prescaler;
unsigned char volatile second;          // count seconds
 
 
ISR (TIMER1_COMPA_vect)
{
/************************************************************************/
/*          Insert Key Debouncing Here          */
/************************************************************************/
 
#if F_CPU % DEBOUNCE                     // bei rest
  OCR1A = F_CPU / DEBOUNCE - 1;      // compare DEBOUNCE - 1 times
#endif
  if( --prescaler == 0 ){
    prescaler = (unsigned char)DEBOUNCE;
    second++;               // exact one second over
#if F_CPU % DEBOUNCE         // handle remainder
    OCR1A = F_CPU / DEBOUNCE + F_CPU % DEBOUNCE - 1; // compare once per second
#endif
  }
}

/* 	Beliebigen Port wird als Ausgang konfiguriert und dann per Software UART 
	verwendet. In dem Beispiel wird der normale txd Anschluß genutzt aufgrund 
	der Hardwarebeschaltung auf meiner Platine 
	nutzt die _delay_us der AVRLib */

void Debug(char *Ptr)
{
   #define DEBUG_DDR 	DDRD
   #define DEBUG_PORT	PORTD
   #define DEBUG_BIT	PD1

//Wartezeit in us 
   // org #define MINDELAY		104   	// fuer 9600 bps
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

static void showBinaryTime(unsigned char second, unsigned char minute) {
    minute &= 0x03;         // There are only 2 LEDs left
    minute = minute <<6;    // Use the top two ones
    LED_PORT = ~(second | minute);
}

static void showTimeOnUart(unsigned char second, unsigned char minute) {
    char buffer[5];

    Debug("Sekunde: ");
    Debug(itoa(second, buffer, 10));
    Debug("  Minute: ");
    Debug(itoa(minute, buffer, 10));
    Debug("\r\n");
}


 
int main( void ) {
  LED_DIR = 0xFF;
 
  TCCR1B = 1<<WGM12 | 1<<CS10;          // divide by 1
                                        // clear on compare
  OCR1A = F_CPU / DEBOUNCE - 1;          // Output Compare Register
  TCNT1 = 0;                            // Timmer startet mit 0
  second = 0;
  prescaler = (unsigned char)DEBOUNCE;          //software teiler
 
  TIMSK = 1<<OCIE1A;                    // beim Vergleichswertes Compare Match                    
                                        // Interrupt (SIG_OUTPUT_COMPARE1A)
  sei();
 
  unsigned char lastSecond=100, minute=0;
  for(;;){
    if (lastSecond != second) {
        if( second == 60 ) {
          second = 0;
          minute++;
        }
        lastSecond = second;

        showTimeOnUart(second, minute);
        showBinaryTime(second, minute);
    }
  }
}


/* Vim: use [i to show definition under cursor */
/* vim:set path=/usr/avr/include: */
