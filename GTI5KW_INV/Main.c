/*
 * Main.c
 *
 *  Created on: Mar 19, 2014
 *      Author: Angga
 */
#include <stdio.h>
#include <DAVE3.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)

#include "common/delay.h"
#include "common/global.h"
#include "common/look_up_table.h"
#include "common/my_multiplexer.h"

#include "peripheral/IO_app.h"
#include "peripheral/UART_app.h"
#include "peripheral/ADC_app.h"
#include "peripheral/PWM_app.h"
#include "peripheral/DAC_app.h"
#include "peripheral/ERU_app.h"
#include "peripheral/I2C_SW_app.h"
#include "peripheral/ISR_app.h"
#include "peripheral/SPI_app.h"

#include "external/EEP25LC1024_app.h"
#include "external/display_app.h"
#include "external/AD5328_app.h"
#include "external/MCP79410_app.h"
#include "external/button_app.h"
#include "external/relay_app.h"

#include "algorithm/inverter.h"
#include "algorithm/PLL.h"
#include "algorithm/droop.h"


void update_data(void);

int main(void){

	PWM_Inverter_Disable();

	DAVE_Init();			// Initialization of DAVE Apps
	My_DAVE_MUX_Init();
	//ERU_Init();
	DAC_Init();

	UART_Init();

	DO_Profiling_Init();
	IO_Slave_Status_Init();
	IO_COM_Init();

	PWM_Inverter_Init();

	ADC_Init();

	PWM_Inverter_Start();

	PWM_Enable_Interrupt_NVIC();
	//int i;
	//for(i=0;i<10;i++) {
	//My_Delay_ms(100);

	//DCPT1_OFFSET += DCPT1_AIN_Result;
	//ACPT2_OFFSET += ACPT2_AIN_Result;
	//GRPT2_OFFSET += GRPT2_AIN_Result;
	//CT5_OFF += CT5_Result;
	//CT7_OFF += CT7_Result;}

	//DCPT2_OFFSET = DCPT2_OFFSET*0.1;
	//ACPT2_OFFSET = ACPT2_OFFSET*0.1;
	//GRPT2_OFFSET = GRPT2_OFFSET*0.1;
	//CT5_OFF = CT5_OFF*0.1;
	//CT7_OFF = CT7_OFF*0.1;

	My_Delay_s(7);

	int i;
	for(i=0;i<10;i++) {
	My_Delay_ms(100);
	//DCPT1_OFFSET += DCPT1_AIN_Result;
	//DCPT2_OFFSET += DCPT2_AIN_Result;
	ACPT1_OFFSET += ACPT1_AIN_Result;
	//ACPT2_OFFSET += ACPT2_AIN_Result;
	ACPT3_OFFSET += ACPT3_AIN_Result;
	GRPT1_OFFSET += GRPT1_AIN_Result;
	//GRPT2_OFFSET += GRPT2_AIN_Result;
	GRPT3_OFFSET += GRPT3_AIN_Result;
	CT1_OFF += CT1_Result;
	CT2_OFF += CT2_Result;
	CT3_OFF += CT3_Result;
	CT4_OFF += CT4_Result;
	CT5_OFF += CT5_Result;
	CT6_OFF += CT6_Result;
	CT7_OFF += CT7_Result;
	CT8_OFF += CT8_Result;
	}

	//DCPT1_OFFSET = DCPT1_OFFSET*0.1;
	//DCPT2_OFFSET = DCPT2_OFFSET*0.1;
	ACPT1_OFFSET = ACPT1_OFFSET*0.1;
	//ACPT2_OFFSET = ACPT2_OFFSET*0.1;
	ACPT3_OFFSET = ACPT3_OFFSET*0.1;
	GRPT1_OFFSET = GRPT1_OFFSET*0.1;
	//GRPT2_OFFSET = ACPT2_OFFSET+5; // ini untuk GTI 1 aja ada + 5
	GRPT3_OFFSET = GRPT3_OFFSET*0.1;
	CT1_OFF = CT1_OFF*0.1;
	CT2_OFF = CT2_OFF*0.1;
	CT3_OFF = CT3_OFF*0.1;
	CT4_OFF = CT4_OFF*0.1;
	CT5_OFF = CT5_OFF*0.1;
	CT6_OFF = CT6_OFF*0.1;
	CT7_OFF = CT7_OFF*0.1;
	CT8_OFF = CT8_OFF*0.1;





	//inisiasi_inverter();

	comparepwm4=0;
	comparepwm5=1;

	//Tetaref=0;


	while(1)
	{
		update_data();

	}
	return 0;
}

void update_data(void){
	float buffer_freq;
	if (flag_update_send_xmc == 1){
		data_uart_xmc[0]= comparepwm4;
		data_uart_xmc[1]= comparepwm5;
		data_uart_xmc[2]= Qset;//
		data_uart_xmc[6]=ACPT2_AIN_Result;
		data_uart_xmc[7]= 0;//fault_frekuensi;
		data_uart_xmc[8]= fixed2float(voac_peak_pll,20);
		buffer_freq = fixed2float(frekuensi_pll,20)*1000;
		data_uart_xmc[9]= buffer_freq;//CT5_fix;//CT5_Result*10;ACPT2_AIN_Result*10;
		data_uart_xmc[10]= DCPT2_fix;//CT7_fix;//CT7_Result*10;GRPT2_AIN_Result*10;//
		data_uart_xmc[11]= CT5_fix;//CT5_Result*10;//
		data_uart_xmc[12]= CT7_fix;//CT7_Result*10;//
		data_uart_xmc[13]= Pow_Filt;//CT5_Result*10;//
		data_uart_xmc[14]= Q_Filt;//CT7_Result*10;//

		uart_xmc_send();
		flag_update_send_xmc=0;
	}
	if(flag_update_receive_xmc == 1){
		uart_xmc_receive();
		string_read_xmc_increment=0;

		//comparepwm4= data_uart_xmc[0];
		//comparepwm5= data_uart_xmc[1];
		//Ppv = data_uart_xmc[2]; // data ppv //Q10
		//data_in[1] = data_uart_xmc[3]; // data Qref
		Qset =data_uart_xmc[2];
		MODE=data_uart_xmc[4];
		vreff_inv=data_uart_xmc[5];
		voac_peak_ref_fix= vreff_inv*1024;

		flag_update_receive_xmc = 0;
	}
}


