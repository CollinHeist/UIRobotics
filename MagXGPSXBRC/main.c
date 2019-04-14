// System included files
#include "config_bits.h"
#include "hardware.h"
#include "main.h"
#include "Pot.h"
#include <plib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "i2c_lib.h"
#include "GPS_I2C.h"
#include "MLX90393.h"
#include "Gamepad.h"
#include "ADC_TEMP.h"

// Platform common included files
#include "swDelay.h"

// Application included files
#include "uart4.h"  // Monitor UART
#include "uart2.h"  // GPS UART
#include "led7.h"
#include "LCDlib.h"
#include "RC.h"
#include "DMA_UART2.h"

#define RC_CW   0   // RC Direction of rotation
#define RC_CCW  1
#define HOST_DATA_SIZE  (4)

char GetMsg(char Char) { return Char;}
int calc_ck_sum(char *str);
void loop(void);
void do_command(int *p_host_data);
void do_read_eeprom_word(int i);
void print_pretty_table(int use_uart);
int InitializeModules(I2C_RESULT* I2cResultFlag);
int MagInitMessage();



// Global variables
static char Char;
int gps_message = 0;        // Active GPS sentence 
extern int16_t led_value;
extern BOOL led_flag;
int host_data[HOST_DATA_SIZE] = { 0, 0, 0, 0 };
int field = 0;
int multiplier = 1;


int main(void)
{
	// Local variables
	I2C_RESULT I2cResultFlag;   // I2C Init Result flag
	I2C_RESULT I2cReadFlag;     // I2C Read result flag

	unsigned GPSInterval = 1000;
	unsigned MagInterval = 150000;
	unsigned ADCTemperatureInterval = 250000;

	unsigned ActualGPSInterval = GPSInterval;
	unsigned ActualMagInterval = MagInterval;
	unsigned ActualADCInterval = ADCTemperatureInterval;
    
    DmaIntFlag = 0;
    
	// Initialization
	I2cResultFlag = InitializeModules(&I2cResultFlag);		// Initialize all I/O modules
	MagInitMessage();							// Initialize mag and send message
    DmaUartRxInit(); 

    
    SetDefaultServoPosition();

	while (1)  // Forever process loop	
	{

		//
        
        if(DmaIntFlag)          // clear the interrupt flag
        {
            DmaIntFlag=0;       // Reset DMA Rx block flag
            printf("message received %s\n", dmaBuff);
            DmaUartRx();
            HandleInput();							// Handles all user input from the XB device
        }
        
        if(SW0())				// Checking Switch '0' on board
        {
            SetDefaultServoPosition();	//set servo back to the default position
        }

		// GPS receives data
		if (ActualGPSInterval == 0) //Update GPS on specified interval
		{
            Move(); //Read and update controller inputs
			ActualGPSInterval = GPSInterval;	// Reset the interval
			//I2cReadFlag = ReportGPS(TRUE);		// Read from the GPS and display to the screen
		}
		else
		{
			ActualGPSInterval--;				// Decrement the counter
		}


		// Mag receives data
 		if (ActualMagInterval == 0) //Update magnetometer readings on specified interval
		{
			ActualMagInterval = MagInterval;	// Resets the original interval
			loop();								// Gets data from the magnetometer
		}
		else
		{
			ActualMagInterval--;				// Decrement the counter
		}


		// Get data from the ADC temperature sensors
		if (ActualADCInterval == 0)
		{
          //  printf("1: %s              2: %s \n", (mPORTCRead() & BIT_13) ? "NONZERO" : "ZERO   " , 
           //          (mPORTCRead() & BIT_14) ? "NONZERO" : "ZERO");
            
			ActualADCInterval = ADCTemperatureInterval;		// Resets the interval
			read_temperature_store();						// Reads in the temperature
		}
		else
		{
			ActualADCInterval--;							// Decrement the counter	
		}

        
	}

	return EXIT_FAILURE; // Code execution should never get to this statement 
}


//
//
//
//
int InitializeModules(I2C_RESULT* I2cResultFlag)
{

	Hardware_Setup();					// Initialize common IO
	uart4_init(38400, NO_PARITY);		// PC Terminal
	uart2_init(9600, NO_PARITY);		// XBEE
	initLCD();							// Init the LCD screen
	seg7_init();						// Initialize the seven seg display


	putsU2("\n\rXBee online\n\r");		// Send message to PC
	init_analog();						// Initialize AN2 to read Pot
	
	init_temperature();
    

	led_flag = 1;						// Enable 4 digit 7 segment LED display
    
    *I2cResultFlag = I2C_Init(I2C1, 100000);

    initChangeNotice();

    
	return 0; // Init I2c and GPS

}

//
//
//
//
int MagInitMessage()
{
	uint8_t zyxt = 0x0f;
	char lcdStr[20];


	// Begin Mag init
	putsLCD("MLX90393");
	mlx90393_command(MLX90393_CMD_SM, zyxt, 0, 0);
	printf("SM Status: ");
	printf("0x%2x\n\r", mlx90393_data_buffer[0]);
	mlx90393_tref = mlx90393_read_memory_word(MLX90393_TREF);
	clrLCD();
	putsLCD("MLX90393");
	gotoLCD(16);

	sprintf(lcdStr, "SM: %d", mlx90393_data_buffer[0]);
	putsLCD(lcdStr);
	msDelay(1000);

	mlx90393_command(MLX90393_CMD_RM, zyxt, 0, 0);
	mlx90393_decode(zyxt);
	print_pretty_table(1);
	msDelay(100);
	return 0;
}

//
//
//
//
void print_pretty_table(int use_uart)
{
char lcdStr[40];
int tempC, tempF;
  tempC = (int) ((35 + ( (10 * mlx90393_t) - (10 * mlx90393_tref) ) / 452 ));
  tempF = ((tempC * 9) / 5) + 32;
  if(use_uart)
  {
    printf("RAW data values\n\r");
    printf("Tref:%6d ", mlx90393_tref);
    printf("Temp:%6d ", mlx90393_t);
    printf("X:%6d ",mlx90393_x);
    printf("Y:%6d ",mlx90393_y);
    printf("Z:%6d\n\r ",mlx90393_z);
  }
  sprintf(lcdStr,"X:%5d Y:%5d",mlx90393_x, mlx90393_y );
  clrLCD();
  putsLCD(lcdStr);
  gotoLCD(16);
  sprintf(lcdStr,"T1:%5d T:%5d",GetTemp1(), GetTemp2());
  putsLCD(lcdStr); 
  led_flag = 1;   // Enable 4 digit 7 segment LED display
  led_value = mlx90393_data_buffer[0];
}

//
//
//
//
void do_read_eeprom_word(int i)
{
  mlx90393_read_memory_word(i);
  printf("%d%d%d\n\r ",mlx90393_data_buffer[0],mlx90393_data_buffer[1],
                       mlx90393_data_buffer[2]);
}
  
//
//
//
//
void do_command(int *p_host_data)
{
int i;

  for(i=0; i<HOST_DATA_SIZE  ; i++)
  {
    printf("[%d] ", p_host_data[i]);
  }  
  printf("\n\r");
      
  switch (p_host_data[0])
  {
    case 0:
      // Read EEPROM word.
      printf("Read EEPROM word\n\r");
      do_read_eeprom_word(p_host_data[1]);
      return;

    case 1:
      // Read EEPROM full.
      printf("Read EEPROM full\n\r");
      for (i=0; i<63; i++) 
      {
        do_read_eeprom_word(i);
      }
      return;

    case 2:
      // Write register.
      printf("Write register\n\r");
      mlx90393_write_memory_word(p_host_data[1],p_host_data[2]);
      break;

    case 3:
      printf("Not implemented.\n\r");
      return;

    case 4:
      // Reset.
      printf("Reset\n\r");
      mlx90393_command(MLX90393_CMD_RT,0,0,0);
      break;

    case 5:
      // Exit.
      printf("Exit\n\r");
      mlx90393_command(MLX90393_CMD_EX,0,0,0);
      break;

    case 6:
      // Single measurement.
      printf("Single measurement\n\r");
      mlx90393_command(MLX90393_CMD_SM,p_host_data[1],0,0);
      break;

    case 7:
      // Read measurement.
      printf("Read measurement\n\r");
      mlx90393_command(MLX90393_CMD_RM,p_host_data[1],0,0);
      for(i=0; i<5; i++)
      {
          printf("0x%2x:  ",mlx90393_data_buffer[i]);
      }
      printf("\r\n");
      for(i=5; i<11; i++)
      {
          printf("0x%2x:  ",mlx90393_data_buffer[i]);
      }
      printf("\r\n");
      break;

    case 8:
      // Burst.
      printf("Burst\n\r");
      mlx90393_command(MLX90393_CMD_SB,p_host_data[1],0,0);
      break;

    case 9:
      // Wake up on change.
      printf("Wake up on change\n\r");
      mlx90393_command(MLX90393_CMD_SWOC,p_host_data[1],0,0);
      break;

    case 10:
      // Memory recall.
      printf("Memory recall\n\r");
      mlx90393_command(MLX90393_CMD_HR,0,0,0);
      break;

    case 11:
      // Memory store.
      printf("Memory store\n\r");
      mlx90393_command(MLX90393_CMD_HS,0,0,0);
      break;
      
    default:
      printf("Unhandled\n\r");
      return;
  }
  printf("%4d\n\r",mlx90393_data_buffer[0]);
}
 
//
//
//
//
void loop(void)
{
static uint8_t i = 1;
static uint8_t ff = 0;
static uint8_t sec2 = 0;
static uint8_t zyxt = 0x0f;
static char ch;
  
  zyxt = MLX90393_T | MLX90393_X | MLX90393_Y | MLX90393_Z;
  if(1)
  {
    if(ff)
    {
      invLED4();
      mlx90393_command(MLX90393_CMD_RM, zyxt, 0, 0);
      mlx90393_decode(zyxt);
      print_pretty_table(0);
      ff = 0;
      sec2 = 0;
    }
    else
    {
      mlx90393_command(MLX90393_CMD_SM, zyxt, 0, 0);
      ff = 1;
      sec2 = 0;
    }
  }
  else sec2 = 1;

  if (getcU4(&ch))
  {
    putcU4(ch);
    if (ch==',')
    {
      field += 1; // Next field.
      host_data[field] = 0; // Clear field.
      multiplier = 1; // Reset multiplier.
    }
    else 
    {
        if (ch>='0' && ch<='9' && field < HOST_DATA_SIZE)
        {
            host_data[field] = multiplier*host_data[field] + (ch-'0');
            multiplier *= 10;
        }
        else
        {
            if (field == HOST_DATA_SIZE-1) 
            {
                do_command(host_data);
            }
            field = 0; // First field.
            host_data[field] = 0; // Clear field.
            multiplier = 1;
        }
    }
  }
}
