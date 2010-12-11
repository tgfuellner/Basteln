#include <LiquidCrystal.h>


/* Example for usage of LCD found in ISDN Tel Agfeo SysTel ST30
** Name of LCD Module: TM244AD P-6
** It is a 4x24 Display with two Controlers.
** Normaly there are two enable pins on similar types of LCD's
** This one has a select pin. Low enables controler for upper two lines.
** High enables controler for bottom two lines.
**
** Pins of LCD Modul:
**
** 1  GND
** 2  Vplus
** 3  V contrast
** 4  RS
** 5  RW
** 6  E
** 7-14  DB0-DB7
** 15 V backlight
** 16 GND backlight
** 17 Select  <------- not so usual
** 18 Not connected
*/

#define SELECT_PIN 6
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void selectTop() {
    digitalWrite(SELECT_PIN, LOW);
}
void selectBottom() {
    digitalWrite(SELECT_PIN, HIGH);
}

void setup() {
  pinMode(SELECT_PIN, OUTPUT);

  // First init the controler for the top two lines
  selectTop(); 
  // set up the LCD's number of columns and rows:
  lcd.begin(24, 2);
  lcd.print("Line1");
  lcd.setCursor(0,1);
  lcd.print("Line2");


  // Now init the controler for the bottom two lines
  selectBottom();
  lcd.begin(24, 2);
  lcd.print("Line3");
  lcd.setCursor(0,1);
  lcd.print("Line4");
}

void loop() {
  selectBottom();
  lcd.setCursor(15, 1);
  // print the number of seconds since reset:
  lcd.print(millis()/1000);
  
  selectTop();
  lcd.setCursor(10,0);
  lcd.print("Seconds");
  lcd.setCursor(10,1);
  lcd.print("since Start");
}


/* vim:set filetype=cpp: */
