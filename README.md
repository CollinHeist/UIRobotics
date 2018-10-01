## University of Idaho - Robotics Club
---
#### This branch holds the working code for the XBee-Project, posted by Dr. Wall
---

Here are which files I'd like each of us to work on, ignore the /Robotics Code/ folder.
* Aaron:
  1. `comm_lib.c`
  2. `hardware.c`
  3. `led7.h`
  4. `switches.c`
* Collin:
  1. `comm_lib.h`
  2. `hardware.h`
  3. `RC-GPS.c`
  4. `switches.h`
* Damon:
  1. `GPS.c`
  2. `LCDlib.c`
  3. `RC-GPS.h`
* Dylan:
  1. `config_bits.h`
  2. `LDClib.h`
  3. `swDelay.c`
* Sam:
  1. `GPS.h`
  2. `led7.c`
  3. `swDelay.h`
  
The purpose of this branch is to merge only the necessary code to our __existing__ project folder. So to do this, go through your assigned file and create a pull request of the necessary code into whatever file you'd like. If there is already existing file, like `hardware.h` for example, just add whatever parts are necessary or __submit a pull request of what should be added and we can review it for bugs with Dr. Wall__.

---

Before adding any new code / functions, or submitting pull requests in general, take a look at the style I'm using in the `main.c` file of the Robotics Code folder.

* In general, we want __every function and major block of code__ to be commented, so that we are not constantly playing explaining either the functionality, usage, or purpose of each other's code.

* Try and utilize self-explanatory variable and function names. I realize that I am a bit more long-winded than necessary with many of my names, but I fild it helps a lot with mentally walking through the code.
  * i.e. as opposed to using `pMotors()` or `pm()`, I am using `powerMotors()`

* Each function should have a block of comments before it's implementation that describes a few key attributes
  1. __Syntax__ - How the function should be called *inline* (ex: `int x = someFunction(int a, int b)`)
  2. __Description__ - A description of the functions purpose. General overview (where it's called, etc.)
  3. __Parameters__ - A list of the parameters passed to the function. Include their type, expected values (if applicable) and loose description of purpose
  4. __Return Value__ - A list of possible returns from the function, as well as what each return would mean. If it's not necessarily a discrete subset of possible returns, describe what the return is and means.
  
* Try and segment your code (reasonably) so that each subsection is compartmentalized, and performs either a specific function or a logical operation. For example, for the function that turns our heading, it is split into shifting the rotation angle, accounting for a zero denominator, powering our motors, and returning
