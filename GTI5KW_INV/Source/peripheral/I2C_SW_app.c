/*
 * I2C_sw_xmc.c
 *
 *  Created on: Dec 23, 2013
 *      Author: Hanif
 */

#include "peripheral/I2C_SW_app.h"

////Debug
//char kal[32];

void I2C_Init()
{
	SET_SDA_OUT();
	SET_SCL_OUT();
	BusIdle();
}

void I2C_Disable()
{
	SET_SDA_IN();
	SET_SCL_IN();
}

int8_t I2C_start_cond()
{
	SDA_L();
	My_Delay_us(DELAY_CLK);
	SCL_L();
	My_Delay_us(DELAY_CLK);

	return 1;
}

int8_t I2C_wryte_byte(uint8_t byte)
{
	int8_t bit=7;

	while(bit >=0)
	{
		WRITE_SDA((byte>>bit)&0x01);
		SCL_H();
		My_Delay_us(DELAY_CLK);
		SCL_L();
		bit--;
		My_Delay_us(DELAY_CLK);
	}

	//release SDA
	SET_SDA_IN();
	SDA_L();
	SCL_H();

	//Check for Acknowledgment
	if(READ_SDA())
	{
//		//Not ACK Detected
//		sprintf(kal,"NACK\r\n");
//		UART001_WaitWriteDataMultiple(UART_TTL,kal,strlen(kal));
		return 0;
	}

	My_Delay_us(DELAY_CLK);
	//Pull SCL Low
	SCL_L();
	//Take SDA
	SET_SDA_OUT();
	My_Delay_us(DELAY_CLK);

	return 1;
}


int8_t I2C_write_data(uint8_t addr,uint8_t addreg,uint8_t* data,uint8_t bytes)
{
	uint8_t index;
	uint8_t success;

	I2C_Init();
	My_Delay_us(DELAY_CLK);
	My_Delay_us(DELAY_CLK);

	if(!I2C_start_cond())
		return 0;

	if(!I2C_wryte_byte(addr))
	{

		return 0;
	}

	if(!I2C_wryte_byte(addreg))
		return 0;

	for(index=0;index<bytes;index++)
	{
		//err --> LOW
		//success --> HIGH
		success=I2C_wryte_byte(data[index]);
		if(!success)
		{
//			sprintf(kal,"break %d\r\n",index);
//			UART001_WaitWriteDataMultiple(UART_TTL,kal,strlen(kal));

			//Stop Condition
			SCL_H();
			My_Delay_us(DELAY_CLK);
			SDA_H();
			return 0;
		}
	}
	//Stop Condition
	SCL_H();
	My_Delay_us(DELAY_CLK);
	SDA_H();
	My_Delay_us(DELAY_CLK);

	return 1;
}

int8_t I2C_read_byte(uint8_t* data,uint8_t index,uint8_t bytes)
{
	uint8_t byte=0;
	int8_t bit;

	//Release SDA
	SET_SDA_IN();
	for(bit=0;bit<8;bit++)
	{
		if(READ_SDA())
		{
			byte|=(1<<(7-bit));
		}
		SCL_H();
		My_Delay_us(DELAY_CLK);
		SCL_L();
		My_Delay_us(DELAY_CLK);
	}

	data[index]=byte;

	//take SDA
	SET_SDA_OUT();
	if(index < (bytes-1))
	{
		SDA_L();
		SCL_H();
		My_Delay_us(DELAY_CLK);
		//Pull SCL low
		SCL_L(); //end of byte with acknowledgment
		//Release SDA
		SET_SDA_IN();
		My_Delay_us(DELAY_CLK);
	}
	else //send NACK on the last byte
	{
		SDA_H();
		SCL_H();
		My_Delay_us(DELAY_CLK);
		//Pull SCL low
		SCL_L();
		//Release SDA
		My_Delay_us(DELAY_CLK);
	}
	return 1;
}

int8_t I2C_read_data(uint8_t addr,uint8_t addreg,uint8_t* data,uint8_t bytes)
{
	uint8_t index;
	//uint8_t ack;

	I2C_Init();
	My_Delay_us(DELAY_CLK);
	My_Delay_us(DELAY_CLK);

	if(!I2C_start_cond())
		return 0;

	if(!I2C_wryte_byte(addr))
		return 0;

	if(!I2C_wryte_byte(addreg))
			return 0;

	//Stop Condition
	SCL_H();
	My_Delay_us(DELAY_CLK);
	SDA_H();
	My_Delay_us(DELAY_CLK);

	if(!I2C_start_cond())
	{
//		sprintf(kal,"re-start_err %d\r\n",index);
//		UART001_WaitWriteDataMultiple(UART_TTL,kal,strlen(kal));
		return 0;
	}

	if(!I2C_wryte_byte(addr|1))
	{
//		sprintf(kal,"set_read_err %d\r\n",index);
//		UART001_WaitWriteDataMultiple(UART_TTL,kal,strlen(kal));
		return 0;
	}


	for(index=0;index<bytes;index++)
	{
		I2C_read_byte(data,index,bytes);
	}

	//Stop Condition
	SCL_H();
	My_Delay_us(DELAY_CLK);
	SDA_H();
	My_Delay_us(DELAY_CLK);

	return 1;
}


