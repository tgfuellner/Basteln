/************************************************************************/
/*									*/
/*			Access Dallas 1-Wire Devices			*/
/*                                                                      */
/*              Author: Peter Dannegger                                 */
/*                      danni@specs.de                                  */
/*                                                                      */
/************************************************************************/
#include "main.h"
#include <util/delay.h>


bit w1_reset(void)
{
  bit err;

  W1_OUT &= ~(1<<W1_PIN);
  W1_DDR |= 1<<W1_PIN;
  _delay_us(480);			// 480 us
  cli();
  W1_DDR &= ~(1<<W1_PIN);
  _delay_us(64);
  err = W1_IN & (1<<W1_PIN);			// no presence detect
  sei();
  _delay_us(480 - 64);
  if( (W1_IN & (1<<W1_PIN)) == 0 )		// short circuit
    err = 1;
  return err;
}


uchar w1_bit_io( bit b )
{
  cli();
  W1_DDR |= 1<<W1_PIN;
  _delay_us(1);
  if( b )
    W1_DDR &= ~(1<<W1_PIN);
  //_delay_us(15 - 1);
  _delay_us(7);
  if( (W1_IN & (1<<W1_PIN)) == 0 )
    b = 0;
  _delay_us(60 - 15);
  W1_DDR &= ~(1<<W1_PIN);
  sei();
  _delay_us(10);
  return b;
}


uint w1_byte_wr( uchar b )
{
  uchar i = 8, j;
  do{
    j = w1_bit_io( b & 1 );
    b >>= 1;
    if( j )
      b |= 0x80;
  }while( --i );
  return b;
}


uint w1_byte_rd( void )
{
  return w1_byte_wr( 0xFF );
}


uchar w1_rom_search( uchar diff, uchar idata *id )
{
  uchar i, j, next_diff;
  bit b;

  if( w1_reset() )
    return PRESENCE_ERR;			// error, no device found
  w1_byte_wr( SEARCH_ROM );			// ROM search command
  next_diff = LAST_DEVICE;			// unchanged on last device
  i = 8 * 8;					// 8 bytes
  do{
    j = 8;					// 8 bits
    do{
      b = w1_bit_io( 1 );			// read bit
      if( w1_bit_io( 1 ) ){			// read complement bit
	if( b )					// 11
	  return DATA_ERR;			// data error
      }else{
	if( !b ){				// 00 = 2 devices
	  if( diff > i ||
	    ((*id & 1) && diff != i) ){
	    b = 1;				// now 1
	    next_diff = i;			// next pass 0
	  }
	}
      }
      w1_bit_io( b );     			// write bit
      *id >>= 1;
      if( b )					// store bit
	*id |= 0x80;
      i--;
    }while( --j );
    id++;					// next byte
  }while( i );
  return next_diff;				// to continue search
}


void w1_command( uchar command )
{
  w1_reset();
  w1_byte_wr( SKIP_ROM );			// to all devices
  w1_byte_wr( command );
}
