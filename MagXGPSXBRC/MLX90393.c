#include "hardware.h"

#include <plib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "I2C_lib.h"
#include "MLX90393.h"

// Up to 9 bytes may be returned.
uint8_t mlx90393_data_buffer[10];
uint8_t mlx90393_status = 0;
uint16_t mlx90393_t = 0;
uint16_t mlx90393_x = 0;
uint16_t mlx90393_y = 0;
uint16_t mlx90393_z = 0;
uint16_t mlx90393_tref = 0;

uint8_t mlx90393_memory[64*2]; // 64 16-bit words.

void MLX90393_Init(void) {
	I2C_Init(I2C1, 100000);
}

I2C_RESULT mlx90393_command(uint8_t cmd, uint8_t zyxt, uint8_t address, uint16_t data) {
	I2C_RESULT i2c_flag;
	I2C_DATA_BLOCK blk;
	uint8_t rx_size = 1;
	uint8_t tx_size = 1;

	cmd &= 0xf0;
	zyxt &= 0x0f;
	address &= 0x3f;

	switch (cmd) {
		case MLX90393_CMD_SB:
		case MLX90393_CMD_SWOC:
		case MLX90393_CMD_SM:
			cmd |= zyxt;
			break;
		case MLX90393_CMD_RM:
			cmd |= zyxt;
			if ((zyxt & MLX90393_T)!=0) rx_size += 2;
			if ((zyxt & MLX90393_X)!=0) rx_size += 2;
			if ((zyxt & MLX90393_Y)!=0) rx_size += 2;
			if ((zyxt & MLX90393_Z)!=0) rx_size += 2;
			break;
		case MLX90393_CMD_RR:
			mlx90393_data_buffer[1] = address << 2;
			rx_size += 2;
			tx_size = 2;	
			break;
		case MLX90393_CMD_WR:
			mlx90393_data_buffer[1] = (data&0xff00) >> 8;
			mlx90393_data_buffer[2] = data & 0x00ff;
			mlx90393_data_buffer[3] = address << 2;
			tx_size = 4;
			break;
		case MLX90393_CMD_NOP:
		case MLX90393_CMD_EX:
		case MLX90393_CMD_RT:
		case MLX90393_CMD_HR:
		case MLX90393_CMD_HS:
			break;
	}

	mlx90393_data_buffer[0] = cmd;
	
	blk.reg_addr = cmd;
	blk.dev_id = MLX90393_I2C_ADDRESS;
	blk.i2c_channel = I2C1;  
	// cmd is in buffer[0] and in reg_addr. 
	if (tx_size > 0) {
	// cmd is in buffer[0] and in reg_addr. 
		blk.data = mlx90393_data_buffer + 1;
		blk.block_size = tx_size - 1; // 
		i2c_flag = I2C_WriteDev(blk);
	}
	
	if (rx_size > 0) {
		blk.data = mlx90393_data_buffer;
		blk.block_size = rx_size;
		i2c_flag = I2C_ReadDev(blk);
	}

	return i2c_flag;
}

void mlx90393_decode(uint8_t zyxt) {
	uint8_t *p = (uint8_t *)mlx90393_data_buffer;
	mlx90393_status = *p;
	p += 1;
	
	if ((zyxt & MLX90393_T)!=0) {
		mlx90393_t = assemble_16(p);
		p += 2;
	}
	if ((zyxt & MLX90393_X)!=0) {
		mlx90393_x = assemble_16(p);
		p += 2;
	}
	if ((zyxt & MLX90393_Y)!=0) {
		mlx90393_y = assemble_16(p);
		p += 2;
	}
	if ((zyxt&MLX90393_Z)!=0) {
		mlx90393_z = assemble_16(p);
		p += 2;
	}	
}

uint16_t mlx90393_read_memory_word(uint8_t address) {
	unsigned int x = 0;
	I2C_RESULT i2c_flag;

	i2c_flag = mlx90393_command(MLX90393_CMD_RR,0,address,0);

	// mlx90393_data_buffer[0] contains status code.
	if(i2c_flag == I2C_SUCCESS) {
		x = assemble_16((uint8_t*) &mlx90393_data_buffer[1]);
	}

	return x; 
}

void mlx90393_write_memory_word(uint8_t address, uint8_t data) {
	mlx90393_command(MLX90393_CMD_WR,0,address,data);
}

void mlx90393_read_memory(uint8_t *p_dst, uint8_t address, uint8_t size) {
	uint16_t val;
	while (size != 0) {
		val = mlx90393_read_memory_word(address);
		*p_dst = (val>>8)&0xff;
		p_dst += 1;
		*p_dst = val&0xff;
		p_dst += 1;
		address += 1;
		size -= 1;
	}
}

uint16_t assemble_16(uint8_t *p_data) {
	uint16_t result;
	result = p_data[0];
	result = (result<<8) + p_data[1];

	return result;
}

uint32_t assemble_32(uint8_t *p_data) {
	int i;
	uint32_t result;
	result = p_data[0];
	for (i=1; i<4; i++) {
		result = (result<<8) + p_data[i];
	}

	return result;
}

void serial_hexdump_buffer(uint8_t *p_data, uint8_t data_size) {
	int i;
	for (i=0; i<data_size; i++) {
		printf(" 2x", p_data[i]);
	}
	printf(" - ");
	for (i=0; i<data_size; i++) {
		if (p_data[i]>31 && p_data[i] < 127) 
			printf("%c",p_data[i]);
		else 
			printf(".");
	}
	printf("/n/r");
}

void serial_hexdump(uint8_t *p_data, uint16_t data_size, uint8_t bytes_per_line) {
	uint16_t s;
	while (data_size != 0) {
		s = data_size<bytes_per_line? data_size : bytes_per_line;
		serial_hexdump_buffer(p_data,s);
		p_data += bytes_per_line;
		data_size -= s;
	}
}

