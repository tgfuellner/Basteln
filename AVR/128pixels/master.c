/* -----------------------------------------------------------------------
 * Title:    8x8 LED dot matrix animations
 * Author:   Alexander Weber alex@tinkerlog.com
 * Date:     21.12.2008
 * Hardware: ATtiny2313V
 * Software: AVRMacPack
 * 
 */

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "font_5x8.h"

// Change these values to adjust scroll speeds and animation iterations
#define TEXT_SCROLL_SPEED 120      // how fast to scroll the text
#define REPEAT_TEXT 5              // how often to repeat the text if in cycling mode

// How to add a new message:
// * add the new message (only upper case, see font.h)
// * adjust MAX_MESSAGES
// * add the new message to messages
// NOTE: messages may not be longer than 59 chars. Otherwise they will not fit in the buffer.
//                                              123456789012345678901234567890123456789012345678901234567890
const prog_char PROGMEM message_00[] PROGMEM = "  .HALLO Welt 1234. ";
const prog_char PROGMEM message_01[] PROGMEM = "  'Zweiter TEXT' ";
/*
const prog_char PROGMEM message_02[] PROGMEM = "   MATTHIAS ";
const prog_char PROGMEM message_03[] PROGMEM = "   ANDREAS  ";
const prog_char PROGMEM message_04[] PROGMEM = "   GFUELLNER  ";
const prog_char PROGMEM message_05[] PROGMEM = "   MARIA ";
const prog_char PROGMEM message_06[] PROGMEM = "   THOMAS ";
*/


#define MAX_MESSAGES 2
PGM_P PROGMEM messages[] = {
  message_00
  ,message_01
//  ,message_02
//  ,message_03
//  ,message_04
//  ,message_05
//  ,message_06
//  ,message_07
//  ,message_08
//  ,message_09
//  ,message_10
}; 

uint8_t mode_ee EEMEM = 0;                      // stores the mode in eeprom
static uint8_t screen_mem[8];			        // screen memory
static uint8_t active_row;		            	// active row
static uint8_t buffer[60];                      // stores the active message or sprite
static uint8_t message_ptr = 0;                 // points to the active char in the message
static uint8_t message_displayed = 0;           // how often has the message been displayed?
static uint8_t active_char = 0;                 // stores the active char
static uint8_t message_length = 0;              // stores the length of the active message
static uint8_t char_ptr = 0;                    // points to the active col in the char
static volatile uint16_t counter = 0;           // used for delay function

const static uint8_t CHAR_LENGTH = 5;           // stores the length of the active char

// prototypes
void delay_ms(uint16_t delay);
void display_active_row(void);
void show_char();
void clear_screen(void);



/*
 * ISR TIMER0_OVF_vect
 * Handles overflow interrupts of timer 0.
 *
 * 4MHz
 * ----
 * Prescaler 8 ==> 1953.1 Hz
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
    PORTD &= ~(1 << PD5);
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

/*
 * show_char
 * Displays the actual message. 
 * Scrolls the screen to the left and draws new pixels on the right.
 */
void show_char() {
  int8_t i;
  uint8_t b;
  static char needForSpace=1;

  // blit the screen to the left
  // And send lost Byte to next Display
  for (i = 0; i < 8; i++) {
    // Show Bit for reading
    if (screen_mem[i] & 1<<7) {
      PORTD |= (1 << PD6);
    } else {
      PORTD &= ~(1 << PD6);
    }
    delay_ms(4);

    // Trigger for next Bit
    if (screen_mem[i] & 1<<7) {
      PORTD &= ~(1 << PD6);
    } else {
      PORTD |= (1 << PD6);
    }
    delay_ms(2);

    screen_mem[i] <<= 1; 
  }
  PORTD &= ~(1 << PD6);

  // advance a char if needed
  if (char_ptr == CHAR_LENGTH) {
    message_ptr++;
    if (message_ptr == message_length) {
      message_ptr = 0;
      message_displayed++;
    }
    active_char = buffer[message_ptr] - CHAR_OFFSET;
    char_ptr = 0;

    // If char start with a gap throw away this gap
    b = pgm_read_byte(&font[active_char][char_ptr]);
    if (b==0) {
        char_ptr++;
        b = pgm_read_byte(&font[active_char][char_ptr]);
        if (b==0)
            char_ptr++;
    }

    if (needForSpace) {
        return; // this makes the space between two chars
    } else {
        needForSpace=1;
    }
  }
  // read pixels for current column of char
  b = pgm_read_byte(&font[active_char][char_ptr++]);

  // Remove gap at end of char
  if (b==0 && char_ptr == CHAR_LENGTH) {
      needForSpace = 0;
  }

  char row=0;
  for (i = 7; i >= 0; i--) {
    if ((b & (1 << i)) == (1 << i)) {
      screen_mem[row] |= 0x01;
    } 
    row++;
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


int main(void) {

  uint8_t i = 0;
  uint8_t mode = 0;
  uint8_t cycle = 0;

  // timer 0 setup, prescaler 8
  TCCR0B |= (1 << CS01);
 
  // enable timer 0 interrupt
  TIMSK |= (1 << TOIE0);	

  // define outputs
  DDRA |= 0x03;  
  DDRB |= 0xFF;
  DDRD |= 0x3F;

  // output to give signal for dependent controler to draw next col 
  DDRD |= (1 << PD6);

  // shut down all rows and columns, enable column 1
  PORTA = (1 << PA0) | (1 << PA1);
  PORTB = (0 << PB5) | (0 << PB4) | (0 << PB2) | (1 << PB0) | 
          (1 << PB7) | (1 << PB1) | (1 << PB6) | (1 << PB3);
  PORTD &= ~((1<<PD6) | (1 << PD4) | (1 << PD3) | (1 << PD1) | (1 << PD0));
  PORTD |= (1 << PD5) | (1 << PD2);



  // read last mode from eeprom
  // 0 mean cycle through all modes and messages
  mode = eeprom_read_byte(&mode_ee);
  if ((mode == 0) || (mode >= (MAX_MESSAGES + 1))) {
    mode = 1;
    cycle = 1;
  }
  eeprom_write_byte(&mode_ee, mode + 1);  

  // say hello, toggle row 1 (pixel 0,0)
  for (i = 0; i < 5; i++) {
    PORTD &= ~(1 << PD5);
    if (cycle == 1) {
        PORTA &= ~(1 << PA1);
    }
    _delay_ms(50);
    PORTD |= (1 << PD5);
    if (cycle == 1) {
        PORTA |= ~(1 << PA1);
    }
    _delay_ms(50);
  }

  sei();

  while (1) {

    strcpy_P((char *)buffer, (const char*)pgm_read_word(&(messages[mode-1])));
    message_length = strlen((char *)buffer);
    while (message_displayed < REPEAT_TEXT) {

      PORTD |= (1 << PD6);   // Trigger dependent Controler
      delay_ms(2);

      show_char();
      delay_ms(TEXT_SCROLL_SPEED);
    }
    message_displayed = 0;

    // cycle through all modes
    if (cycle) {
      mode++;
      clear_screen();
      if (mode >= (MAX_MESSAGES + 1)) {
        mode = 1;
      }
    }

  }

  return 0;

}

