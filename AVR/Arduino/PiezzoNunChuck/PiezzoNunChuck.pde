/*
 * WiiChuckDemo -- 
 *
 * 2008 Tod E. Kurt, http://thingm.com/
 *
 */

/* Melody
 * (cleft) 2005 D. Cuartielles for K3
 *
 * This example uses a piezo speaker to play melodies.  It sends
 * a square wave of the appropriate frequency to the piezo, generating
 * the corresponding tone.
 *
 * The calculation of the tones is made following the mathematical
 * operation:
 *
 *       timeHigh = period / 2 = 1 / (2 * toneFrequency)
 *
 * where the different tones are described as in the table:
 *
 * note 	frequency 	period 	timeHigh
 * c 	        261 Hz 	        3830 	1915 	
 * d 	        294 Hz 	        3400 	1700 	
 * e 	        329 Hz 	        3038 	1519 	
 * f 	        349 Hz 	        2864 	1432 	
 * g 	        392 Hz 	        2550 	1275 	
 * a 	        440 Hz 	        2272 	1136 	
 * b 	        493 Hz 	        2028	1014	
 * C	        523 Hz	        1912 	956
 *
 * http://www.arduino.cc/en/Tutorial/Melody
 */
  


#include <Wire.h>
#include "nunchuck_funcs.h"

int loop_cnt=0;

byte accx,accy,zbut,cbut;
int ledPin = 13;


int speakerPin = 9;

void playTone(unsigned char tone) {
    char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
    int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tones[tone]);
//    delayMicroseconds(1700);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tones[tone]);
//    delayMicroseconds(1700);
}

void playNote(char note) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
  
  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i]);
    }
  }
}



void setup()
{
    Serial.begin(9600);
    pinMode(speakerPin, OUTPUT);
    
    nunchuck_setpowerpins();
    nunchuck_init(); // send the initilization handshake
    
    Serial.print("WiiChuckPiezzo ready\n");
}


void loop()
{
    static unsigned char tone = 5;
    static char play = 0;
  
    if( loop_cnt > 20 ) { // every n msecs get new data
        loop_cnt = 0;

        nunchuck_get_data();
        int xaccel = nunchuck_accelx();
        tone = (xaccel-73) / 13;
        if (tone > 7) tone=7;
        //Serial.println((int)tone);
        play = nunchuck_zbutton();
    }

    if (play) {
        playTone(tone);
    } else {
        delay(5);
    }


    loop_cnt++;
}

