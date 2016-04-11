#include <avr/io.h>
#include <avr/interrupt.h>
#include "driveCommands.h"
#include "OI.h"


int main(void)
{
   initialize();
// Part 1: get to wall
   findWall();
   alignToWall();

// Part 2: PID control
   //followWall();
   shiftFollowWall();
   return 0;
}
