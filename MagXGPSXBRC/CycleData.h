/* 
 * File:   CycleData.h
 * Author: Damon
 *
 * Created on February 10, 2019, 3:43 PM
 */

#include "Gamepad.h"

#pragma once

typedef struct cycleData
{
	// Motor and Servo data
	int Servo3Position;
	int Servo4Position;
	int Motor1Speed;
	int Motor2Speed;

	// GPS data
	unsigned int GPS_UTCTime;
	unsigned int GPS_Date;
	char GPS_NorthSouth;
	char GPS_Status;
	char GPS_EastWest;
	float GPS_Latitude;
	float GPS_Longitude;
	float GPS_Speed;
	float GPS_Angle;


} Cycle;

