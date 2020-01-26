/* 
 * File:   Gamepad.h
 * Author: Damon
 *
 * Created on January 26, 2019, 4:30 PM
 */

#ifndef __GAMEPAD_H__
	#define __GAMEPAD_H__

	// Configuration Macros
	#define GAMEPAD_INPUT_STRING_SIZE		(512)


	// Function Prototypes
	void Move();
	int HandleInput();
	int GamepadInit();
	void ClearLeftStick();
#endif