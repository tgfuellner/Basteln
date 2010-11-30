#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

/*
 * State transitions:
 *
 * There are two Buttons on the device: Left (Tast1) and Right (Tast2)
 *
 * powerDown: Device is sleeping
 * powerSave: after a time without butten presses, time is not displayed anymore
 * init: current time is shown not blinking
 * count: The device is counting from current time to zero
 * adjust*: time is blinking, we are in adjust mode
 * wait: no counting time is frozen
 * alarm: Buzzer, Blink all LED's
 *
 *
 * powerDown -- Left --  use storedValue1 --> init
 *
 * init         -- Left -->  count
 * init         -- Right -- use next storedValue --> init
 * init         -- Left+Right --> adjustSecond
 * init         -- LongLeft -- use first storedValue --> init
 * init         -- LongRight --> powerDown
 * init         -- timeOut --> powerDown
 *
 * count        -- Left --> wait
 * count        -- Right --> init
 * count        -- Left+Right --> adjustSecond
 * count        -- LongRight --> powerDown
 * count        -- timeOut --> powerSave
 *
 * wait         -- Left --> count
 * wait         -- Right --> init
 * wait         -- Left+Right --> adjustSecond
 * wait         -- LongRight --> powerDown
 * wait         -- timeOut --> powerDown
 *
 * adjustSecond -- Left -- dec --> adjustSecond
 * adjustSecond -- Right -- inc --> adjustSecond
 * adjustSecond -- LongLeft -- dec10 --> adjustSecond
 * adjustSecond -- LongRight -- inc10 --> adjustSecond
 * adjustSecond -- Left+Right -- adjustMinute
 * adjustSecond -- timeOut --> powerDown
 *
 * adjustMinute -- Left -- dec --> adjustMinute
 * adjustMinute -- Right -- inc --> adjustMinute
 * adjustMinute -- LongLeft -- dec10 --> adjustMinute
 * adjustMinute -- LongRight -- inc10 --> adjustMinute
 * adjustMinute -- Left+Right --> adjustHour
 * adjustMinute -- timeOut --> powerDown
 *
 * adjustHour   -- Left -- dec --> adjustHour
 * adjustHour   -- Right -- inc --> adjustHour
 * adjustHour   -- LongRight --> count
 * adjustHour   -- LongRight  --> wait
 * adjustHour   -- Left+Right -- store --> init
 * adjustHour   -- timeOut --> powerDown
 *
 * alarm        -- Left -- restore Value --> init
 * alarm        -- LongRight --> powerDown
 * alarm        -- timeOut --> init
 *
 * powerSave    -- Left --> count
 */

#include <countdown.h>

// Timebase 32ms
static const uint8_t REPEAT_START = 32;     // after 1024ms
static const uint8_t REPEAT_NEXT = 9;       // every 224ms
static uint8_t deactivateEvents = 7;        // Needed for wakeup

// Timebase 512ms
static const uint8_t TIMEOUT_TIME = 100;
static const uint8_t ALARM_TIME = 40;   // Less then TIMEOUT_TIME


typedef enum {
    LEFT=0,RIGHT,LEFT_RIGHT,LONG_LEFT,LONG_RIGHT,TIMEOUT,  LastEvent
} __attribute__ ((packed)) Event;

typedef State (*stateFunction)(void);

typedef enum {
    NotPressed=0, Pressed, LongPressed
} __attribute__ ((packed)) ButtonState;


State currentState = INIT;


/*
 * Return State of button.
 */
static ButtonState getButton(const uint8_t portBit, uint8_t * const buttonPressTime) {
    ButtonState state = NotPressed;

    if ( (PIND & (1<<portBit)) && *buttonPressTime == 0 ) {
        // Not pressed and not pressed before.
        return state;
    }
    if ( (PIND & (1<<portBit)) && *buttonPressTime > 0) {
        // Not pressed but was pressed before.
        if (*buttonPressTime <  REPEAT_START - REPEAT_NEXT) {
            state = Pressed;
        }
        *buttonPressTime = 0;
        return state;
    }
    if ( (PIND & (1<<portBit)) == 0 && *buttonPressTime < REPEAT_START ) {
        // Pressed but not long enough for repeat.
        (*buttonPressTime)++;
        return state;
    }
    if ( (PIND & (1<<portBit)) == 0 && *buttonPressTime >= REPEAT_START ) {
        // Pressed longer than repeat time.
        *buttonPressTime = REPEAT_START - REPEAT_NEXT;
        return LongPressed;
    }

    return state;
}

static void incSecond(int16_t inc) {

    if (second+inc<0) {
        return;
    } else if (second+inc>MAX_SECOND) {
        return;
    }

    second += inc;
}

State init(void) {
    // for (uint8_t i=0;i<=7;i++) currentColValues[i] = 3;
    beeper(0);
    initSecond();
    return INIT;
}
State initNextStoredValue(void) {
    firstSecondInEEprom++;

    if (firstSecondInEEprom > ((uint16_t *)EEPROM_STORE_START)+9) {
        firstSecondInEEprom = (uint16_t *)EEPROM_STORE_START;
    }

    return init();
}
State initFirstStoredValue(void) {
    firstSecondInEEprom = (uint16_t *)EEPROM_STORE_START;
    return init();
}

static State storeInit(void) {
    eeprom_write_word(firstSecondInEEprom, second);
    setColumnValues(second);
    return INIT;
}
static State count(void) {
    return COUNT;
}
static State adjustSecond(void) {
    currentColValues[0] = 5;
    currentColValues[1] = 5;
    currentColValues[2] = 5;
    return ADJUSTSECOND;
}
static State powerDown(void) {
    cli();

    beeper(0);
    DDRA = 0; PORTA = 0xff;
    DDRB = 0; PORTB = 0xff;
    DDRD = 0; PORTD = 0xff;

    MCUCR &= ~0x3;                  // levelgesteuerter Interrupt an INT0
    GIMSK |= (1 << INT0);           // externen Interrupt freigeben
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sei();
    sleep_mode();                   // in den Schlafmodus wechseln

    // hier wachen wir wieder auf
    cli();
    GIMSK &= ~(1 << INT0);          // externen Interrupt sperren
                                    // WICHTIG! falls der externe LOW Puls an INT0
                                    // sehr lange dauert
    
    initIO();
    deactivateEvents = 7;           // Debounce wakeup

    sei();
    return INIT;
}

// externer Interrupt INT0 
 
// Die Interruptroutine kann leer sein, ABER sie muss existieren!
// Sonst springt der AVR nach dem Aufwachen zum Reset, weil kein sinnvoller
// Interruptvektor eingetragen ist!
 
ISR(INT0_vect) {
}


static State wait(void) {
    currentColValues[6] = 3;
    return WAIT;
}
static State powerSave(void) {
    return POWERSAVE;
}
static State adjustSecondDec(void) {
    incSecond(-1);
    return ADJUSTSECOND;
}
static State adjustSecondInc(void) {
    incSecond(1);
    return ADJUSTSECOND;
}
static State adjustSecondDec10(void) {
    incSecond(-10);
    return ADJUSTSECOND;
}
static State adjustSecondInc10(void) {
    incSecond(10);
    return ADJUSTSECOND;
}
static State adjustMinute(void) {
    currentColValues[3] = 5;
    currentColValues[4] = 5;
    currentColValues[5] = 5;
    return ADJUSTMINUTE;
}
static State adjustMinuteDec(void) {
    incSecond(-60);
    return ADJUSTMINUTE;
}
static State adjustMinuteInc(void) {
    incSecond(60);
    return ADJUSTMINUTE;
}
static State adjustMinuteDec10(void) {
    incSecond(-600);
    return ADJUSTMINUTE;
}
static State adjustMinuteInc10(void) {
    incSecond(600);
    return ADJUSTMINUTE;
}
static State adjustHour(void) {
    currentColValues[6] = 5;
    return ADJUSTHOUR;
}
static State adjustHourDec(void) {
    incSecond(-3600);
    return ADJUSTHOUR;
}
static State adjustHourInc(void) {
    incSecond(3600);
    return ADJUSTHOUR;
}

static const stateFunction StateTransitionTable[LastState][LastEvent] PROGMEM = {
    [POWERSAVE][LEFT] = count,
    [INIT][LEFT] = count, [INIT][RIGHT] = initNextStoredValue, [INIT][LEFT_RIGHT] = adjustSecond,
        [INIT][LONG_LEFT] = initFirstStoredValue, [INIT][LONG_RIGHT] = powerDown,
        [INIT][TIMEOUT] = powerDown,
    [COUNT][LEFT] = wait, [COUNT][RIGHT] = init, [COUNT][LEFT_RIGHT] = adjustSecond,
        [COUNT][LONG_RIGHT] = powerDown, [COUNT][TIMEOUT] = powerSave,
    [WAIT][LEFT] = count, [WAIT][RIGHT] = init, [WAIT][LEFT_RIGHT] = adjustSecond,
        [WAIT][LONG_RIGHT] = powerDown, [WAIT][TIMEOUT] = powerDown,
    [ADJUSTSECOND][LEFT] = adjustSecondDec, [ADJUSTSECOND][RIGHT] = adjustSecondInc,
        [ADJUSTSECOND][LEFT_RIGHT] = adjustMinute,
        [ADJUSTSECOND][LONG_LEFT] = adjustSecondDec10,
        [ADJUSTSECOND][LONG_RIGHT] = adjustSecondInc10, [ADJUSTSECOND][TIMEOUT] = powerDown,
    [ADJUSTMINUTE][LEFT] = adjustMinuteDec, [ADJUSTMINUTE][RIGHT] = adjustMinuteInc,
        [ADJUSTMINUTE][LEFT_RIGHT] = adjustHour,
        [ADJUSTMINUTE][LONG_LEFT] = adjustMinuteDec10,
        [ADJUSTMINUTE][LONG_RIGHT] = adjustMinuteInc10,
        [ADJUSTMINUTE][TIMEOUT] = powerDown,
    [ADJUSTHOUR][LEFT] = adjustHourDec, [ADJUSTHOUR][RIGHT] = adjustHourInc,
        [ADJUSTHOUR][LEFT_RIGHT] = storeInit, [ADJUSTHOUR][LONG_LEFT] = count,
        [ADJUSTHOUR][LONG_RIGHT] = powerDown, [ADJUSTHOUR][TIMEOUT] = powerDown,
    [ALARM][LEFT] = init, [ALARM][LONG_RIGHT] = powerDown, [ALARM][TIMEOUT] = powerDown,
};

inline static Event makeEvent(const ButtonState state1, const ButtonState state2) {
    if (state1 == Pressed && state2 == Pressed) return LEFT_RIGHT;
    if (state1 == LongPressed && state2 == LongPressed) return LEFT_RIGHT;
    if (state1 == LongPressed) return LONG_LEFT;
    if (state2 == LongPressed) return LONG_RIGHT;
    if (state1 == Pressed) return LEFT;
    if (state2 == Pressed) return RIGHT;

    return LastEvent;
}


/*
 * Called every shortTimeBase ~30-50ms
 */
void handleEvent(void) {
    static uint8_t button1PressTime=0;
    static uint8_t button2PressTime=0;
    ButtonState state2=NotPressed;
    Event ev = LastEvent;

    if (deactivateEvents) {
        --deactivateEvents;
        return;
    }

    if (timeBase512ms > TIMEOUT_TIME) {
        ev = TIMEOUT;
        timeBase512ms = 0;
    } else {

        ButtonState state1 = getButton(TAST1_OnPortD, &button1PressTime);

        // Don't request Buttonstate when alarming, otherwise You hear it.
        if (currentState != ALARM) {
            state2 = getButton(TAST2_OnPortD, &button2PressTime);
        }

        if (second == 0 && (currentState == COUNT || currentState == POWERSAVE)) {
            currentState = ALARM;
            initSecond();
            timeBase512ms = TIMEOUT_TIME - ALARM_TIME;
            return;
        } else {
            if (state1 == NotPressed && state2 == NotPressed) return;
        }
        timeBase512ms = 0;

        ev = makeEvent(state1, state2);
    }

    stateFunction f = (stateFunction)pgm_read_word(&StateTransitionTable[currentState][ev]);
    if (f != 0) {
        currentState = f();
    }
}

