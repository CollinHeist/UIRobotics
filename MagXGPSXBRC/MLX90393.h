#ifndef __MLX90393_H__
	#define __MLX90393_H__


	#include "hardware.h"
	#include "I2C_lib.h"
	#include <plib.h>

	// Possible I2C addresses.
    #define MLX90393_I2C_ADDRESS  (0x0c)

	// Key commands.
	#define MLX90393_CMD_NOP  (0x00)  /* No OPeration */
	#define MLX90393_CMD_SB  (0x10)  /* Start Burst mode */
	#define MLX90393_CMD_SWOC  (0x20)  /* Start Wake-up On Change */
	#define MLX90393_CMD_SM  (0x30)  /* Start Measurement (polling mode) */
	#define MLX90393_CMD_RM  (0x40)  /* Read Measurement */
	#define MLX90393_CMD_RR  (0x50)  /* Read from a Register */
	#define MLX90393_CMD_WR  (0x60)  /* Write from a Register */
	#define MLX90393_CMD_EX  (0x80)  /* EXit */
	#define MLX90393_CMD_HR  (0xd0)  /* Memory Recall */
	#define MLX90393_CMD_HS  (0xe0)  /* Memory Store */
	#define MLX90393_CMD_RT  (0xf0)  /* Reset */

	// Flags to use with "zyxt" variables.
	#define MLX90393_T  (0x01)      // Temperature
	#define MLX90393_X  (0x02)      // X-axis
	#define MLX90393_Y  (0x04)      // Y-axis
	#define MLX90393_Z  (0x08)      // Z-axis 
	#define MLX90393_TREF (0x24)    // Tref

	// Memory areas.
	#define MLX90393_CUSTOMER_AREA_BEGIN  (0x00)
	#define MLX90393_CUSTOMER_AREA_END  (0x1f)
	#define MLX90393_MELEXIS_AREA_BEGIN  (0x20)
	#define MLX90393_MELEXIS_AREA_END  (0x3f)

	// Up to 9 bytes may be returned.
	extern uint8_t mlx90393_data_buffer[10];
	extern uint8_t mlx90393_status;
	extern uint16_t mlx90393_t;
	extern uint16_t mlx90393_x;
	extern uint16_t mlx90393_y;
	extern uint16_t mlx90393_z;
	extern uint16_t mlx90393_tref;
	extern uint8_t mlx90393_memory[64*2]; // 64 16-bit words.

	// Function Prototypes
	void MLX90393_Init(void);
	I2C_RESULT mlx90393_command(uint8_t cmd, uint8_t zyxt, uint8_t address, uint16_t data);
	void mlx90393_decode(uint8_t zyxt);
	uint16_t mlx90393_read_memory_word(uint8_t address);
	void mlx90393_write_memory_word(uint8_t address, uint8_t data);
	void mlx90393_read_memory(uint8_t *p_dst, uint8_t address, uint8_t size);

	// Similar to printf("%02x",val);
	void serial_print_hex(uint8_t val); // Produce a readable hexdump of a buffer.
	// Produce a readable hexdump.
	void serial_hexdump(uint8_t *p_data, uint16_t data_size, uint8_t bytes_per_line);

	uint16_t assemble_16(uint8_t *p_data);
	uint32_t assemble_32(uint8_t *p_data);

	// Generic I2C <start><write><repeated start><read><stop>.
	// One buffer is used for TX and RX so you better make it
	// large enough for both operations.
	uint8_t i2c_write_read(uint8_t i2c_address, uint8_t *p_data, uint8_t tx_size, uint8_t rx_size);

#endif