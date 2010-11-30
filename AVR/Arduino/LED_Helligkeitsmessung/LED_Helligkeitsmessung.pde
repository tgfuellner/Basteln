/*
  Blink
 
 Turns on an LED on for one second, then off for one second, repeatedly.
 
 The circuit:
 * LED connected from digital pin 13 to ground.
 
 * Note: On most Arduino boards, there is already an LED on the board
 connected to pin 13, so you don't need any extra components for this example.
 
 
 Created 1 June 2005
 By David Cuartielles
 
 http://arduino.cc/en/Tutorial/Blink
 
 based on an orginal by H. Barragan for the Wiring i/o board
 
 */

int ledPin =  13;    // LED connected to digital pin 13

// The setup() method runs once, when the sketch starts

void setup()   {                
  // initialize the digital pin as an output:
  pinMode(ledPin, OUTPUT);     
  pinMode(12, OUTPUT);     
  Serial.begin(9600);
}

// the loop() method runs over and over again,
// as long as the Arduino has power

int val=0;


/* Haut nicht recht hin ???
 */
void adWandler() {
  if (val < 60 ) {
  pinMode(11, OUTPUT);
  digitalWrite(11, LOW);  
  digitalWrite(ledPin, HIGH);   // set the LED on
  digitalWrite(12, HIGH);   // set the LED on
  analogWrite(0, 0);
  delay(500); // wait for a second
  }
  
  digitalWrite(ledPin, LOW);    // set the LED off
  digitalWrite(12, LOW);    // set the LED off
  digitalWrite(11, LOW); 
  pinMode(11, INPUT);
  //delay(800);           // wait for a second
  
  val = 0;
  for (int i=0; i<10; i++) {
    delay(2);
    val += analogRead(0);
  }
  val = val / 10;
  //Serial.println(val);
  //delay(200);
}

#define ANODE 12
#define KATHODE 11

unsigned int getBrightness() {
  unsigned int j;

  // Apply reverse voltage, charge up the pin and led capacitance
  pinMode(ANODE,OUTPUT);
  pinMode(KATHODE,OUTPUT);
  digitalWrite(KATHODE,HIGH);
  digitalWrite(ANODE,LOW);
  delay(1);

  // Isolate the pin 2 end of the diode
  pinMode(KATHODE,INPUT);
  digitalWrite(KATHODE,LOW);  // turn off internal pull-up resistor

  // Count how long it takes the diode to bleed back down to a logic zero
  for ( j = 0; j < 30000; j++) {
    if ( digitalRead(KATHODE)==0) break;
  }

  return j;
}

void loop()                     
{
  unsigned int bright;
  bright = getBrightness();
  if (bright > 29500) {
    pinMode(ANODE,OUTPUT);
    pinMode(KATHODE,OUTPUT);
    digitalWrite(KATHODE,LOW);
    digitalWrite(ANODE,HIGH);
  }
  Serial.println(bright);
  delay(500);
}

