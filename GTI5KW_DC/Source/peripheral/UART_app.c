/*
 * UART_app.c
 *
 *  Created on: Mar 19, 2014
 *   Update on: Jan 13, 2016
 *      Author: angga, Arif
 */

#include "peripheral/UART_app.h"

volatile int string_read_xmc_increment = 0;
volatile int string_read_rabbit_increment = 0;
volatile char string_read_xmc[64];
volatile char string_read_rabbit[256];
volatile int flag_update_send_xmc =0;
volatile int flag_update_receive_xmc =0;
volatile int flag_update_send_rabbit =0;
volatile int flag_update_receive_rabbit =0;
volatile int flag_data_receive_xmc;
volatile int data_uart_xmc[16];
volatile int data_uart_rabbit[50];
volatile int data_uart_rabbit_update[50];

//DI
volatile int flag_invalid_K2=2;
//volatile uint8_t DI = 0;
//volatile uint8_t DIS1_Flag, DIS2_Flag, DIS3_Flag, DI_K2F_Flag = 0;

//DO
volatile uint8_t DO, DOS = 0;
volatile uint8_t DO_K1_Flag, DO_K2_Flag, DO_HK1_Flag, DO_HK2_Flag, DOS1_Flag, DOS2_Flag = 0;

volatile uint8_t batterylevel, softwareversion = 0;

//Fault Software
volatile uint8_t FSW1, FSW2, FSW3, FSW4, FSW5, FSW6, FSW7, FSW8 = 0;
volatile uint8_t GEN_FB, K3_FB, K2_FB, K1_FB, GCTT_OL, GCTS_OL, GCTR_OL, GPTT_UV = 0;
volatile uint8_t GPTS_UV, GPTR_UV, GPTT_OV, GPTS_OV, GPTR_OV, ACCTT_OL, ACCTS_OL, ACCTR_OL = 0;
volatile uint8_t ACPTT_UV, ACPTS_UV, ACPTR_UV, ACPTT_OV, ACPTS_OV, ACPTR_OV, DCPT2_UV, DCPT2_OV = 0;
volatile uint8_t DCCT1_OC, GEN_SYNC, STF, FSW, FHW, RESET_TYPE = 0;

//Fault Hardware
volatile uint8_t ACPT_FHW, GRPT_FHW, DCPT_FHW, CT_FHW, GD_FHW = 0;
volatile uint8_t GRPT1_HWP, GRPT2_HWP, GRPT3_HWP;
volatile uint8_t ACPT1_HWP, ACPT2_HWP, ACPT3_HWP = 0;
volatile uint8_t DCPT1_HWP, DCPT2_HWP = 0;
volatile uint8_t CT1_HWP, CT234_HWP, CT56_HWP, CT78_HWP = 0;
volatile uint8_t GD1_HWP, GD2_HWP = 0;

void UART_Init(void)
{

	//Standar receive event RCI mode
	SET_BIT(UART_RS485->RBCTR,USIC_CH_RBCTR_RNM_Pos);
	//RCI control
	WR_REG(UART_RS485->RBCTR,USIC_CH_RBCTR_RCIM_Msk,USIC_CH_RBCTR_RCIM_Pos,2);
	//Enable Interrupt
	SET_BIT(UART_RS485->RBCTR,USIC_CH_RBCTR_SRBIEN_Pos);

	//Set SEL_485 as output
	WR_REG(PORT0->IOCR8,PORT0_IOCR8_PC10_Msk,PORT0_IOCR8_PC10_Pos,0x10);
	//Set SEL_485 as R/T
	CLR_BIT(PORT0->OUT,SEL_485);

	//Enable Frame Finished Interrupt
	//SET_BIT(UART_RS485->PCR_ASCMode,USIC_CH_PCR_ASCMode_FFIEN_Pos);

	//Set Interrupt Priority
	NVIC_SetPriority(USIC1_0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),60,0));
	//Enable CCU81_0_IRQHandler (PWM_Inverter) interrupt
	NVIC_EnableIRQ(USIC1_0_IRQn);

	//Standar receive event RCI mode
	SET_BIT(UART_RS232->RBCTR,USIC_CH_RBCTR_RNM_Pos);
	//RCI control
	WR_REG(UART_RS232->RBCTR,USIC_CH_RBCTR_RCIM_Msk,USIC_CH_RBCTR_RCIM_Pos,2);
	//Enable Interrupt
	SET_BIT(UART_RS232->RBCTR,USIC_CH_RBCTR_SRBIEN_Pos);

	//Set Interrupt Priority
	NVIC_SetPriority(USIC0_5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),60,0));
	//Enable CCU81_0_IRQHandler (PWM_Inverter) interrupt
	NVIC_EnableIRQ(USIC0_5_IRQn);

	//Standar receive event RCI mode
	SET_BIT(UART_U1->RBCTR,USIC_CH_RBCTR_RNM_Pos);
	//RCI control
	WR_REG(UART_U1->RBCTR,USIC_CH_RBCTR_RCIM_Msk,USIC_CH_RBCTR_RCIM_Pos,2);
	//Enable Interrupt
	SET_BIT(UART_U1->RBCTR,USIC_CH_RBCTR_SRBIEN_Pos);

	//Set Interrupt Priority
	NVIC_SetPriority(USIC2_2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),60,0));
	//Enable CCU81_0_IRQHandler (PWM_Inverter) interrupt
	NVIC_EnableIRQ(USIC2_2_IRQn);
}


void kirimnullchar(USIC_CH_TypeDef* UartRegs)
{
	  while(USIC_IsTxFIFOfull(UartRegs));
	  UartRegs->IN[0] = NULL;
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
	char kal[32];
	if (command == 1){ // give update early for initialization
		sprintf(kal,"X");
		UART001_WaitWriteDataMultiple(UART_U1,kal,strlen(kal));
	}
	else if ( command == 2 ){
		sprintf(kal,"Y");
		UART001_WaitWriteDataMultiple(UART_U1,kal,strlen(kal));
	}
	else if ( command == 3 ){
		sprintf(kal,"Z");
		UART001_WaitWriteDataMultiple(UART_U1,kal,strlen(kal));
	}
	else if ( command == 0 ){
		sprintf(kal,"I");
		UART001_WaitWriteDataMultiple(UART_U1,kal,strlen(kal));
	}
	else if ( command == 4 ){
		sprintf(kal,"A");
		UART001_WaitWriteDataMultiple(UART_U1,kal,strlen(kal));
	}
	else if ( command == 5 ){
		sprintf(kal,"B");
		UART001_WaitWriteDataMultiple(UART_U1,kal,strlen(kal));
	}
	else if ( command == 6 ){
		sprintf(kal,"C");
		UART001_WaitWriteDataMultiple(UART_U1,kal,strlen(kal));
	}
	else if ( command == 7 ){
		sprintf(kal,"D");
		UART001_WaitWriteDataMultiple(UART_U1,kal,strlen(kal));
	}
	else if ( command == 8 ){
		sprintf(kal,"[");
		UART001_WaitWriteDataMultiple(UART_U1,kal,strlen(kal));
	}
	else if ( command == 9 ){
		sprintf(kal,"]");
		UART001_WaitWriteDataMultiple(UART_U1,kal,strlen(kal));
	}
	else if ( command == 10){
		sprintf(kal,"E");
		UART001_WaitWriteDataMultiple(UART_U1,kal,strlen(kal));
	}
	else if ( command == 11 ){
		sprintf(kal,"F");
		UART001_WaitWriteDataMultiple(UART_U1,kal,strlen(kal));
	}
	else if ( command == 12){
		sprintf(kal,"K");
		UART001_WaitWriteDataMultiple(UART_U1,kal,strlen(kal));
		}
	else if ( command == 13 ){
		sprintf(kal,"k");
		UART001_WaitWriteDataMultiple(UART_U1,kal,strlen(kal));
		}
	else if ( command == 14){
			sprintf(kal,"G");
			UART001_WaitWriteDataMultiple(UART_U1,kal,strlen(kal));
			}
		else if ( command == 15 ){
			sprintf(kal,"g");
			UART001_WaitWriteDataMultiple(UART_U1,kal,strlen(kal));
			}
		else if ( command == 16){
				sprintf(kal,"H");
				UART001_WaitWriteDataMultiple(UART_U1,kal,strlen(kal));
				}
			else if ( command == 17 ){
				sprintf(kal,"h");
				UART001_WaitWriteDataMultiple(UART_U1,kal,strlen(kal));
				}
			else if ( command == 18){
				sprintf(kal,"J");
				UART001_WaitWriteDataMultiple(UART_U1,kal,strlen(kal));
							}
		else if ( command == 19 ){
				sprintf(kal,"j");
				UART001_WaitWriteDataMultiple(UART_U1,kal,strlen(kal));
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
    UART001_WaitWriteDataMultiple(UART_U1,buf_out,strlen(buf_out));
}

/*
void uart_rabbit_receive(){
	const char s[2] = "-"; // ini terima aja buat memotong2 data
	char *token; /// ini terima aja buat memotong2 data
	int increment=0; // ini variabel buat increment aja
    token = strtok(string_read_rabbit,s);
    while(token){
      data_uart_rabbit_update[increment]=atof(token);
      token = strtok(NULL,s);
      increment++;
      }
}

*/

/*
void uart_rabbit_receive(){
	const char s[2] = "-"; // ini terima aja buat memotong2 data
	char *token; /// ini terima aja buat memotong2 data
	int increment=0; // ini variabel buat increment aja
	char *save_str;
    token = strtok_r(string_read_rabbit,s,&save_str);
    while(token){
      data_uart_rabbit_update[increment]=atof(token);
      token = strtok_r(NULL,s,&save_str);
      increment++;
      }
}

*/
/*
void uart_rabbit_receive(){
	const char s[2] = "-"; // ini terima aja buat memotong2 data
	char *token; /// ini terima aja buat memotong2 data
    char *save_str = string_read_rabbit;
	int increment=0; // ini variabel buat increment aja
    while((token = strtok_r(save_str,s, &save_str))){
      data_uart_rabbit_update[increment]=atof(token);
      increment++;
      }
}
*/

volatile int start_scan =1;
volatile char *save_str;
volatile char *token; /// ini terima aja buat memotong2 data

void uart_rabbit_receive(int increment){
	const char s[2] = ","; // ini terima aja buat memotong2 data
    if (start_scan == 1){
    	token = strtok_r(string_read_rabbit,s,&save_str);
    	start_scan = 0;
    	}
    if (token != NULL){
      data_uart_rabbit_update[increment]=atof(token);
      token = strtok_r(NULL,s,&save_str);
      }
}

/*
void uart_rabbit_receive(){
	//UART001_WaitWriteDataMultiple(UART_RS232,string_read_rabbit,strlen(string_read_rabbit));
	const char s[2] = ","; // ini terima aja buat memotong2 data
	char *token; /// ini terima aja buat memotong2 data
	int increment=0; // ini variabel buat increment aja
	char *save_str;
    token = strtok_r(string_read_rabbit,s,&save_str);
    while(token){
      data_uart_rabbit_update[increment]=atof(token);
      token = strtok_r(NULL,s,&save_str);
      increment++;
      }
}

*/

void data_rabbit(){

	data_uart_rabbit[0] = 0;//
		data_uart_rabbit[1]= 0;//
		data_uart_rabbit[2]= fixed2float(DCPT1_fix,10);//
		data_uart_rabbit[3]= CT1_fix;//Power//
		data_uart_rabbit[4]= fixed2float(PV_power,10);//
		data_uart_rabbit[5]= 0;//
		data_uart_rabbit[6]= 0;//
		data_uart_rabbit[7]= 0;//
		data_uart_rabbit[8]= 0;//
		data_uart_rabbit[9]= 0;//
		data_uart_rabbit[10]= 0;//
		data_uart_rabbit[11]= 0;//
		data_uart_rabbit[12]= 0;//
		data_uart_rabbit[13]= flag_invalid_K2+flag_K2+1;//
		data_uart_rabbit[14]= 0;//
		data_uart_rabbit[15]= 0;//
		data_uart_rabbit[16]= 0;//
		data_uart_rabbit[17]= 0;//
		data_uart_rabbit[18]= 0;//
		data_uart_rabbit[19]= 0;//



	/*
	data_uart_rabbit[0] = 0;//ACCTR,
	//ACCTS,
	data_uart_rabbit[2]= 0;//ACCTT,
	data_uart_rabbit[3]= 0;//GCTR,
	//GCTS,
	data_uart_rabbit[5]= 0;//GCTT,
	//DCPT1,
	data_uart_rabbit[7]= 0;//GPTR,
	//GPTS,
	data_uart_rabbit[9]= 0;//GPTT,
	//TF1,
	//TF2,
	//DCCT1,
	//IVCTR,
	//IVCTS,
	data_uart_rabbit[15]= 0;//IVCTT,
	//DCPT2,
	data_uart_rabbit[17]= 0;//ACPTR,
	//ACPTS,
	data_uart_rabbit[19]= 0;//ACPTT,
	data_uart_rabbit[20]= 0;//FREQR,
	data_uart_rabbit[21]= 0;//FREQS,
	data_uart_rabbit[22]= 0;//FREQT,

		//DI
		//DIS1_Flag = RD_REG(PORT3->IN,PORT3_IN_P4_Msk,PORT3_IN_P4_Pos);
		//DIS2_Flag = RD_REG(PORT3->IN,PORT3_IN_P3_Msk,PORT3_IN_P3_Pos);
		//DIS3_Flag = RD_REG(PORT3->IN,PORT3_IN_P15_Msk,PORT3_IN_P15_Pos);
		//DI_K2F_Flag = RD_REG(PORT0->IN,PORT0_IN_P3_Msk,PORT0_IN_P3_Pos);

		data_uart_rabbit[23] = flag_invalid_K2+flag_K2+1;//((DI_K2F_Flag<<3) | (DIS3_Flag<<2) | (flag_K2<<1) | (DIS1_Flag<<0));

		//DO
		//DO_K1_Flag 	= RD_REG(PORT1->OUT,PORT1_OUT_P10_Msk,PORT1_OUT_P10_Pos);
		//DO_K2_Flag 	= RD_REG(PORT1->OUT,PORT1_OUT_P11_Msk,PORT1_OUT_P11_Pos);
		//DO_HK1_Flag = RD_REG(PORT1->OUT,PORT1_OUT_P12_Msk,PORT1_OUT_P12_Pos);
		//DO_HK2_Flag = RD_REG(PORT1->OUT,PORT1_OUT_P13_Msk,PORT1_OUT_P13_Pos);
		//DOS1_Flag 	= RD_REG(PORT2->OUT,PORT2_OUT_P11_Msk,PORT2_OUT_P11_Pos);
		//DOS2_Flag 	= RD_REG(PORT2->OUT,PORT2_OUT_P12_Msk,PORT2_OUT_P12_Pos);

		data_uart_rabbit[24] = 0;//data_uart_rabbit_update[25];//((DO_HK2_Flag<<3) | (DO_HK1_Flag<<2) | (DO_K2_Flag<<1) | (DO_K1_Flag<<0));

		data_uart_rabbit[25] = 0;//((DOS2_Flag<<1) | (DOS1_Flag<<0));

		data_uart_rabbit[26] = 0;//batterylevel,
		data_uart_rabbit[27] = 0;//state,
		data_uart_rabbit[28] = 0;//softwareversion,



		//FSW
		data_uart_rabbit[29] = 0;//((GEN_FB<<3) | (K3_FB<<2) | (K2_FB<<1) | (K1_FB<<0)); // FSW1
		data_uart_rabbit[30] = 0;//((GCTT_OL<<3) | (GCTS_OL<<2) | (GCTR_OL<<1) | (GPTT_UV<<0)); // FSW2
		data_uart_rabbit[31] = 0;//((GPTS_UV<<3) | (GPTR_UV<<2) | (GPTT_OV<<1) | (GPTS_OV<<0)); // FSW3
		data_uart_rabbit[32] = 0;//((GPTR_OV<<3) | (ACCTT_OL<<2) | (ACCTS_OL<<1) | (ACCTR_OL<<0)); // FSW4
		data_uart_rabbit[33] = 0;//((ACPTT_UV<<3) | (ACPTS_UV<<2) | (ACPTR_UV<<1) | (ACPTT_OV<<0)); // FSW5
		data_uart_rabbit[34] = 0;//((ACPTS_OV<<3) | (ACPTR_OV<<2) | (DCPT2_UV<<1) | (DCPT2_OV<<0)); // FSW6
		data_uart_rabbit[35] = 0;//((DCCT1_OC<<3) | (GEN_SYNC<<2) | (STF<<1) | (FSW<<0)); // FSW 7
		if( flag_fault > 2){
			data_uart_rabbit[36] = 1;//((FHW<<3) | (RESET_TYPE<<2)); // FSW8
		}
		else {
			data_uart_rabbit[36] = 0;
		}
		//FHW
		/*
		GRPT1_HWP	= RD_REG(PORT1->IN,PORT1_IN_P1_Msk,PORT1_IN_P1_Pos); // GRPT1_HWP
		GRPT2_HWP	= RD_REG(PORT1->IN,PORT1_IN_P2_Msk,PORT1_IN_P2_Pos); // GRPT2_HWP
		GRPT3_HWP	= RD_REG(PORT1->IN,PORT1_IN_P3_Msk,PORT1_IN_P3_Pos); // GRPT3_HWP
		ACPT1_HWP	= RD_REG(PORT2->IN,PORT2_IN_P4_Msk,PORT2_IN_P4_Pos); // ACPT1_HWP
		ACPT2_HWP	= RD_REG(PORT2->IN,PORT2_IN_P6_Msk,PORT2_IN_P6_Pos); // ACPT2_HWP
		ACPT3_HWP	= RD_REG(PORT2->IN,PORT2_IN_P7_Msk,PORT2_IN_P7_Pos); // ACPT3_HWP
		DCPT1_HWP	= RD_REG(PORT2->IN,PORT2_IN_P2_Msk,PORT2_IN_P2_Pos); // DCPT1_HWP
		DCPT2_HWP	= RD_REG(PORT2->IN,PORT2_IN_P3_Msk,PORT2_IN_P3_Pos); // DCPT2_HWP
		CT1_HWP		= RD_REG(PORT2->IN,PORT2_IN_P0_Msk,PORT2_IN_P0_Pos); // CT1_HWP
		CT234_HWP	= RD_REG(PORT2->IN,PORT2_IN_P5_Msk,PORT2_IN_P5_Pos); // CT234_HWP
		CT56_HWP	= RD_REG(PORT0->IN,PORT0_IN_P9_Msk,PORT0_IN_P9_Pos); // CT56_HWP
		CT78_HWP	= RD_REG(PORT3->IN,PORT3_IN_P2_Msk,PORT3_IN_P2_Pos); // CT78_HWP
		GD1_HWP		= RD_REG(PORT0->IN,PORT0_IN_P12_Msk,PORT0_IN_P12_Pos); // GD1_HWP
		GD2_HWP		= RD_REG(PORT0->IN,PORT0_IN_P2_Msk,PORT0_IN_P2_Pos); // GD2_HWP

		 //
		data_uart_rabbit[37] = 0;//((GRPT3_HWP<<2) | (GRPT2_HWP<<1) | (GRPT1_HWP<<0)); // GRPT_FHW
		data_uart_rabbit[38] = 0;//((ACPT3_HWP<<2) | (ACPT2_HWP<<1) | (ACPT1_HWP<<0)); // ACPT_FHW
		data_uart_rabbit[39] = 0;//((DCPT1_HWP<<1) | (DCPT2_HWP<<0)); // DCPT_FHW
		data_uart_rabbit[40] = 0;//((CT78_HWP<<3) | (CT56_HWP<<2) | (CT234_HWP<<1) | (CT1_HWP<<0)); // CT_FHW
		data_uart_rabbit[41] = 0;//((GD2_HWP<<1) | (GD1_HWP<<0)); // GD_FHW

		data_uart_rabbit[42] = 0;//spare1
		data_uart_rabbit[43] = fixed2float(PV_power,10);//spare2
		data_uart_rabbit[44] = 0;//spare3
		data_uart_rabbit[45] = 0;//spare4
		data_uart_rabbit[46] = 0;//spare5
		data_uart_rabbit[47] = 0;//spare6
		data_uart_rabbit[48] = 0;//spare7
*/

}

void uart_rabbit_send(int data_part){
	/*
	char buf_out[16],buf[16];

	strcpy(buf_out,"");
	if (data_part == 0){

	    sprintf(buf,"H");
		strcat(buf_out,buf);
	}
	else if (data_part == 1){

	    sprintf(buf,"G");
		strcat(buf_out,buf);
	}
	else if (data_part == 2){

	    sprintf(buf,"T");
		strcat(buf_out,buf);
	}
	else if (data_part == 3){

	    sprintf(buf,"I");
		strcat(buf_out,buf);
	}
	else if (data_part == 4){

	    sprintf(buf,"2");
		strcat(buf_out,buf);
	}
	else if ( data_part > 4 && data_part <= 27 ){
	    sprintf(buf,"%+06d,",data_uart_rabbit[data_part-5]);
	    strcat(buf_out, buf);
	}
	//  /*
	    sprintf(buf,"%+06d,",data_uart_rabbit[1]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%+06d,",data_uart_rabbit[2]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%+06d,",data_uart_rabbit[3]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%+06d,",data_uart_rabbit[4]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%+06d,",data_uart_rabbit[5]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%+06d,",data_uart_rabbit[6]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%+06d,",data_uart_rabbit[7]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%+06d,",data_uart_rabbit[8]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%+06d,",data_uart_rabbit[9]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%+06d,",data_uart_rabbit[10]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%+06d,",data_uart_rabbit[11]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%+06d,",data_uart_rabbit[12]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%+06d,",data_uart_rabbit[13]);
	    strcat(buf_out, buf);
   	    sprintf(buf,"%+06d,",data_uart_rabbit[14]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%+06d,",data_uart_rabbit[15]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%+06d,",data_uart_rabbit[16]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%+06d,",data_uart_rabbit[17]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%+06d,",data_uart_rabbit[18]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%+06d,",data_uart_rabbit[19]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%+06d,",data_uart_rabbit[20]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%+06d,",data_uart_rabbit[21]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%+06d,",data_uart_rabbit[22]);
	    strcat(buf_out, buf);
	    // 161

	//
	else if ( data_part > 27 && data_part <= 46 ){
		    sprintf(buf,"%X,",data_uart_rabbit[data_part-5]);
		    strcat(buf_out, buf);
		}
//	    /*
	    sprintf(buf,"%X,",data_uart_rabbit[24]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%X,",data_uart_rabbit[25]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%X,",data_uart_rabbit[26]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%X,",data_uart_rabbit[27]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%X,",data_uart_rabbit[28]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%X,",data_uart_rabbit[29]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%X,",data_uart_rabbit[30]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%X,",data_uart_rabbit[31]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%X,",data_uart_rabbit[32]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%X,",data_uart_rabbit[33]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%X,",data_uart_rabbit[34]);
	    strcat(buf_out, buf);
		sprintf(buf,"%X,",data_uart_rabbit[35]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%X,",data_uart_rabbit[36]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%X,",data_uart_rabbit[37]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%X,",data_uart_rabbit[38]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%X,",data_uart_rabbit[39]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%X,",data_uart_rabbit[40]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%X,",data_uart_rabbit[41]);
	    strcat(buf_out, buf);

	    //38

//
	else if ( data_part > 46 && data_part <= 52 ){
		    sprintf(buf,"%+06d,",data_uart_rabbit[data_part-5]);
		    strcat(buf_out, buf);
		}
	///*
	    sprintf(buf,"%+06d,",data_uart_rabbit[43]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%+06d,",data_uart_rabbit[44]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%+06d,",data_uart_rabbit[45]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%+06d,",data_uart_rabbit[46]);
	    strcat(buf_out, buf);
	    sprintf(buf,"%+06d,",data_uart_rabbit[47]);
	    strcat(buf_out, buf);
	//
	else if (data_part == 53)    {
	    sprintf(buf,"%+01d",data_uart_rabbit[data_part -5]);
	    strcat(buf_out, buf);
	}
	    // 43
	else if (data_part == 54){
	    sprintf(buf,"T");
	    strcat(buf_out, buf);
	}
	else if (data_part == 55){
		//while(USIC_IsTxFIFOfull(UART_RS485));
		//UART_RS485->IN[0] = NULL;
		//while(USIC_IsTxFIFOfull(UART_RS232));
		//UART_RS232->IN[0] = NULL;
		//kirimnullchar(UART_RS232);
		kirimnullchar(UART_RS485);
	}


	    //sprintf(buf,"%d",strlen(buf_out));
		//UART001_WaitWriteDataMultiple(UART_RS232,buf,strlen(buf));
		//UART001_WaitWriteDataMultiple(UART_RS232,buf,strlen(buf_out));

	   UART001_WaitWriteDataMultiple(UART_RS485,buf_out,strlen(buf_out));
*/

	char buf_out[16],buf[16];

	strcpy(buf_out,"");
	if (data_part == 0){

	    sprintf(buf,"H");
		strcat(buf_out,buf);
	}
	else if (data_part == 1){

	    sprintf(buf,"G");
		strcat(buf_out,buf);
	}
	else if (data_part == 2){

	    sprintf(buf,"T");
		strcat(buf_out,buf);
	}
	else if (data_part == 3){

	    sprintf(buf,"I");
		strcat(buf_out,buf);
	}
	else if (data_part == 4){

	    sprintf(buf,"3");
		strcat(buf_out,buf);
	}

	// 5
	else if ( data_part > 4 && data_part <= 16 ){
	    sprintf(buf,"%+06d,",data_uart_rabbit[data_part-5]);
	    strcat(buf_out, buf);
	}

	// 84
	else if ( data_part > 16 && data_part <= 23 ){
		    sprintf(buf,"%X,",data_uart_rabbit[data_part-5]);
		    strcat(buf_out, buf);
		}
	//13
	else if (data_part == 24){
		    sprintf(buf,"%X",data_uart_rabbit[data_part-5]);
		    strcat(buf_out, buf);
		}
	else if (data_part == 25){
	    sprintf(buf,"T");
	    strcat(buf_out, buf);
	}
	else if (data_part == 26){

		kirimnullchar(UART_RS232);
		kirimnullchar(UART_RS485);
	}


	   UART001_WaitWriteDataMultiple(UART_RS485,buf_out,strlen(buf_out));
	   UART001_WaitWriteDataMultiple(UART_RS232,buf_out,strlen(buf_out));

}




