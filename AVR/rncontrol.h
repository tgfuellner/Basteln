/* RN-Control specific
*/


#define F_CPU      16000000L      // nominal value
// #define F_CPU        16000000L   // after measuring deviation: 1.5s/d

#define LED_DIR     DDRC
#define LED_PORT    PORTC


#define BAUD 9600L


/******************* End Config **************************/


#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)   // clever runden
#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))     // Reale Baudrate
#define BAUD_ERROR ((BAUD_REAL*1000)/BAUD-1000) // Fehler in Promille 
 
#if ((BAUD_ERROR>10) || (BAUD_ERROR<-10))
  #error Systematischer Fehler der Baudrate grösser 1% und damit zu hoch! 
#endif


extern inline void uartInit(void);
extern void uartPutc(unsigned char c);
extern void uartPuts (char *s);
