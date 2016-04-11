#ifndef INCLUDE_OI_H
#define INCLUDE_OI_H

#define OC_START 128
#define OC_FULL_MODE 132
#define OC_SETLEDS 139
#define OC_READ_SENSORS 142 
#define OC_DRIVE 137
#define OC_WAIT_TIME 155
#define OC_WAIT_EVENT 158
#define OC_QUERY 149
#define OC_DRIVE_DIRECT 145

#define LED_BOTHON 10
#define LED_BOTHOFF 0
#define LED_FULLGREEN 0
#define LED_FULLRED 255
#define LED_ORANGE 40
#define LED_FULLINTENSE 255
#define LED_PLAY_ON 2
#define LED_ADVANCE_ON 8

#define SEN_BUMPS_AND_WHEELS_DROP 7
#define SEN_BUTTONS 18
#define SEN_REMOTE 17
#define SEN_CLIFF_L 9
#define SEN_CLIFF_FL 10
#define SEN_CLIFF_FR 11
#define SEN_CLIFF_R 12

#define DRIVE_STOP_VELOCITY 0
#define DRIVE_STOP_RADIUS 0
#define DRIVE_STRAIGHT1 128
#define DRIVE_STRAIGHT2 0
#define DRIVE_CW1 255
#define DRIVE_CW2 255
#define DRIVE_CCW1 0
#define DRIVE_CCW2 1
#define DRIVE_SPEED 200

// Driving at 100 mm/s with Drive Direct
#define midPosVelHigh   0
#define midPosVelLow    100
#define midNegVelHigh   255
#define midNegVelLow    156

#define TURN_45 175
#define TURN_15 88

#define WAIT_EVENT_PLAY 17
#define WAIT_EVENT_ADVANCE 16

#define RESPONSE_TIMER_RESET 100
#define PRINTER_TIMER_RESET 100

#define SERIAL_CREATE 1
#define SERIAL_USB 2

#define PACKET_NUMBERS 6

#define Q_BATTERY_CHARGE 25
#define Q_WALL_SENSOR 27
#define Q_CLIFF_L 28
#define Q_CLIFF_FL 29
#define Q_CLIFF_FR 30
#define Q_CLIFF_R 31

#define REMOTE_FORWARD 130
#define REMOTE_LEFT 129
#define REMOTE_RIGHT 131

#define WALL_THRESHOLD 5
#define SET_DELTA 15
#define SET_POINT 35
#define P_GAIN 1
#define I_GAIN 1
#define D_GAIN 1
#define DELTA_T 1
#define BASE_VELOCITY 100
#define MIN_VELOCITY 0
#define MAX_VELOCITY 150
#define DATA_SIZE 10

#endif
