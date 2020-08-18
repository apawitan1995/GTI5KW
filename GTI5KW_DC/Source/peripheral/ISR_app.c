/*
 * ISR_app.c
 *
 *  Created on: Sep 26, 2014
 *   Update on: Jan 13, 2016
 *      Author: angga + bima + iqbal + mukhlis + arif
 */

#include "peripheral/ISR_app.h"

volatile unsigned short int state=0; // state for FSM (Finite State Machine) from State Diagram
volatile unsigned short int previous_state=0;
volatile int flag_fault = 0;
volatile int32_t power_inv=100;

// test pembacaan power
volatile float prePow;
volatile float P_Filt;
volatile float Q_Filt;
volatile float preQ;


volatile int flag_activation = 0;
volatile int flag_deactivation = 0;


volatile int flag_gd1;
volatile int flag_gd2;

volatile int32_t Pset;
volatile int32_t Qset;

volatile int32_t flag_rabbit=0;

volatile int32_t counter1120=0;


// fungsi penerima karakter uart RS485 yang digunakan untuk jalur komunikasi rabbit dengan microcontroller
void UART_RS485_Handler(void)	//interrupt handler UART485 U1Ch1
{
	char data;
	static char kal[32];

	data=UART_RS485->OUTR;

	string_read_rabbit[string_read_rabbit_increment]= data;


	if( data == 'T'){

		if (flag_rabbit == 0){
			flag_rabbit = 1;
			string_read_rabbit_increment = -2;
		}
		else{
			flag_rabbit = 0;
			flag_update_receive_rabbit=1;
			string_read_rabbit_increment = -2;
			NVIC_DisableIRQ(USIC1_0_IRQn);

			// untuk debug apakah ada komunikasi dari rabbit yang masuk ke infineonnya
			//sprintf(kal,"masuk!\r\n");
			//UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

		}

	}

	string_read_rabbit_increment++;

	SET_BIT(UART_RS485->TRBSCR,USIC_CH_TRBSCR_CSRBI_Pos);
}


// fungsi penerima karakter uart RS232 yang digunakan untuk jalur komunikasi antara komputer ke controller
// fungsi ini biasanya digunakan untuk debug dan memberikan perintah manual ke XMC

void UART_RS232_Handler(void)	//interrupt handler UART232 U0Ch1
{
	static char ch=0;
	static char kal[32];

	ch=UART_RS232->OUTR;

	switch(ch)
	{
		case '1'	: // tekan 1 untuk mengeluarkan pembacaan semua ADC dari controller XMC 1

			sprintf(kal,"\r\r\n- ADC INFINEON 1 -\r\n");
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

			sprintf(kal,"CT1: %d; %f\r\n", CT1_Result, fixed2float(CT1_fix,10));
			UART001_WaitWriteDataMultiple(UART_RS232,kal, strlen(kal));

			sprintf(kal,"CT2: %d; %f\r\n", CT2_Result, fixed2float(CT2_fix,10));
			UART001_WaitWriteDataMultiple(UART_RS232,kal, strlen(kal));

			sprintf(kal,"CT3: %d; %f\r\n", CT3_Result, fixed2float(CT3_fix,10));
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

			sprintf(kal,"CT5: %d; %f\r\n", CT5_Result, fixed2float(CT5_fix,10));
			UART001_WaitWriteDataMultiple(UART_RS232,kal, strlen(kal));

			sprintf(kal,"CT7: %d; %f\r\n", CT7_Result, fixed2float(CT7_fix,10));
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

			sprintf(kal,"DCPT1: %d; %f\r\n", DCPT1_AIN_Result, fixed2float(DCPT1_fix,10));
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

			sprintf(kal,"DCPT2: %d; %f\r\n", DCPT2_AIN_Result, fixed2float(DCPT2_fix,10));
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

			sprintf(kal,"ACPT2: %d; %f\r\n", ACPT2_AIN_Result, fixed2float(ACPT2_fix,10));
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

			sprintf(kal,"GRPT2: %d; %f\r\n", GRPT2_AIN_Result, fixed2float(GRPT2_fix,10));
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));


		break;

		case 'Q'	: // Tekan Q untuk menyalakan saklar K1
			sprintf(kal,"DO_K1 ON\r\n");
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
			SET_BIT(PORT1->OUT,DO_K1);// flag switch
			flag_K1 = 1; // close
		break;

		case 'q'	:
			sprintf(kal,"DO_K1 OFF\r\n"); // tekan q untuk mematikan saklar K1
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
			CLR_BIT(PORT1->OUT,DO_K1);
			// flag switch
			flag_K1 = 0; // open
		break;

		case 'W'	:
			sprintf(kal,"DO_K2 ON\r\n"); // Tekan W untuk menyalakan saklar K2 ( Di GTI ini dipakai untuk menyalakan relay antara inverter dan grid jadi hati2 pakainya )
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
			Close_K2();

			//uart_xmc_request(15); // matikan pll
			flag_interrupt = 1;
			//uart_xmc_request(6); //matikan PLL
			flag_MPPT =1;

			// matikan proteksi sementara
			NVIC_DisableIRQ(ERU0_0_IRQn);
			NVIC_DisableIRQ(ERU0_1_IRQn);
			//NVIC_DisableIRQ(ERU0_2_IRQn);
			//NVIC_DisableIRQ(ERU0_3_IRQn);

			// flag switch
			flag_K2 = 1; // close
		break;

		case 'w'	: // Tekan w untuk mematikan saklar K2
			sprintf(kal,"DO_K2 OFF\r\n");
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
			Open_K2();
			// flag switch
			flag_K2=0; // open
			 //un comment ketika akan droop
			uart_xmc_request(4); // matikan droop
			MODE =1 ;
			flag_test == 0;
			//uart_xmc_request(7); // nyalakan PLL

		break;

		case 'E'	:
			sprintf(kal,"DO_HK1 ON\r\n");
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
			SET_BIT(PORT1->OUT,DO_HK1);
			// flag switch
			flag_HK1=0; // open
		break;

		case 'e'	:
			sprintf(kal,"DO_HK1 OFF\r\n");
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
			CLR_BIT(PORT1->OUT,DO_HK1);
			// flag switch
			flag_HK1=1; // close
		break;

		case 'R'	:
			sprintf(kal,"DO_HK2 ON\r\n");
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
			SET_BIT(PORT1->OUT,DO_HK2);
			// flag switch
			flag_HK2=0; // open
		break;

		case 'r'	:
			sprintf(kal,"DO_HK2 OFF\r\n");
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
			CLR_BIT(PORT1->OUT,DO_HK2);
			// flag switch
			flag_HK2=1; // close
		break;

		case 'A'	:
			sprintf(kal,"DOS1 ON\r\n");
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
			SET_BIT(PORT2->OUT,DOS1);
		break;

		case 'a'	:
			//sprintf(kal,"DOS1 OFF\r\n");
			//UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
			//CLR_BIT(PORT2->OUT,DOS1);

			sprintf(kal,"PANIC BUTTON\r\n");
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

			fault_count = 6;
			flag_activation==0;
			flag_deactivation==0;
			GST_LOW();
			restart_control();
			uart_xmc_request(2);
			control_mode =0 ;

		break;

		case 'S'	:

			flag_invalid_K2=0;
			sprintf(kal,"Invalid Off\r\n");
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

		break;

		case 's'	:

			flag_invalid_K2=2;
			sprintf(kal,"Invalid On\r\n");
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

		break;

		case 'D'	:
			sprintf(kal,"BYPASS OS : PWM ON\r\n");
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
			GST_HIGH();
		break;

		case 'd'	:
			sprintf(kal,"BYPASS OS : PWM OFF\r\n");
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
			GST_LOW();
		break;

		case 'F'	:
			sprintf(kal,"BYPASS OS : CLOSED LOOP\r\n");
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
			control_mode= 1;
		break;

		case 'f'	:
			sprintf(kal,"BYPASS OS : OPEN LOOP\r\n");
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
			restart_control();
			control_mode =0;
		break;

		case 'G'	:
			sprintf(kal,"PWM Inverter on\r\n");
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
			uart_xmc_request(3);
		break;

		case 'g'	:
			sprintf(kal,"PWM Inverter off\r\n");
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
			uart_xmc_request(2);
		break;

		case 'x'	:
			sprintf(kal,"PLL off\r\n");
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

			uart_xmc_request(6);
		break;

		case 'X'	:
			sprintf(kal,"PLL on\r\n");
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

			uart_xmc_request(7);
		break;

		case '='	:
			comparepwm1 = comparepwm1+50;
			if(comparepwm1 > 3747)
			{comparepwm1 = 3747;}
			comparepwm2 = comparepwm1+50;
			if(comparepwm2 > 3747)
			{comparepwm2 = 3747;}


			sprintf(kal,"pwm compare = %d\r\n",comparepwm1);
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
		break;

		case '-'	:
			comparepwm1 = comparepwm1-50;
			if(comparepwm1 < 750)
			{comparepwm1 = 750;}
			comparepwm2 = comparepwm2-50;
			if(comparepwm2 < 750)
			{comparepwm2 = 750;}

			sprintf(kal,"pwm compare = %d\r\n",comparepwm1);
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
		break;

		case 'k'	:
			//uart_xmc_request(8);
			Vreff_inv= Vreff_inv +10;
			if (Vreff_inv > 330){
				Vreff_inv = 330;
			}
			//if(Vreff_inv >=310){
			//	flag_invalid_K2=0;
			//}
			//else{
			//	flag_invalid_K2=2;
			//}
			flag_update_send_xmc = 1;
			sprintf(kal,"Vreff inv : %d \r\n",Vreff_inv);
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
		break;

		case 'l'	:
			//uart_xmc_request(9);
			Vreff_inv= Vreff_inv - 10;
			if (Vreff_inv < 10){
				Vreff_inv = 10;
			}
			flag_update_send_xmc =1;
			sprintf(kal,"Vreff inv: %d \r\n",Vreff_inv);
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
		break;

		case '.'	:
			uart_xmc_request(11);

			sprintf(kal,"sinkronisasi on\r\n",comparepwm4);
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
		break;

		case ','	:
			uart_xmc_request(10);

			sprintf(kal,"sinkronisasi off\r\n");
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
		break;

		case '['	:
			Vreff = Vreff+ 10;
			if(Vreff > 400)
			{Vreff = 400;}
			sprintf(kal,"Vreff = %.2f \r\n",Vreff);
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
		break;

		case ']'	:
			Vreff = Vreff -10;
			if(Vreff < 0)
			{Vreff = 0;}
			sprintf(kal,"Vreff = %.2f \r\n",Vreff);
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
		break;

		case 'C'	:

			break;

		case 'c'	:
			Vreff_inv= Vreff_inv +1;
			if (Vreff_inv > 330){
				Vreff_inv = 330;
			}
			flag_update_send_xmc = 1;
			sprintf(kal,"Vreff inv : %d \r\n",Vreff_inv);
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

			break;

		case 'V'	:

			break;

		case 'v'	:
			Vreff_inv= Vreff_inv -1;
			if (Vreff_inv < 10){
				Vreff_inv = 10;
			}
			flag_update_send_xmc = 1;
			sprintf(kal,"Vreff inv : %d \r\n",Vreff_inv);
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

			break;

		case 'B'	:

			break;

		case 'b'	:
			power_inv= power_inv +50;
			if (power_inv > 5000){
				power_inv = 5000;
			}
			flag_update_send_xmc = 1;
			sprintf(kal,"power inv : %d \r\n",power_inv);
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

			break;

		case 'N'	:

			break;

		case 'n'	:
			power_inv= power_inv -50;
						if (power_inv < 0){
							power_inv = 0;
						}
						flag_update_send_xmc = 1;
						sprintf(kal,"power inv : %d \r\n",power_inv);
						UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
			break;

		case 'M'	:
			uart_xmc_request(17);

					sprintf(kal,"pll voltage on\r\n");
					UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
			break;

		case 'm'	:
			uart_xmc_request(16);

					sprintf(kal,"pll voltage off\r\n");
					UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
			break;

		case 'J'	:
			uart_xmc_request(12);

					sprintf(kal,"Teta - 10240\r\n");
					UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
			break;

		case 'j'	:
			uart_xmc_request(13);

					sprintf(kal,"Teta + 10240\r\n");
					UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
			break;

		case 'T'	:
			//flag_MPPT = 1;

			//uart_xmc_request(18);
			MODE = 2;
							sprintf(kal,"MODE MPPT DC DC\r\n");
							UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
		break;

		case 't'	:
			MODE =1 ;

			//uart_xmc_request(19);

							sprintf(kal,"MODE constant voltage DC DC\r\n");
							UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
		break;

		case 'Y'	:

			break;

		case 'y'	:
			sprintf(kal,"P = %f Q = %f \r\n",P_Filt, Q_Filt);
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

			//uart_xmc_request(1);

			break;

		case 'U'	:


			break;

		case 'u'	:


			sprintf(kal,"\r\r\n Data Infineon 2\r\n",Vreff);
								UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

								sprintf(kal,"CT1: %d; %f\r\n", CT1_infineon_2, fixed2float(((CT1_infineon_2-1759)*-70),10));
								UART001_WaitWriteDataMultiple(UART_RS232,kal, strlen(kal));

								sprintf(kal,"CT5: %d; %f\r\n", CT5_infineon_2, fixed2float((( CT5_infineon_2-1617)*78),10));
								UART001_WaitWriteDataMultiple(UART_RS232,kal, strlen(kal));

								sprintf(kal,"CT7: %d; %f\r\n", CT7_infineon_2, fixed2float(((CT7_infineon_2-1752)*72),10));
								UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

								sprintf(kal,"DCPT1: %d; %f\r\n", DCPT1_infineon_2, fixed2float(((DCPT1_infineon_2-2441)*353),10));
								UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

								sprintf(kal,"DCPT2: %d; %f\r\n", DCPT2_infineon_2, fixed2float(((DCPT2_infineon_2-2450)*353),10));
								UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

								sprintf(kal,"ACPT2: %d; %f\r\n", ACPT2_infineon_2, fixed2float(((ACPT2_infineon_2-2149)*326),10));
								UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

								sprintf(kal,"GRPT2: %d; %f\r\n", GRPT2_infineon_2, fixed2float(((GRPT2_infineon_2-2151)*326),10));
								UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));



			uart_xmc_request(1);
			break;

		case 'I'	:
			uart_xmc_request(15); // matikan pll
			break;

		case 'i'	:

			sprintf(kal,"Kirim Data!\r\n");
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
			flag_update_send_rabbit=1;

			break;

		case 'o'	:
			uart_xmc_request(4);
			sprintf(kal,"Droop Off\r\n");
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
		break;

		case 'p'	:

			uart_xmc_request(5);
			sprintf(kal,"Droop ON\r\n");
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
		break;

		case '3'	:
			 sprintf(kal,"Iref MPPT = %f dV = %f dI = %f dP = %f\r\n",fixed2float(Iref,10),fixed2float(dV,10),fixed2float(dI,10),fixed2float(dP,10));
			 UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

			break;

		case '4'	:
			 current_test = current_test + 104858;
			 if (current_test > 10485760){ current_test = 10485760;}
			 sprintf(kal,"Constant Current = %f\r\n",fixed2float(current_test,20));
			 UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

			break;


		case '5'	:
			 current_test = current_test - 104858;
			 if (current_test < 104858){ current_test = 104858;}
			 sprintf(kal,"Constant Current = %f\r\n",fixed2float(current_test,20));
			 UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

			 break;


		case '6'	:
			 flag_test = 0;
			 sprintf(kal,"Constant Current OFF\r\n");
			 UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

			break;


		case '7'	:
			 flag_test = 1;
			 sprintf(kal,"Constant Current ON\r\n");
			 UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

			 break;

		case '8'	:
			uart_xmc_request(19);
			sprintf(kal,"MODE constant voltage Inverter\r\n");
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

		break;

		case '9'	:
			uart_xmc_request(18);
			sprintf(kal,"MODE MPPT Inverter\r\n");
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

		break;


		case '2'	:
			fault_count = 0;
			sprintf(kal,"Fault Count Reset\r\n");
			flag_fault=0;
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
			GST_HIGH();

			NVIC_EnableIRQ(ERU0_0_IRQn);
			NVIC_EnableIRQ(ERU0_1_IRQn);
			//flag_gd1=0;
			//flag_gd2=0;
			//NVIC_EnableIRQ(ERU0_2_IRQn);
			//NVIC_EnableIRQ(ERU0_3_IRQn);
			Open_HK2();
			break;

		default		:
		break;

	}

	//sprintf(kal,"%c",ch);
	//UART001_WaitWriteDataMultiple(UART_U1,kal,strlen(kal));

	//clear ch
	ch=0;

	//Clear UART Receive Interrupt
	SET_BIT(UART_RS232->TRBSCR,USIC_CH_TRBSCR_CSRBI_Pos);
}



void UART_XMC_Handler(void)
	{
	char data;

		data=UART_U1->OUTR;

		string_read_xmc[string_read_xmc_increment]= data;

		if(data == 'T'){
			flag_update_receive_xmc=1;
			string_read_xmc_increment=-1;
		}
		else if(data == 'X'){
			flag_data_receive_xmc =1;
			string_read_xmc_increment=-1;
		}

		string_read_xmc_increment++;

		SET_BIT(UART_U1->TRBSCR,USIC_CH_TRBSCR_CSRBI_Pos);
	}

volatile int32_t preVDCPT1_fix;
volatile int32_t VDCPT1_Filt_fix;
volatile int32_t preVDCPT1_z1_fix;
volatile int32_t VDCPT1_Filt_z1_fix;


volatile int32_t preCT1_fix;
volatile int32_t CT1_Filt_fix;
volatile int32_t preCT1_z1_fix;
volatile int32_t CT1_Filt_z1_fix;

void PWM1A1B_IRQ_Handler(void)	//interrupt handler PWM_1A_1B CCU81.SR3
{
	int comparepwm;

	while(CheckBit(VADC_G0->RESD[DCPT1_AIN_Result_Register],VADC_G_RESD_VF_Pos)==0);
	DCPT1_AIN_Result = VADC_G0->RES[DCPT1_AIN_Result_Register] & VADC_G_RES_RESULT_Msk;
	//DCPT1_AIN_Result =  DCPT1_Filter(DCPT1_AIN_Result);
	DCPT1_fix = ((int32_t)DCPT1_AIN_Result-DCPT1_OFFSET)*DCPT1_GRAD_FIX;


	preVDCPT1_fix = (DCPT1_fix>>10)*1; //Q0,  P before filtering, max val 2k
	VDCPT1_Filt_fix = preVDCPT1_fix*2055 + 2055*preVDCPT1_z1_fix+1020*(VDCPT1_Filt_z1_fix>>10); //b0 = b1 =  0.0019596476; Q20
	preVDCPT1_z1_fix = preVDCPT1_fix;
	VDCPT1_Filt_z1_fix =VDCPT1_Filt_fix;	//after filtering


	//DCPT1 = ((int32_t)DCPT1_AIN_Result-DCPT1_OFFSET)*DCPT1_GRAD;
	data_in[0]=VDCPT1_Filt_fix>>10; //Q10
	//data_uart_rabbit[6] = DCPT1_fix>>10;

	while(CheckBit(VADC_G0->RESD[DCPT2_AIN_Result_Register],VADC_G_RESD_VF_Pos)==0);
	DCPT2_AIN_Result = VADC_G0->RES[DCPT2_AIN_Result_Register] & VADC_G_RES_RESULT_Msk;
	//DCPT2_AIN_Result =  DCPT2_Filter(DCPT2_AIN_Result);
	DCPT2_fix = ((int32_t)DCPT2_AIN_Result-DCPT2_OFFSET)*DCPT2_GRAD_FIX;
	//DCPT2 = ((int32_t)DCPT2_AIN_Result-DCPT2_OFFSET)*DCPT2_GRAD;
	data_in[1]=DCPT2_fix; //Q10
	//data_uart_rabbit[16]=DCPT2_fix>>10;

	while(CheckBit(VADC_G0->RESD[CT1_Result_Register],VADC_G_RESD_VF_Pos)==0);
	CT1_Result = VADC_G0->RES[CT1_Result_Register] & VADC_G_RES_RESULT_Msk;
	//CT1_Result = ACPT2_Filter(CT1_Result);
	CT1_fix = -((int32_t)CT1_Result-CT1_OFF)*CT1_GRAD_FIX;
	//CT1 = ((int32_t)CT1_Result-CT1_OFF)*CT1_GRAD;


	preCT1_fix = (CT1_fix>>4)*1; //Q6,  P before filtering, max val 2k
		CT1_Filt_fix = preCT1_fix*2055 + 2055*preCT1_z1_fix+1020*(CT1_Filt_z1_fix>>10); //b0 = b1 =  0.0019596476; Q26
		preCT1_z1_fix = preCT1_fix;
		CT1_Filt_z1_fix =CT1_Filt_fix;	//after filtering



	data_in[2]=CT1_Filt_fix>>16; // Q10
	//data_uart_rabbit[12]=CT1_fix;

	while(CheckBit(VADC_G0->RESD[CT2_Result_Register],VADC_G_RESD_VF_Pos)==0);
	CT2_Result = VADC_G0->RES[CT2_Result_Register] & VADC_G_RES_RESULT_Msk;
	CT2_fix = ((int32_t)CT2_Result-CT2_OFF)*CT2_GRAD_FIX;
	//CT2 = ((int32_t)CT2_Result-CT2_OFF)*CT2_GRAD;
	data_in[3]=CT2_fix; // Q10
	//data_uart_rabbit[13]=CT2_fix;
	//DAC0_Output(CT3_Result);


	while(CheckBit(VADC_G0->RESD[CT3_Result_Register],VADC_G_RESD_VF_Pos)==0);
	CT3_Result = VADC_G0->RES[CT3_Result_Register] & VADC_G_RES_RESULT_Msk;
	CT3_fix = ((int32_t)CT3_Result-CT3_OFF)*CT3_GRAD_FIX;
	//CT3 = ((int32_t)CT3_Result-CT3_OFF)*CT3_GRAD;
	data_in[4]=CT3_fix; // Q10
	//data_uart_rabbit[14]=CT3_fix;
	//DAC1_Output(DCPT2_AIN_Result);


	//while(CheckBit(VADC_G0->RESD[CT4_Result_Register],VADC_G_RESD_VF_Pos)==0);
	//CT4_Result = VADC_G0->RES[CT4_Result_Register] & VADC_G_RES_RESULT_Msk;
	//CT4_fix = ((int32_t)CT4_Result-CT4_OFF)*CT4_GRAD_FIX;
	//CT4 = ((int32_t)CT4_Result-CT4_OFF)*CT4_GRAD;

	data_in[5]=0;

	//if( MODE == 1){
		Qset=power_inv;
	//}
	//else if ( MODE == 2){
	//	Pset = data_out[2];
	//}


	/* ---------------------- CHECK FAULT -----------------------*/
	//if((RD_REG(PORT0->IN,PORT0_IN_P12_Msk,PORT0_IN_P12_Pos))==1){
	//	flag_fault=3;
	//	flag_gd1 =1;
	//}

	//if((RD_REG(PORT0->IN,PORT0_IN_P2_Msk,PORT0_IN_P2_Pos))==1){
	//	flag_fault=3;
	//	flag_gd2=1;
	//}

	if(fault_count>2) {
		//PWM_Boost_Disable();
		GST_LOW();
		if (flag_fault == 0){
		flag_fault =1;
		uart_xmc_request(2); // Disable Inverter
		//uart_xmc_request(6);
		//if ( flag_gd1 == 1){
		//	char kal[32];
		//	sprintf(kal,"Fault GD1!\r\n");
		//	UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
		//	}
		//if ( flag_gd2 == 1){
		//	char kal[32];
		//	sprintf(kal,"Fault GD2!\r\n");
		//			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
		//			}
		}
		Open_K2();
		Close_HK2();

		flag_K2=0; // open
		flag_HK2=0; // open
		comparepwm1=3749;
		comparepwm2=3749;
		restart_control();
		control_mode =0;
		flag_invalid_K2=2;
		}
	/* ------------------END OF CHECK FAULT ---------------------*/

	if (control_mode == 0 ){
		//PWM_Boost_Disable();
		PWM1A1B_Set_Compare(comparepwm1,comparepwm1);
		//calculate_pwm_currentloop_inv();
		//flag_count_pwm = 1;
		}
	else if (control_mode == 1){
		//calculate_pwm_currentloop1();
		//PWM1A1B_Set_Compare(data_out[0],data_out[0]);
		//comparepwm=calculate_pwm_currentloop1();
		//PWM_Boost_Enable();
		calculate_pwm();

		PWM1A1B_Set_Compare(data_out[0],data_out[0]);
	}

	//float VoAC_90 = 0;
	//static float SOGI_int1,SOGI_int2  = 0;
	//static float prePow_z1;
	//static float Pow_Filt_z1;
	//static float Q_Filt_z1;
	//static float preQ_z1;

//	// ---- SOGI ----- // to generate 90 deg delayed voltage
	//SOGI_int1 = SOGI_int1 + (0.0000625 * fixed2float(ACPT2_fix,10)  * 314.16);
	//VoAC_90 = SOGI_int1 - fixed2float(ACPT2_fix,10)  - SOGI_int2;
	//SOGI_int2 = SOGI_int2 + (0.0000625 * VoAC_90 * 314.16);
	// ------- End SOGI ------- //

	//prePow = fixed2float(ACPT2_fix,10) * fixed2float(CT7_fix,10);; // P before filtering
	//P_Filt = prePow*0.00098078482 + 0.00098078482*prePow_z1 + 0.99803843*Pow_Filt_z1;
	//prePow_z1 = prePow;
	//Pow_Filt_z1 = P_Filt;	//after filtering

	//preQ = VoAC_90 * fixed2float(CT7_fix,10); // Q before filtering
	//Q_Filt = preQ*0.00098078482 + 0.00098078482*preQ_z1 + 0.99803843*Q_Filt_z1;
	//preQ_z1 = preQ;
	//Q_Filt_z1 = Q_Filt;	//after filtering

	if (counter1120 < 1120){
		counter1120++;
		}
	else{
		counter1120=0;
		if(fault_count < 2){
			uart_xmc_request(1);
		}
	}

	//Clear Interrupt
	SET_BIT(PWM_1A_1B->SWR,CCU8_CC8_SWR_RCM1D_Pos);
	SET_BIT(PWM_1A_1B->SWR,CCU8_CC8_SWR_RCM2D_Pos);
}

void PWM2A2B_IRQ_Handler(void)	//interrupt handler PWM_2A_2B CCU81.SR1
{

	int comparepwm;

	if (control_mode == 0 ){

		PWM2A2B_Set_Compare(comparepwm2,comparepwm2);
		}
	else if (control_mode == 1){
		//calculate_pwm_currentloop2();
		//PWM1A1B_Set_Compare(data_out[1],data_out[1]);
		//comparepwm=calculate_pwm_currentloop2();
		PWM2A2B_Set_Compare(data_out[1],data_out[1]);
	}




	//Clear Interrupt
	SET_BIT(PWM_2A_2B->SWR,CCU8_CC8_SWR_RCM1D_Pos);
	SET_BIT(PWM_2A_2B->SWR,CCU8_CC8_SWR_RCM2D_Pos);
}

void PWM3A3B_IRQ_Handler(void)	//interrupt handler PWM_3A_3B CCU81.SR2
{
	while(CheckBit(VADC_G2->RESD[CT5_Result_Register],VADC_G_RESD_VF_Pos)==0);
	CT5_Result = VADC_G2->RES[CT5_Result_Register] & VADC_G_RES_RESULT_Msk;
	CT5_fix = ((int32_t)CT5_Result-CT5_OFF)*CT5_GRAD_FIX; // Q12
	//CT5_fix = ((int32_t)CT5_Result*CT5_GRAD_FIX)-CT5_OFF; // Q12
	//CT5 = ((int32_t)CT5_Result-CT5_OFF)*CT5_GRAD; // Q12
	//data_in[10]=CT5_fix;
	//data_uart_rabbit[1]=CT5_fix>>10; // GTI 2 ngasih ke S

	while(CheckBit(VADC_G2->RESD[CT6_Result_Register],VADC_G_RESD_VF_Pos)==0);
	CT6_Result = VADC_G2->RES[CT6_Result_Register] & VADC_G_RES_RESULT_Msk;
	//CT6_fix = ((int32_t)CT6_Result-CT6_OFF)*CT6_GRAD_FIX; // Q12
	//CT6 = ((int32_t)CT6_Result-CT6_OFF)*CT6_GRAD;

	while(CheckBit(VADC_G1->RESD[CT7_Result_Register],VADC_G_RESD_VF_Pos)==0);
	CT7_Result = VADC_G1->RES[CT7_Result_Register] & VADC_G_RES_RESULT_Msk;
	CT7_fix = ((int32_t)CT7_Result-CT7_OFF)*CT7_GRAD_FIX; // Q12
	//CT7 = ((int32_t)CT7_Result-CT7_OFF)*CT7_GRAD;
	//data_in[8]=CT7_fix;
	//data_uart_rabbit[4] = CT7_fix>>10;


	//while(CheckBit(VADC_G1->RESD[CT8_Result_Register],VADC_G_RESD_VF_Pos)==0);
	//CT8_Result = VADC_G1->RES[CT8_Result_Register] & VADC_G_RES_RESULT_Msk;
	//CT8_fix = ((int32_t)CT8_Result-CT8_OFF)*CT8_GRAD_FIX; // Q12
	//CT8 = ((int32_t)CT8_Result-CT8_OFF)*CT8_GRAD;

	//while(CheckBit(VADC_G2->RESD[ACPT1_AIN_Result_Register],VADC_G_RESD_VF_Pos)==0);
	//ACPT1_AIN_Result = VADC_G2->RES[ACPT1_AIN_Result_Register] & VADC_G_RES_RESULT_Msk;
	//ACPT1_fix = ((int32_t)ACPT1_AIN_Result-ACPT1_OFFSET)*ACPT1_GRAD_FIX; //Q12
	//ACPT1 = ((int32_t)ACPT1_AIN_Result-ACPT1_OFFSET)*ACPT1_GRAD;

	while(CheckBit(VADC_G2->RESD[ACPT2_AIN_Result_Register],VADC_G_RESD_VF_Pos)==0);
	ACPT2_AIN_Result = VADC_G2->RES[ACPT2_AIN_Result_Register] & VADC_G_RES_RESULT_Msk;
	//ACPT2_AIN_Result=ACPT2_Filter(ACPT2_AIN_Result);
	ACPT2_fix = ((int32_t)ACPT2_AIN_Result-ACPT2_OFFSET)*ACPT2_GRAD_FIX; //Q12
	//ACPT2 = ((int32_t)ACPT2_AIN_Result-ACPT2_OFFSET)*ACPT2_GRAD;
	//data_in[9]=ACPT2_fix;
	//data_uart_rabbit[18] = ACPT2_fix>>10;

	//while(CheckBit(VADC_G2->RESD[ACPT3_AIN_Result_Register],VADC_G_RESD_VF_Pos)==0);
	//ACPT3_AIN_Result = VADC_G2->RES[ACPT3_AIN_Result_Register] & VADC_G_RES_RESULT_Msk;
	//ACPT3_fix = ((int32_t)ACPT3_AIN_Result-ACPT3_OFFSET)*ACPT3_GRAD_FIX; //Q12
	//ACPT3 = ((int32_t)ACPT3_AIN_Result-ACPT3_OFFSET)*ACPT3_GRAD;

	//while(CheckBit(VADC_G2->RESD[GRPT1_AIN_Result_Register],VADC_G_RESD_VF_Pos)==0);
	//GRPT1_AIN_Result = VADC_G2->RES[GRPT1_AIN_Result_Register] & VADC_G_RES_RESULT_Msk;
	//GRPT1_fix = ((int32_t)GRPT1_AIN_Result-GRPT1_OFFSET)*GRPT1_GRAD_FIX; //Q12
	//GRPT1 = ((int32_t)GRPT1_AIN_Result-GRPT1_OFFSET)*GRPT1_GRAD;

	while(CheckBit(VADC_G2->RESD[GRPT2_AIN_Result_Register],VADC_G_RESD_VF_Pos)==0);
	GRPT2_AIN_Result = VADC_G2->RES[GRPT2_AIN_Result_Register] & VADC_G_RES_RESULT_Msk;
	GRPT2_fix = ((int32_t)GRPT2_AIN_Result-GRPT2_OFFSET)*GRPT2_GRAD_FIX; //Q12
	//GRPT2 = ((int32_t)GRPT2_AIN_Result-GRPT2_OFFSET)*GRPT2_GRAD;
	//data_uart_rabbit[8] = GRPT2_fix>>10;

	//while(CheckBit(VADC_G2->RESD[GRPT3_AIN_Result_Register],VADC_G_RESD_VF_Pos)==0);
	//GRPT3_AIN_Result = VADC_G2->RES[GRPT3_AIN_Result_Register] & VADC_G_RES_RESULT_Msk;
	//GRPT3_fix = ((int32_t)GRPT3_AIN_Result-GRPT3_OFFSET)*GRPT3_GRAD_FIX; //Q12
	//GRPT3 = ((int32_t)GRPT3_AIN_Result-GRPT3_OFFSET)*GRPT3_GRAD;

	PWM3A3B_Set_Compare(3749,3749);


	//data_uart_rabbit[10]=(((int32_t)TQ1_AIN_Result-TQ1_OFF)/TQ1_GRAD)*100;

	//data_uart_rabbit[11]=(((int32_t)TQ2_AIN_Result-TQ2_OFF)/TQ2_GRAD)*100;



	//Clear Interrupt
	SET_BIT(PWM_3A_3B->SWR,CCU8_CC8_SWR_RCM1D_Pos);
	SET_BIT(PWM_3A_3B->SWR,CCU8_CC8_SWR_RCM2D_Pos);
}
