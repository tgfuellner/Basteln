#ifndef __COUNTDOWN_H
#define __COUNTDOWN_H

#define MAX_SECOND (3600*6-1)
#define EEPROM_STORE_START 1

// Input
#define TAST1_OnPortD PD2               // Left
#define TAST2_OnPortD PD5               // Right - Also Buzzer+


typedef int8_t bool;

typedef enum {
    INIT=0,COUNT,WAIT,ADJUSTSECOND,ADJUSTMINUTE,ADJUSTHOUR,ALARM,POWERSAVE, LastState
} __attribute__ ((packed)) State;

extern int16_t volatile second;     // The remaining seconds
extern State currentState;
extern uint16_t *firstSecondInEEprom;
extern uint8_t timeBase512ms;       // Used for timeout recognition
extern void setColumnValues(int16_t sec);

// Are calculatet every changing second
// 7 Column values for 7 Rows
extern uint8_t currentColValues[7];


void handleEvent(void);
void initSecond(void);
State init(void);
void beeper (uint8_t on);
void initIO(void);

#endif /* __COUNTDOWN_H */
