/*
  Differentialregelung
 
 
 Created 2010-03-06
 By Thomas Gfuellner

 AD und Werte in Abhaengigkeit vom Widerstand:
 1000Ohm --> 210
 1194Ohm --> 240
 1200Ohm --> 241
 1347Ohm --> 263
 1400Ohm --> 271

 PT1000 Temperatur in Abhaengigkeit vom Widerstad (Aus Tabelle: http://www.delta-r.de/Pt-Elemte/Widerstandstabelle_Pt_1000/widerstandstabelle_pt_1000.html
 1000Ohm --> 0C
 1194Ohm --> 50C
 1347Ohm --> 90C

 */

const int Temp_Critical = 263;    // AD Wert bei dem beide Ventile geschaltet werden.

const int PowerLEDPin = 2;
const int Temp1LEDPin = 3;
const int Temp2LEDPin = 4;

const int Ventil1Pin = 10;
const int Ventil2Pin = 12;

const int SensorUmschalterPin = 11;
const int Sensor1Aktiv = 0;
const int Sensor2Aktiv = 1;

const int Temp1SensorPin = 1;
const int Temp2SensorPin = 0;


// The setup() method runs once, when the sketch starts

void setup()   {                
  // initialize the digital pin as an output:
  pinMode(PowerLEDPin, OUTPUT);
  pinMode(Temp1LEDPin, OUTPUT);
  pinMode(Temp2LEDPin, OUTPUT);
  pinMode(Ventil1Pin, OUTPUT);
  pinMode(Ventil2Pin, OUTPUT);
  pinMode(SensorUmschalterPin, OUTPUT);

  
  Serial.begin(9600);
  Serial.println("Hallo Derk.");
}

float ad2Celsius(int ad) {
    const int AD_AT_0_Celsius = 210;
    const float PT1000 = 259.7402597; /* 1/a  for const a see http://de.wikipedia.org/wiki/Pt1000 */

    float r = 1000.0 + 6.557377 * (ad-AD_AT_0_Celsius);
    return PT1000 * (r / 1000.0 - 1.0);
}

void printTemp(char *label, int ad) {
  Serial.print(label);
  Serial.print(" = ");
  Serial.print(ad, DEC);
  Serial.print(" -> ");
  Serial.print(ad2Celsius(ad));
  Serial.println(" Celsius");
}

// the loop() method runs over and over again,
// as long as the Arduino has power

int second = 0;
void loop()                     
{
  digitalWrite(PowerLEDPin, HIGH);   // set the LED on
  delay(500);                        // wait for a 0.5 second
  digitalWrite(PowerLEDPin, LOW);    // set the LED off
  delay(1500);

  int t1_ad = analogRead(Temp1SensorPin);
  int t2_ad = analogRead(Temp2SensorPin);
  printTemp("t1_ad", t1_ad);
  printTemp("t2_ad", t2_ad);

  if (second > 60) {
      second = 0;
  }

  second += 2;

  if (second != 2) {
      return;
  }

  if (t1_ad >= t2_ad) {
      digitalWrite(Temp1LEDPin, HIGH);
      digitalWrite(Temp2LEDPin, LOW);
      digitalWrite(SensorUmschalterPin, Sensor1Aktiv);
      digitalWrite(Ventil1Pin, LOW);
      if (t2_ad > Temp_Critical) {
          digitalWrite(Ventil2Pin, LOW);
      } else {
          digitalWrite(Ventil2Pin, HIGH);
      }
  } else if (t1_ad < t2_ad) {
      digitalWrite(Temp1LEDPin, LOW);
      digitalWrite(Temp2LEDPin, HIGH);
      digitalWrite(SensorUmschalterPin, Sensor2Aktiv);
      digitalWrite(Ventil2Pin, LOW);
      if (t1_ad > Temp_Critical) {
          digitalWrite(Ventil1Pin, LOW);
      } else {
          digitalWrite(Ventil1Pin, HIGH);
      }
  }

  if (t1_ad == t2_ad) {
      digitalWrite(Temp2LEDPin, HIGH);
  }
}

/* vim:set filetype=cpp: */
