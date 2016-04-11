#include <avr/io.h>
#include <avr/interrupt.h>
#include "driveCommands.h"
#include "OI.h"

int main(void)
{
   uint16_t bCO, wSO, cLO, cFLO, cFRO, cRO;
   char myBuffer[100];
   initialize();

   for(;;)
   {
      if(responseTimer == 0)
      {
         setSerialDestination(SERIAL_CREATE);
         /* (1.1) Read the robot's battery charge, wall signal
            and the four cliff sensor values.*/
	 sendSensors();

         getSensors(&bCO, &wSO, &cLO, &cFLO, 
                    &cFRO, &cRO);
         
         setSerialDestination(SERIAL_USB); 
        
         /* (1.2) Display those sensor values in a human-
            readable format, identified with English text
            strings, on the workstation's screen. */
         sprintf(myBuffer,"This is the battery charge: %u", bCO);
	 print(myBuffer);
         sprintf(myBuffer,"This is the wall sensor: %u", wSO);
         print(myBuffer);
         sprintf(myBuffer, "This is the left cliff sensor: %u", cLO);
         print(myBuffer);
         sprintf(myBuffer, "This is the front left cliff sensor: %u", cFLO);
         print(myBuffer);
         sprintf(myBuffer, "This is the front right cliff sensor: %u", cFRO);
         print(myBuffer);
         sprintf(myBuffer, "This is the right cliff sensor: %u", cRO);
         print(myBuffer);
         sprintf(myBuffer, "\n");
         print(myBuffer);
         
         /* (1.3) Repeat once per second */
         responseTimer = RESPONSE_TIMER_RESET;
      }
   }

   return 0;
}
