#ifndef _SUPPRESS_PLIB_WARNING	/* Suppress plib obsolesce warnings */
	#define _SUPPRESS_PLIB_WARNING
#endif

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

#define MODE_TEST
#define MODE_NORM

// Possible gamepad messages
enum GamepadVariables{
    GAME_TIME,
    GAME_TRIGGER_LEFT,
    GAME_BUMPER_LEFT,
    GAME_TRIGGER_RIGHT,
    GAME_BUMPER_RIGHT,
    GAME_STICK_LEFT_X,
	GAME_STICK_LEFT_Y,
    GAME_D_PAD_X,
	GAME_D_PAD_Y,
    GAME_STICK_RIGHT_X,
	GAME_STICK_RIGHT_Y,
    GAME_BUTTON_Y,
    GAME_BUTTON_B,
    GAME_BUTTON_A,
    GAME_BUTTON_X,
    GAME_BUTTON_START,
    GAME_BUTTON_BACK
};

typedef struct pair
{
	int m_CompOne;
	int m_CompTwo;
} 
Pair;

typedef struct buttons
{
	int m_ButtonY;
	int m_ButtonB;
	int m_ButtonA;
	int m_ButtonX;
}
Buttons;

typedef struct gamepadInput
{
	unsigned int m_LeftTrigger;
	unsigned int m_RightTrigger;

	int m_SendTime;
	int m_LeftBumper;
	int m_RightBumper;
	int m_StartButton;
	int m_BackButton;
	int m_Offset;

	Pair m_LeftSticks;
    Pair m_RightSticks;
	Pair m_DirPad;
	Buttons m_Buttons;

	char m_InputString[512];
} 
GamepadInput;

// The main object that handles input
static GamepadInput GamepadInputManager;

static void MoveLeft(int movement);

//
// GamepadInit()
// Should be called once prior to using the GamepadInputManager
// Properly initializes all of the members of this object.
//
int GamepadInit()
{
    int i = 0;
     
    GamepadInputManager.m_Offset = 0;
    for(i = 0; i < 512; i++)
    {
        GamepadInputManager.m_InputString[i] = 0;
    }
    
    GamepadInputManager.m_BackButton = 0;
    GamepadInputManager.m_LeftTrigger = 0;
    GamepadInputManager.m_RightTrigger = 0;
    GamepadInputManager.m_LeftBumper = 0;
    GamepadInputManager.m_RightBumper = 0;
    GamepadInputManager.m_StartButton = 0;
    GamepadInputManager.m_BackButton = 0;
    
    GamepadInputManager.m_DirPad.m_CompOne = 0;
    GamepadInputManager.m_DirPad.m_CompTwo = 0;
    GamepadInputManager.m_RightSticks.m_CompOne = 0;
    GamepadInputManager.m_RightSticks.m_CompTwo = 0;
    GamepadInputManager.m_LeftSticks.m_CompOne = 0;
    GamepadInputManager.m_LeftSticks.m_CompTwo = 0;
    
    GamepadInputManager.m_Buttons.m_ButtonA = 0;
    GamepadInputManager.m_Buttons.m_ButtonB = 0;
    GamepadInputManager.m_Buttons.m_ButtonY = 0;
    GamepadInputManager.m_Buttons.m_ButtonX = 0;

}

//
// ParseInput()
// Parses the string and places all of the 
// approp. variables into their corresponding
// vars in the GamepadInputManager object.
//
int ParseInput(char* String)
{
	// Local variables
	int i = 0;
	int j = 0;
	int Variable = GAME_TIME;
	int FullVariable = 0;
	char Temp[16];
	int time = 0;
	char* end;

	// Verifying that the sting received is valid
	if (String == nullptr)
	{
		printf("string received was nullptr\n");
		return -1;
	}

	// Parsing the string
	for (i = 0; i < 512; i++)
	{

		if (!isspace(String[i]))
		{
			// Copying contents of this variable
			for (j = 0; j < 16 && !isspace(String[i]); j++, i++)
			{
				Temp[j] = String[i];
			}

			Temp[j] = 0;			// Append null termination
			end = &(Temp[j - 1]);	// ptr to end of this section

			// Case for each of the different variables in the string
			switch (Variable)
			{
			case GAME_TIME:

				time = strtol(Temp, &end, 10);
            #ifdef Debug
                printf("GAME_TIME %s\n", Temp);
            #endif
                
				break;
			case GAME_TRIGGER_LEFT:

				time = strtol(Temp, &end, 10);
            #ifdef Debug
				printf("GAME_TRIGGER_LEFT %s\n", Temp);
            #endif

				break;
			case GAME_BUMPER_LEFT:
				time = strtol(Temp, &end, 10);
                #ifdef Debug
				printf("GAME_BUMPER_LEFT %s\n", Temp);
                #endif

				break;
			case GAME_TRIGGER_RIGHT:
				time = strtol(Temp, &end, 10);
                #ifdef Debug
				printf("GAME_TRIGGER_RIGHT %s\n", Temp);
                #endif

				break;
			case GAME_BUMPER_RIGHT:
				time = strtol(Temp, &end, 10);
				//printf("GAME_BUMPER_RIGHT %d\n", time);

				break;
			case GAME_STICK_LEFT_X:
                
				time = strtol(Temp, &end, 10);
                   
                if(~mPORTCRead() & (BIT_13 | BIT_14))
                {
                   GamepadInputManager.m_LeftSticks.m_CompOne = 50;
                }
                else GamepadInputManager.m_LeftSticks.m_CompOne = time;


				//printf("GAME_STICK_LEFT %d\n", time);
				break;
			case GAME_STICK_LEFT_Y:

				time = strtol(Temp, &end, 10);
                
                GamepadInputManager.m_LeftSticks.m_CompTwo = time;

				//printf("GAME_STICK_LEFT %d\n", time);
				break;
			case GAME_D_PAD_X:
				time = strtol(Temp, &end, 10);
				//printf("GAME_D_PAD %d\n", time);

				break;
			case GAME_D_PAD_Y:
				time = strtol(Temp, &end, 10);
				//printf("GAME_D_PAD %d\n", time);

				break;
			case GAME_STICK_RIGHT_X:

				time = strtol(Temp, &end, 10);
				//printf("GAME_STICK_RIGHT %d\n", time);

				break;
			case GAME_STICK_RIGHT_Y:

				time = strtol(Temp, &end, 10);
				//printf("GAME_STICK_RIGHT %d\n", time);

				break;
			case GAME_BUTTON_Y:

				time = strtol(Temp, &end, 10);
				//printf("GAME_BUTTON_Y %d\n", time);

				break;
			case GAME_BUTTON_B:

				time = strtol(Temp, &end, 10);
				//printf("GAME_BUTTON_B %d\n", time);

				break;
			case GAME_BUTTON_A:

				time = strtol(Temp, &end, 10);
                GamepadInputManager.m_Buttons.m_ButtonA = time;				
                //printf("GAME_BUTTON_A %d\n", time);

				break;
			case GAME_BUTTON_X:

				time = strtol(Temp, &end, 10);
				//printf("GAME_BUTTON_X %d\n", time);

				break;
			case GAME_BUTTON_START:

				time = strtol(Temp, &end, 10);
				//printf("GAME_BUTTON_START %d\n", time);

				break;
			case GAME_BUTTON_BACK:

				time = strtol(Temp, &end, 10);
				//printf("GAME_BUTTON_BACK %d\n", time);

				break;

			default:
				break;
			}

			// Move to the next variable
			Variable++;
		}
	}
}

//
// HandleInput()
// Call this function to handle all input from the 
// gamepad and XBee modules.
//
int HandleInput()
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
            GamepadInputManager.m_InputString[GamepadInputManager.m_Offset] = dmaBuff[i];
            // Incrementing the offset of this array
            GamepadInputManager.m_Offset++;
        }

		// If the null char was present
		
			// Resets the offset and...
			GamepadInputManager.m_Offset = 0;

			// Indicates an entire string was received, moves to parsing
			//printf("%s \nFull string received\n", GamepadInputManager.m_InputString);
            
            putsU2("ACK\0");
            
			ParseInput(GamepadInputManager.m_InputString);

	}
	return 0;
}



void MoveLeft(int movement)
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

void Move()
{
    int temp = 0;
    
    if(GamepadInputManager.m_Buttons.m_ButtonA)
    {
        SetDefaultServoPosition();
        GamepadInputManager.m_Buttons.m_ButtonA = 0;
    }
    if((temp = GamepadInputManager.m_LeftSticks.m_CompOne) < 0)   
    {
        //TurnLeftPos(-5);
        TurnLeftPos((temp /= 20) ? -1 * temp : -1);
        printf("tempL %d\n", temp);
    }
    else if((temp = GamepadInputManager.m_LeftSticks.m_CompOne) > 0 )
    {
        TurnRightPos((temp /= 20) ? temp : 1);
        //TurnRightPos(5);
        printf("tempR %d\n", temp);
    }
    DelayMs(20);
}

void ClearLeftStick()
{
    GamepadInputManager.m_LeftSticks.m_CompOne = 0;
    GamepadInputManager.m_LeftSticks.m_CompTwo = 0;
}