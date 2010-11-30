#include <avr/io.h>
#include <../rncontrol.h>


inline void uartInit() {
    UCSRB |= (1<<TXEN);                // UART TX einschalten
    UCSRC |= (1<<URSEL)|(3<<UCSZ0);    // Asynchron 8N1 
    UBRRH = UBRR_VAL >> 8;
    UBRRL = UBRR_VAL & 0xFF;
}

void uartPutc(unsigned char c) {
    while (!(UCSRA & (1<<UDRE)))  /* warten bis Senden moeglich */
    {}                             
 
    UDR = c;                      /* sende Zeichen */
}

void uartPuts (char *s) {
    while (*s) {
        uartPutc(*s);
        s++;
    }
}
