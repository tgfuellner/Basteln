/***********************************************************************/
/*                                                                     */
/*  PROJECT		:Atmel AVR Design Contest 2006: AT3244	    	       */
/*  FILE        :HID.c                                                 */
/*  DATE        :April 26, 2006	                                       */
/*  DESCRIPTION :This file allows to implement the Human Interface     */
/*				 Device (HID) class				   	                   */
/*                                                                     */
/***********************************************************************/


#include <string.h>
#include <avr/pgmspace.h>
#include "usbdrv.h"
#include "HID.h"


static uchar    reportBuffer[2];    // Buffer for HID reports 
static uchar    idleRate;           // In 4 ms units


PROGMEM char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = { /* USB report descriptor */
    0x05, 0x01,                     // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                     // USAGE (Keyboard)
    0xa1, 0x01,                     // COLLECTION (Application)
    0x05, 0x07,                     //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                     //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                     //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                     //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                     //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                     //   REPORT_SIZE (1)
    0x95, 0x08,                     //   REPORT_COUNT (8)
    0x81, 0x02,                     //   INPUT (Data,Var,Abs)
    0x95, 0x01,                     //   REPORT_COUNT (1)
    0x75, 0x08,                     //   REPORT_SIZE (8)
    0x25, 0x65,                     //   LOGICAL_MAXIMUM (101)
    0x19, 0x00,                     //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65,                     //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                     //   INPUT (Data,Ary,Abs)
    0xc0                            // END_COLLECTION
};


// We use a simplifed keyboard report descriptor which does not support the
// boot protocol. We don't allow setting status LEDs and we only allow one
// simultaneous key press (except modifiers). We can therefore use short
// 2 byte input reports.
// The report descriptor has been created with usb.org's "HID Descriptor Tool"
// which can be downloaded from http://www.usb.org/developers/hidpage/.
// Redundant entries (such as LOGICAL_MINIMUM and USAGE_PAGE) have been omitted
// for the second INPUT item.

const unsigned char keyReport[NUM_KEYS][2] PROGMEM = {
    {0, 0},                                 // ASCII NUL: No key pressed
    {0, 0},                                 // ASCII SOH
    {0, 0},                                 // ASCII STX
    {0, 0},                                 // ASCII ETX
    {0, 0},                                 // ASCII EOT
    {0, 0},                                 // ASCII ENQ
    {0, 0},                                 // ASCII ACK
    {0, 0},                                 // ASCII BEL
    {0, 0},                                 // ASCII BS
    {0, HID_KEY_TAB},                       // ASCII HT
    {0, HID_KEY_RETURN},                    // ASCII LF
    {0, 0},                                 // ASCII VT
    {0, 0},                                 // ASCII FF
    {0, HID_KEY_RETURN},                    // ASCII CR
    {0, 0},                                 // ASCII SO
    {0, 0},                                 // ASCII SI
    {0, 0},                                 // ASCII DLE
    {0, 0},                                 // ASCII DC1
    {0, 0},                                 // ASCII DC2
    {0, 0},                                 // ASCII DC3
    {0, 0},                                 // ASCII DC4
    {0, 0},                                 // ASCII NAK
    {0, 0},                                 // ASCII SYN
    {0, 0},                                 // ASCII ETB
    {0, 0},                                 // ASCII CAN
    {0, 0},                                 // ASCII EM
    {0, 0},                                 // ASCII SUB
    {0, 0},                                 // ASCII ESC
    {0, 0},                                 // ASCII FS
    {0, 0},                                 // ASCII GS
    {0, 0},                                 // ASCII RS
    {0, 0},                                 // ASCII US
    {0, HID_KEY_SPACEBAR},                  // ASCII SP
    {MOD_SHIFT_LEFT, HID_KEY_1},            // ASCII !
    {MOD_SHIFT_LEFT, HID_KEY_2},            // ASCII "
    {MOD_ALT_RIGHT,  HID_KEY_3},            // ASCII #
    {MOD_SHIFT_LEFT, HID_KEY_4},            // ASCII $
    {MOD_SHIFT_LEFT, HID_KEY_5},            // ASCII %
    {MOD_SHIFT_LEFT, HID_KEY_6},            // ASCII &
    {0, HID_KEY_APOSTROPHE},                // ASCII '
    {MOD_SHIFT_LEFT, HID_KEY_8},            // ASCII (
    {MOD_SHIFT_LEFT, HID_KEY_9},            // ASCII )
    {MOD_SHIFT_LEFT, HID_KEY_PLUS},         // ASCII *
    {0, HID_KEY_PLUS},                      // ASCII +
    {0, HID_KEY_COMMA},                     // ASCII ,
    {0, HID_KEY_MINUS},                     // ASCII -
    {0, HID_KEY_PERIOD},                    // ASCII .
    {MOD_SHIFT_LEFT, HID_KEY_7},            // ASCII /
    {0, HID_KEY_0},                         // ASCII 0
    {0, HID_KEY_1},                         // ASCII 1
    {0, HID_KEY_2},                         // ASCII 2
    {0, HID_KEY_3},                         // ASCII 3
    {0, HID_KEY_4},                         // ASCII 4
    {0, HID_KEY_5},                         // ASCII 5
    {0, HID_KEY_6},                         // ASCII 6
    {0, HID_KEY_7},                         // ASCII 7
    {0, HID_KEY_8},                         // ASCII 8
    {0, HID_KEY_9},                         // ASCII 9
    {MOD_SHIFT_LEFT, HID_KEY_PERIOD},       // ASCII :
    {MOD_SHIFT_LEFT, HID_KEY_COMMA},        // ASCII ;
    {0, 0},                                 // ASCII <
    {MOD_SHIFT_LEFT, HID_KEY_0},            // ASCII =
    {0, 0},                                 // ASCII >
    {MOD_SHIFT_LEFT, HID_KEY_APOSTROPHE},   // ASCII ?
    {MOD_ALT_RIGHT,  HID_KEY_2},            // ASCII @
    {MOD_SHIFT_LEFT, HID_KEY_A},            // ASCII A
    {MOD_SHIFT_LEFT, HID_KEY_B},            // ASCII B    
    {MOD_SHIFT_LEFT, HID_KEY_C},            // ASCII C
    {MOD_SHIFT_LEFT, HID_KEY_D},            // ASCII D
    {MOD_SHIFT_LEFT, HID_KEY_E},            // ASCII E
    {MOD_SHIFT_LEFT, HID_KEY_F},            // ASCII F
    {MOD_SHIFT_LEFT, HID_KEY_G},            // ASCII G
    {MOD_SHIFT_LEFT, HID_KEY_H},            // ASCII H
    {MOD_SHIFT_LEFT, HID_KEY_I},            // ASCII I
    {MOD_SHIFT_LEFT, HID_KEY_J},            // ASCII J
    {MOD_SHIFT_LEFT, HID_KEY_K},            // ASCII K
    {MOD_SHIFT_LEFT, HID_KEY_L},            // ASCII L
    {MOD_SHIFT_LEFT, HID_KEY_M},            // ASCII M
    {MOD_SHIFT_LEFT, HID_KEY_N},            // ASCII N
    {MOD_SHIFT_LEFT, HID_KEY_O},            // ASCII O
    {MOD_SHIFT_LEFT, HID_KEY_P},            // ASCII P
    {MOD_SHIFT_LEFT, HID_KEY_Q},            // ASCII Q
    {MOD_SHIFT_LEFT, HID_KEY_R},            // ASCII R
    {MOD_SHIFT_LEFT, HID_KEY_S},            // ASCII S
    {MOD_SHIFT_LEFT, HID_KEY_T},            // ASCII T
    {MOD_SHIFT_LEFT, HID_KEY_U},            // ASCII U
    {MOD_SHIFT_LEFT, HID_KEY_V},            // ASCII V
    {MOD_SHIFT_LEFT, HID_KEY_W},            // ASCII W
    {MOD_SHIFT_LEFT, HID_KEY_X},            // ASCII X
    {MOD_SHIFT_LEFT, HID_KEY_Y},            // ASCII Y
    {MOD_SHIFT_LEFT, HID_KEY_Z},            // ASCII Z
    {0, 0},                                 // ASCII [
    {0, 0},                                 // ASCII 
    {0, 0},                                 // ASCII ]
    {0, 0},                                 // ASCII ^
    {MOD_SHIFT_LEFT, HID_KEY_MINUS},        // ASCII _
    {0, 0},                                 // ASCII `
    {0, HID_KEY_A},                         // ASCII a
    {0, HID_KEY_B},                         // ASCII b
    {0, HID_KEY_C},                         // ASCII c
    {0, HID_KEY_D},                         // ASCII d
    {0, HID_KEY_E},                         // ASCII e
    {0, HID_KEY_F},                         // ASCII f
    {0, HID_KEY_G},                         // ASCII g
    {0, HID_KEY_H},                         // ASCII h
    {0, HID_KEY_I},                         // ASCII i
    {0, HID_KEY_J},                         // ASCII j
    {0, HID_KEY_K},                         // ASCII k
    {0, HID_KEY_L},                         // ASCII l
    {0, HID_KEY_M},                         // ASCII m
    {0, HID_KEY_N},                         // ASCII n
    {0, HID_KEY_O},                         // ASCII o
    {0, HID_KEY_P},                         // ASCII p
    {0, HID_KEY_Q},                         // ASCII q
    {0, HID_KEY_R},                         // ASCII r
    {0, HID_KEY_S},                         // ASCII s
    {0, HID_KEY_T},                         // ASCII t
    {0, HID_KEY_U},                         // ASCII u
    {0, HID_KEY_V},                         // ASCII v
    {0, HID_KEY_W},                         // ASCII w
    {0, HID_KEY_X},                         // ASCII x
    {0, HID_KEY_Y},                         // ASCII y
    {0, HID_KEY_Z},                         // ASCII z
    {0, 0},                                 // ASCII {
    {MOD_ALT_RIGHT, HID_KEY_1},             // ASCII |
    {0, 0},                                 // ASCII }
    {0, 0},                                 // ASCII ~
    {0, 0}                                  // ASCII DEL
};




/*****************************************************************************
Name:        HID_BuildReport       
Parameters:  key -> character ASCII code
Returns:     None
Description: This function allows to build the HID report buffer for a key
*****************************************************************************/
void HID_BuildReport (unsigned char key)
{
    *(int *)reportBuffer = pgm_read_word(keyReport[key]);   // Look-up table
}


/*****************************************************************************
Name:        usbFunctionSetup        
Parameters:  data -> data[8]
Returns:     Length of the data
Description: This function is called when the driver receives a SETUP 
             transaction from the host which is not answered by the driver
             itself (in practice: class and vendor requests)
*****************************************************************************/
uchar usbFunctionSetup (uchar data[8])
{
    usbRequest_t *rq = (void *)data;


    usbMsgPtr = reportBuffer;
    if ((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {    // Class request type
        if (rq->bRequest == USBRQ_HID_GET_REPORT) {                     // wValue: ReportType (highbyte), ReportID (lowbyte)
            // We only have one report type, so don't look at wValue
            HID_BuildReport(0);
            return sizeof(reportBuffer);
        } 
        else if (rq->bRequest == USBRQ_HID_GET_IDLE) {
            usbMsgPtr = &idleRate;
            return 1;
        }
        else if (rq->bRequest == USBRQ_HID_SET_IDLE) {
            idleRate = rq->wValue.bytes[1];
        }
    }
    else {
            // No vendor specific requests implemented
    }

	return 0;
}


/*****************************************************************************
Name:        HID_SendString
Parameters:  message -> ASCII character string
Returns:     None
Description: This function allows to send a string through the USB HID class
*****************************************************************************/
void HID_SendString (const char *message)
{
    unsigned char i;


    for (i = 0; i < strlen(message); i++) {
        HID_SendChar (0);               // Dummy       
        HID_SendChar (message[i]);      // Send character string one by one
        HID_SendChar (0);               // Dummy
    }
}


/*****************************************************************************
Name:        HID_SendChar       
Parameters:  character -> ASCII code
Returns:     None
Description: This function allows to send a character through the USB HID class
*****************************************************************************/
void HID_SendChar (const char character)
{
    while (!usbInterruptIsReady());     // Wait until previous transmission is finished

    HID_BuildReport (character);        // Build HID report buffer for one character
    usbSetInterrupt (reportBuffer, sizeof(reportBuffer));
}
