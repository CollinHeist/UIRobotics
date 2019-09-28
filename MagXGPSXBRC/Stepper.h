// 
// File:   Stepper.h
// Author: Damon
//
// Created on April 20, 2019, 12:16 PM
//

// Prevent Multiple Inclusion
#pragma once

#include "hardware.h"
#include <plib.h>

#define STEPPER_MODE_DIR	PORTSetDigitalOut(IOPORT_F, BIT_1)	//RF1
#define STEPPER_MODE(a)		PORTSetBits(IOPORT_F, BIT_1)		//RF1

#define FULL_STEP			(int) 2
#define HALF_STEP			(int) 1
#define CW					(int) 1
#define CCW					(int) -1

#define STEPSPREREV		 1625	//48
#define RPM2MSPERHALFSTEP   (370/2)   // 6250	//1/10 ms per half step per RPM  
#define REVS				(65*25*10+5)

// Function Prototypes
void stepper_init(void);
void step(int dir, int mode);