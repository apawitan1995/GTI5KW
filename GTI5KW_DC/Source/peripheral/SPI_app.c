/*
 * SPI_app.c
 *
 *  Created on: 5 Jan 2015
 *      Author: asus
 */

//uint8_t ReadData = 0;
//
//uint8_t Idx;
//uint8_t cReply[] = {0x05, 0x0F,0x32};

#include "peripheral/SPI_app.h"

volatile uint16_t tes_spi_data;
volatile uint16_t tes_spi_uc[3];

SPI001_ConfigType SPI001_Config1;

void SPI_uC_Write_1Byte_Receive_3Byte(uint32_t transmit_data, uint16_t* p_spi_uc)
{
	uint16_t tmp,i;
	uint16_t Data = 0;
	uint8_t Status1 = 0;
	uint8_t Status2 = 0;

	SPI001_Config1.Mode = SPI001_STANDARD_FULLDUPLEX;

	/*Change the frame length to 8 for sending
	* Write Enable command to flash chip */
	SPI001_Config1.FrameLen = 32;

	/* Configure the SPI Channel */
	SPI001_Configure(&SPI001_Handle1,&SPI001_Config1);

	/* Clear Standard receive indication and Alternative receive indication flag */
	SPI001_ClearFlag(&SPI001_Handle1,SPI001_RECV_IND_FLAG);
	SPI001_ClearFlag(&SPI001_Handle1,SPI001_ALT_RECV_IND_FLAG);
	Data = transmit_data;
	SPI001_WriteData(&SPI001_Handle1,&Data,SPI001_STANDARD);
	/* Wait till dummy data is received from flash chip */
	do
	{
	   Status1 = SPI001_GetFlagStatus(&SPI001_Handle1,SPI001_RECV_IND_FLAG);
	   Status2 = SPI001_GetFlagStatus(&SPI001_Handle1,SPI001_ALT_RECV_IND_FLAG);
	}while(!((Status1 == SPI001_SET) || (Status2 == SPI001_SET)));
	/* Do a dummy read*/
	SPI001_ReadData(&SPI001_Handle1,&tmp);

	for (i=0; i<3; i++)
	{
		SPI001_ClearFlag(&SPI001_Handle1,SPI001_RECV_IND_FLAG);
		SPI001_ClearFlag(&SPI001_Handle1,SPI001_ALT_RECV_IND_FLAG);
		/* Enable end of frame */
		//EnableEndOfFrame(SPI001_Handle0);
		Data = DUMMY;
		/* Send dummy data to flash chip */
		SPI001_WriteData(&SPI001_Handle1,&Data,SPI001_STANDARD);
		/* wait for data to be received */
		do
		{
		  Status1 = SPI001_GetFlagStatus(&SPI001_Handle1,SPI001_RECV_IND_FLAG);
		  Status2 = SPI001_GetFlagStatus(&SPI001_Handle1,SPI001_ALT_RECV_IND_FLAG);
		}while(!((Status1 == SPI001_SET) || (Status2 == SPI001_SET)));
		/* Read data received from flash chip to buffer */
		SPI001_ReadData(&SPI001_Handle1,p_spi_uc);
		p_spi_uc+=1;
	}
}

uint16_t SPI_uC_Read_Data(uint32_t addr)
{
	uint16_t tmp,i;
	uint16_t Data = 0;
	uint8_t Status1 = 0;
	uint8_t Status2 = 0;

	SPI001_Config1.Mode = SPI001_STANDARD_FULLDUPLEX;

	/*Change the frame length to 8 for sending
	* Write Enable command to flash chip */
	SPI001_Config1.FrameLen = 16;

	/* Configure the SPI Channel */
	SPI001_Configure(&SPI001_Handle1,&SPI001_Config1);

	/* Clear Standard receive indication and Alternative receive indication flag */
	SPI001_ClearFlag(&SPI001_Handle1,SPI001_RECV_IND_FLAG);
	SPI001_ClearFlag(&SPI001_Handle1,SPI001_ALT_RECV_IND_FLAG);
	Data = addr;
	SPI001_WriteData(&SPI001_Handle1,&Data,SPI001_STANDARD);
	Data = addr+1;
	SPI001_WriteData(&SPI001_Handle1,&Data,SPI001_STANDARD);
	/* Wait till dummy data is received from flash chip */
	do
	{
	   Status1 = SPI001_GetFlagStatus(&SPI001_Handle1,SPI001_RECV_IND_FLAG);
	   Status2 = SPI001_GetFlagStatus(&SPI001_Handle1,SPI001_ALT_RECV_IND_FLAG);
	}while(!((Status1 == SPI001_SET) || (Status2 == SPI001_SET)));
	/* Do a dummy read*/
	SPI001_ReadData(&SPI001_Handle1,&tes_spi_uc[0]);

//	SPI001_ClearFlag(&SPI001_Handle1,SPI001_RECV_IND_FLAG);
//	SPI001_ClearFlag(&SPI001_Handle1,SPI001_ALT_RECV_IND_FLAG);
////	Data = addr+1;
////	SPI001_WriteData(&SPI001_Handle1,&Data,SPI001_STANDARD);
////	/* wait for data to be received */
//	do
//	{
//	  Status1 = SPI001_GetFlagStatus(&SPI001_Handle1,SPI001_RECV_IND_FLAG);
//	  Status2 = SPI001_GetFlagStatus(&SPI001_Handle1,SPI001_ALT_RECV_IND_FLAG);
//	}while(!((Status1 == SPI001_SET) || (Status2 == SPI001_SET)));
//	/* Read data received from flash chip to buffer */
//	SPI001_ReadData(&SPI001_Handle1,&tes_spi_uc[1]);

	return tmp;
}
