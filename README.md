## University of Idaho - Robotics Club
---
#### This Github repository is where we'll be keep the final version of our code used to run the Autonomous Boat.
---
For those have not yet used Github, I recommend reading through [this](https://guides.github.com/activities/hello-world/ "Github Walkthrough") quick introduction.

1. `/Robotics Code/` is the main _project_ file for the board. You can download that folder and directly open it as a project file in MPLAB X

2. I've also posted a walkthrough of sorts you should all read [here](https://docs.google.com/document/d/1drxDmMMY8L0Jcg8FvCODpBT69w2KSG8qKTnTIxLXedo/edit?usp=sharing "Google Doc Walkthrough").

--

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
