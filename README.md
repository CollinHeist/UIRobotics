# University of Idaho - Robotics Club
---
## This Github repository is where we'll be keep the working version of our code used to run the Autonomous Boat.
---
For those have not yet used Github, I recommend reading through [this](https://guides.github.com/activities/hello-world/ "Github Walkthrough") quick introduction.

1. `/Robotics Code/` is the main _project_ file for the board. You can download that folder and directly open it as a project file in MPLAB X

2. I've also posted a walkthrough of sorts you should all read [here](https://docs.google.com/document/d/1drxDmMMY8L0Jcg8FvCODpBT69w2KSG8qKTnTIxLXedo/edit?usp=sharing "Google Doc Walkthrough").

---

## Coding Standards

### Naming Convention

* Make all variable names __camel case__.
	```c
	int testVariable = 0;
	```
* Make all function names __camel case__.
	```c
	float testFunction(int parameterOne, int parameterTwo) {
		// Test code
	}
	```
* Make all _macros_ (__#define__ statements) __snake case__.
	```c
	#define TEST_MACRO_NAME		( 500 )
	```
* All file names should be __pascal case__.
	```c
	#include "TestFileName.h"
	#include "UARTFileName.h"
	```
* Always choose more verbose names over smaller names. As an example, choose `updateRCFlag` as opposed to `RCFlag`.

### Comments

* Add in-line (`//`) comments to all large sections of code. There is no need for line-by-line comments, but give section-wise descriptions of your code's functionality.

* Each function block should have the following comment section before it:
	```c
	/**
	 *	Summary
	 *		Brief description of the function's functionality / purpose.
	 *	Parameters
	 *		parameterOne: Describe the purpose or meaning of this first parameter.
	 *		paramterTwo: Describe the purpose and meaning of this second paramter.
	 *	Return
	 *		What the return value means.
	 *	Notes
	 *		(Optional) Any additional notes that the user should know about this function.
	 **/
	 float testFunction(int paramaterOne, int* parameterTwo) {
	 	// Code
	 }
	```
