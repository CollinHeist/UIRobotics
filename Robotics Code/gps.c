//gps.c
#include "gps.h"

void UpdateAngleToGoal(VariablesList *VariablesList) {	//Pass pointer to Variable list struct
	// Find deltaCoords
	double delta[2];
	//Find difference between the destination coord and the current
	delta[0] = VariablesList -> goalXCoord - VariablesList -> roboXCoord;
	delta[1] = VariablesList -> goalYCoord - VariablesList -> roboYCoord;

	//	X is neg?
	int xNeg = 0;	//Initialize as no
	if (delta[0] < 0)
		xNeg = 1;	//Yes
	
	// Y is neg?
	int yNeg = 0;	//Initialize as no
	if (delta[1] < 0)
		yNeg = 1;	//Yes
	
	// Switch X and Y - reorienting the coordinates so that the angles calculated will have the zero degree as North
	{
		double temp = delta[0];
		delta[0] = delta[1];
		delta[1] = -1 * temp;
	}

	// Find Angle
	double angle = 0;
	if (delta[0] == 0 && delta[1] == 0)
		return;
	else
		angle = asin(delta[1]/(sqrt(delta[0]*delta[0]+delta[1]*delta[1])))*(180/(4*atan(1)));  //using 4atan(1) for pi
	
	// Quadrant?
	int Quadrant = 1;
	if (xNeg == 1 && yNeg == 0)
		Quadrant = 2;
	else if (xNeg == 1 && yNeg == 1)
		Quadrant = 3;
	else if (xNeg == 0 && yNeg == 1)
		Quadrant = 4;
	
	// Finalizing Angles
	if (Quadrant == 1)
		angle = angle * 1;
	else if (Quadrant == 2)
		angle = angle * 1;
	else if (Quadrant == 3)
		angle = (180 - angle);
	else if (Quadrant == 4)
		angle = -1 * (180 + angle);

	// Set var Angle
	VariablesList -> angleToGoalCalc = angle;
}
