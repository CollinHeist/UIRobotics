## University of Idaho - Robotics Club
---
#### This Github repository is where we'll be keep the final version of our code used to run the Autonomous Boat.
---
Here are which files I'd like each of us to work on, ignore the /Robotics Code/ folder.
* Aaron:
  1. `AD22100.c`
  2. `config_bits.h`
  3. `GPS_UART.h`
  4. `I2C_lib_.c`
  5. `LDClib.h`
  6. `MAG3110.c`
  7. `newxc32_header.h`
  8. `TMP03.c`
* Collin:
  1. `AD22100.h`
  2. `DHT.c`
  3. `GPS-I2C.c`
  4. `I2C_lib_.h`
  5. `led7.c`
  6. `MAG3110.h`
  7. `RC.c`
* Damon:
  1. `comm_lib.c`
  2. `DHT.h`
  3. `hardware.c`
  4. `i2c_lib.c`
  5. `led7.h`
  6. `main.c`
  7. `RC.h`
* Dylan:
  1. `comm_lib.h`
  2. `GPS_I2C.h`
  3. `hardware.h`
  4. `i2c_lib.h`
  5. `mag.c`
  6. `main.h`
  7. `swDelay.c`
* Sam:
  1. `compass.c`
  2. `GPS_UART.c`
  3. `hardware.h`
  4. `LCDlib.c`
  5. `mag.h`
  6. `map.c`
  7. `swDelay.h`
  8. `TMP03.h`

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
