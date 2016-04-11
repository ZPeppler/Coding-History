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
extern volatile uint16_t rotateCount;
extern volatile uint8_t rotateRunning;
extern volatile uint16_t responseTimer;
extern volatile uint16_t printerTimer;

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
void getSensors(uint8_t *bCO, uint16_t *wSO);
void initialize(void);
void createRedLED(void);
void createOrangeLED(void);
uint8_t getBump(void);
uint16_t getWallSensor(void);
void driveDirectCreate(uint8_t rHigh, uint8_t rLow, uint8_t lHigh, uint8_t lLow);
void driveDirectTime(uint8_t rHigh, uint8_t rLow, uint8_t lHigh, uint8_t lLow, uint16_t timeMs);
void findWall(void);
void alignToWall(void);
void splitWord(uint16_t in, uint8_t *high, uint8_t *low);
int shiftPID(int eCurr, int ePrev, int *eData);
void shiftFollowWall(void);

#endif
