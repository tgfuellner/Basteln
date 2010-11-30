#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>



#define F_CPU 1000000L
#include <util/delay.h>

// PA3 kaputgeloetet! #define LED_SENSE_PLUS PA3
//                    #define DDR_SENSE_PLUS DDRA
//                    #define PORT_SENSE_PLUS PORTA
#define LED_SENSE_PLUS PB4
#define DDR_SENSE_PLUS DDRB
#define PORT_SENSE_PLUS PORTB

#define LED_SENSE_MINUS PA0

#define LED PB1

#define BUTTON PA5


// Timerinterrupt every 200ms = 0.2s
// 1500 * 0.3 = 300s = 5 minutes
#define MEASURE_PERIOD 1500

typedef enum {
    OFF=0,DARK,MIDDLE,BRIGHT, BLINK, LastState
} __attribute__ ((packed)) State; 


typedef State (*stateFunction)(void);

State ledOff(void) {
    TCCR0A &= ~((1<<COM0A1) | (1<<WGM00)); // Kein PWM
    PORTB &= ~(1<<LED);
    return DARK;
}
State ledDark(void) {
    TCCR0A = (1<<COM0A1) | (1<<WGM00); // Phase correct bis 0xff
    TCCR0B = (1<<CS00);                // Prescaler = 1
    // OCR0A =4;                       // Blinkt leider; evtl. wegen Einsparungsversuch im Schaltwandler
    OCR0A = 8;                         // kleine Werte sind dunkel
    return MIDDLE;
}
State ledMiddle(void) {
    TCCR0A = (1<<COM0A1) | (1<<WGM00); // Phase correct bis 0xff
    TCCR0B = (1<<CS00);                // Prescaler = 1
    OCR0A = 70;
    return BRIGHT;
}
State ledBright(void) {
    TCCR0A &= ~((1<<COM0A1) | (1<<WGM00)); // Kein PWM
    PORTB |= (1<<LED);
    return BLINK;
}
State ledBlink(void) {
    TCCR0A = (1<<COM0A1) | (1<<WGM00); // Phase correct bis 0xff
    TCCR0B = (1<<CS02) | (1<<CS00);    // Prescaler = 1024
    OCR0A = 127;
    return OFF;
}

State ledLowBattery(void) {
    TCCR0A = (1<<COM0A1) | (1<<WGM00); // Phase correct bis 0xff
    //TCCR0B = (1<<CS01) | (1<<CS00);    // Prescaler = 64
    TCCR0B = (1<<CS02);                // Prescaler = 256
    OCR0A = 20;
    return OFF;
}

const stateFunction StateTable[LastState] = {
    [OFF] = ledOff, [DARK] = ledDark,
    [MIDDLE] = ledMiddle, [BRIGHT] = ledBright,
    [BLINK] = ledBlink,
};

volatile State nextState = DARK;
volatile State currentState = OFF;

volatile unsigned int timerValue;
volatile unsigned char pwm;
volatile unsigned char debounce200ms = 0;


volatile char incr = 1;


void waitms(int ms) {
  int i;
  for (i=0;i<ms;i++) _delay_ms(1);
}

void initControler(void) {
  // Folgende drei Pins sind Output
  DDRA |= (1<<LED_SENSE_MINUS);
  DDR_SENSE_PLUS |= 1<<LED_SENSE_PLUS;
  DDRB |=  1<<LED;  // PWM

  // Der Taster ist Input mit Pullup
  DDRA &= ~(1<<BUTTON);
  PORTA |= 1<<BUTTON;

  // Taster macht PCINT0 Interrupt
  GIMSK |= (1<<PCIE0);
  PCMSK0 |= (1<<BUTTON);

  // Timer 0 macht PWM
  // TCCR0A = (1<<COM0A1) | (1<<WGM00); // Phase correct bis 0xff
  TCCR0B = (1<<CS00); // Prescaler = 1
  OCR0A = 2; // kleine Werte sind dunkel

  // Timer 1 soll periodisch einen Interrupt feuern
  TCCR1A = (1<<WGM11); // CTC Modus
  TCCR1B = (1<<CS12) | (1<<CS10);  // Prescaler = 1024
  OCR1A = 200-1; // 1000000/1024 * 200 = 200ms
  TIMSK1 |= (1<<OCIE1A); // Compare Interrupt erlauben

  
  DIDR0 |= (1<<ADC2D) | (1<<ADC1D); // ADC aus
  DIDR0 |= (1<<AIN0D); // Analog Comparator IO
  ACSR  |= (1<<ACD); // Analog Comparator aus
  MCUCR |= (1<<BODS); // Brown out aus
  MCUCR |= (1<<SE);   // Sleep mode enable
  // Sleep mode idle is default

  // Vorbereitung Batterie Messung
  ADMUX |= 0b01000110;  // 1.1V Reference und Vbat Pin
}



/*
 * Siehe www.reaktivlicht.de kopiert aus kochbuch.pdf
 *
 * Prinzip der Helligkeitsmessung mit
 * Hilfe einer LED
 * 
 * Die Kapazitaet einer Leuchtdiode (LED) in Sperrrichtung ist abhaengig von der Beleuchtung. Im be-
 * leuchteten Zustand ist sie kleiner als im unbeleuchteten.
 * Bei der Schaltung wird die Kapazitaet der LED regelmaeßig aufgeladen und
 * nach einer bestimmten Zeit die verbleibende Spannung nachgemessen. Aufgrund der Selbstentladung
 * wird sie kleiner als die urspruenglich aufgeladene Spannung sein. Je groesser die Kapazitaet der LED,
 * desto groesser ist die verbleibende Spannung. Somit ist die Restspannung uber die LED ein Indiz fuer
 * die Helligkeit der Beleuchtung der LED.
 * Durch den schmalen Abstrahlwinkel der LED ist gewaehrleistet, dass die Schaltung nur bei direkter
 * Beleuchtung aktiviert wird. Es hat sich herausgestellt, dass am Besten superhelle rote LEDs mit
 * transparentem Gehaeuse geeignet sind.
 *
 * Liefert true, wenn in angegebener Zeit so wenig Licht vorhanden war, dass
 * sich der LED-Kondensator nicht entladen konnte.
 */
unsigned char isItDark(int zeit) {
    unsigned char chargeState;

    PORT_SENSE_PLUS &= ~(1<<LED_SENSE_PLUS );     // auf Masse schalten
    PORTA |= (1<<LED_SENSE_MINUS);      // auf +Ub schalten, um die LED zu ’laden’
    _delay_us(60);                      // Ladezeit, kann ggf. noch verkleinert werden
    DDRA &= ~(1<<LED_SENSE_MINUS);      // fuer Abfrage der LED-Ladung auf ’Eingang’ schalten
    PORTA &= ~(1<<LED_SENSE_MINUS);     // Pullup abschalten, sonst geht’s nicht!
    waitms(zeit);                       // Entladezeit zeit_ms - je kleiner, je unempfindlicher
    chargeState = (PINA & (1<<LED_SENSE_MINUS));  // Ladezustand einlesen
    DDRA |= (1<<LED_SENSE_MINUS);       // wieder auf Ausgang schalten
    PORTA &= ~(1<<LED_SENSE_MINUS);     // auf Masse schalten
    return chargeState;
}

// Batterie Spannung messen
unsigned int measureVBat(void) {
    ADCSRA = (1<<ADEN) | (1<<ADPS1) | (1<<ADPS0);  // Enable und Divisor 8
    ADCSRA |= (1<<ADSC);                  // eine ADC-Wandlung
    while (ADCSRA & (1<<ADSC) );          // auf Abschluss der Konvertierung warten
    unsigned int vbat = ADC;              // Erste Messung ist Muell
    vbat = 0;
    for (int i=0; i<4;i++) {
      ADCSRA |= (1<<ADSC);                // eine ADC-Wandlung
      while (ADCSRA & (1<<ADSC) );        // auf Abschluss der Konvertierung warten
      vbat += ADC;
    }
    vbat = vbat / 4;
    vbat = vbat >> 2;                     // Niederwertigen 2 bits weg
    ADCSRA = 0;       // Disable ADC

    return vbat;
}

void checkAndWarn_lowBattery(void) {

    // 0x74 := 1.0V   0x68 := 0.9V   0x5d := 0.8V
    if (measureVBat() < 0x6c) {
        ledLowBattery();
    }
}

void showBatteryHealth(void) {
    unsigned char i;

    PORT_SENSE_PLUS |= (1<<LED_SENSE_PLUS); // Messung signalisieren ein
    waitms(1000);
    PORT_SENSE_PLUS &= ~(1<<LED_SENSE_PLUS); // Messung signalisieren aus
    waitms(300);

    // 0x74 := 1.0V   0x68 := 0.9V   0x5d := 0.8V
    unsigned int vbat = measureVBat();
    unsigned char blinkCount;

    if (vbat >=0x7f) {
        blinkCount = 3;
    } else if (vbat >= 0x74) {
        blinkCount = 2;
    } else {
        blinkCount =1;
    }

    for (i=0; i<blinkCount; i++) {
        PORT_SENSE_PLUS |= (1<<LED_SENSE_PLUS); // Messung signalisieren ein
        waitms(400);
        PORT_SENSE_PLUS &= ~(1<<LED_SENSE_PLUS); // Messung signalisieren aus
        waitms(300);
    }

}


int main(void)
{
  initControler();

  sei();

  while (1) {
      if (timerValue % MEASURE_PERIOD == 0) {
          // Helligkeit messen und bei ausreichender Dunkelheit den aktuell eingestellten
          // LED Zustand aktivieren
          DIDR0 &= ~((1<<ADC3D) | (1<<ADC0D)); // ADC ein
          timerValue = 1;               // vermeide Ueberlauf und nochmaliges Messen

          PORT_SENSE_PLUS |= (1<<LED_SENSE_PLUS); // Messung signalisieren ein
          waitms(50);
          PORT_SENSE_PLUS &= ~(1<<LED_SENSE_PLUS); // Messung signalisieren aus

          ledOff();                      // Helligkeitsmessung nicht stoeren
          /*
           * 350 Klappte 2x
           */
          if (isItDark(50)) {
              StateTable[currentState]();
              checkAndWarn_lowBattery();
          }
      }

      if (debounce200ms == 3) {
          // Taste wurde eben gedrueckt
          showBatteryHealth();
      }

      DIDR0 |= (1<<ADC3D) | (1<<ADC0D); // ADC aus
      sleep_mode();                     // in den Schlafmodus wechseln
  }
}

inline void toggleSenseLED(void) {
    PORT_SENSE_PLUS ^= (1<<LED_SENSE_PLUS);
}

inline void toggleLED(void) {
    PORTB ^= (1<<LED);
}

// Button Press
ISR (PCINT0_vect)
{
    PCMSK0 &= ~(1<<BUTTON);  // Prellen unterdruecken
    debounce200ms = 3;       // Nach 3 timer interrups wieder Taste aktivieren
    currentState = nextState;
    nextState = StateTable[currentState]();
    timerValue = MEASURE_PERIOD - 25;  // Zeige Status 25*0.2s unabhaengig von der Helligkeit

}

// Called every ~200ms
ISR (TIM1_COMPA_vect)
{
    if (debounce200ms == 0) {
        PCMSK0 |= (1<<BUTTON); // Taste wieder aktiviert
    } else {
        debounce200ms--;
    }
    //toggleSenseLED();
    timerValue++;
}
