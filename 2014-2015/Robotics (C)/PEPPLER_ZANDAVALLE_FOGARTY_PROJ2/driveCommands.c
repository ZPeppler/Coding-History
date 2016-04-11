#include <avr/io.h>
#include <avr/interrupt.h>
#include "driveCommands.h"
#include "OI.h"

volatile uint16_t timerCount =0;
volatile uint8_t timerRunning =0;
volatile uint16_t responseTimer = RESPONSE_TIMER_RESET;

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
   //high = byteRx();
   //low = byteRx();
   return ((byteRx() << 8)| byteRx());
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
   byteTx(PACKET_NUMBERS);
   byteTx(Q_BATTERY_CHARGE);
   byteTx(Q_WALL_SENSOR);
   byteTx(Q_CLIFF_L);
   byteTx(Q_CLIFF_FL);
   byteTx(Q_CLIFF_FR);
   byteTx(Q_CLIFF_R);
}

/* Gets the value of the 6 sensor readings */

void getSensors(uint16_t *bCO, uint16_t *wSO, 
                uint16_t *cLO, uint16_t *cFLO, 
                uint16_t *cFRO, uint16_t *cRO)
{
   *bCO  = byteRx16();
   *wSO  = byteRx16();
   *cLO  = byteRx16();
   *cFLO = byteRx16();
   *cFRO = byteRx16();
   *cRO  = byteRx16();
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
