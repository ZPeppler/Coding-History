#ifndef INCLUDE_DRIVECOMMANDS_H
#define INCLUDE_DRIVECOMMANDS_H

void setupRightLED(void);
void rightLEDOn(void);
void rightLEDOff(void);
void setupLeftLED(void);
void leftLEDOn(void);
void leftLEDOff(void);
void setupTimer(void);

extern volatile uint16_t timerCount;
extern volatile uint8_t timerRunning;
extern volatile uint16_t responseTimer;

void delayMs(uint16_t timeMs);
SIGNAL(TIMER1_COMPA_vect);
void setupSerialPort(void);
void byteTx(uint8_t value);
uint8_t byteRx(void);
uint16_t byteRx16(void);
void driveStraight(uint8_t velocity);
void stop(void);
void turnClockwise(uint8_t velocity);
void turnCounterClockwise(uint8_t velocity);
void waitTime(uint8_t time);
void waitEvent(uint8_t event);
void driveStraightTime(uint8_t velocity, uint16_t timeMs);
void turnCCWTime(uint8_t velocity, uint16_t timeMs);
void turnCWTime(uint8_t velocity, uint16_t timeMs);
void setSerialDestination(uint8_t dest);
int bumpTrigger(void);
int cliffTrigger(void);
int dropTrigger(void);
int getRemote(void);
int checkForward(void);
int checkTurn(void);
void print(const char *in);
void sendSensors(void);
void getSensors(uint16_t *bCO, uint16_t *wSO, 
                uint16_t *cLO, uint16_t *cFLO, 
                uint16_t *cFRO, uint16_t *cRO);
void initialize(void);
void createRedLED(void);
void createOrangeLED(void);

#endif
