/*  Gamepad.c
 *  Author: Damon
 *  Commenter: Sam
 *
 *  Purpose: Integration of XBox Controller as control surface
 *
 */
#ifndef _SUPPRESS_PLIB_WARNING
    #define _SUPPRESS_PLIB_WARNING
#endif

// File Inclusion
#include "uart2.h"
#include "uart4.h"
#include "hardware.h"
#include "RC.h"

#include "DMA_UART2.h"
#include <plib.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define nullptr ((void*)0)
//#define Debug

#define MODE_TEST   //IDTTDA
#define MODE_NORM   //IDTTDA

// Possible gamepad messages	//Information coming from the controller
enum GamepadVariables{
    gameTime,		//Time info was sent
    gameTriggerLeft,
    gameBumperLeft,
    gameTriggerRight,
    gameBumperRight,
    gameStickLeftX,
    gameStickLeftY,
    gameDPadX,
    gameDPadY,
    gameStickRightX,
    gameStickRightY,
    gameButtonY,
    gameButtonB_,
    gameButtonA,
    gameButtonX,
    gameButtonStart,
    gameButtonBack
};

typedef struct pair
{
	int varCompOne;
	int varCompTwo;
} 
Pair;

typedef struct buttons
{
	int varButtonY;
	int varButtonB_;
	int varButtonA;
	int varButtonX;
}
Buttons;

typedef struct gamepadInput
{
	unsigned int varTriggerLeft;
	unsigned int varTriggerRight;

	int varTimeSendTime;
	int varBumperLeft;
	int varBumperRight;
	int varButtonStart;
	int varButtonBack;
	int varOffset;

	Pair varSticksLeft;		//A "Pair" struct of the left stick variables - x and y values
	Pair varSticksRight;		//A "Pair" struct of the right stick variables - x and y values
	Pair varDPad;			//A "Pair" struct of the Dpad variables - x and y values
	Buttons varButtons;		//A "Buttons" struct of the different controller buttons

	char varInputString[512];
} 
gamepadInput_;

// The main object that handles input
static gamepadInput_ gamepadInputManager;

static void moveLeft(int movement);

//
// gamepadInit()
// Should be called once prior to using the gamepadInputManager
// Properly initializes all of the members of this object.
//
int gamepadInit()
{
    int i = 0;
     
    gamepadInputManager.varOffset = 0;
    for(i = 0; i < 512; i++)
    {
        gamepadInputManager.varInputString[i] = 0;
    }
    
    gamepadInputManager.varButtonBack = 0;
    gamepadInputManager.varTriggerLeft = 0;
    gamepadInputManager.varTriggerRight = 0;
    gamepadInputManager.varBumperLeft = 0;
    gamepadInputManager.varBumperRight = 0;
    gamepadInputManager.varButtonStart = 0;
    gamepadInputManager.varButtonBack = 0;
    
    gamepadInputManager.varDPad.varCompOne = 0;
    gamepadInputManager.varDPad.varCompTwo = 0;
    gamepadInputManager.varSticksRight.varCompOne = 0;
    gamepadInputManager.varSticksRight.varCompTwo = 0;
    gamepadInputManager.varSticksLeft.varCompOne = 0;
    gamepadInputManager.varSticksLeft.varCompTwo = 0;
    
    gamepadInputManager.varButtons.varButtonA = 0;
    gamepadInputManager.varButtons.varButtonB_ = 0;
    gamepadInputManager.varButtons.varButtonY = 0;
    gamepadInputManager.varButtons.varButtonX = 0;

}

//
// parseInput()
// Parses the string and places all of the 
// appropriate variables into their corresponding
// variables in the gamepadInputManager object.
//
int parseInput(char* stringRef)	    //stringRef, currently, is only the input string from the XBee
{
	// Local variables
	int i = 0;		    //traversal variable
	int j = 0;		    //traversal variable
	int variable = gameTime;
	char temp[16];		    //character string to 
	int time = 0;
	char* end;

	// Verifying that the sting received is valid
	if (stringRef == nullptr)
	{
		printf("stringRef received was nullptr\n");
		return;
	}

	// Parsing the stringRef
	for (i = 0; i < 512; i++)
	{

		if (!isspace(stringRef[i]))	//If location in stringRef is empty
		{
			// Copying contents of this variable
			for (j = 0; j < 16 && !isspace(stringRef[i]); j++, i++)
			{
				temp[j] = stringRef[i];
			}

			temp[j] = 0;			// Append null termination
			end = &(temp[j - 1]);	// pointer to end of this section

			// Case for each of the different variables in the string
			switch (variable)
			{
			case gameTime:

				time = strtol(temp, &end, 10);
			    #ifdef Debug
				printf("gameTime %s\n", temp);
			    #endif
                
				break;
			case gameTriggerLeft:

				time = strtol(temp, &end, 10);
			    #ifdef Debug
						printf("gameTriggerLeft %s\n", temp);
			    #endif

				break;
			case gameBumperLeft:
				time = strtol(temp, &end, 10);
			    #ifdef Debug
					    printf("gameBumperLeft %s\n", temp);
			    #endif

				break;
			case gameTriggerRight:
				time = strtol(temp, &end, 10);
			    #ifdef Debug
					    printf("gameTriggerRight %s\n", temp);
			    #endif

				break;
			case gameBumperRight:
				time = strtol(temp, &end, 10);
				//printf("gameBumperRight %d\n", time);

				break;
			case gameStickLeftX:
                
				time = strtol(temp, &end, 10);
                   
				// if(~mPORTCRead() & (BIT_13 | BIT_14))
			       //  {
				//    gamepadInputManager.varSticksLeft.varCompOne = 50;
			       //  }
				 gamepadInputManager.varSticksLeft.varCompOne = time;


				//printf("GAME_STICK_LEFT %d\n", time);
				break;
			case gameStickLeftY:

				time = strtol(temp, &end, 10);
                
                gamepadInputManager.varSticksLeft.varCompTwo = time;

				//printf("GAME_STICK_LEFT %d\n", time);
				break;
			case gameDPadX:
				time = strtol(temp, &end, 10);
				//printf("GAME_D_PAD %d\n", time);

				break;
			case gameDPadY:
				time = strtol(temp, &end, 10);
				//printf("GAME_D_PAD %d\n", time);

				break;
			case gameStickRightX:

				time = strtol(temp, &end, 10);
				//printf("GAME_STICK_RIGHT %d\n", time);

				break;
			case gameStickRightY:

				time = strtol(temp, &end, 10);
				//printf("GAME_STICK_RIGHT %d\n", time);

				break;
			case gameButtonY:

				time = strtol(temp, &end, 10);
				//printf("gameButtonY %d\n", time);

				break;
			case gameButtonB_:

				time = strtol(temp, &end, 10);
				//printf("gameButtonB_ %d\n", time);

				break;
			case gameButtonA:

				time = strtol(temp, &end, 10);
                gamepadInputManager.varButtons.varButtonA = time;				
                //printf("gameButtonA %d\n", time);

				break;
			case gameButtonX:

				time = strtol(temp, &end, 10);
				//printf("gameButtonX %d\n", time);

				break;
			case gameButtonStart:

				time = strtol(temp, &end, 10);
				//printf("gameButtonStart %d\n", time);

				break;
			case gameButtonBack:

				time = strtol(temp, &end, 10);
				//printf("gameButtonBack %d\n", time);

				break;

			default:
				break;
			}

			// Move to the next variable
			variable++;
		}
	}
}

//
// handleInput()
// Call this function to handle all input from the 
// gamepad and XBee modules.
//
int handleInput()
{
	char Char;
	int CharReceived = 1 ;
    int i = 0;

	//int rx2_flag = getcU2(&Char);  // Poll for XBee character received
	//if (rx2_flag) CharReceived = 1;

	// If a char was received
	if (CharReceived)
	{
		// Adding this character to the approp. position in the object
        for(i; dmaBuff[i] != 0; i++)
        {
            gamepadInputManager.varInputString[gamepadInputManager.varOffset] = dmaBuff[i];
            // Incrementing the offset of this array
            gamepadInputManager.varOffset++;
        }

		// If the null char was present
		
			// Resets the offset and...
			gamepadInputManager.varOffset = 0;

			// Indicates an entire string was received, moves to parsing
			//printf("%s \nFull string received\n", gamepadInputManager.varInputString);
            
            
			parseInput(gamepadInputManager.varInputString);
	}
	return 0;
}



void moveLeft(int movement)
{
    static int received = 0;
    
    if(movement > 0)
        received = 1;
    
    if(movement>0 || received)
    {
        TurnLeftPos(-3);
    }
    else
    {
        //SetDefaultServoPosition();
    }
    
    
}



void MoveRight(int movement)
{
    static int received = 0;
    
    if(movement > 0)
        received = 1;
    
    if(movement>0 || received)
    {
        TurnRightPos(3);
    }
    else
    {
        //SetDefaultServoPosition();
    }
    
    
}

void move()
{
    int temp = 0;
    
    if(gamepadInputManager.varButtons.varButtonA)
    {
        SetDefaultServoPosition();
        gamepadInputManager.varButtons.varButtonA = 0;
    }
    if((temp = gamepadInputManager.varSticksLeft.varCompOne) < 0)   
    {
        //TurnLeftPos(-5);
        TurnLeftPos(-temp);
        //printf("tempL %d\n", temp);
    }
    else if((temp = gamepadInputManager.varSticksLeft.varCompOne) > 0 )
    {
        TurnRightPos(temp);
       // TurnRightPos(5);
       // printf("tempR %d\n", temp);
    }
    if((temp = gamepadInputManager.varSticksLeft.varCompOne) < 0)   
    {
        //TurnLeftPos(-5);
        BackwardPos(-temp);
        //printf("tempL %d\n", temp);
    }
    else if((temp = gamepadInputManager.varSticksLeft.varCompOne) > 0 )
    {
        ForwardPos(temp);
       // TurnRightPos(5);
       // printf("tempR %d\n", temp);
    }
    DelayMs(20);
}

void clearStickLeft()
{
    gamepadInputManager.varSticksLeft.varCompOne = 0;
    gamepadInputManager.varSticksLeft.varCompTwo = 0;
}