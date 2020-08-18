/*
 * MCP79410.c
 *
 *  Created on: Feb 24, 2014
 *      Author: RISNIF
 */

#include "external/MCP79410_app.h"

uint8_t RTC_Init(uint8_t day,uint8_t date,uint8_t month,uint8_t year,uint8_t hour,uint8_t minute,uint8_t second)
{
	uint8_t dozens;
	uint8_t unit;
	uint8_t data=0;

//	//Debug
//	char kal[32];
//
//	sprintf(kal,"\r\nRTC INIT START\r\n");
//	UART001_WaitWriteDataMultiple(UART_TTL,kal,strlen(kal));

	data=0;
	data=(1<<MFP)|(1<<SQWE);
	if(!I2C_write_data(RTC_ADDR,RTC_Control,&data,1))
	{
//		sprintf(kal,"year_err\r\n");
//		UART001_WaitWriteDataMultiple(UART_TTL,kal,strlen(kal));
		return 0;
	}


	dozens=year/10;
	unit=year%10;
	data=(dozens << 4)|unit;

	if(!I2C_write_data(RTC_ADDR,RTC_Year,&data,1))
	{
//		sprintf(kal,"year_err\r\n");
//		UART001_WaitWriteDataMultiple(UART_TTL,kal,strlen(kal));
		return 0;
	}


	dozens=month/10;
	unit=month%10;
	data=0;
	data=(dozens << 4)|unit;

	if(!I2C_write_data(RTC_ADDR,RTC_Month,&data,1))
	{
//		sprintf(kal,"month_err\r\n");
//		UART001_WaitWriteDataMultiple(UART_TTL,kal,strlen(kal));
		return 0;
	}

	dozens=date/10;
	unit=date%10;
	data=0;
	data=(dozens << 4)|unit;
	if(!I2C_write_data(RTC_ADDR,RTC_Date,&data,1))
	{
//		sprintf(kal,"date_err\r\n");
//		UART001_WaitWriteDataMultiple(UART_TTL,kal,strlen(kal));
		return 0;
	}

	data=0;
	data=(1 << VBATEN)|day;
	if(!I2C_write_data(RTC_ADDR,RTC_Day,&data,1))
	{
//		sprintf(kal,"day_err\r\n");
//		UART001_WaitWriteDataMultiple(UART_TTL,kal,strlen(kal));
		return 0;
	}

	dozens=hour/10;
	unit=hour%10;
	data=0;
	data=(dozens << 4)|unit;
	if(!I2C_write_data(RTC_ADDR,RTC_Hour,&data,1))
	{
//		sprintf(kal,"hour_err\r\n");
//		UART001_WaitWriteDataMultiple(UART_TTL,kal,strlen(kal));
		return 0;
	}

	dozens=minute/10;
	unit=minute%10;
	data=0;
	data=(dozens << 4)|unit;
	if(!I2C_write_data(RTC_ADDR,RTC_Minute,&data,1))
	{
//		sprintf(kal,"minute_err\r\n");
//		UART001_WaitWriteDataMultiple(UART_TTL,kal,strlen(kal));
		return 0;
	}

	dozens=second/10;
	unit=second%10;
	data=0;
	data=(1 << ST)|(dozens << 4)|unit;
	if(!I2C_write_data(RTC_ADDR,RTC_Second,&data,1))
	{
//		sprintf(kal,"second_err\r\n");
//		UART001_WaitWriteDataMultiple(UART_TTL,kal,strlen(kal));
		return 0;
	}

	return 1;
}

void RTC_get_time(uint8_t* hour,uint8_t* minute,uint8_t* second)
{
	uint8_t temp;

	I2C_read_data(RTC_ADDR,RTC_Hour,hour,1);
	temp=(*hour>>4)&0x03;
	*hour=(temp*10)+(*hour&0x0F);

	I2C_read_data(RTC_ADDR,RTC_Minute,minute,1);
	temp=(*minute>>4)&0x07;
	*minute=(temp*10)+(*minute&0x0F);

	I2C_read_data(RTC_ADDR,RTC_Second,second,1);
	temp=(*second>>4)&0x07;
	*second=(temp*10)+(*second&0x0F);
}

void RTC_get_date(uint8_t* year,uint8_t* month,uint8_t* date)
{
	uint8_t temp;

	I2C_read_data(RTC_ADDR,RTC_Date,date,1);
	temp=(*date>>4)&0x03;
	*date=(temp*10)+(*date&0x0F);

	I2C_read_data(RTC_ADDR,RTC_Month,month,1);
	temp=(*month>>4)&0x01;
	*month=(temp*10)+(*month&0x0F);

	I2C_read_data(RTC_ADDR,RTC_Year,year,1);
	temp=(*year>>4)&0x0F;
	*year=(temp*10)+(*year&0x0F);
}

void RTC_get_time_date_str(char *str)
{
	uint8_t hour,minute,second,date,month,year;

	RTC_get_time(&hour,&minute,&second);
	RTC_get_date(&year,&month,&date);

	sprintf(str,"%02i%02i%02i %02i:%02i:%02i \r\n",date,month,year,hour,minute,second);
}

//void RTC_set_time(uint8_t date, uint8_t month, uint8_t year, uint8_t hour, uint8_t minute, uint8_t second)
//{
//	//	setting parameter RTC
//	uint8_t tahun	= year; // 0001 0100 = 14
//	uint8_t bulan 	= month; // 0000 1000 = 8
//	uint8_t tanggal = date; // 0001 0010 = 12
//	uint8_t hari 	= 0b00001000; // 0000 1000 = 0 , bit 3 = Vbaten
//	uint8_t jam 	= hour; // 0001 0000 = 10, bit6 12/24, bit5 AM/PM
//	uint8_t menit 	= minute; // 0010 0000 = 20,
//	uint8_t detik 	= 0b10000000 | second; // 1000 0000 = 0 , bit 7 = ST
//
//	//	Setting RTC
//	I2C_write_data(RTC_ADDR,RTC_Year,&tahun,1);
//	I2C_write_data(RTC_ADDR,RTC_Month,&bulan,1);
//	I2C_write_data(RTC_ADDR,RTC_Date,&tanggal,1);
//	I2C_write_data(RTC_ADDR,RTC_Day,&hari,1);
//	I2C_write_data(RTC_ADDR,RTC_Hour,&jam,1);
//	I2C_write_data(RTC_ADDR,RTC_Minute,&menit,1);
//	I2C_write_data(RTC_ADDR,RTC_Second,&detik,1);
//}
//		I2C_read_data(RTC_ADDR,RTC_Day,&temp,1);
//		sprintf(kal,"OSCON=%d!\r\n",(temp>>5)&0x01);
//		UART001_WaitWriteDataMultiple(UART_TTL,kal,strlen(kal));
//		sprintf(kal,"Vbat=%d!\r\n",(temp>>4)&0x01);
//		UART001_WaitWriteDataMultiple(UART_TTL,kal,strlen(kal));
//		sprintf(kal,"VBATEN=%d!\r\n",(temp>>3)&0x01);
//		UART001_WaitWriteDataMultiple(UART_TTL,kal,strlen(kal));
//		sprintf(kal,"Day=%d!\r\n",temp&0x07);
//		UART001_WaitWriteDataMultiple(UART_TTL,kal,strlen(kal));
//
//		I2C_read_data(RTC_ADDR,RTC_Second,&temp,1);
//		sprintf(kal,"ST=%d!\r\n",(temp>>7)&0x01);
//		UART001_WaitWriteDataMultiple(UART_TTL,kal,strlen(kal));
//		sprintf(kal,"second=%d!\r\n",temp&0x0F);
//		UART001_WaitWriteDataMultiple(UART_TTL,kal,strlen(kal));
