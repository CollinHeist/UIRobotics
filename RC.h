/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _RC_H    /* Guard against multiple inclusion */
#define _RC_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */

    /*  A brief description of a section can be given directly below the section
        banner.
     */
    #define cfgRC1()    PORTSetPinsDigitalOut(IOPORT_D, BIT_9)
    #define cfgRC2()    PORTSetPinsDigitalOut(IOPORT_D, BIT_11)
    #define cfgRC3()    PORTSetPinsDigitalOut(IOPORT_D, BIT_10)
    #define cfgRC4()    PORTSetPinsDigitalOut(IOPORT_D, BIT_8)
// RC Channel pin assignment macros
	#define RC_1(a)     LATDbits.LATD9 = a
	#define RC_2(a) 	LATDbits.LATD11 = a
	#define RC_3(a) 	LATDbits.LATD10 = a
	#define RC_4(a) 	LATDbits.LATD8 = a

	#define NRC		4	// Number of RC channels

// RC Setting range
	#define RC_MIN		100
	#define	RC_MAX		200
	#define RC_SPAN		100


    /* ************************************************************************** */
    /** Descriptive Constant Name

      @Summary
        Brief one-line summary of the constant.
    
      @Description
        Full description, explaining the purpose and usage of the constant.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
      @Remarks
        Any additional remarks
     */
//#define EXAMPLE_CONSTANT 0


    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */


    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */
    
	void initRC(void);
    void set_RC(int ch, int ctrl);
	int check_switches(void);

#endif /* _RC_H */

/* *****************************************************************************
 End of File
 */
