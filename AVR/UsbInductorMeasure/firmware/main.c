/* Name: main.c
 * Project: Inductance measurement based on AVR USB driver
 * Author: Thomas Gfüllner
 * Creation Date: 2009-09-07
 * Tabsize: 4
 * Based on: V-USB, http://www.a-reinert.de/Atmega_Projekte/L-Meter/l-meter.html
 *              and http://www.terra.es/personal/ccossio/atmelAVR2006.htm
 * Copyright: (c) 2006 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: Proprietary, free under certain conditions. See Documentation.
 */

#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>

#include "HID.h"

#include "usbdrv.h"
#include "oddebug.h"


//- #define BIT_LED 4
#define BIT_KEY 1

#ifndef NULL
#define NULL    ((void *)0)
#endif

#define uchar unsigned char
#define uint unsigned int


/* ------------------------------------------------------------------------- */

// Die Konstante C=2533029590000.0 ist zu gross, weil hier statt 63Hz nur 62.94Hz vorliegen
//
// L = C/(f*f) = C / (f*f*s*s) = C/(s*s) * 1/f*f
// s = 63/62.94 = 1.00095328884652049571 
// s*s = 1.00190748645266589179
// 2533029590000.0 / s*s = 2528207069265.8
//

#define CalcConst 2533029590000000.0

// Nehme doch C, da alle bekannte Spulen mit zu kleinen Werten bestimmt wurden.
//#define CalcConst 2528207069265.8
//
static double        calInduct;
static uchar         calibrationMode=1;

static char buffer[20];
/* ------------------------------------------------------------------------- */

unsigned int volatile t0over=0;
ISR(TIMER0_OVF_vect)
{
      t0over++;
}


/* ------------------------------------------------------------------------- */


static void printInductance(double l)
{
    HID_SendChar (0); // Dummy

    /* Zu viel Code
    if (l<0) {
        l = 0;
    }
    */

    ultoa((unsigned long)l, buffer, 10);
    HID_SendString(buffer);

    HID_SendString(" nH\n");
    HID_SendChar (0); // Dummy
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
            calibrationMode = 1;
        }
    }
}

/* ------------------------------------------------------------------------- */

static void timerPoll(void)
{
    static uchar timerCnt;
    static uchar printSlot = 0;
    static double inductance;
    static unsigned long ticksInASecond;

    if(TIFR & (1 << TOV1)){
        TIFR = (1 << TOV1); /* clear overflow */
        keyPoll();
        if(++timerCnt >= 63){       /* ~ 1 second interval */
            if (printSlot==2) {
                printSlot = 0;

                if (calibrationMode) {
                    calibrationMode = 0;

                    ultoa(ticksInASecond, buffer, 10);
                    HID_SendString("Frequency ");
                    HID_SendString(buffer);

                    HID_SendString(" Calibration ");
                    printInductance(calInduct);
                } else {
                    printInductance(inductance - calInduct);
                }
            } else {
                if (printSlot == 1) {
                    ticksInASecond = ((unsigned long)t0over<<8)+(unsigned long)TCNT0;
                    inductance = CalcConst / ((double)ticksInASecond*(double)ticksInASecond);

                    // Experimentell durch Vergleich mit Frequenzzaehler
                    // Noetig da keine genaue Sekunde.
                    // ticksInASecond = (double)ticksInASecond * 1.00337;   
                    if (calibrationMode) {
                        calInduct = CalcConst / ((double)ticksInASecond*(double)ticksInASecond);
                    }
                }

                printSlot++;
            }

            TCNT0 = 0; t0over = 0; timerCnt = 0;
        }
    }
}

/* ------------------------------------------------------------------------- */

static void timerInit(void)
{
    TCCR1 = 0x0b;           /* select clock: 16.5M/1k -> overflow rate = 16.5M/256k = 62.94 Hz */
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

    // Using PCINT3 for D+ and don't see where in the usb code the remaining PCINT's are disabled
    PCMSK = 0;

    //Frequenzzaehleingang Timer 0 Input with Pullup
    DDRB &= ~(1<<PB2);
    PORTB |= (1<<PB2);

    // Timer 0 als externer Impulszaehler
    // mit Interrupt bei Overflow
    //TCCR0B = 0b00000110;
    TCCR0B |= (1<<CS02) | (1<<CS01);
    TIMSK |= (1<<TOIE0);

    //TCNT0 = 0; t0over = 0;  // set counter to 0


    calibrationValue = eeprom_read_byte(0); /* calibration value from last time */
    if(calibrationValue != 0xff){
        OSCCAL = calibrationValue;
    }
    odDebugInit();
    usbDeviceDisconnect();
    for(i=0;i<20;i++){  /* 300 ms disconnect */
        _delay_ms(15);
    }
    usbDeviceConnect();
    PORTB |= 1 << BIT_KEY;  /* pull-up on key input */
    //wdt_enable(WDTO_1S);
    timerInit();
    //- adcInit();
    usbInit();
    sei();
    for(;;){    /* main event loop */
        //wdt_reset();
        usbPoll();
        timerPoll();
    }
    return 0;
}
