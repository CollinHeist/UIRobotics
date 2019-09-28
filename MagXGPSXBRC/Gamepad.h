/* 
 * File:   Gamepad.h
 * Author: Damon
 *
 * Created on January 26, 2019, 4:30 PM
 */

#pragma once

void Move();
int HandleInput();	// Call to handle gamepad input
int GamepadInit();  // Call to initialize this input library
void ClearLeftStick();