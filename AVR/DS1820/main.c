#include "main.h"
//
// Siehe: http://www.mikrocontroller.net/topic/14792
// Und Elektor Halbleiterheft 7/8-2010 Seite 49

int main( void )
{
  W1_DDR |= (1<<LED_MOD1) | (1<<LED_MOD5);

  sei();

  for(;;){				// main loop
      start_meas();
      // Start indication
      W1_OUT |= (1<<LED_MOD1) | (1<<LED_MOD5);
      _delay_ms(450);
      W1_OUT &= ~((1<<LED_MOD1) | (1<<LED_MOD5));
      _delay_ms(300);
      showTempUsing2LEDs(read_meas());
      _delay_ms(2000);
  }
}
