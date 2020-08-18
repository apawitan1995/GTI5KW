/*
 * 25LC1024_app.c
 *
 *  Created on: Feb 28, 2014
 *      Author: Hanif
 */
#include "external/EEP25LC1024_app.h"

/* SPI channel configuration structure */
SPI001_ConfigType SPI001_Config0;

volatile uint32_t counter_spi=0;
volatile uint32_t tes_spi_addr=0xAB;

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

int8_t EEP25LC_Read_Data_Dave_App(int32_t addr)
{
	int8_t bit=0;
	int8_t instruction=0;
	uint16_t byte=0;
	int8_t dummy=0;

	//Initialization
//	EnableStartOfFrame(SPI001_Handle0);
//	SPI001_WriteData(SPI_EEP,&instruction,SPI001_STANDARD);
//	EnableEndOfFrame(SPI001_HandleType);

	instruction=eep_READ;
//	SCK_LOW();
//	SO_LOW();
//	CS_EEP_HIGH();

//	//WREN Initialization
//	CS_EEP_LOW();

	//Send Instruction
//	for(bit=0;bit<8;bit++)
//	{
//		WRITE_SO(instruction & 0x80);
//		TOGGLE_SCK();
//		My_Delay_us(EEP_CLOCK);
//		TOGGLE_SCK();
//		instruction<<=1;
//		My_Delay_us(EEP_CLOCK);
//	}
	USIC0_CH1->TCSR|=USIC_CH_TCSR_WLEMD_Msk;
	USIC0_CH1->IN[7]=instruction;

	//Send addr
//	for(bit=0;bit<24;bit++)
//	{
//		WRITE_SO(addr & 0x00800000);
//		TOGGLE_SCK();
//		My_Delay_us(EEP_CLOCK);
//		TOGGLE_SCK();
//		addr<<=1;
//		My_Delay_us(EEP_CLOCK);
//	}

//	SPI001_WriteData(SPI_EEP,&addr,SPI001_STANDARD);

	USIC0_CH1->IN[15]=((addr>>8)&&0xFFFF);
	USIC0_CH1->IN[7]=((addr)&&0xFF);
	//USIC0_CH1->IN[7]=((addr)&&0xFF);

	USIC0_CH1->IN[23]=(dummy);
	SPI001_ReadData(SPI_EEP,&byte);

//	//Read Data
//	for(bit=0;bit<8;bit++)
//	{
//		if(READ_SI())
//		{
//			byte|=(1<<(7-bit));
//		}
//		TOGGLE_SCK();
//		My_Delay_us(EEP_CLOCK);
//		TOGGLE_SCK();
//		My_Delay_us(EEP_CLOCK);
//	}
//
//	//finish
//	CS_EEP_HIGH();
//	//Give some delay
//	My_Delay_us(1);

	return byte;
}

void EEP_Write_Enable()
{
   uint16_t tmp;
   uint16_t Data = 0;
   uint8_t Status1 = 0;
   uint8_t Status2 = 0;

   SPI001_Config0.Mode = SPI001_STANDARD_FULLDUPLEX;

   /*Change the frame length to 8 for sending
    * Write Enable command to flash chip */
   SPI001_Config0.FrameLen = 8;

   /* Configure the SPI Channel */
   SPI001_Configure(&SPI001_Handle0,&SPI001_Config0);

   /* Clear Standard receive indication and Alternative receive indication flag */
   SPI001_ClearFlag(&SPI001_Handle0,SPI001_RECV_IND_FLAG);
   SPI001_ClearFlag(&SPI001_Handle0,SPI001_ALT_RECV_IND_FLAG);

   Data = eep_WRITE;

   /* Send Write Enable Command */
   SPI001_WriteData(&SPI001_Handle0,&Data,SPI001_STANDARD);

   /* Wait till dummy data is received from flash chip */
   do
   {
	   Status1 = SPI001_GetFlagStatus(&SPI001_Handle0,SPI001_RECV_IND_FLAG);
	   Status2 = SPI001_GetFlagStatus(&SPI001_Handle0,SPI001_ALT_RECV_IND_FLAG);
   }while(!((Status1 == SPI001_SET) || (Status2 == SPI001_SET)));

   /* Do a dummy read*/
   SPI001_ReadData(&SPI001_Handle0,&tmp);

}

uint16_t EEP_Read_Data(uint32_t Address)
{
   uint16_t tmp, i;
   uint16_t Data = 0;
   uint8_t Status1 = 0;
   uint8_t Status2 = 0;
   SPI001_Config0.Mode = SPI001_STANDARD_FULLDUPLEX;
   SPI001_Config0.FrameLen = 40;

   /* Configure SPI channel with given configuration*/
   SPI001_Configure(&SPI001_Handle0,&SPI001_Config0);

   /* while reading page we are transmitting 256bytes of dummy data in one
    * SPI frame for receiving data. If we specify frame length has 64bit then actual frame length
    * will be indefinite and user needs to control start and end of frame
    *
    * User can control the start and end of frame by calling
    * EnableStartOfFrame() and EnableEndOfFrame() macro  functions
    * */
   //EnableStartOfFrame(SPI001_Handle0);

   SPI001_ClearFlag(&SPI001_Handle0,SPI001_RECV_IND_FLAG);
   SPI001_ClearFlag(&SPI001_Handle0,SPI001_ALT_RECV_IND_FLAG);
   Data = eep_READ;
   /* Send read page command */
   SPI001_WriteData(&SPI001_Handle0,&Data,SPI001_STANDARD);
   /*wait for dummy to be received*/
   do
   {
	   Status1 = SPI001_GetFlagStatus(&SPI001_Handle0,SPI001_RECV_IND_FLAG);
	   Status2 = SPI001_GetFlagStatus(&SPI001_Handle0,SPI001_ALT_RECV_IND_FLAG);
   }while(!((Status1 == SPI001_SET) || (Status2 == SPI001_SET)));
   SPI001_ReadData(&SPI001_Handle0,&tmp);	// dummy read

   SPI001_ClearFlag(&SPI001_Handle0,SPI001_RECV_IND_FLAG);
   SPI001_ClearFlag(&SPI001_Handle0,SPI001_ALT_RECV_IND_FLAG);
   Data = ((Address & 0x00FF0000)>>16);
   /* Send 3rd byte of 24bit start address	*/
   SPI001_WriteData(&SPI001_Handle0,&Data,SPI001_STANDARD);
   /* wait for dummy to be received */
   do
   {
	   Status1 = SPI001_GetFlagStatus(&SPI001_Handle0,SPI001_RECV_IND_FLAG);
	   Status2 = SPI001_GetFlagStatus(&SPI001_Handle0,SPI001_ALT_RECV_IND_FLAG);
   }while(!((Status1 == SPI001_SET) || (Status2 == SPI001_SET)));
   SPI001_ReadData(&SPI001_Handle0,&tmp);  // dummy read

   SPI001_ClearFlag(&SPI001_Handle0,SPI001_RECV_IND_FLAG);
   SPI001_ClearFlag(&SPI001_Handle0,SPI001_ALT_RECV_IND_FLAG);
   Data = ((Address & 0x0000FF00)>>8);
   /* Send 2nd byte of 24bit start address */
   SPI001_WriteData(&SPI001_Handle0,&Data,SPI001_STANDARD);
   /* wait for dummy to be received */
   do
   {
	   Status1 = SPI001_GetFlagStatus(&SPI001_Handle0,SPI001_RECV_IND_FLAG);
	   Status2 = SPI001_GetFlagStatus(&SPI001_Handle0,SPI001_ALT_RECV_IND_FLAG);
   }while(!((Status1 == SPI001_SET) || (Status2 == SPI001_SET)));
   SPI001_ReadData(&SPI001_Handle0,&tmp);	// dummy read

   SPI001_ClearFlag(&SPI001_Handle0,SPI001_RECV_IND_FLAG);
   SPI001_ClearFlag(&SPI001_Handle0,SPI001_ALT_RECV_IND_FLAG);
   Data = ((Address & 0x000000FF)>>0);
   /* Send 1st byte of 24bit start address */
   SPI001_WriteData(&SPI001_Handle0,&Data,SPI001_STANDARD);
   /* wait for dummy to be received */
   do
   {
	   Status1 = SPI001_GetFlagStatus(&SPI001_Handle0,SPI001_RECV_IND_FLAG);
	   Status2 = SPI001_GetFlagStatus(&SPI001_Handle0,SPI001_ALT_RECV_IND_FLAG);
   }while(!((Status1 == SPI001_SET) || (Status2 == SPI001_SET)));
	SPI001_ReadData(&SPI001_Handle0,&tmp);	// dummy read

	SPI001_ClearFlag(&SPI001_Handle0,SPI001_RECV_IND_FLAG);
	SPI001_ClearFlag(&SPI001_Handle0,SPI001_ALT_RECV_IND_FLAG);
	/* Enable end of frame */
	//EnableEndOfFrame(SPI001_Handle0);
	Data = DUMMY;
	/* Send dummy data to flash chip */
	SPI001_WriteData(&SPI001_Handle0,&Data,SPI001_STANDARD);
	/* wait for data to be received */
	do
	{
	  Status1 = SPI001_GetFlagStatus(&SPI001_Handle0,SPI001_RECV_IND_FLAG);
	  Status2 = SPI001_GetFlagStatus(&SPI001_Handle0,SPI001_ALT_RECV_IND_FLAG);
	}while(!((Status1 == SPI001_SET) || (Status2 == SPI001_SET)));
	/* Read data received from flash chip to buffer */
	SPI001_ReadData(&SPI001_Handle0,&tmp);

	return tmp;
}

