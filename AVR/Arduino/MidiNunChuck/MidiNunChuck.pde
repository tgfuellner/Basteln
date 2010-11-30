// vim: filetype=cpp

/*
 * Use Nunhuck to make Midi Music
 *
 * To send MIDI, attach a MIDI out jack (female DIN-5) to Arduino.
 * DIN-5 pinout is:                               _____ 
 *    pin 2 - Gnd                                /     \
 *    pin 4 - 220 ohm resistor to +5V           | 3   1 |  MIDI jack
 *    pin 5 - Arduino D1 (TX)                   |  5 4  |
 *    all other pins - unconnected               \__2__/
 * On my midi jack, the color of the wires for the pins are:
 *   3 = n/c 
 *   5 = black  (blue)
 *   2 = red    (red)
 *   4 = orange (yellow)
 *   1 = brown
 *
 * Based off of Tom Igoe's work at:
 *    http://itp.nyu.edu/physcomp/Labs/MIDIOutput
 */



#include <Wire.h>
#include "nunchuck_funcs.h"

// Comment if You want Debug messages 
#define MIDIOUT



int ledPin = 13;


void setup()
{
#ifdef MIDIOUT
    Serial.begin(31250);   // MIDI Baudrate
#else
    Serial.begin(38400);    Serial.print("HI");
#endif

    pinMode(ledPin, OUTPUT);
    
    nunchuck_setpowerpins();
    nunchuck_init(); // send the initilization handshake
    
}

// Send a MIDI note-on message.  Like pressing a piano key
// channel ranges from 0-15
void noteOn(byte channel, byte note, byte velocity) {
  midiMsg( (0x90 | channel), note, velocity);
}

// Send a MIDI note-off message.  Like releasing a piano key
void noteOff(byte channel, byte note, byte velocity) {
  midiMsg( (0x80 | channel), note, velocity);
}

// Send a general MIDI message
void midiMsg(byte cmd, byte data1, byte data2) {
  digitalWrite(ledPin,HIGH);  // indicate we're sending MIDI data

#ifdef MIDIOUT
  Serial.print(cmd, BYTE);
  Serial.print(data1, BYTE);
  Serial.print(data2, BYTE);
#else
  Serial.print("MIDI Message: 0x"); Serial.print(cmd,HEX);
  Serial.print(", "); Serial.print(data1,DEC);
  Serial.print(", "); Serial.println(data2,DEC);
  
#endif

  digitalWrite(ledPin,LOW);
}


byte getNote(void) {
    byte tone;
    int xaccel = nunchuck_accelx();  // Left Right turns
    byte cButton = nunchuck_cbutton();

    if (!cButton) {
        // whole tones C, D, E, F, G, A, B
        static const byte tones[] = {60,62,64,65,67,69,71};
        tone = (xaccel-73) / 15;
        if (tone > 6) tone=6;
        return tones[tone];
    } else {
        // half tones C#, D#, F#, G#, A#
        static const byte tones[] = {61,63,66,68,70};
        tone = (xaccel-73) / 18;
        if (tone > 4) tone=4;
        return tones[tone];
    }
}

byte getVelocity() {
    byte vel = nunchuck_joyy();
    static byte velocity = 97;
#ifndef MIDIOUT
    Serial.print("velocity "); Serial.println(vel,DEC);
#endif

    if (vel > 150 && velocity < 118)
      velocity += 1;
    if (vel < 100 && velocity > 36)
      velocity -= 1;      
    
    return velocity;
}


void loop()
{
    static unsigned char toneOld = 0;
    static char playOld = 0;
    byte tone, play;
    
  
    nunchuck_get_data();
    tone = getNote();
    play = nunchuck_zbutton();

    byte velocity = getVelocity();

    if (play != playOld) {
      if (play) {
        noteOn(0,tone,velocity);
      } else {
        noteOff(0,toneOld,velocity);
      }
      toneOld=tone;
      playOld=play;
    }

#ifdef MIDIOUT
    delay(10);
#else
    delay(500);
#endif
}

