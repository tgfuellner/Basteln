#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "../rncontrol.h"
 
 
#ifndef OCR1A
#define OCR1A OCR1  // 2313 support
#endif
 
#ifndef WGM12
#define WGM12 CTC1  // 2313 support
#endif
 
 
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

static void showBinaryTime(unsigned char second, unsigned char minute) {
    minute &= 0x03;         // There are only 2 LEDs left
    minute = minute <<6;    // Use the top two ones
    LED_PORT = ~(second | minute);
}
 
static void showTimeOnUart(unsigned char second, unsigned char minute) {
    char buffer[5];

    uartPuts("Sekunde: ");
    uartPuts(itoa(second, buffer, 10));
    uartPuts("  Minute: ");
    uartPuts(itoa(minute, buffer, 10));
    uartPuts("\r\n");
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
  uartInit();

  sei();
 
  unsigned char lastSecond=100, minute=0;
  for(;;){
    if (lastSecond != second) {
        if( second == 60 ) {
          second = 0;
          minute++;
        }
        lastSecond = second;

        showBinaryTime(second, minute);
        showTimeOnUart(second, minute);
    }
  }
}


/* Vim: use [i to show definition under cursor */
/* vim:set path=/usr/avr/include: */
