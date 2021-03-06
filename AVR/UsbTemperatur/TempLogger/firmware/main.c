/* Name: main.c
 * Project: Thermostat based on AVR USB driver
 * Author: Christian Starkjohann
 * Creation Date: 2006-04-23
 * Tabsize: 4
 * Copyright: (c) 2006 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: Proprietary, free under certain conditions. See Documentation.
 * This Revision: $Id: main.c 537 2008-02-28 21:13:01Z cs $
 */

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "usbdrv.h"
#include "oddebug.h"

/*
Pin assignment:
PB1 = key input (active low with pull-up)
PB3 = analog input (ADC3)
PB4 = LED output (active high)

PB0, PB2 = USB data lines
*/

//- #define BIT_LED 4
#define BIT_KEY 1

#define W1_PIN  PB4
#define W1_IN   PINB
#define W1_OUT  PORTB
#define W1_DDR  DDRB


#define UTIL_BIN4(x)        (uchar)((0##x & 01000)/64 + (0##x & 0100)/16 + (0##x & 010)/4 + (0##x & 1))
#define UTIL_BIN8(hi, lo)   (uchar)(UTIL_BIN4(hi) * 16 + UTIL_BIN4(lo))

#ifndef NULL
#define NULL    ((void *)0)
#endif

#define uchar unsigned char
#define uint unsigned int

#define MATCH_ROM   0x55
#define SKIP_ROM    0xCC
#define SEARCH_ROM  0xF0 
    
#define CONVERT_T   0x44        // DS1820 commands
#define READ        0xBE
#define WRITE       0x4E
#define EE_WRITE    0x48
#define EE_RECALL   0xB8
  
#define SEARCH_FIRST    0xFF        // start new search
#define PRESENCE_ERR    0xFF
#define DATA_ERR    0xFE
#define LAST_DEVICE 0x00        // last device found


/* ------------------------------------------------------------------------- */

static uchar    reportBuffer[2];    /* buffer for HID reports */
static uchar    idleRate;           /* in 4 ms units */

static uchar    isRecording;

static uchar    valueBuffer[16];
static uchar    *nextDigit;

/* ------------------------------------------------------------------------- */

PROGMEM char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = { /* USB report descriptor */
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0                           // END_COLLECTION
};
/* We use a simplifed keyboard report descriptor which does not support the
 * boot protocol. We don't allow setting status LEDs and we only allow one
 * simultaneous key press (except modifiers). We can therefore use short
 * 2 byte input reports.
 * The report descriptor has been created with usb.org's "HID Descriptor Tool"
 * which can be downloaded from http://www.usb.org/developers/hidpage/.
 * Redundant entries (such as LOGICAL_MINIMUM and USAGE_PAGE) have been omitted
 * for the second INPUT item.
 */

/* Keyboard usage values, see usb.org's HID-usage-tables document, chapter
 * 10 Keyboard/Keypad Page for more codes.
 */
#define MOD_CONTROL_LEFT    (1<<0)
#define MOD_SHIFT_LEFT      (1<<1)
#define MOD_ALT_LEFT        (1<<2)
#define MOD_GUI_LEFT        (1<<3)
#define MOD_CONTROL_RIGHT   (1<<4)
#define MOD_SHIFT_RIGHT     (1<<5)
#define MOD_ALT_RIGHT       (1<<6)
#define MOD_GUI_RIGHT       (1<<7)

#define KEY_1       30
#define KEY_2       31
#define KEY_3       32
#define KEY_4       33
#define KEY_5       34
#define KEY_6       35
#define KEY_7       36
#define KEY_8       37
#define KEY_9       38
#define KEY_0       39
#define KEY_RETURN  40
#define KEY_DOT     55
#define KEY_MINUS   45

/* ------------------------------------------------------------------------- */

static void buildReport(void)
{
uchar   key = 0;

    if(nextDigit != NULL){
        key = *nextDigit;
    }
    reportBuffer[0] = 0;    /* no modifiers */
    reportBuffer[1] = key;
}

static void evaluateMeas(uint value)
{
    uchar   digit;

    nextDigit = &valueBuffer[sizeof(valueBuffer)];
    *--nextDigit = 0xff;/* terminate with 0xff */
    *--nextDigit = 0;
    *--nextDigit = KEY_RETURN;


    uchar preComma = value >> 4;

    if ( value & 0x8000 )  {
        // negative
        preComma ^= 0xff;  // convert to positive => (twos complement)++
        preComma++;
    }

    if (value & (1<<3)) {
        // 0.5
        *--nextDigit = 0;
        if ( value & 0x8000 )  {
            *--nextDigit = KEY_0;
        } else {
            *--nextDigit = KEY_5;
        }
    } else {
        *--nextDigit = 0;
        if ( value & 0x8000 )  {
            *--nextDigit = KEY_5;
        } else {
            *--nextDigit = KEY_0;
        }
    }
    *--nextDigit = 0;
    *--nextDigit = KEY_DOT;


    do{
        digit = preComma % 10;
        preComma /= 10;
        *--nextDigit = 0;
        if(digit == 0){
            *--nextDigit = KEY_0;
        }else{
            *--nextDigit = KEY_1 - 1 + digit;
        }
    }while(preComma != 0);

    if ( value & 0x8000 )  {
        // negative
        *--nextDigit = 0;
        *--nextDigit = KEY_MINUS;
    }
}

/* ------------------------------------------------------------------------- */

static void setIsRecording(uchar newValue)
{
    isRecording = newValue;
}

/* ------------------------------------------------------------------------- */

static void keyPoll(void)
{
static uchar    keyMirror;
uchar           key;

    key = PINB & (1 << BIT_KEY);
    if(keyMirror != key){   /* status changed */
        keyMirror = key;
        if(!key){           /* key was pressed */
            setIsRecording(!isRecording);
        }
    }
}

/* ------------------------------------------------------------------------- */

uchar w1_reset(void)
{
  uchar err;

  W1_OUT &= ~(1<<W1_PIN);
  W1_DDR |= 1<<W1_PIN;
  _delay_us(480);           // 480 us
  cli();
  W1_DDR &= ~(1<<W1_PIN);
  _delay_us(64);
  err = W1_IN & (1<<W1_PIN);            // no presence detect
  sei();
  _delay_us(480 - 64);
  if( (W1_IN & (1<<W1_PIN)) == 0 )      // short circuit
    err = 1;
  return err;
}

uchar w1_bit_io( uchar b )
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


void w1_command( uchar command )
{       
  w1_reset();
  w1_byte_wr( SKIP_ROM );           // to all devices
  w1_byte_wr( command );
}       


void start_meas( void ){
  if( W1_IN & 1<< W1_PIN ){
    w1_command( CONVERT_T );
    W1_OUT |= 1<< W1_PIN;
    W1_DDR |= 1<< W1_PIN;           // parasite power on
  }
} 

uchar w1_rom_search( uchar diff, uchar *id )
{
  uchar i, j, next_diff;
  uchar b;

  if( w1_reset() )
    return PRESENCE_ERR;            // error, no device found
  w1_byte_wr( SEARCH_ROM );         // ROM search command
  next_diff = LAST_DEVICE;          // unchanged on last device
  i = 8 * 8;                    // 8 bytes
  do{
    j = 8;                  // 8 bits
    do{
      b = w1_bit_io( 1 );           // read bit
      if( w1_bit_io( 1 ) ){         // read complement bit
    if( b )                 // 11
      return DATA_ERR;          // data error
      }else{
    if( !b ){               // 00 = 2 devices
      if( diff > i ||
        ((*id & 1) && diff != i) ){
        b = 1;              // now 1
        next_diff = i;          // next pass 0
      }
    }
      }
      w1_bit_io( b );               // write bit
      *id >>= 1;
      if( b )                   // store bit
    *id |= 0x80;
      i--;
    }while( --j );
    id++;                   // next byte
  }while( i );
  return next_diff;             // to continue search
}


static uint read_meas(void)
{
  uchar id[8], diff;
  uint temp;

  for( diff = SEARCH_FIRST; diff != LAST_DEVICE; ){
    diff = w1_rom_search( diff, id );

    if( diff == PRESENCE_ERR || diff == DATA_ERR ){
      //uputsnl( (uchar *)"No Sensor found" );
      break;
    }

    w1_byte_wr( READ );         // read command
    temp = w1_byte_rd();            // low byte
    temp |= (uint)w1_byte_rd() << 8;        // high byte
    if( id[0] == 0x10 ) {       // 9 -> 12 bit
      temp <<= 3;
    }

    return temp;
  }
  return 0;
}

static void timerPoll(void)
{
    static uchar timerCnt;
    static uchar measPending;

    if(TIFR & (1 << TOV1)){
        TIFR = (1 << TOV1); /* clear overflow */
        keyPoll();
        if(++timerCnt >= 63){       /* ~ 1 second interval */
            timerCnt = 0;
            if(isRecording){
                if (measPending) {
                    measPending = 0;
                    uint val = read_meas();
                    evaluateMeas(val);
                } else {
                    measPending = 1;
                    start_meas();
                }
            }
        }
    }
}

/* ------------------------------------------------------------------------- */

static void timerInit(void)
{
    TCCR1 = 0x0b;           /* select clock: 16.5M/1k -> overflow rate = 16.5M/256k = 62.94 Hz */
}

#ifdef WEG
static void adcInit(void)
{
    ADMUX = UTIL_BIN8(1001, 0011);  /* Vref=2.56V, measure ADC0 */
    ADCSRA = UTIL_BIN8(1000, 0111); /* enable ADC, not free running, interrupt disable, rate = 1/128 */
}
#endif

/* ------------------------------------------------------------------------- */
/* ------------------------ interface to USB driver ------------------------ */
/* ------------------------------------------------------------------------- */

uchar	usbFunctionSetup(uchar data[8])
{
usbRequest_t    *rq = (void *)data;

    usbMsgPtr = reportBuffer;
    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* class request type */
        if(rq->bRequest == USBRQ_HID_GET_REPORT){  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
            /* we only have one report type, so don't look at wValue */
            buildReport();
            return sizeof(reportBuffer);
        }else if(rq->bRequest == USBRQ_HID_GET_IDLE){
            usbMsgPtr = &idleRate;
            return 1;
        }else if(rq->bRequest == USBRQ_HID_SET_IDLE){
            idleRate = rq->wValue.bytes[1];
        }
    }else{
        /* no vendor specific requests implemented */
    }
	return 0;
}

/* ------------------------------------------------------------------------- */
/* ------------------------ Oscillator Calibration ------------------------- */
/* ------------------------------------------------------------------------- */

/* Calibrate the RC oscillator to 8.25 MHz. The core clock of 16.5 MHz is
 * derived from the 66 MHz peripheral clock by dividing. Our timing reference
 * is the Start Of Frame signal (a single SE0 bit) available immediately after
 * a USB RESET. We first do a binary search for the OSCCAL value and then
 * optimize this value with a neighboorhod search.
 * This algorithm may also be used to calibrate the RC oscillator directly to
 * 12 MHz (no PLL involved, can therefore be used on almost ALL AVRs), but this
 * is wide outside the spec for the OSCCAL value and the required precision for
 * the 12 MHz clock! Use the RC oscillator calibrated to 12 MHz for
 * experimental purposes only!
 */
static void calibrateOscillator(void)
{
uchar       step = 128;
uchar       trialValue = 0, optimumValue;
int         x, optimumDev, targetValue = (unsigned)(1499 * (double)F_CPU / 10.5e6 + 0.5);

    /* do a binary search: */
    do{
        OSCCAL = trialValue + step;
        x = usbMeasureFrameLength();    /* proportional to current real frequency */
        if(x < targetValue)             /* frequency still too low */
            trialValue += step;
        step >>= 1;
    }while(step > 0);
    /* We have a precision of +/- 1 for optimum OSCCAL here */
    /* now do a neighborhood search for optimum value */
    optimumValue = trialValue;
    optimumDev = x; /* this is certainly far away from optimum */
    for(OSCCAL = trialValue - 1; OSCCAL <= trialValue + 1; OSCCAL++){
        x = usbMeasureFrameLength() - targetValue;
        if(x < 0)
            x = -x;
        if(x < optimumDev){
            optimumDev = x;
            optimumValue = OSCCAL;
        }
    }
    OSCCAL = optimumValue;
}
/*
Note: This calibration algorithm may try OSCCAL values of up to 192 even if
the optimum value is far below 192. It may therefore exceed the allowed clock
frequency of the CPU in low voltage designs!
You may replace this search algorithm with any other algorithm you like if
you have additional constraints such as a maximum CPU clock.
For version 5.x RC oscillators (those with a split range of 2x128 steps, e.g.
ATTiny25, ATTiny45, ATTiny85), it may be useful to search for the optimum in
both regions.
*/

void    usbEventResetReady(void)
{
    calibrateOscillator();
    eeprom_write_byte(0, OSCCAL);   /* store the calibrated value in EEPROM */
}


/* ------------------------------------------------------------------------- */
/* --------------------------------- main ---------------------------------- */
/* ------------------------------------------------------------------------- */

int main(void)
{
uchar   i;
uchar   calibrationValue;

    calibrationValue = eeprom_read_byte(0); /* calibration value from last time */
    if(calibrationValue != 0xff){
        OSCCAL = calibrationValue;
    }
    odDebugInit();
    start_meas();
    usbDeviceDisconnect();
    for(i=0;i<20;i++){  /* 300 ms disconnect */
        _delay_ms(15);
    }
    usbDeviceConnect();
    PORTB |= 1 << BIT_KEY;  /* pull-up on key input */
    wdt_enable(WDTO_1S);
    timerInit();
    //- adcInit();
    usbInit();
    sei();
    for(;;){    /* main event loop */
        wdt_reset();
        usbPoll();
        if(usbInterruptIsReady() && nextDigit != NULL){ /* we can send another key */
            buildReport();
            usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
            if(*++nextDigit == 0xff)    /* this was terminator character */
                nextDigit = NULL;
        }
        timerPoll();
    }
    return 0;
}
