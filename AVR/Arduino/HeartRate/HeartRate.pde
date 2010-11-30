/* Heart rate
 */

#include <avr/io.h>
#include <avr/interrupt.h>
//#include <LiquidCrystal.h>

int ledPin = 13;
//char volatile count = 1;    // If 0 Pulses are not counted, the heart is never so quick
unsigned int volatile pulse = 0;

//LiquidCrystal lcd(6, 2, 12,     8, 9, 10, 11);

// Für attin2313 wahrscheinlich: ANA_COMP_vec
ISR(ANALOG_COMP_vect)
{
  ACSR &= ~(1<<ACIE);     // Analog Comparator Interrupt Disable
//  if (count) {
//    count = 0;
    pulse++;
//  }
}


void setup()
{
  //lcd.print("hello, world!");
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  // prints title with ending line break
  Serial.println("Heart rate counter started.");

  
  ACSR &= ~(1<<ACIE);     // Analog Comparator Interrupt Disable
  ADCSRB &= ~(1<<ACME);   // No AIN1 multiplexing
  ACSR &= ~(1<<ACD);      // Analog Comparator enable
  ACSR &= ~(1<<ACBG);     // No Analog Comparator Bandgap Select for AIN0
  ACSR &= ~(1<<ACIC);     // Analog Comparator Input Capture Disabled
  ACSR |= (1<<ACIS1) | (1<<ACIS0);  // Comparator Interrupt on Rising Output Edge.
  
  ACSR |= 1<<ACIE;        // Analog Comparator Interrupt Enable

  DIDR1 |= (1<< AIN1D) | (1<<AIN0D); //  AIN1, AIN0 Digital Input Disable (to reduce power consumption)

  delay(10);

}

void loop()
{
  static unsigned int lastPulse=0;
  static unsigned long lastStamp=0;
  
  if (lastPulse != pulse) {
    Serial.println(pulse, DEC);
    digitalWrite(ledPin, HIGH);
    lastPulse = pulse;
    lastStamp = millis();
    return;
  }
  
  if (millis() - lastStamp > 150) {
    digitalWrite(ledPin, LOW);
    ACSR |= 1<<ACIE;        // Analog Comparator Interrupt Enable
//    count = 1;
  }
  
  delay(10);
}
