// BYU Mars Rover Header for the Main Controller Board
// Authors:
// 		Steve Carlson
// 		<Your Name Here>

#ifndef ROVER_MAIN_DEF_H
#define ROVER_MAIN_DEF_H

//*** Preprocessor Directives
// Behavioral Switches
// #define DEBUG	// Uncomment to activate any debugging behaviors to be retained in the final release

// Behavioral Parameters
#define SOME_IMPORTANT_BEHAVIORAL_CONSTANT 1

// System Parameters
#define ROVER_VOLTS_MAX		15000	// Value in mV (15.00 V)
#define ROVER_VOLTS_MIN		10000	// Value in mV (10.00 V), Danger zone for battery damage

// System Constants
#define HIGH			1 // Both of these macros are used by the WizNet, are defined in that file.
#define LOW				0
#define ENABLED			1
#define DISABLED		0

// Port Definitions and Macros
// 		Usually in a Vanilla-Flavored (Non-PSOC) embedded project, we might define ports and buttons here.
// 		In this case, I am defining the button behavior here, as follows:
#define BUTTON_PRESSED	!SW_Read()


// Included Headers
#include <stdint.h>
#include "roverlink.h"


//*** Global Variables
// General and Timing
static volatile uint32_t timer1ms = 0; 
static volatile uint32_t timer1us_p = 0;


// Drive and Gimbal PWM
// static volatile uint16_t pwmValues[CHANNELS] = {2000,2000,2000,2000,2000,2000,2000,2000}; // In uSec, 
// static uint16_t pwmFailsafes[CHANNELS]; // Same thing as neutral trim values, wheels stopped, gimbal centered.

// Video Mux, Science Instruments, etc

// LRS Backup-link (Dragonlink)
static volatile uint8_t lrs_ch;
static volatile uint16_t lrs_ppmFrameSum; // Must be able to contain the accumulated sum of pwmValues[]
static uint16_t lrs_frameErrors;

// Rover Power System
static struct{
	uint16_t volts;
	uint16_t amps;
} power;

// State Machine
const enum BOARD_ID board_id = MAIN_CONTROLLER; // Feels clunky, redo enum mechanism
enum SYSTEM_STATE system_state = STARTUP;

// RoverLink Variables


static uint8_t GPS_STRING[80] = "$GPGGA,230416.000,4014.8247,N,11138.8521,W,1,05,3.8,1437.5,M,-16.4,M,,0000*54";


//*** Function Prototypes
void setup(void);
void loop(void);
uint8_t cypressInit(void);







#endif // ROVER_MAIN_DEF_H