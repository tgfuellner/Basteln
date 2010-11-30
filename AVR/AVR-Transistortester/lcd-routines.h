// Ansteuerung eines HD44780 kompatiblen LCD im 4-Bit-Interfacemodus
// http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial
//
#include <avr/eeprom.h>

void lcd_data(unsigned char temp1);
void lcd_send(unsigned char temp1);
void lcd_string(char *data);
void lcd_command(unsigned char temp1);
void lcd_enable(void);
void lcd_init(void);
void lcd_clear(void);
void lcd_eep_string(const unsigned char *data);
 
// Hier die verwendete Taktfrequenz in Hz eintragen, wichtig!
 
#define F_CPU 1000000UL
 
// LCD Befehle
 
#define CLEAR_DISPLAY 0x01
 
// Pinbelegung für das LCD, an verwendete Pins anpassen
 
#define LCD_PORT      PORTD
#define LCD_DDR       DDRD
#define LCD_RS        PD4
#define LCD_EN1       PD5
