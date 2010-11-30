#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "countdown.h"

/*
 * Simple Countdown timer. 
 *
 * User input with two buttons to adjust start time.
 * For more info see state.c
 *
 * The remaining time is displayed on a 5x7 LED matrix.
 * The first 2 rows (9LEDs) show the seconds 0-9 (one LED for a digit).
 * The third row shows the 10th seconds 10,20,30,40,50
 * The fourth and 5th row the minutes 0-9
 * The 6th row shows the 10th minutes.
 * The last and 7th row shows hours 0-5.
 *
 * So the max displayable time is 5:59:59
 */
 
 
#ifndef OCR1A
#define OCR1A OCR1  // 2313 support
#endif
 
#ifndef WGM12
#define WGM12 CTC1  // 2313 support
#endif
 
 

// Output
#define BUZZER_PLUS_OnPortD PD5
#define BUZZER_MINUS_OnPortB PB2

#define COL1_OnPortD PD0
#define COL2_OnPortD PD1
#define COL3_OnPortD PD4
#define COL4_OnPortB PB0
#define COL5_OnPortB PB1

#define ROW1_OnPortB PB7
#define ROW2_OnPortB PB6
#define ROW3_OnPortB PB5
#define ROW4_OnPortB PB4
#define ROW5_OnPortB PB3
#define ROW6_OnPortD PD6
#define ROW7_OnPortD PD3

int16_t volatile second;                       // count seconds
uint8_t currentColValues[7] = {0,0,0,0,0,0,0}; // are calculatet every changing second
static uint8_t timeBaseShort;
uint8_t timeBase512ms = 0;

uint16_t *firstSecondInEEprom = (uint16_t *)EEPROM_STORE_START;
 
static uint16_t prescaler;
 
static void showRemainingTimeOnLEDMatrix(void);

void beeper (uint8_t on)	
{
	if (on)
	{
		// Beeper an
        DDRB |= ( 1 << BUZZER_MINUS_OnPortB);  // MAKE_OUT (PORTB_2);
        DDRD |= ( 1 << BUZZER_PLUS_OnPortD);   // MAKE_OUT (PORTD_5);
		
        // PWM: Mode #3 for Timer0
        // dutyA = dutyB = 50%
        OCR0A = 128;
        OCR0B = 128;
        // Prescale = 1 (Timer0 @ 1MHz) --> PWM @ 3.9kHz
        TCCR0A = (1 << WGM01) | (1 << WGM00) | (1 << COM0A1) | (1 << COM0B1) | (1 << COM0B0);
		TCCR0B = 1 << CS00;
	}
	else
	{
		// Beeper aus
		DDRB &= ~(1<<BUZZER_MINUS_OnPortB);  // MAKE_IN (PORTB_2);
		DDRD &= ~(1<<BUZZER_PLUS_OnPortD);   // MAKE_IN (PORTD_5);
		PORTB |= (1<<BUZZER_MINUS_OnPortB);  // SET (PORTB_2);
		PORTD |= (1<<BUZZER_PLUS_OnPortD);   // SET (PORTD_5);
		TCCR0A = 0;
		TCCR0B = 0;
	}
}

#define IRQS_PER_SECOND    512L        // debounce clock (512Hz = 2msec)
#define SHORT_TIME_DIV     22          // ~44ms
 
ISR (TIMER1_COMPA_vect)
{
/************************************************************************/
/*          Insert Key Debouncing Here          */
/************************************************************************/
  if (prescaler%SHORT_TIME_DIV == 0) {
    // True every IRQS_PER_SECOND / SHORT_TIME_DIV times
    timeBaseShort++;
  }

  if( --prescaler == 0 ){
    prescaler = (uint16_t)IRQS_PER_SECOND;
    if (currentState == COUNT || currentState == POWERSAVE) {
      second--;               // exact one second over
    }
#if F_CPU % IRQS_PER_SECOND         // handle remainder
    OCR1A = F_CPU / IRQS_PER_SECOND + F_CPU % IRQS_PER_SECOND - 1; // compare once per second
#endif
  }

  showRemainingTimeOnLEDMatrix();
}


/* Activate one row and never more at the same time.
 * Otherwise a column portbin could be damaged.
 *
 * row: Row, 0=Row1, 1=Row2, ...
 */
static void setRow(uint8_t row) {
    // switch off all rows
    PORTB |= (1<<ROW1_OnPortB)|(1<<ROW2_OnPortB)|(1<<ROW3_OnPortB)
               |(1<<ROW4_OnPortB)|(1<<ROW5_OnPortB);
    PORTD |= (1<<ROW6_OnPortD)|(1<<ROW7_OnPortD);

    if (row==0) {PORTB &= ~(1<<ROW1_OnPortB); return;}
    if (row==1) {PORTB &= ~(1<<ROW2_OnPortB); return;}
    if (row==2) {PORTB &= ~(1<<ROW3_OnPortB); return;}
    if (row==3) {PORTB &= ~(1<<ROW4_OnPortB); return;}
    if (row==4) {PORTB &= ~(1<<ROW5_OnPortB); return;}
    if (row==5) {PORTD &= ~(1<<ROW6_OnPortD); return;}
    if (row==6) {PORTD &= ~(1<<ROW7_OnPortD); return;}
}

/* Activate one column and never more at the same time.
 *
 * col: 0 no active Column, 1=Col1, 2=Col2, ...
 */
static void setCol(uint8_t col) {
    // switch off all columns
    PORTB &= ~( (1<<COL4_OnPortB)|(1<<COL5_OnPortB) );
    PORTD &= ~( (1<<COL1_OnPortD)|(1<<COL2_OnPortD)|(1<<COL3_OnPortD) );

    if (currentState == POWERSAVE) return;

    if (col==1) {PORTD |= (1<<COL1_OnPortD); return;}
    if (col==2) {PORTD |= (1<<COL2_OnPortD); return;}
    if (col==3) {PORTD |= (1<<COL3_OnPortD); return;}
    if (col==4) {PORTB |= (1<<COL4_OnPortB); return;}
    if (col==5) {PORTB |= (1<<COL5_OnPortB); return;}
}

/*
 * Values greater than 5 do no harm because setCol() will not show them.
 */
static uint8_t row1_1to5Seconds(const int16_t seconds) {
    return seconds % 10;
}
static uint8_t row2_6to9Seconds(const int16_t seconds) {
    uint8_t x = (seconds-5)%10;
    if (x>4) x=0;
    return x;
}
/*
 * Values greater than 5 do no harm because setCol() will not show them.
 */
static uint8_t row3_10to50Seconds(const int16_t seconds) {
    return (seconds%60) / 10;
}
/*
 * Values greater than 5 do no harm because setCol() will not show them.
 */
static uint8_t row4_1to5Minutes(const int16_t seconds) {
    return (seconds/60) % 10;
}
static uint8_t row5_6to9Minutes(const int16_t seconds) {
    uint8_t x = ((seconds/60)-5)%10;
    if (x>4) x=0;
    return x;
}
/*
 * Values greater than 5 do no harm because setCol() will not show them.
 */
static uint8_t row6_10to50Minutes(const int16_t seconds) {
    return ((seconds/60)%60) / 10;
}
static uint8_t row7_1to5hours(const int16_t seconds) {
    return seconds/3600;
}

static void showRemainingTimeOnLEDMatrix(void) {
    static uint8_t row=0;

    // switch off all rows
    // Otherwise a LED in the next Row is fluorescing
    PORTB |= (1<<ROW1_OnPortB)|(1<<ROW2_OnPortB)|(1<<ROW3_OnPortB)
               |(1<<ROW4_OnPortB)|(1<<ROW5_OnPortB);
    PORTD |= (1<<ROW6_OnPortD)|(1<<ROW7_OnPortD);

    setCol(currentColValues[row]);
    setRow(row++);

    if (row > 6) {
        row=0;
    }
}

static void initTimer(void) {
    TCCR1B = 1<<WGM12 | 1<<CS10;          // divide by 1
                                          // clear on compare
    OCR1A = F_CPU / IRQS_PER_SECOND - 1;  // Output Compare Register
    TCNT1 = 0;                            // Timmer startet mit 0
    prescaler = (uint16_t)IRQS_PER_SECOND;//software teiler

    TIMSK = 1<<OCIE1A;                    // beim Vergleichswertes Compare Match 
                                          // Interrupt (SIG_OUTPUT_COMPARE1A)
}


void initIO(void) {
                               // 2 Buzzer- is toggled as input output in beeper()
    DDRB = 0xff & (~(1<<BUZZER_MINUS_OnPortB)); // 0,1 Col LED // 3-7 Row LED
    PORTB = 0;

    DDRD = 0;
    DDRD |= (1<<COL1_OnPortD) | (1<<COL2_OnPortD) | (1<<ROW7_OnPortD)
           |(1<<COL3_OnPortD) | (1<<ROW6_OnPortD);
    PORTD = 0;
    PORTD |= (1<<TAST1_OnPortD);  // Pullup for Tast1

    // Tast2 is switched only periodically to input, othertimes its output Row7

    beeper(0);

}

void setColumnValues(int16_t sec) {
     currentColValues[0] = row1_1to5Seconds(sec);
     currentColValues[1] = row2_6to9Seconds(sec);
     currentColValues[2] = row3_10to50Seconds(sec);
     currentColValues[3] = row4_1to5Minutes(sec);
     currentColValues[4] = row5_6to9Minutes(sec);
     currentColValues[5] = row6_10to50Minutes(sec);
     currentColValues[6] = row7_1to5hours(sec);
}

void initSecond(void) {
    uint16_t s = eeprom_read_word(firstSecondInEEprom);
    if (s>MAX_SECOND) 
        s=5;
    second = s;
}
 
int main( void ) {

    uint8_t beeperToggle = 1; 

	ACSR = 1 << ACD;   // Disable AnalogComparator
    
    initIO();
    initTimer();

    initSecond();
    init();

    sei();

    int16_t lastSecond=-1;
    uint8_t lastTimeBaseShort=0;
    for(;;){
      if (lastSecond != second) {
          lastSecond = second;

          setColumnValues(second);
      }

      if (lastTimeBaseShort != timeBaseShort) {
        lastTimeBaseShort = timeBaseShort;

        handleEvent();

        if (timeBaseShort%12 == 0) {
          // ~0.5s

          timeBase512ms++;

          if (currentState == ALARM) {
            beeper(beeperToggle);
            beeperToggle ^= 1;
          }
        }
      }
    }
}


/* Vim: use [i to show definition under cursor */
/* vim:set path=/usr/avr/include: */
/* vim:set path=/usr/local/AVRMacPack/avr/include/avr: */
