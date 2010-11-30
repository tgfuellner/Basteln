/*
 * Temeperaturmessung mit ADC
 *
 Copyright 2008 Thomas Gfüllner
 * */

#include <avr/io.h>
#include "temperature.h"

/*
 * Wenn mux==0 wird von PC0 (Pin7 auf CON3 der uWebSrv Hardware) gelesen.
 * Wenn mux==1 wird von PC1 (Pin8 auf CON3 der uWebSrv Hardware) gelesen.
 * ...
 */
uint16_t ReadADCChannel(uint8_t mux)
{
  uint8_t i;
  uint16_t result;
 
  ADMUX = mux;                      // Kanal waehlen
  // ADMUX |= (1<<REFS1) | (1<<REFS0); // interne Referenzspannung nutzen
  // Lasse REFS1 und REFS0 auf 0 dadurch wird externes AREF verwendet
  ADMUX |= (1<<REFS0); // AVCC als Referenz
 
  ADCSRA = (1<<ADEN) | (1<<ADPS1) | (1<<ADPS0);    // Frequenzvorteiler 
                               // setzen auf 8 und ADC aktivieren
 
  /* nach Aktivieren des ADC wird ein "Dummy-Readout" empfohlen, man liest
     also einen Wert und verwirft diesen, um den ADC "warmlaufen zu lassen" */
  ADCSRA |= (1<<ADSC);              // eine ADC-Wandlung 
  while ( ADCSRA & (1<<ADSC) ) {
     ;     // auf Abschluss der Konvertierung warten 
  }
  result = ADCW;  // ADCW muss einmal gelesen werden,
                  // sonst wird Ergebnis der nächsten Wandlung
                  // nicht übernommen.
 
  /* Eigentliche Messung - Mittelwert aus 4 aufeinanderfolgenden Wandlungen */
  result = 0; 
  for( i=0; i<4; i++ )
  {
    ADCSRA |= (1<<ADSC);            // eine Wandlung "single conversion"
    while ( ADCSRA & (1<<ADSC) ) {
      ;   // auf Abschluss der Konvertierung warten
    }
    result += ADCW;		    // Wandlungsergebnisse aufaddieren
  }
  ADCSRA &= ~(1<<ADEN);             // ADC deaktivieren
 
  result /= 4;                     // Summe durch vier teilen = arithm. Mittelwert
 
  return result;
}

