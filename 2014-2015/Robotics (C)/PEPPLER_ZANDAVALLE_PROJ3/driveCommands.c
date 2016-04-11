#include <avr/io.h>
#include <avr/interrupt.h>
#include "driveCommands.h"
#include "OI.h"

volatile uint16_t timerCount = 0;
volatile uint8_t timerRunning =0;
volatile uint16_t responseTimer = RESPONSE_TIMER_RESET;
volatile uint16_t printerTimer = PRINTER_TIMER_RESET;
volatile uint16_t rotateCount = 0;
volatile uint8_t rotateRunning = 0;

/* Set the fifth bit of the direction register 
   for port D to one. This sets pin 5 of Port D, which
   controls the right LED, to output mode. Done once before
   calling rightLEDOn() */
void setupRightLED(void)
{
   DDRD != (1 << 5);
}

/* Since these LEDs use an active low system, we write a 0 to
   turn the LEDs on. This sets of the fifth bit of port D to
   0, which will turn on the right LED on. */
void rightLEDOn(void)
{
   PORTD &= ~(1 << 5);
}

/* Since these LEDs use an active low system, we write a 1 to
   turn the LEDs on. This sets of the fifth bit of port D to
   1, which will turn on the right LED off. */
void rightLEDOff(void)
{
   PORTD |= 0x20;
}

/* Set the sixth bit of the direction register 
   for port D to one. This sets pin 6 of Port D, which
   controls the left LED, to output mode. Done once before
   calling leftLEDOn() */
void setupLeftLED(void)
{
   DDRD != (1 << 6);
}

/* Since these LEDs use an active low system, we write a 0 to
   turn the LEDs on. This sets of the sixth bit of port D to
   0, which will turn on the left LED on. */
void leftLEDOn(void)
{
   PORTD &= ~(1 << 6);
}

/* Since these LEDs use an active low system, we write a 1 to
   turn the LEDs on. This sets of the sixth bit of port D to
   1, which will turn on the right LED off. */
void leftLEDOff(void)
{
   PORTD |= 0x40;
}

/* Set up the timer 1 interrupt to be called every 1 ms. This 
   is treated as a black box. Except for the 71, these are all
   special codes for which details appear in the ATMega168 data
   sheet. The 71 is a computed value based on processor speed and 
   the amount of "scaling of the timer" that gives us 1ms intervals. */
void setupTimer(void)
{
   TCCR1A = 0x00;
   TCCR1B = 0x0C;
   OCR1A = 71;
   TIMSK1 = 0x02;
}

/* Delay for a number of milliseconds. Call setupTimer() before this. */
void delayMs(uint16_t timeMs)
{
   timerCount = timeMs;
   timerRunning = 1;
   while(timerRunning != 0)
   {
    //do nothing
   }
}

/* Interrupt handler called every 1ms. We decrement the counter variable
   to allow delayMs to keep time */
SIGNAL(TIMER1_COMPA_vect)
{
   if(timerRunning != 0)
   {
      if(timerCount != 0)
      {
         timerCount--;
      }
      else
      {
         timerRunning = 0;
      }
   }

   if(responseTimer != 0)
   {
      responseTimer--;
   }
   if(printerTimer != 0)
   {
      printerTimer--;
   }
   if(rotateRunning)
   {
      if(rotateCount != 0)
      {
         rotateCount--;
      }
      else
      {
         rotateCount = 0;
      }
   }
}

/* Set the transmission speed to 57600 baud, which is what the Create
   expects unless we tell it otherwise.*/
void setupSerialPort(void)
{
   UBRR0 = 19;
   UCSR0B = 0x18;
   UCSR0C = 0x06;
}

/* There is a 15 ms delay before and after each transmit. This is 
   the time it takes the UART to obtain each byte. After that we
   transmit one byte to the robot, wait for the buffer to be 
   empty, and then send the byte. */
void byteTx(uint8_t value)
{
   while(!(UCSR0A & 0x20))
   { 
    //do nothing
   }

   UDR0 = value;
}

/* This is a function that will receive one byte from the robot.
   Calling setupSerialPort() should be done first. It will wait
   for a byte to arrive in the receive buffer before returning
   that byte. */
uint8_t byteRx(void)
{
   while(!(UCSR0A & 0x80));

   return UDR0;
}

uint16_t byteRx16(void)
{
   uint8_t high, low;
   high = byteRx();
   low = byteRx();
   return ((high << 8)|low);
}

/* Using the OP code to drive, this function will drive the 
   robot forward at any velocity (mm/s) that's between 0 and 255, 
   since it only takes in an eight bit unsigned integer. 
   A special code is used for the radius in order to make it
   drive straight, which was provided in the Open Interface. */
void driveStraight(uint8_t velocity)
{
   byteTx(OC_DRIVE);
   byteTx(0);
   byteTx(velocity);
   byteTx(DRIVE_STRAIGHT1);
   byteTx(DRIVE_STRAIGHT2);
}

/* Using the OP code for drive, this function will make
   the velocity of the wheels to be zero. While this can be
   done using driveStraight(0), having a speific function
   telling it to stop is more useful and makes the code 
   easier to read. */
void stop(void)
{
   byteTx(OC_DRIVE);
   byteTx(DRIVE_STOP_VELOCITY);
   byteTx(DRIVE_STOP_VELOCITY);
   byteTx(DRIVE_STOP_RADIUS);
   byteTx(DRIVE_STOP_RADIUS);
}

/* Using the OP code for drive, this function will turn the
   robot in place going clockwise. Again, like driveStraight
   this is limited to a velocity between 0 and 255. A special
   code was used to make this robot turn in place, provided
   by the Open Interface. */
void turnClockwise(uint8_t velocity)
{
   byteTx(OC_DRIVE);
   byteTx(0);
   byteTx(velocity);
   byteTx(DRIVE_CW1);
   byteTx(DRIVE_CW2);
}

/* Using the OP code for drive, this function will turn the
   robot in place going counterclockwise. Again, like driveStraight
   this is limited to a velocity between 0 and 255. A special
   code was used to make this robot turn in place, provided
   by the Open Interface. */
void turnCounterClockwise(uint8_t velocity)
{
   byteTx(OC_DRIVE);
   byteTx(0);
   byteTx(velocity);
   byteTx(DRIVE_CCW1);
   byteTx(DRIVE_CCW2);
}

/* Using an Op Code for waiting, this function will wait a 
   certain amount of time. */
void waitTime(uint8_t time)
{
   byteTx(OC_WAIT_TIME);
   byteTx(time);
//   return 0;
}

/* Using an Op code for waiting, this function will wait
   until a predetermined event occurs. */
void waitEvent(uint8_t event)
{
   byteTx(OC_WAIT_EVENT);
   byteTx(event);
}

/* Using a combination of the driveStraight() and delayMs()
   functions, this function will drive straight at a specific
   velocity for a specific time. */
void driveStraightTime(uint8_t velocity, uint16_t timeMs)
{
   driveStraight(velocity);
   delayMs(timeMs);
}

/* Using a combination of the turnCounterClockwise() and delayMs()
   functions, this function will have the robot turn in place
   counterclockwise for a specific amount of time. */
void turnCCWTime(uint8_t velocity, uint16_t timeMs)
{
   turnCounterClockwise(velocity);
   delayMs(timeMs);
}

/* Using a combination of the turnClockwise() and delayMs()
   functions, this function will have the robot turn in place
   clockwise for a specific amount of time. */
void turnCWTime(uint8_t velocity, uint16_t timeMs)
{
   turnClockwise(velocity);
   delayMs(timeMs);
}

/* The code in the create notes to send data to and from the
   robot */
void setSerialDestination(uint8_t dest)
{
   delayMs(10);

   if(dest == SERIAL_CREATE)
   {
      PORTB &= ~0x10;
   }
   else
   {
      PORTB |= 0x10;
   }

   delayMs(10);
}

/* Checks to see if either bump sensor has been triggered. 
   Returns a 1 if either have been or a 0 if they haven't. */

int bumpTrigger(void)
{
  byteTx(OC_READ_SENSORS);
  byteTx(SEN_BUMPS_AND_WHEELS_DROP);
  uint8_t bumpDrop = byteRx();
      
  return (( bumpDrop & 1) || (bumpDrop & 2));
}

/* Checks to see if any of the cliff sensors have been
   triggered. Returns a 1 if one has or a 0 if none have. */

int cliffTrigger(void)
{
   byteTx(OC_READ_SENSORS);
   byteTx(SEN_CLIFF_L);
   uint8_t cliffL = byteRx();
   byteTx(OC_READ_SENSORS);
   byteTx(SEN_CLIFF_FL);
   uint8_t cliffFL = byteRx();      
   byteTx(OC_READ_SENSORS);
   byteTx(SEN_CLIFF_FR);
   uint8_t cliffFR = byteRx();
   byteTx(OC_READ_SENSORS);
   byteTx(SEN_CLIFF_R);
   uint8_t cliffR = byteRx();
   return (cliffL | cliffFL | cliffFR | cliffR);
}

/* Checks to see if any of the drop sensors have been
   triggered. Returns a 1 if one has or a 0 if none
   have */

int dropTrigger(void)
{
  byteTx(OC_READ_SENSORS);
  byteTx(SEN_BUMPS_AND_WHEELS_DROP);
  uint8_t bumpDrop = byteRx();
  return ((bumpDrop & 4) || (bumpDrop & 8) || (bumpDrop & 16));
}

/* Returns the value of the remote sensor */

int getRemote(void)
{
   byteTx(OC_READ_SENSORS);
   byteTx(SEN_REMOTE);
   return byteRx();
}

/* Checks the bump, wheel drop and cliff sensors to 
   determine if it is safe for the robot to move forward.
   If it is not safe, the LED will blink red-orange. */

int checkForward(void)
{
    if((!bumpTrigger()) && (!dropTrigger()) && 
       (!cliffTrigger()))
    {
        createRedLED();
        return 1;
    } 
    else
    {
        createOrangeLED();
        return 0;
    }
}

/* Checks the drop sensor to determine if it is safe
   for the robot to turn. If it is not safe, the LED
   will be a solid orange color */

int checkTurn(void)
{
   if(dropTrigger() == 1) 
   {
      createOrangeLED();
      return 0;
   }
   else
   {
      createRedLED();
      return 1;
   }
}

/* Taking in a character array, it will send every
   character in that array until it reaches the null
   character that is present at the end of every string.
   After which, it will return a new line */

void print(const char *in)
{
   int i = 0;
   while(in[i] != '\0')
   {
      byteTx(in[i]);
      i++;
   }
   byteTx('\n');
}

/* Basic initialization for the robot. Goes through all
   the setups. Enters it into full mode. Turns the 
   Power LED to red */

void initialize(void)
{
   // Disable interrupts
   cli();

   // One-time setup
   setupRightLED();
   setupLeftLED();
   setupSerialPort();
   setupTimer();

   // Enable interrupts
   sei();
   byteTx(OC_START);
   byteTx(OC_FULL_MODE);

   createRedLED();
}

/* Sends a query with 6 packets for various sensor */

void sendSensors(void)
{
   byteTx(OC_QUERY);
   byteTx(2);
   byteTx(7);
   byteTx(Q_WALL_SENSOR);
}

/* Gets the value of the 6 sensor readings */

void getSensors(uint8_t *bCO, uint16_t *wSO)
{
   *bCO  = byteRx();
   *wSO  = byteRx16();
}

/* Basic functionality to turn the power LED to red */

void createRedLED(void)
{
   byteTx(OC_SETLEDS);
   byteTx(LED_BOTHOFF);
   byteTx(LED_FULLRED);
   byteTx(LED_FULLINTENSE);
}

/* Basic functionality to turn the power LED to orange */

void createOrangeLED(void)
{
   byteTx(OC_SETLEDS);
   byteTx(LED_BOTHOFF);
   byteTx(LED_ORANGE);
   byteTx(LED_FULLINTENSE);
}

/* Ask the sensors for the value of the bumps and wheel drop */

uint8_t getBump(void)
{
  byteTx(OC_READ_SENSORS);
  byteTx(SEN_BUMPS_AND_WHEELS_DROP);
  return byteRx();
}

/* Query the robot for the value of the wall sensor */

uint16_t getWallSensor(void)
{
   byteTx(OC_QUERY);
   byteTx(1);
   byteTx(Q_WALL_SENSOR);
   return byteRx16();
}

/* Drive direct command */

void driveDirectCreate(uint8_t rHigh, uint8_t rLow, uint8_t lHigh, uint8_t lLow)
{
   byteTx(OC_DRIVE_DIRECT);
   byteTx(rHigh);
   byteTx(rLow);
   byteTx(lHigh);
   byteTx(lLow);
}

/* Drive the robot forward until it either picks up
   a wall sensor reading or it runs into a wall. */

void findWall(void)
{
   uint16_t wallSensor  = getWallSensor();
   uint8_t bump = getBump();
   int forwardSafe;
   int turnSafe;
   while((wallSensor < WALL_THRESHOLD) && (bump == 0))
   {
      wallSensor  = getWallSensor();
      bump        = getBump();
      driveDirectCreate(midPosVelHigh, midPosVelLow, midPosVelHigh, midPosVelLow);
    }
}

/* Align the robot on to a wall, making a counter-
   clockwise turn to keep the wall sensor (which is 
   on the right side) aligned with the wall */

void alignToWall(void)
{
   driveDirectCreate(midPosVelHigh, midPosVelLow, midNegVelHigh, midNegVelLow);
   while(getWallSensor() < SET_POINT);
}


/* Split an unsigned 16 bit integer into two unsigned 
   8 bit integers. */

void splitWord(uint16_t in, uint8_t *high, uint8_t *low)
{
   *high = (in >> 8) & 0xFF;
   *low  = in & 0xFF;
}

/* Drive the robot using the drive direct command for a
   set amount of time */

void driveDirectTime(uint8_t rHigh, uint8_t rLow, uint8_t lHigh, uint8_t lLow, uint16_t timeMs)
{
   driveDirectCreate(rHigh, rLow, lHigh, lLow);
   timerRunning = 1;
   timerCount = timeMs;
}

/* Calculate a PID state output using bitwise shifting
   in order to simulate floating point calculations. */

int shiftPID(int eCurr, int ePrev, int *eData)
{
   //Calculate p
   int p = P_GAIN*eCurr;
   //Calculate i
   int eSum = 0;
   for(int i = 0; i < DATA_SIZE; i++)
   {
      eSum += eData[i];
   }
   int i = I_GAIN*DELTA_T*(eSum);
   //Calculate d
   int d = ((D_GAIN*(eCurr - ePrev))/DELTA_T);
   // Return the sum of p+i+d with shifts in the
   // final output to simulate floats
   return (p >> 2) + (i >> 5) + (d >> 2);
}

/* This code will execute assuming that the robot has
   found a wall and is aligned to it. Using a PID controller
   it will align itself to a wall and follow it until it 
   either reaches another wall (by triggering a bump sensor) 
   or runs out of wall (by the wall sensor giving a group of 
   0's. At which point it'll either turn left until it 
   no longer has a bump triggered, or it'll take a hard right
   turn to realign itself to a wall.*/

void shiftFollowWall(void)
{
   // Set LED to orange to signal mode change
   createOrangeLED();
   //Variables used in this function
   uint8_t bump;
   int wallSensor = getWallSensor();
   int ePrev = 0;
   int eCurr = getWallSensor()-SET_POINT;
   int eData [DATA_SIZE];
   int insertIndex = 0;
   int state;
   uint16_t lVelocity;
   uint16_t rVelocity;
   uint8_t lVHigh = 0;
   uint8_t lVLow  = 0;
   uint8_t rVHigh = 0;
   uint8_t rVLow  = 0;

   for(;;)
   {
      // Check for bumps outside the timer
      // eliminate chance for robot to grind 
      // on a wall
      bump = getBump();
      if(bump)
      {
         alignToWall();
      }

      if(responseTimer == 0)
      {
         // Handle the error values to send in
         ePrev = eCurr;
         eCurr = getWallSensor()-SET_POINT;
         eData[insertIndex] = eCurr;
         insertIndex++;
         if(insertIndex >= DATA_SIZE)
         {
            insertIndex = 0;
         }
         state = (shiftPID(eCurr, ePrev, eData) << 1);
         lVelocity = BASE_VELOCITY - state;
         rVelocity = BASE_VELOCITY + state;
         // Set a min and max velocity to ensure
         // the robot drives within this range
         if(lVelocity <= MIN_VELOCITY)
         {
            lVelocity = MIN_VELOCITY;
         }
         if(lVelocity >= MAX_VELOCITY)
         {
            lVelocity = MAX_VELOCITY;
         }
         if(rVelocity <= MIN_VELOCITY)
         {
            rVelocity = MIN_VELOCITY;
         } 
         if(rVelocity >= MAX_VELOCITY)
         {
            rVelocity = MAX_VELOCITY;
         }
         splitWord(lVelocity, &lVHigh, &lVLow);
         splitWord(rVelocity, &rVHigh, &rVLow);
         driveDirectCreate(rVHigh, rVLow, lVHigh, lVLow);
         responseTimer = RESPONSE_TIMER_RESET;
      }
   }
} 
