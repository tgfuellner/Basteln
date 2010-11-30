#ifndef _HID_H
#define _HID_H


#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>


#define HIGH                1
#define LOW             	0

#define NUM_KEYS            128


// Keyboard usage values, see usb.org's HID-usage-tables document, chapter
// 10 Keyboard/Keypad Page for more codes.

#define MOD_CONTROL_LEFT    (1<<0)
#define MOD_SHIFT_LEFT      (1<<1)
#define MOD_ALT_LEFT        (1<<2)
#define MOD_GUI_LEFT        (1<<3)
#define MOD_CONTROL_RIGHT   (1<<4)
#define MOD_SHIFT_RIGHT     (1<<5)
#define MOD_ALT_RIGHT       (1<<6)
#define MOD_GUI_RIGHT       (1<<7)

#define HID_KEY_A           4
#define HID_KEY_B           5
#define HID_KEY_C           6
#define HID_KEY_D           7
#define HID_KEY_E           8
#define HID_KEY_F           9
#define HID_KEY_G           10
#define HID_KEY_H           11
#define HID_KEY_I           12
#define HID_KEY_J           13
#define HID_KEY_K           14
#define HID_KEY_L           15
#define HID_KEY_M           16
#define HID_KEY_N           17
#define HID_KEY_O           18
#define HID_KEY_P           19
#define HID_KEY_Q           20
#define HID_KEY_R           21
#define HID_KEY_S           22
#define HID_KEY_T           23
#define HID_KEY_U           24
#define HID_KEY_V           25
#define HID_KEY_W           26
#define HID_KEY_X           27
#define HID_KEY_Y           28
#define HID_KEY_Z           29

#define HID_KEY_1           30
#define HID_KEY_2           31
#define HID_KEY_3           32
#define HID_KEY_4           33
#define HID_KEY_5           34
#define HID_KEY_6           35
#define HID_KEY_7           36
#define HID_KEY_8           37
#define HID_KEY_9           38
#define HID_KEY_0           39

#define HID_KEY_RETURN      40
#define HID_KEY_ESC         41
#define HID_KEY_BACKSPACE   42
#define HID_KEY_TAB         43
#define HID_KEY_SPACEBAR    44
#define HID_KEY_APOSTROPHE  45
#define HID_KEY_EQUAL       46
#define HID_KEY_PLUS        48
#define HID_KEY_COMMA       54
#define HID_KEY_PERIOD      55
#define HID_KEY_MINUS       56

#define HID_KEY_F1          58
#define HID_KEY_F2          59
#define HID_KEY_F3          60
#define HID_KEY_F4          61
#define HID_KEY_F5          62
#define HID_KEY_F6          63
#define HID_KEY_F7          64
#define HID_KEY_F8          65
#define HID_KEY_F9          66
#define HID_KEY_F10         67
#define HID_KEY_F11         68
#define HID_KEY_F12         69


void HID_BuildReport (unsigned char key);
void HID_SendString (const char *message);
void HID_SendChar (const char character);


#endif
