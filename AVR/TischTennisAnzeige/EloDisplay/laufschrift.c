/*
 * laufschrift.c
 *
 * Ein einfache "Laufschrift" auf dem Ping-Pong Board.
 *
 * Kompilierbar mittels AVR Studio 4 oder WinAVR 
 *
 * Der Sourcecode und das Hexfile dürfen frei verwendet werden.
 * Nutzung erfolgt auf eigene Gefahr.
 *
 * Ver.     Date         Author           Comments
 * -------  ----------   --------------   ------------------------------
 * 1.00	    07.11.2009   Sascha Bader     initial 
 */


/* -----------------------------------------
 * Defines (Präprozessor Makros)
 * -----------------------------------------*/

#define F_CPU 8000000UL	                       /* CPU Takt (für delay-Routine) */

#define WIDTH 12                               /* Breite des Displays */
#define HEIGHT 10                              /* Höhe des Displays */
#define FONTWIDTH 8                            /* Breite des Zeichensatzes */
#define FONTHEIGHT 10                          /* Höhe des Zeichensatzes */

#define GetPixel(x,y) leds[y]&(1<<x)           /* Makro: Ein "Pixel" auslesen */
#define SetPixel(x,y) leds[y]|=1<<x            /* Makro: Ein "Pixel" setzen */
#define ClearPixel(x,y) leds[y]&=~(1<<x)       /* Makro: Ein "Pixel" löschen */


/* -----------------------------------------
 * Includes
 * -----------------------------------------*/
#include <inttypes.h>		                   /* Definition der Datentypen uint8_t usw. */
#include <avr/interrupt.h>                     /* Interruptbehandlungsroutinen (für Timerinterrupt) */
#include <util/delay.h>		                   /* Definition der Verzögerungsfunktionen (_delay_ms) */
#include <avr/pgmspace.h>                      /* Hilfsfunktionen um Daten aus dem Flash zu lesen */
#include "font.h"                              /* Definition des Zeichensatzes */


/* -----------------------------------------
 * Globale Variablen
 * -----------------------------------------*/
uint16_t leds[WIDTH];                          /* Inhalt der LED-Matrix */
prog_uint8_t * fnt = (prog_uint8_t *) font;    /* Zeiger auf den Zeichensatz im Flash */
volatile uint8_t col = 0;                      /* Aktuelle Spalte (für Interruptroutine)
                                                  "volatile", da durch Interrupt verändert */

/* -----------------------------------------
 * Text der Laufschrift (Globele Variable)
 * -----------------------------------------*/
prog_uint8_t text[] =
" Es gibt eine Theorie, die besagt, wenn jemals irgendwer genau herausfindet,\
 wozu das Universum da ist und warum es da ist, dann verschwindet es auf der Stelle\
 und wird durch noch etwas Bizarreres und Unbegreiflicheres ersetzt.\
 - Es gibt eine andere Theorie, nach der das schon passiert ist. \
 ~";  /* Ende-Kennzeichen (nicht vergessen) */

/* -----------------------------------------
 * Prototypen der Funktionen
 * -----------------------------------------*/
void PrintScrollColumn(uint8_t c, int pixelx, int y);
void ScrollLeft(void);


void showText(const char *text) {
	uint8_t softx;
    uint8_t moreThanOneChar = 1;

    if (*(text+1) == '\0') {
        moreThanOneChar =0;
    }

	for (;*text!='\0';text++)   // Aktuelles Zeichen lesen
	{
		  for (softx=0;softx<FONTWIDTH;softx++)       // Pixel des Zeichens abarbeiten
		  {
            if (moreThanOneChar) {
                if (*(text+1)=='\0' && softx == FONTWIDTH-2) { 
                    // Das letzte Zeichen braucht keinen rechten Rand aber nur wenn es nicht alleine ist
                    break;
                }
                if (softx == FONTWIDTH-1) {
                    // Zwischenraum nur eine Spalte
                    continue;
                }
                if (*text == '1' && softx == FONTWIDTH-3) {
                    // Die eins verkleinern
                    continue;
                }
            }
			ScrollLeft();                             // Platz schaffen und Zeilen nach links schieben
			PrintScrollColumn(*text,softx,0);         // Ganz rechts eine Spalte des Zeichens ausgeben
			_delay_ms(35);                            // Ein bischen warten damit es nicht zu schnell wird
		  }
	}
}

/* -------------------------------------------------------------------------
 * Main Funktion
 *
 * Initialisiert den Timer Interrupt und
 * behandelt die Laufschrift
 * -------------------------------------------------------------------------*/
int main(void)
{
	cli();                              // Interrupts sperren (damit keiner dazwischenfunkt)

	/*---------------------------------------------------
	 * Ports konfigurieren (Ein-/Ausgänge)
	 *---------------------------------------------------*/
	DDRC = 0x0f;   // ( 0x0f PORTC als AD-Eingang)
	DDRB = 0xff;   //  Portb = Output
	DDRD = 0xff;   //  Portd = Output

	/*---------------------------------------------------------------------------
	 * 8-Bit Timer TCCR0 für das Multiplexing der LEDs initialisieren
	 * Es wird ca. alle 2 Millisekunden ein Overflow0 Interrupt ausgelöst
	 * Berechnung: T = Vorteiler * Wertebereich Zähler / Taktfreuenz
	 * = 64 * 256 / ( 8000000 Hz ) = 2,048 ms
	 *---------------------------------------------------------------------------*/
	TCCR0 |= (1<<CS01) | (1<<CS00);		// 8-bit Timer mit 1/64 Vorteiler
	TIFR |= (1<<TOV0); 					// Clear overflow flag (TOV0)
	TIMSK |= (1<<TOIE0); 				// timer0 will create overflow interrupt

	sei();							    // Interrupts erlauben

	/*---------------------------------------------------
	 * Hauptschleife (Laufschrift erzeugen)
	 *---------------------------------------------------*/
	while(1)                                              // Endlosschleife
	{
        showText("17");
        while (1);
	}
	return 0;
}

/* -------------------------------------------------------------------------
 * Funktion PrintScrollColumn
 *
 * Aktualisiert die Spalte ganz rechts mit
 * einem 1 "Pixel" breitem Ausschnitt des
 * Lauftextes.
 *
 * \param c       Auszugebendes Zeichen
 * \param pixelx  Auszugebende Spalte des Zeichens
 * \param y       Vertikale Vverschiebnung
 * -------------------------------------------------------------------------*/
void PrintScrollColumn(uint8_t c, int pixelx, int y)
{
  unsigned char fontbyte = 0;
  uint8_t pixelpos;
  uint8_t fonty;
  uint8_t mask;

  pixelpos = pixelx & 0x07;                  /* Auf 8 Pixel pro Zeichen limitieren */

  for (fonty=0;fonty<FONTHEIGHT;fonty++)
  {
	fontbyte = pgm_read_byte_near(fnt+c*FONTHEIGHT+fonty);  /* Ein Byte (Zeile) des aktuellen Zeichens lesen */

    mask = 1<<pixelpos;                      /* Maske auf die gewünschte Spalte zurechtschieben */
    if ((fontbyte & mask) != 0)              /* Prüfen ob das Bit in der Spalte des Zeichens gesetzt ist */
    {
        leds[WIDTH-1]|=1<<fonty;             /* Setzen eines Pixels im Display ganz rechts */
    }
    else
    {
        leds[WIDTH-1]&=~(1<<fonty);          /* Löschen eines Pixels im Display ganz rechts */
    }
  }
}

/* -------------------------------------------------------------------------
 * Funktion ScrollLeft
 *
 * Verschiebt den Inhalt LED-Matrix um eine Spalte nach links.
 * Die erste Spalte tritt dabei an die Position der letzten Spalte.
 * -------------------------------------------------------------------------*/
void ScrollLeft(void)
{
  uint8_t xcol;                           /* Spaltenzähler */
  uint16_t first;                         /* Zwischenspeicher der ersten Spalte */

  first = leds[0];                        /* Erste Spalte sichern */
  for (xcol=0;xcol<WIDTH-1;xcol++)
  {
	  leds[xcol]=leds[xcol+1];            /* Spalten nach links verschieben */
  }
  leds[WIDTH-1] = first;                  /* Erste Spalte an letzte Spalte kopieren */
}

/* -------------------------------------------------------------------------
 * Interrupt Routine
 *
 * Gibt nacheinander alle Spalten mit LED-Daten aus.
 * Dazu wird mittels der Schieberegister die aktuelle Spalte
 * ausgewählt und dann das Bitmuster derselben auf die Ports
 * gegeben.
 * Beim nächsten Interrupt ist dann die nächste Spalte dran.
 * -------------------------------------------------------------------------*/
// interrupt routine
SIGNAL (SIG_OVERFLOW0)
{
	uint16_t ledval;
	uint8_t portcout;
	uint8_t portdout;

	cli();							/* Interrupts verbieten */

	/*--------------------------------------------------
	 * Aktuelle Spalte ermitteln
	 *--------------------------------------------------*/
	col++;
	if (col == 12)
	{
		col = 0;
	}

	/*--------------------------------------------------
	 * Ports initialisieren
	 *--------------------------------------------------*/
	PORTD = 0;
	PORTB = 0;
	PORTC = 0;

	/*---------------------------------------------------
	 * Eine einzelne 0 durch die Schiebergister schieben
	 *---------------------------------------------------*/
	if ( col == 0 )
	{
		PORTB &= ~(1 << 4);        /* Bei der ersten Spalte eine 0 ausgeben (PB4 = 0) */
		                           /* Diese 0 geht auf die Reise durch die Schieberegister */
	}
	else
	{
		PORTB |= (1 << 4);         /* Danach Einsen hinterherschicken (PB4 = 1) */
	}

	/*---------------------------------------------------
	 * Impulse für die Schieberegister generieren
	 *---------------------------------------------------*/
	PORTB |= (1 << 3);             /* PB3 = 1 (cl) */
	PORTB &= ~(1 << 3);            /* PB3 = 0 (!cl) */

	PORTB |= (1 << 2);             /* PB2 = 1 (str) */
	PORTB &= ~(1 << 2);            /* PB2 = 0 (!str) */

	/*---------------------------------------------------
	 * Daten der Spalte holen und auf die Ports verteilen
	 *---------------------------------------------------*/
	ledval = leds[col];
	portdout = ledval & 0xff;      /* low byte */
	portcout = portdout & 0x0f;    /* low nibble */
	portdout = portdout & 0xf0;    /* high nibble */

	PORTD = portdout & 0xff;
	PORTC = portcout & 0xff;
	PORTB = (ledval >> 8) & 0x03;  /* high byte */

	sei();						   /* Interrupts wieder erlauben */
}
