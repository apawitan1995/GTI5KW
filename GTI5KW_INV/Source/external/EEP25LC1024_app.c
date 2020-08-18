/*
 * 25LC1024_app.c
 *
 *  Created on: Feb 28, 2014
 *      Author: Hanif
 */
#include "external/EEP25LC1024_app.h"

void EEP25LC_Init(void)
{
	//I/O Initialization

	//Set output Serial clock
	WR_REG(EEP_PORT->IOCR4,PORT1_IOCR4_PC6_Msk,PORT1_IOCR4_PC6_Pos,0x10);
	CLR_BIT(EEP_PORT->OUT,SCK);

	//Set output Serial data
	WR_REG(EEP_PORT->IOCR4,PORT1_IOCR4_PC7_Msk,PORT1_IOCR4_PC7_Pos,0x10);
	CLR_BIT(EEP_PORT->OUT,SO);

	//Set output Chip select
	WR_REG(EEP_PORT->IOCR0,PORT1_IOCR0_PC0_Msk,PORT1_IOCR0_PC0_Pos,0x10);
	CLR_BIT(EEP_PORT->OUT,CS_EEP);

	//Set Input Serial data In
	WR_REG(PORT5->IOCR0,PORT5_IOCR0_PC0_Msk,PORT5_IOCR0_PC0_Pos,0x00);
}

void EEP25LC_Write_Data(int32_t addr,int32_t data)
{
	int8_t bit=0;
	int8_t instruction=0;

	//Initialization
	instruction=eep_WREN;
	SCK_LOW();
	SO_LOW();
	CS_EEP_HIGH();

	//WREN Initialization
	CS_EEP_LOW();

	//Send Instruction
	for(bit=0;bit<8;bit++)
	{
		WRITE_SO(instruction & 0x80);
		TOGGLE_SCK();
		My_Delay_us(EEP_CLOCK);
		TOGGLE_SCK();
		instruction<<=1;
		My_Delay_us(EEP_CLOCK);
	}

	//End WREN Instruction
	CS_EEP_HIGH();

	//Give some Delay
	My_Delay_us(1);

	instruction=eep_WRITE;
	SCK_LOW();
	SO_LOW();
	//Start Write Data
	CS_EEP_LOW();

	//Send Instruction
	for(bit=0;bit<8;bit++)
	{
		WRITE_SO(instruction & 0x80);
		TOGGLE_SCK();
		My_Delay_us(EEP_CLOCK);
		TOGGLE_SCK();
		instruction<<=1;
		My_Delay_us(EEP_CLOCK);
	}

	//Send addr
	for(bit=0;bit<24;bit++)
	{
		WRITE_SO(addr & 0x00800000);
		TOGGLE_SCK();
		My_Delay_us(EEP_CLOCK);
		TOGGLE_SCK();
		addr<<=1;
		My_Delay_us(EEP_CLOCK);
	}

	//Send Data
	for(bit=0;bit<8;bit++)
	{
		WRITE_SO(data & 0x80);
		TOGGLE_SCK();
		My_Delay_us(EEP_CLOCK);
		TOGGLE_SCK();
		data<<=1;
		My_Delay_us(EEP_CLOCK);
	}

	//finish
	CS_EEP_HIGH();
	//Give at least 6ms delay
	My_Delay_ms(8);
}

void EEP25LC_Write_Multiple_Data(int32_t addr,int8_t* data)
{
	int8_t bit=0;
	int8_t i=0;
	int8_t instruction=0;

	//Initialization
	instruction=eep_WREN;
	SCK_LOW();
	SO_LOW();
	CS_EEP_HIGH();

	//WREN Initialization
	CS_EEP_LOW();

	//Send Instruction
	for(bit=0;bit<8;bit++)
	{
		WRITE_SO(instruction & 0x80);
		TOGGLE_SCK();
		My_Delay_us(EEP_CLOCK);
		TOGGLE_SCK();
		instruction<<=1;
		My_Delay_us(EEP_CLOCK);
	}

	//End WREN Instruction
	CS_EEP_HIGH();

	//Give some Delay
	My_Delay_us(1);

	instruction=eep_WRITE;
	SCK_LOW();
	SO_LOW();
	//Start Write Data
	CS_EEP_LOW();

	//Send Instruction
	for(bit=0;bit<8;bit++)
	{
		WRITE_SO(instruction & 0x80);
		TOGGLE_SCK();
		My_Delay_us(EEP_CLOCK);
		TOGGLE_SCK();
		instruction<<=1;
		My_Delay_us(EEP_CLOCK);
	}

	//Send addr
	for(bit=0;bit<24;bit++)
	{
		WRITE_SO(addr & 0x00800000);
		TOGGLE_SCK();
		My_Delay_us(EEP_CLOCK);
		TOGGLE_SCK();
		addr<<=1;
		My_Delay_us(EEP_CLOCK);
	}

	//Send Data
	for(i=0;i<sizeof(data);i++)
	{
		for(bit=0;bit<8;bit++)
		{
			WRITE_SO(data[i] & 0x80);
			TOGGLE_SCK();
			My_Delay_us(EEP_CLOCK);
			TOGGLE_SCK();
			data[i]<<=1;
			My_Delay_us(EEP_CLOCK);
		}
	}

	//finish
	CS_EEP_HIGH();
	//Give at least 6ms delay
	My_Delay_ms(8);
}

int8_t EEP25LC_Read_Data(int32_t addr)
{
	int8_t bit=0;
	int8_t instruction=0;
	int8_t byte=0;

	//Initialization
	instruction=eep_READ;
	SCK_LOW();
	SO_LOW();
	CS_EEP_HIGH();

	//WREN Initialization
	CS_EEP_LOW();

	//Send Instruction
	for(bit=0;bit<8;bit++)
	{
		WRITE_SO(instruction & 0x80);
		TOGGLE_SCK();
		My_Delay_us(EEP_CLOCK);
		TOGGLE_SCK();
		instruction<<=1;
		My_Delay_us(EEP_CLOCK);
	}

	//Send addr
	for(bit=0;bit<24;bit++)
	{
		WRITE_SO(addr & 0x00800000);
		TOGGLE_SCK();
		My_Delay_us(EEP_CLOCK);
		TOGGLE_SCK();
		addr<<=1;
		My_Delay_us(EEP_CLOCK);
	}

	//Read Data
	for(bit=0;bit<8;bit++)
	{
		if(READ_SI())
		{
			byte|=(1<<(7-bit));
		}
		TOGGLE_SCK();
		My_Delay_us(EEP_CLOCK);
		TOGGLE_SCK();
		My_Delay_us(EEP_CLOCK);
	}

	//finish
	CS_EEP_HIGH();
	//Give some delay
	My_Delay_us(1);

	return byte;
}

