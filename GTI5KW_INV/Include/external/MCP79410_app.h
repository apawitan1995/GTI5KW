/*
 * MCP79410.h
 *
 *  Created on: Feb 24, 2014
 *      Author: RISNIF
 */

#ifndef MCP79410_H_
#define MCP79410_H_

#include <DAVE3.h>
#include <stdio.h>
#include "peripheral/I2C_SW_app.h"

////Debug
//#include "UART_app.h"

#define VBATEN 3
#define ST 7
#define TIME_FORMAT 6
#define OSCON 5
#define SQWE 6
#define MFP 7

#define RTC_ADDR 0xDE

//MCP79410 address register
#define RTC_Second 0x00
#define RTC_Minute 0x01
#define RTC_Hour 0x02
#define RTC_Day 0x03
#define RTC_Date 0x04
#define RTC_Month 0x05
#define RTC_Year 0x06
#define RTC_Control 0x07
#define RTC_Calibration 0x08
#define RTC_UID 0x09

#define ALARM0_Second 0x0A
#define ALARM0_Minute 0x0B
#define ALARM0_Hour 0x0C
#define ALARM0_Day 0x0D
#define ALARM0_Date 0x0E
#define ALARM0_Month 0x0F

#define ALARM1_Second 0x11
#define ALARM1_Minute 0x12
#define ALARM1_Hour 0x13
#define ALARM1_Day 0x14
#define ALARM1_Date 0x15
#define ALARM1_Month 0x16

uint8_t RTC_Init(uint8_t day,uint8_t date,uint8_t month,uint8_t year,uint8_t hour,uint8_t minute,uint8_t second);
void RTC_get_time(uint8_t* hour,uint8_t* minute,uint8_t* second);
void RTC_get_date(uint8_t* year,uint8_t* month,uint8_t* date);
void RTC_get_time_date_str(char *str);
#endif /* MCP79410_H_ */
