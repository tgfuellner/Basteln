/* -----------------------------------------------------------------------
 * Slave Display; extend to the left
 * Title:    8x8 LED dot matrix animations
 * Author:   Alexander Weber alex@tinkerlog.com
 * Date:     21.12.2008
 * Hardware: ATtiny2313V
 * Software: AVRMacPack
 * 
 */

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "font_5x8.h"



static uint8_t screen_mem[8];			        // screen memory
static uint8_t active_row;		            	// active row
static volatile uint16_t counter = 0;           // used for delay function

// prototypes
void delay_ms(uint16_t delay);
void display_active_row(void);
void clear_screen(void);



/*
 * ISR TIMER0_OVF_vect
 * Handles overflow interrupts of timer 0.
 *
 * 4MHz
 * ----
 * Prescaler 8 * 256 ==> 1953.1 Hz
 * Complete display = 244 Hz
 *
 */
ISR(TIMER0_OVF_vect) {	
  display_active_row();
  counter++;
}



/*
 * delay_ms
 * Uses the counter that is incremented by the ISR.
 * Max delay is 32767ms.
 */
void delay_ms(uint16_t delay) {
  uint16_t t = delay * 2;
  counter = 0;
  while (counter < t) {}
}



/*
 * display_active_row
 * Deactivates the active row and displays the next one.
 * Data is read from screen_mem.
 *
 *      ATtiny2313
 * 16 - PD0    PB7 - 1
 * 15 - PD1    PB6 - 2
 * 14 - PA1    PB5 - 3
 * 13 - PA0    PB4 - 4
 * 12 - PD2    PB3 - 5
 * 11 - PD3    PB2 - 6
 * 10 - PD4    PB1 - 7
 *  9 - PD5    PB0 - 8
 *
 * NFM-12883 common anode          |
 *     A0B5B4D4B2D3D1D0      +-----+
 * PD5 o o o o o o o o       |     |
 * PA1 o o o o o o o o      _+_    |
 * PB0 o o o o o o o o      \ /    |
 * PD2 o o o o o o o o     __V__   |
 * PB7 o o o o o o o o       |     |
 * PB1 o o o o o o o o    ---+-----C---
 * PB6 o o o o o o o o             |
 * PB3 o o o o o o o o
 *
 */
void display_active_row(void) {

  uint8_t row;

  // shut down all rows and columns
  PORTA &= ~(1 << PA0);
  PORTA |= (1 << PA1);

  PORTB = (0 << PB5) | (0 << PB4) | (0 << PB2) | (1 << PB0) | 
          (1 << PB7) | (1 << PB1) | (1 << PB6) | (1 << PB3);

  PORTD &= ~((1 << PD4) | (1 << PD3) | (1 << PD1) | (1 << PD0));
  PORTD |= (1 << PD5) | (1 << PD2);
  // PORTD |=  (1 << PD2);   // Zum debuggen

  // next row
  active_row = (active_row+1) % 8;
  row = screen_mem[active_row];

  // output all columns, switch leds on.
  // column 1
  if ((row & 0x80) == 0x80) {
    PORTA |= (1 << PA0);    
  }
  // column 2
  if ((row & 0x40) == 0x40) {
    PORTB |= (1 << PB5);    
  }
  // column 3
  if ((row & 0x20) == 0x20) {
    PORTB |= (1 << PB4);    
  }
  // column 4
  if ((row & 0x10) == 0x10) {
    PORTD |= (1 << PD4);    
  }
  // column 5
  if ((row & 0x08) == 0x08) {
    PORTB |= (1 << PB2);    
  }
  // column 6
  if ((row & 0x04) == 0x04) {
    PORTD |= (1 << PD3);    
  }
  // column 7
  if ((row & 0x02) == 0x02) {
    PORTD |= (1 << PD1);    
  }
  // column 8
  if ((row & 0x01) == 0x01) {
    PORTD |= (1 << PD0);    
  }

  // activate row
  switch (active_row) {
  case 0:
    PORTD &= ~(1 << PD5);  // Zum debuggen auskommentieren
    break;
  case 1:
    PORTA &= ~(1 << PA1);
    break;
  case 2:
    PORTB &= ~(1 << PB0);
    break;
  case 3:
    PORTD &= ~(1 << PD2);
    break;
  case 4:
    PORTB &= ~(1 << PB7);
    break;
  case 5:
    PORTB &= ~(1 << PB1);
    break;
  case 6:
    PORTB &= ~(1 << PB6);
    break;
  case 7:
    PORTB &= ~(1 << PB3);
    break;
  }

}



/* blit the screen to the left
 * Add right most column
 */
void addColumnToScreen(uint8_t col) {
  int8_t i;

  for (i = 0; i < 8; i++) {
    screen_mem[i] <<= 1; 

    if (col & (1<<i)) {
        screen_mem[i] |= 1;
    } // seting right bit to zero is not needed.
  }
}




/*
 * clear_screen
 */
void clear_screen(void) {
  uint8_t i;
  for (i = 0; i < 8; i++) {
    screen_mem[i] = 0x00;
  }
}

uint8_t readNextColumn() {
  uint8_t col=0;

  uint8_t i;
  for (i = 0; i < 8; i++) {
      // PORTD ^= ( 1 << PD5 );  // Nur zum debuggen

      if (PIND & (1 << PD6)) {
          col |= (1 << i);
          while ((PIND & (1 << PD6))) {}   // Wait untill PD6 is Low
      } else {
          while (!(PIND & (1 << PD6))) {}   // Wait untill PD6 is High
      }

      _delay_ms(4);
  }

  return col;
}

int main(void) {

  uint8_t i = 0;

  // timer 0 setup, prescaler 8
  TCCR0B |= (1 << CS01);
 
  // enable timer 0 interrupt
  TIMSK |= (1 << TOIE0);	

  // define outputs
  DDRA = 0x03;  
  DDRB = 0xFF;
  DDRD = 0x3F;

  // shut down all rows and columns, enable column 1
  PORTA = (1 << PA0) | (1 << PA1);
  PORTB = (0 << PB5) | (0 << PB4) | (0 << PB2) | (1 << PB0) | 
          (1 << PB7) | (1 << PB1) | (1 << PB6) | (1 << PB3);
  PORTD &= ~((1<<PD6) | (1 << PD4) | (1 << PD3) | (1 << PD1) | (1 << PD0));
  PORTD |= (1 << PD5) | (1 << PD2);

  // say hello, toggle row 1 (pixel 0,0)
  for (i = 0; i < 5; i++) {
    PORTD &= ~(1 << PD5);
    _delay_ms(50);
    PORTD |= (1 << PD5);
    _delay_ms(50);
  }

  _delay_ms(50);  // Give master a bit time

  sei();

  // uint8_t columnBits[LEFT_DISPLAY_POS*8] = { 0 };  // All zero
  // int8_t indexIntoColumnBits = 0;
  uint8_t col;

  while (1) {

      while (!(PIND & (1 << PD6))) {}   // Wait untill PD6 is High
      _delay_ms(4);

      /*
      col = columnBits[indexIntoColumnBits];
      columnBits[indexIntoColumnBits++] = readNextColumn();
      if (indexIntoColumnBits >= LEFT_DISPLAY_POS*8) {
          indexIntoColumnBits = 0;
      }
      */
      col = readNextColumn();

      addColumnToScreen(col);

      delay_ms(20);
  }

  return 0;

}

