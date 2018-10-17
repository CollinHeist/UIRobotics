## University of Idaho - Robotics Club
---
#### This Github repository is where we'll be working version of Dr. Wall's code for the peripherals. These are to be merged with the master section of code.
---
Here are which files I'd like each of us to work on, ignore the /Robotics Code/ folder.
* Aaron:
  1. `AD22100.c`
  2. `hardware.c` - __Updated__
  3. `i2c_lib.h`
  4. `MAG3110.c`
  5. `RC.c` - __Updated__, Keep in mind our RC.c file in the `master` branch is our main, so implement accordingly
* Collin:
  1. ~~`AD22100.h`~~ - __Finished__
  2. `hardware.h` - __Need to redo__
  3. `GPS-I2C.c` - __Need to redo__
  4. ~~`LCDlib.c`~~ - __Finished__
  5. ~~`MAG3110.h`~~ - __Finished__
  6. `RC.h` - __Need to redo__
  7. ~~`swDelay.c`~~ - __Finished__
  8. ~~`swDelay.h`~~ - __Finished__
* Damon:
  1. `compass.c`
  2. `I2C_lib_.c`
  3. `LCDlib.h`
  4. `main.c` - For this one, keep in mind our 'main' file in the `master` branch is RC.c
  5. `uart2.c`
* Dylan:
  1. ~~`config_bits.h`~~ - __Finished__, Collin
  2. ~~`I2C_lib_.h`~~ - __Finished__, Collin
  3. ~~`led7.h`~~ - __Finished__, Collin
  4. `main.h`
  5. `uart2.h`
* Sam:
  1. `GPS_I2C.h`
  2. `i2c_lib.c` - __Updated__
  3. `led7.c`
  4. `newxc32_header.h`
  5. `uart4.c`
  6. `uart4.h`

---

Before adding any new code / functions, or submitting pull requests in general, take a look at the style I'm using in the `main.c` file.

* In general, we want __every function and major block of code__ to be commented, so that we are not constantly playing explaining either the functionality, usage, or purpose of each other's code.

* Try and utilize self-explanatory variable and function names. I realize that I am a bit more long-winded than necessary with many of my names, but I fild it helps a lot with mentally walking through the code.
  * i.e. as opposed to using `pMotors()` or `pm()`, I am using `powerMotors()`

* Each function should have a block of comments before it's implementation that describes a few key attributes
  1. __Syntax__ - How the function should be called *inline* (ex: `int x = someFunction(int a, int b)`)
  2. __Description__ - A description of the functions purpose. General overview (where it's called, etc.)
  3. __Parameters__ - A list of the parameters passed to the function. Include their type, expected values (if applicable) and loose description of purpose
  4. __Return Value__ - A list of possible returns from the function, as well as what each return would mean. If it's not necessarily a discrete subset of possible returns, describe what the return is and means.
  
* Try and segment your code (reasonably) so that each subsection is compartmentalized, and performs either a specific function or a logical operation. For example, for the function that turns our heading, it is split into shifting the rotation angle, accounting for a zero denominator, powering our motors, and returning
