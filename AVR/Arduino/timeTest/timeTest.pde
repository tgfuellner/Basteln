// Time measurement 

void setup() 
{ 
  Serial.begin(57600); 
  // prints title with ending line break 
  Serial.println("Zeitmessung"); 
 
  // wait for the long string to be sent 
  delay(100); 
} 

volatile int d;
int testFunction(void) {
  delayMicroseconds(30);
  return d;
}
 
void loop() 
{ 
  const unsigned long COUNT = 10000;
  
  unsigned long time = millis();
  for (unsigned long i=0; i<COUNT; i++) {
    testFunction();
  }
  unsigned long interval = millis() - time;
  Serial.print("Gesamtzeit = ");   Serial.print(interval); Serial.print("ms, ");
  
  float single = (interval * 1000.0) / (float)COUNT;
  Serial.print("Eine Iteration = "); Serial.print(single); Serial.println("us");
   
  delay(1000); // allow some time for the Serial data to be sent 
} 
