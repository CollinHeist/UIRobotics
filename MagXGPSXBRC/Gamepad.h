/* 
 * File:   Gamepad.h
 * Author: Damon
 *  Commenter: Sam
 * 
 * Created on January 26, 2019, 4:30 PM
 */

#ifndef _GAMEPAD_H_	//Guard Against multiple inclusion
    #define _GAMEPAD_H_

void move();							    //Referenced by main.c
int handleInput();	// Call to handle gamepad input		    //Referenced by main.c, uart2.c
int gamepadInit();	// Call to initialize this input library
void clearStickLeft();						    //Referenced by Notice.c

#endif