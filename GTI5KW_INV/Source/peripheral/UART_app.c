/*
 * UART_app.c
 *
 *  Created on: Mar 19, 2014
 *      Author: angga
 */

#include "peripheral/UART_app.h"

volatile int string_read_xmc_increment = 0;
volatile char string_read_xmc[64];

volatile int flag_update_send_xmc =0;
volatile int flag_update_receive_xmc =0;
volatile int flag_data_receive_xmc;
volatile int data_uart_xmc[16];

void UART_Init(void)
{
	//Standar receive event RCI mode
	SET_BIT(UART_U2->RBCTR,USIC_CH_RBCTR_RNM_Pos);
	//RCI control
	WR_REG(UART_U2->RBCTR,USIC_CH_RBCTR_RCIM_Msk,USIC_CH_RBCTR_RCIM_Pos,2);
	//Enable Interrupt
	SET_BIT(UART_U2->RBCTR,USIC_CH_RBCTR_SRBIEN_Pos);

	//Set Interrupt Priority
	NVIC_SetPriority(USIC0_5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),63,0));
	//Enable CCU81_0_IRQHandler (PWM_Inverter) interrupt
	NVIC_EnableIRQ(USIC0_5_IRQn);
}

uint32_t UART001_WaitWriteDataMultiple
(
  USIC_CH_TypeDef* UartRegs,
  uint8_t* DataPtr,
  uint32_t Count
)
{
  uint32_t WriteCount = 0x00U;

  /* <<<DD_UART001_API_2>>>*/
  while(Count)
  {
	  while(USIC_IsTxFIFOfull(UartRegs));
	  UartRegs->IN[0] = *DataPtr;
	  Count--;
	  WriteCount++;
	  DataPtr++;
  }

  return WriteCount;
}

uint32_t UART001_WaitReadDataMultiple
(
	USIC_CH_TypeDef* UartRegs,
	uint8_t* DataPtr,
	uint32_t Count
)
{
  uint32_t ReadCount = 0x00U;

  while(Count)
  {
	  //wait data while USART RX FIFO Empty
	  while(USIC_ubIsRxFIFOempty(UartRegs));
	  *DataPtr = (uint16_t)UartRegs->OUTR;
	  Count--;
	  ReadCount++;
	  DataPtr++;
  }

  return ReadCount;
}

void uart_xmc_request(int command){
	char kal[8];
	if (command == 1){ // give update early for initialization
		sprintf(kal,"X");
		UART001_WaitWriteDataMultiple(UART_U2,kal,strlen(kal));
	}
}

void uart_xmc_receive(){
	const char s[2] = ";"; // ini terima aja buat memotong2 data
	char *token; /// ini terima aja buat memotong2 data
	int increment=0; // ini variabel buat increment aja
    token = strtok(string_read_xmc,s);
    while(token){
      data_uart_xmc[increment]=atof(token);
      token = strtok(NULL,s);
      increment++;
      }
}

void uart_xmc_send(){
	char buf_out[128],buf[128];
    strcpy(buf_out,"");
    sprintf(buf,"%d;",data_uart_xmc[0]);
    strcpy(buf_out, buf);
    sprintf(buf,"%d;",data_uart_xmc[1]);
    strcat(buf_out, buf);
    sprintf(buf,"%d;",data_uart_xmc[2]);
    strcat(buf_out, buf);
    sprintf(buf,"%d;",data_uart_xmc[3]);
    strcat(buf_out, buf);
    sprintf(buf,"%d;",data_uart_xmc[4]);
    strcat(buf_out, buf);
    sprintf(buf,"%d;",data_uart_xmc[5]);
    strcat(buf_out, buf);
    sprintf(buf,"%d;",data_uart_xmc[6]);
    strcat(buf_out, buf);
    sprintf(buf,"%d;",data_uart_xmc[7]);
    strcat(buf_out, buf);
    sprintf(buf,"%d;",data_uart_xmc[8]);
    strcat(buf_out, buf);
    sprintf(buf,"%d;",data_uart_xmc[9]);
    strcat(buf_out, buf);
    sprintf(buf,"%d;",data_uart_xmc[10]);
    strcat(buf_out, buf);
    sprintf(buf,"%d;",data_uart_xmc[11]);
    strcat(buf_out, buf);
    sprintf(buf,"%d;",data_uart_xmc[12]);
    strcat(buf_out, buf);
    sprintf(buf,"%d;",data_uart_xmc[13]);
    strcat(buf_out, buf);
    sprintf(buf,"%d;",data_uart_xmc[14]);
    strcat(buf_out, buf);
    sprintf(buf,"%d;",data_uart_xmc[15]);
    strcat(buf_out, buf);
    strcat(buf_out,"T");
    UART001_WaitWriteDataMultiple(UART_U2,buf_out,strlen(buf_out));
}

