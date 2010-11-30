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
 
#define PHASE_A	(PIND & 1<<PIND2)
#define PHASE_B (PIND & 1<<PIND3)


static volatile char  enc_delta=0;    // -128 ... 127
static volatile int count;
 
 
int main( void ) {
  LED_DIR = 0xFF;
  DDRD = 0;     // Input
  PORTD = 0xff; // Pullup

  TCCR0 = 1<<CS01;      //divide by 8 * 256
  TCCR0 = 1<<CS00;      //divide by 64 * 256
  TIMSK = 1<<TOIE0;     //enable timer0 interrupt

  sei();
  for(;;) {

    /*
    cli();
    count += enc_delta >> 1;
    enc_delta = 0;
    sei();
    */

      
    cli();
    LED_PORT = ~(enc_delta >> 1);
    sei();

    // LED_PORT = PIND;
  }
}


/* See
 * http://www.mikrocontroller.net/topic/6526#new
 */
ISR (TIMER0_OVF_vect)
{
  static char enc_last = 0x01;
  char i = 0;

  if( PHASE_A )
    i = 1;

  if( PHASE_B )
    i ^= 3;        // convert gray to binary

  i -= enc_last;      // difference new - last

  if( i & 1 ){        // bit 0 = value (1)
    enc_last += i;      // store new as next last

    enc_delta += (i & 2) - 1;    // bit 1 = direction (+/-)
  }
}

