#include "main.h"


void start_meas( void ){
  if( W1_IN & 1<< W1_PIN ){
    w1_command( CONVERT_T );
    W1_OUT |= 1<< W1_PIN;
    W1_DDR |= 1<< W1_PIN;			// parasite power on

  }else{
    //uputsnl( (uchar *)"Short Circuit !" );
  }
}

void showTempUsing2LEDs(uint temp) {
    bit halfDigit = temp & (1<<3);   // 0.5 yes/no
    uchar preComma = temp >> 4;

    // check for negative 
    if ( temp & 0x8000 )  {
        // negative
        preComma ^= 0xff;  // convert to positive => (twos complement)++
        preComma++;
        W1_OUT |= (1<<LED_MOD1) | (1<<LED_MOD5);
        _delay_ms(200);
        W1_OUT &= ~((1<<LED_MOD1) | (1<<LED_MOD5));
        _delay_ms(400);
    }

    for (uchar i = 0; i<(preComma%5); i++) {
        W1_OUT |= (1<<LED_MOD1);
        _delay_ms(200);
        W1_OUT &= ~(1<<LED_MOD1);
        _delay_ms(300);
    }
    if (halfDigit) {
        W1_OUT |= (1<<LED_MOD1);
        _delay_ms(50);
        W1_OUT &= ~(1<<LED_MOD1);
    }
    _delay_ms(400);
    for (uchar i = 0; i<(preComma/10); i++) {
        W1_OUT |= (1<<LED_MOD5);
        _delay_ms(200);
        W1_OUT &= ~(1<<LED_MOD5);
        _delay_ms(300);
    }
    if ((preComma/5) & 1) {
        // Show add 5 with a short blink
        W1_OUT |= (1<<LED_MOD5);
        _delay_ms(50);
        W1_OUT &= ~(1<<LED_MOD5);
    }
}

uint read_meas( void )
{
  uchar id[8], diff;
  //uchar s[30];
  //uchar i;
  uint temp;

  for( diff = SEARCH_FIRST; diff != LAST_DEVICE; ){
    diff = w1_rom_search( diff, id );

    if( diff == PRESENCE_ERR || diff == DATA_ERR ){
      //uputsnl( (uchar *)"No Sensor found" );
      break;
    }

    // temperature sensor
    /*
    uputs( (uchar *)"ID: " );
    for( i = 0; i < 8; i++ ) {
	  sprintf( s, "%02X ", id[i] );
	  uputs( s );
    }
    */
    w1_byte_wr( READ );			// read command
    temp = w1_byte_rd();			// low byte
    temp |= (uint)w1_byte_rd() << 8;		// high byte
    if( id[0] == 0x10 ) {		// 9 -> 12 bit
      temp <<= 3;
    }
    /*
    sprintf( s, "  T: %04X = ", temp );	// hex value
    uputs( s );
    sprintf( s, "%4d.%01døC", temp >> 4, (temp << 12) / 6553 ); // 0.1øC
    uputsnl( s );
    */

    return temp;
  }

  return 0;
}
