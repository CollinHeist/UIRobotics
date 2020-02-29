#ifndef __MAIN_H__
	#define __MAIN_H__

	// UART Configurations
	#define PC_UART4_BAUD				(38400)
	#define PC_UART4_PARITY				(NO_PARITY)
	#define XBEE_UART2_BAUD				(9600)
	#define XBEE_UART2_PARITY			(NO_PARITY)
    #define ARDUINO_UART3_BAUD          (38400)
    #define ARDUINO_UART3_PARITY        (NO_PARITY)

	// Motor Configurations
	#define MOTOR1_START_PWM_PERCENT	(0)
	#define MOTOR2_START_PWM_PERCENT	(0)
	#define MOTORS_PWM_FREQUENCY		(1000)  // 1kHz timer 2 operation

	// Temperature Configurations
	#define TEMPERATURE_TIMEOUT_MS		(500)

	// Function Prototypes
	unsigned int initializeSystem(void);
#endif