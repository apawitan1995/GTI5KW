/*
 * UART_app.h
 *
 *  Created on: Mar 19, 2014
 *   Update on: Jan 13, 2016
 *      Author: angga, Arif
 */

#ifndef UART_APP_H_
#define UART_APP_H_

#include <DAVE3.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "peripheral/PWM_app.h"
#include "algorithm/converter.h"


extern volatile int string_read_xmc_increment;
extern volatile int string_read_rabbit_increment;
extern volatile char string_read_xmc[64];
extern volatile char string_read_rabbit[256];
extern volatile int flag_update_send_xmc;
extern volatile int flag_update_receive_xmc;
extern volatile int flag_data_receive_xmc;
extern volatile int data_uart_xmc[16];
extern volatile int data_uart_rabbit[50];
extern volatile int data_uart_rabbit_update[50];

extern volatile int flag_invalid_K2;


extern volatile int flag_update_send_rabbit;
extern volatile int flag_update_receive_rabbit;

#define UART_RS232 USIC0_CH1
#define UART_RS485 USIC1_CH1
#define UART_U1 USIC2_CH0

#define UART_RS485_Handle UART001_Handle0
#define UART_RS232_Handle UART001_Handle1
#define UART_U1_Handle UART001_Handle2

//SEL 485
#define SEL_485 10

//#define UART_RS232_Handle UART001_Handle0
//#define UART_RS232_API (UART001_HandleType *)&UART_RS232_Handle

void UART_Init(void);
uint32_t UART001_WaitWriteDataMultiple(USIC_CH_TypeDef* UartRegs,uint8_t* DataPtr,uint32_t Count);
uint32_t UART001_WaitReadDataMultiple(USIC_CH_TypeDef* UartRegs,uint8_t* DataPtr,uint32_t Count);

void kirimnullchar(USIC_CH_TypeDef* UartRegs);
void uart_xmc_request(int command);
void uart_xmc_send(void);
void uart_xmc_receive(void);

void data_rabbit(void);
void uart_rabbit_send(int data_part);
void uart_rabbit_receive(int increment);
//void uart_rabbit_receive(void);

extern volatile int start_scan;

#endif /* UART_APP_H_ */
