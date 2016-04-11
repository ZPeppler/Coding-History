#include <avr/io.h>
#include <avr/interrupt.h>
#include "driveCommands.h"
#include "OI.h"

int main(void)
{
   initialize();

   uint8_t remote;
   int forwardSafe;
   int turnSafe;

   for(;;)
   {

      /* Using functions, this checks to see if the
         robot is safe to move forward, to turn, and if
         the remote has sent a signal */
      forwardSafe = checkForward();
      turnSafe    = checkTurn();
      remote      = getRemote();

      /* The robot received a remote signal to drive forward
         AND it is safe to move forward */
      if((remote == REMOTE_FORWARD) && (forwardSafe==1))
      {
         /* The robot will continue to move forward as long
            as there is a signal to remove forward AND that 
            it is safe to do so */
         while((remote == REMOTE_FORWARD) && (forwardSafe==1))
         {
            forwardSafe = checkForward();
   	    driveStraight(DRIVE_SPEED);
            remote = getRemote();
         }
         stop();
      }

      /* The robot received a remote signal to turn left
         AND it is safe to move forward */
      if((remote == REMOTE_LEFT) && (turnSafe==1))
      {
         turnCCWTime(DRIVE_SPEED, TURN_45);
         stop();
      }

      /* The robot received a remote signal to turn right
         AND it is safe to move forward */ 
      if((remote == REMOTE_RIGHT) && (turnSafe==1))
      {
         turnCWTime(DRIVE_SPEED, TURN_45);
         stop();
      }
     
      /* Extra checks to make sure the robot stops if it is
         not safe to move at all */
      if((!forwardSafe) && (!turnSafe))
      {
         stop();
      }
   }

   return 0;
}
