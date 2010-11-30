/************************************************************************/
/*                                                                      */
/*                      1-Wire Example					*/
/*                                                                      */
/*              Author: Peter Dannegger                                 */
/*                      danni@specs.de                                  */
/*                                                                      */
/************************************************************************/
#ifndef _main_h_
#define _main_h_
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//#include <stdlib.h>
// #include <stdio.h>

#define	BAUD	19200
#define bauddivider (uint)(1.0 * F_CPU / BAUD / 16 - 0.5)


#define uchar unsigned char
#define uint unsigned int
#define bit uchar
#define idata
#define code


/*
#define W1_PIN	PD3
#define W1_IN	PIND
#define W1_OUT	PORTD
#define W1_DDR	DDRD
//
// The red LED: nr of blinks * 5
#define LED_MOD5 PD2
// The green LED: nr of blinks * 1, short blink 0.5
#define LED_MOD1 PD5
*/

#define W1_PIN	PB0
#define W1_IN	PINB
#define W1_OUT	PORTB
#define W1_DDR	DDRB

// The red LED: nr of blinks * 5
#define LED_MOD5 PB3
// The green LED: nr of blinks * 1, short blink 0.5
#define LED_MOD1 PB4

#include "1wire.h"
#include "tempmeas.h"

#endif
