/*
 * Main.c
 *
 *  Created on: Mar 19, 2014 - 2017
 *      Author: Angga, Bima, Iqbal, Mukhlis, Arif
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
#include "peripheral/CAN_app.h"
#include "peripheral/ISR_app.h"

#include "external/EEP25LC1024_app.h"
#include "external/display_app.h"
#include "external/AD5328_app.h"
#include "external/MCP79410_app.h"
#include "external/button_app.h"
#include "external/relay_app.h"

#include "algorithm/converter.h"

#define ON 1
#define OFF 0


// ini untuk FSM
int flag_prep;
int finish_prep;
int flag_timer;
int flag_timer_nobuzz;

int flag_timer_fault;
int flag_menu;
int flag_button;
int timeout;
uint8_t minute_start;
uint8_t second_start;
uint8_t minute_start_state;
uint8_t second_start_state;
uint8_t minute_start_fault;
uint8_t second_start_fault;
uint8_t minute;
uint8_t second;
uint8_t hour;
uint8_t minute_date;
uint8_t second_date;
uint8_t hour_date;
char kal1[15];
char kal2[15];
char kal1_old[15];
char kal2_old[15];
int state_fsm;
int prev_state_fsm;
char buf[15];

// Ini untuk Menu
int flag_change = 0;
int max_PWM = 3745; // ini minimum
int min_PWM = 745; // ini maximum
int max_Index_Modulasi = 3375; // ini minimum
int min_Index_Modulasi = 0; // ini maximum
int max_QREF = 5000; // ini minimum
int min_QREF = 0; // ini maximum
int max_PREF = 5000; // ini minimum
int min_PREF = 500; // ini maximum
int max_ADC = 300;
int min_ADC = 0;
int max_menu_standby = 3;
int min_menu_standby = 2;
int max_menu_operation = 3;
int min_menu_operation = 2;
volatile int temp;
int change = 0;
int state;
int previous_state;
volatile int state_menu;


// untuk tombol
char button[4];
char flag_push[4];

// untuk membuat referensi voltage controller ( debug only )
uint8_t minute_start_reff;
uint8_t second_start_reff;
uint8_t minute_reff;
uint8_t second_reff;
int flag_reff=0;
int timeout_reff=0;
int state_reff=0;

// untuk cek adc
int state_adc;

// untuk otomasi
volatile int time_activate_deactivate = -1;
volatile int time_finish_voltage_dc=0;
volatile int time_finish_voltage_ac=0;
volatile int voltage_reference =0;
volatile int flag_increase_voltage_dc=0;
volatile int flag_increase_voltage_ac=0;
volatile int time_finish_power=0;
volatile int power_reference =0;
volatile int flag_increase_power=0;
volatile int flag_close_rabbit =0;
volatile int count_check = 0;
volatile int check_ac_voltage = 0;
volatile int flag_droop = 0;
volatile int voac_peak=0;
volatile int fault_frekuensi =0;

// untuk fault
volatile int32_t timeout_fault=0;

// untuk otomasi
volatile int32_t timeout_state=1;


// untuk mppt
volatile int32_t timeout_mppt=0;

// untuk close rabbit
volatile int32_t timeout_close_rabbit = 0;

// untuk close rabbit
volatile int32_t timeout_open_rabbit = 0;

// untuk reset tanggal
volatile int tanggal;
volatile int bulan;
volatile int tahun;
volatile int jam;
volatile int menit;
volatile int detik;
volatile int temp_date;
volatile int flag_reset_time;

//untuk cek frekuensi
volatile int32_t frekuensi_grid = 0;

volatile int flag_fault_voltage_over = 0;
volatile int flag_fault_voltage_under = 0;



// Deklarasi fungsi
void FSM (void);
void preparation (void);
void read_button (void);
void print_lcd (void);
void menu_standby(void);
void menu_operation(void);
void update_data(void);
void testing_token(void);
void cek_adc(void);
void reff_signal(void);
void timer_state_nobuzz(int count_num);
void timer_state_fault(int count_num);


int main(void)
{
	DAVE_Init();			// Initialization of DAVE Apps
	My_DAVE_MUX_Init();

	IO_Init();
	GST_HIGH();
	UART_Init();

	ADC_Init();

	ERU_Init();

	Display_Init();

	DAC_Init();

	AD5328_Init();

	//tegangan proteksi naik karena pengubahan offset sensor
	AD5328_Send_Data(ACPT_REF,3700);//harusnya 3480, ini khusus gti 3 //2.4793); // V AC before contactor, 400 V
	AD5328_Send_Data(GRPT_REF,3700);// ini tes 323V //3700);//harusnya 3480, ini khusus gti 3 //2.474); // V AC after contactor, 400 V

	//tegangan proteksi turun karena pengubahan offset sensor
	AD5328_Send_Data(DCPT1_REF,3920);//1900); // set 390v

	//tegangan proteksi turun karena pengubahan offset sensor
	AD5328_Send_Data(DCPT2_REF,3920); //harusnya 3920, ini khusus gti 3// set 450v

	AD5328_Send_Data(CT1_REF,2180); // MAX(CT1) 20A
	AD5328_Send_Data(CT234_REF,2300); //Max(CT234) 20A
	AD5328_Send_Data(CT56_REF, 2300);//2300); // Max(CT56) 40A // test 17A
	AD5328_Send_Data(CT78_REF,2340); // Max (CT78) 40A


	PWM_Enable_Interrupt_NVIC();

	comparepwm4= 100;
	comparepwm5= 0;

	flag_interrupt = 0;

	while(1)
	{

		FSM();
		print_lcd();
		update_data();
		if (flag_interrupt == 1){
			timeout_fault++;
			if (timeout_fault>=6){
				timeout_fault=0;
				flag_interrupt =0;

				// NVIC itu untuk mengatur proteksi via interrupt, bisa di enable dan disable, cek .c nya untuk lihat ERU mana yang melakukan proteksi apa
				NVIC_EnableIRQ(ERU0_0_IRQn);
				NVIC_EnableIRQ(ERU0_1_IRQn);
				//NVIC_EnableIRQ(ERU0_2_IRQn);
				//NVIC_EnableIRQ(ERU0_3_IRQn);
				//uart_xmc_request(5); // untuk nyalain droop ( sekarang dah gak dipake disini soalnya dipakenya di yang mppt )
			}
		}
		if (flag_MPPT == 1){
			timeout_mppt++;
			if (timeout_mppt>=8){
				timeout_mppt=0;
				flag_MPPT =0;
				//NVIC_EnableIRQ(ERU0_0_IRQn);
				//NVIC_EnableIRQ(ERU0_1_IRQn);
				//NVIC_EnableIRQ(ERU0_2_IRQn);
				//NVIC_EnableIRQ(ERU0_3_IRQn);

				uart_xmc_request(5);
			}

		}

		if (flag_fault_voltage_over == 1){
			timer_state_fault(1);
			if (timeout_fault == 1){
				timeout_fault=0;
				flag_fault_voltage_over = 0;
				flag_timer_fault = 0;
				fault_count = 6;
				kode_fault = 2;
			}
		}
		if (flag_fault_voltage_under == 1){
			timer_state_fault(1);
			if (timeout_fault == 1){
				timeout_fault=0;
				flag_fault_voltage_under = 0;
				flag_timer_fault = 0;
				fault_count = 6;
				kode_fault = 4;
			}
		}

		/*
		if (flag_close_rabbit == 1){
					timeout_close_rabbit++;
					if (timeout_close_rabbit>=100){
						timeout_close_rabbit=0;
						Open_K2();
						flag_K2=0; // open
						uart_xmc_request(4); // matikan droop
						flag_close_rabbit = 0;
					}

			}
		*/

		if ( flag_droop == 1){
					timeout_open_rabbit++;
					if (timeout_open_rabbit >=100){
						timeout_open_rabbit=0;
						MODE = 2;
						flag_droop = 0;
					}

			}
		if ( fault_count > 2 && (state_fsm != 7 && state_fsm != 6) ){
					state_fsm = 7;

		}

		//RTC_get_time(&hour_date,&minute_date,&second_date);
			//	if ( hour_date == 4){
				//	flag_reset_time = 0;
				//}

	}

	return 0;
}

void buzzer_50ms(void){
	SET_BIT(PORT1->OUT,RESET_LCD);
	My_Delay_ms(50);
	CLR_BIT(PORT1->OUT,RESET_LCD); //BUZZER P1.14
}

void timer_state(int count_num){
	int count;
	if (flag_timer==0){
		RTC_get_time(&hour,&minute_start,&second_start);
		flag_timer=1;
	}
		RTC_get_time(&hour,&minute,&second);
		count = (minute*60+second) - (minute_start*60+second_start);
		if (count >= count_num ){
			timeout=1;
			buzzer_50ms();
			My_Delay_ms(40);
			buzzer_50ms();
		}
	}

void timer_state_nobuzz(int count_num){
	int count;
	if (flag_timer_nobuzz==0){
		RTC_get_time(&hour,&minute_start_state,&second_start_state);
		flag_timer_nobuzz=1;
	}
		RTC_get_time(&hour,&minute,&second);
		count = (minute*60+second) - (minute_start_state*60+second_start_state);
		if (count >= count_num ){
			timeout_state=1;
		}
	}

void timer_state_fault(int count_num){
	int count;
	if (flag_timer_fault==0){
		RTC_get_time(&hour,&minute_start_fault,&second_start_fault);
		flag_timer_fault=1;
	}
		RTC_get_time(&hour,&minute,&second);
		count = (minute*60+second) - (minute_start_fault*60+second_start_fault);
		if (count >= count_num ){
			timeout_fault=1;
		}
	}

void FSM (void){
	char time[15];
		Button_Read(button,flag_push);
		// check state machine whenever posible moderate priority

			if(state_fsm == 0){
				strcat(kal1,"Preparation");
				lcd_puts(kal1);
				if(flag_prep == 0 && finish_prep == 0){
				preparation();
				flag_prep = 1;
				}
				if (finish_prep == 1){
				flag_prep = 0;
				state_fsm = 1;
				}
			}
			else if ( state_fsm == 1){
				//lcd_clear();
				RTC_get_time_str(time);
				sprintf(kal1,time);
				kal1[0]=0b00011000;
				kal1[15]=0b00011000;
				// buat ngetes fault

				if ( (voac_peak < 294 && voac_peak > 277)){ //277)){
					sprintf(kal2,"4 ");
				}


				if ( (voac_peak <= 277)){ //277)){
					sprintf(kal2,"4+");
					}

				if ( (voac_peak > 326 && voac_peak < 339)){ //277)){
					sprintf(kal2,"2 ");
					}

				if ( (voac_peak >= 339)){
					sprintf(kal2,"2+");
					}


				if(frekuensi_grid > 317140) { //342422978) { ini cobain pake 51 // 51*6.28*1000
					sprintf(kal2,"8 ");
					}

				if(frekuensi_grid >= 320280) { //342422978) { ini cobain pake 51 // 51*6.28*1000
					sprintf(kal2,"6 ");
					}

				if (frekuensi_grid < 301597){//309497692) { ini cobain pake 48.025 // 48.025*6.28*1000
					sprintf(kal2,"7 ");
					}

				strcat(kal2," Standby");

				if (DCPT2_fix > float2fixed(5,10)){

					sprintf(buf," %4.0fV",fixed2float(DCPT2_fix,10));
					strcat(kal2,buf);
				}
				// minta data frekuensi dan peak tegangan terbaru
				if ((DCPT2_fix > float2fixed(330,10)) && (frekuensi_grid < 317140 ) && (voac_peak > 294) && (voac_peak < 328)){ // ngetes VOAC_PEAK pake 292 dulu, harusnya 277
					flag_activation=1;
					state_fsm = 99;
					sprintf(kal2,"");
				}

				if ( button[0] == '0' && button[1] == '0'){
					flag_button=0;
				}
				if ( button[2] == '1'){
					state_fsm = 2;
					flag_push[2]='1';
				}
				if ( button[0] == '1' && button[1] == '1' && flag_button == 0){
					prev_state_fsm = 1;
					state_fsm = 3;
				}


				// akhir ngetes fault
				/*
				timer_state_nobuzz(1);
				if ( timeout_state == 1){
					uart_xmc_request(1);
					timeout_state=0;
					flag_timer_nobuzz=0;
				}
				*/
			}
			else if ( state_fsm == 2){
				//lcd_clear();
				menu_standby();
				timer_state(30);
				flag_invalid_K2=2;
				if ( button[0] == '1' || button[1] == '1' || button[2] == '1'){
					flag_timer = 0;
				}
				if ( button[3] == '1' || timeout == 1){
					state_fsm = 1;
					flag_timer =0;
					flag_menu = 0;
					timeout =0;
					sprintf(kal2,"");
				}
				/*
				timer_state_nobuzz(1);
				if ( timeout_state == 1){
					uart_xmc_request(1);
					timeout_state=0;
					flag_timer_nobuzz=0;
					}
					*/
			}
			else if ( state_fsm == 3){
				//lcd_clear();
				sprintf(kal1,"Hold 2 Sec");

				sprintf(kal2,"");
				timer_state(2);
				if ( button[0] == '0' || button[1] == '0'){
					state_fsm = prev_state_fsm;
					flag_timer = 0;
					sprintf(kal2,"");
				}
				if (timeout == 1){
					if ( prev_state_fsm == 1){

					/*
					state_fsm = 4;
					Open_HK2();
					flag_HK2 = 1;
					Open_HK1();
					flag_HK1 = 1;
					PWM_Boost_Start();
					GST_HIGH();
					*/


						flag_activation=1;
						state_fsm = 99;


					}
					else if ( prev_state_fsm == 4){

					/*

					state_fsm=1;
					Close_HK2();
					flag_HK2 = 0;
					Close_HK1();
					flag_HK1 = 0;
					GST_LOW();
					restart_control();
					uart_xmc_request(2);
					*/


					flag_deactivation=1;
					state_fsm = 99;

					}

					flag_timer = 0;
					timeout=0;
					flag_button=1;
					sprintf(kal2,"");
					}
			}
			else if ( state_fsm == 4){
				//lcd_clear();
				RTC_get_time_str(time);
				sprintf(kal1,time);
				kal1[0]= 0b11111100;
				kal1[15]=0b11111100;

				sprintf(kal2,"P = %+4.0f Watt",fixed2float(temp_pv_power,10));


				if ( (voac_peak < 294 && voac_peak > 277)){ //277)){
					flag_fault_voltage_under =1;
					}
				else{
					timeout_fault=0;
					flag_timer_fault=0;
					flag_fault_voltage_under=0;
					}


				if ( (voac_peak <= 277)){ //277)){
					fault_count = 6;
					kode_fault = 4;
					}

				if ( (voac_peak > 326 && voac_peak < 339)){ //277)){
					flag_fault_voltage_over =1;
					}
				else{
					timeout_fault=0;
					flag_timer_fault=0;
					flag_fault_voltage_over =0;
					}

				if ( (voac_peak >= 339)){
					fault_count = 6;
					kode_fault = 2;
					}


				if(frekuensi_grid >= 320280) { //342422978) { ini cobain pake 51 // 51*6.28*1000
					fault_count = 6;
					kode_fault = 6; // kode over frekuensi
					}
				if (frekuensi_grid < 301597){//309497692) { ini cobain pake 48.025 // 48.025*6.28*1000
					fault_count =6;
					kode_fault = 7; // kode underfrekuensi
					}

				if ( button[0] == '0' && button[1] == '0'){
					flag_button=0;
				}
				if ( button[2] == '1'){
					state_fsm = 5;
				}
				if ( button[0] == '1' && button[1] == '1' && flag_button == 0){
					prev_state_fsm=4;
					state_fsm = 3;
				}
				/*
				timer_state_nobuzz(1);
				if ( timeout_state == 1){
					uart_xmc_request(1);
					timeout_state=0;
					flag_timer_nobuzz=0;
				}
				*/
			}
			else if ( state_fsm == 5){
				//lcd_clear();
				menu_operation();
				timer_state(99);


				if ( (voac_peak < 294 && voac_peak > 277)){ //277)){
					flag_fault_voltage_under =1;
					}
				else{
					timeout_fault=0;
					flag_timer_fault=0;
					flag_fault_voltage_under=0;
					}


				if ( (voac_peak <= 277)){ //277)){
					fault_count = 6;
					kode_fault = 4;
					}

				if ( (voac_peak > 326 && voac_peak < 339)){ //277)){
					flag_fault_voltage_over =1;
					}
				else{
					timeout_fault=0;
					flag_timer_fault=0;
					flag_fault_voltage_over =0;
					}

				if ( (voac_peak >= 339)){
					fault_count = 6;
					kode_fault = 2;
					}

				if(frekuensi_grid >= 320280) { //342422978) { ini cobain pake 51 // 51*6.28*1000
					fault_count = 6;
					kode_fault = 6; // kode over frekuensi
					}
				if (frekuensi_grid < 301597){//309497692) { ini cobain pake 48.025 // 48.025*6.28*1000
					fault_count =6;
					kode_fault = 7; // kode underfrekuensi
					}

				if ( button[0] == '1' || button[1] == '1' || button[2] == '1'){
					flag_timer = 0;
				}
				if ( button[3] == '1' || timeout == 1){
					state_fsm = 4;
					flag_timer =0;
					flag_menu = 0;
					timeout=0;
					sprintf(kal2,"");
				}


				/*
				timer_state_nobuzz(1);
				if ( timeout_state == 1){
					uart_xmc_request(1);
					timeout_state=0;
					flag_timer_nobuzz=0;
				}
*/

			}

			else if ( state_fsm == 6){
					//lcd_clear();
					sprintf(kal1,"Hold 3 Sec");
					timer_state(3);
					if ( button[0] == '0' || button[1] == '0'){
						state_fsm = prev_state_fsm;
						flag_timer = 0;
						sprintf(kal2,"");
					}
					if (timeout == 1){
						CLR_BIT(PORT2->OUT,LED1_BUZZ);
						SET_BIT(PORT2->OUT,LED2);
						CLR_BIT(PORT1->OUT,RESET_LCD);
						fault_count = 0;
						flag_fault=0;
						NVIC_EnableIRQ(ERU0_0_IRQn);
						NVIC_EnableIRQ(ERU0_1_IRQn);
						//NVIC_EnableIRQ(ERU0_2_IRQn);
						//NVIC_EnableIRQ(ERU0_3_IRQn);
						state_fsm=1;

						flag_timer = 0;
						timeout=0;
						flag_button=1;
						sprintf(kal2,"");
						uart_xmc_request(7);// nyalain pll lagi

					}
						}
			else if ( state_fsm == 7){

				SET_BIT(PORT2->OUT,LED1_BUZZ);
				CLR_BIT(PORT2->OUT,LED2);
				SET_BIT(PORT1->OUT,RESET_LCD);

				if (kode_fault == 0){
					sprintf(kal1,"DC OVER CURR");
					sprintf(kal2,"PRESS TO RESTART");
				}
				else if ( kode_fault == 1){
					sprintf(kal1,"DC OVER VOLT");
					sprintf(kal2,"PRESS TO RESTART");
				}
				else if ( kode_fault == 2){
					sprintf(kal1,"GR OVER VOLT");
					//sprintf(kal2,"WAIT TO RESTART");
					//timer_state_fault(10);
					sprintf(kal2,"PRESS TO RESTART");

				}
				else if ( kode_fault == 3){
					sprintf(kal1,"FAULT GD");
					sprintf(kal2,"PRESS TO RESTART");
				}
				else if ( kode_fault == 4){
					sprintf(kal1,"GR UNDER VOLT");
					//sprintf(kal2,"WAIT TO RESTART");
					//timer_state_fault(10);
					sprintf(kal2,"PRESS TO RESTART");

				}
				else if ( kode_fault == 5){
					sprintf(kal1,"PANIC MODE");
					sprintf(kal2,"PRESS TO RESTART");
				}
				else if ( kode_fault == 6){
					sprintf(kal1,"OVER FREQ");
					//sprintf(kal2,"WAIT TO RESTART");
					//timer_state_fault(10);
					sprintf(kal2,"PRESS TO RESTART");

				}
				else if ( kode_fault == 7){
					sprintf(kal1,"UNDER FREQ");
					//sprintf(kal2,"WAIT TO RESTART");
					//timer_state_fault(10);
					sprintf(kal2,"PRESS TO RESTART");

				}

				/*
				if (timeout_fault == 1){
					CLR_BIT(PORT2->OUT,LED1_BUZZ);
					SET_BIT(PORT2->OUT,LED2);
					CLR_BIT(PORT1->OUT,RESET_LCD);
					fault_count = 0;
					flag_fault=0;
					NVIC_EnableIRQ(ERU0_0_IRQn);
					NVIC_EnableIRQ(ERU0_1_IRQn);
					state_fsm=1;
					flag_timer_fault = 0;
					timeout_fault=0;
					sprintf(kal2,"");
					uart_xmc_request(7);// nyalain pll lagi
				}

				*/

				GST_LOW();
				if (flag_fault == 0){
						Vreff_inv = 10;
						flag_fault =1;
						uart_xmc_request(2); // Disable Inverter
						flag_update_send_xmc = 1;
						}
				flag_invalid_K2=2;
				flag_deactivation =0 ;
				flag_activation = 0;

				if ( button[0] == '0' && button[1] == '0'){
					flag_button=0;
				}
				if ( button[0] == '1' && button[1] == '1' && flag_button == 0){
					prev_state_fsm=7;
					state_fsm = 6;
				}
			}




				if (flag_activation==1 && flag_deactivation == 0 ){
					timer_state_nobuzz(1);
					if ( button[2] == '1' && button[3] == '1'){
						fault_count=6;
						kode_fault =5;
						}
					if (time_activate_deactivate == -1){
						time_activate_deactivate = 120;
						time_finish_voltage_dc=0;
						time_finish_voltage_ac=0;
						time_finish_power=0;
					}
					else if(time_activate_deactivate >= 116 && timeout_state ==1){
						sprintf(kal1,"Activate in %d S",(time_activate_deactivate-115));
						PWM_Boost_Start();
					}
					else if (time_activate_deactivate == 115 && timeout_state ==1){
						sprintf(kal1,"Open HK1 & HK2");
						Open_HK2();
						flag_HK2 = 1;
						Open_HK1();
						flag_HK1 = 1;
					}
					else if (time_activate_deactivate == 110 && timeout_state ==1){
						sprintf(kal1,"Start PWM DC-DC");
						GST_HIGH();
						Vreff = fixed2float(DCPT2_fix,10)+20;
						voltage_reference = 380; // kecil dulu untuk debug
						control_mode =1;
						flag_increase_voltage_dc =1;
				    }
					else if(flag_increase_voltage_dc == 1 && timeout_state == 1){
						Vreff = Vreff + 10;
						if (Vreff >= voltage_reference){
							Vreff = voltage_reference;
							flag_increase_voltage_dc=0;
							time_finish_voltage_dc = time_activate_deactivate;
							voltage_reference = 0;
						}
						sprintf(kal2,"Output %.0f V",Vreff);

					}
					else if (time_activate_deactivate == (time_finish_voltage_dc-3) && timeout_state ==1){
						if( DCPT2_fix < float2fixed(360,10)){
							fault_count = 6;
							kode_fault =3;
							}
						else{
							sprintf(kal2,"Finished!");
						}
					}
					else if (time_activate_deactivate == (time_finish_voltage_dc-5) && timeout_state ==1){
						sprintf(kal1,"Start PWM DC-AC");
						uart_xmc_request(3);
						Vreff_inv = 10;
						voltage_reference = 310; // kecil dulu untuk debug

					}
					else if (time_activate_deactivate == (time_finish_voltage_dc-7) && timeout_state ==1){
						sprintf(kal2,"Activate PLL");
						uart_xmc_request(7);
						}
					else if (time_activate_deactivate == (time_finish_voltage_dc-10) && timeout_state ==1){
						sprintf(kal2,"Synchronized!");
						uart_xmc_request(11);
						flag_increase_voltage_ac =1;
						}
					else if(flag_increase_voltage_ac == 1 && timeout_state == 1){
						Vreff_inv = Vreff_inv + 10;

						if (Vreff_inv > voltage_reference){
							Vreff_inv = voltage_reference;
							flag_increase_voltage_ac=0;
							time_finish_voltage_ac = time_activate_deactivate;

							voltage_reference = 0;
						}
						sprintf(kal2,"Output %d Vp",Vreff_inv);
						flag_update_send_xmc = 1;

					}
					else if (time_activate_deactivate == (time_finish_voltage_ac-3) && timeout_state ==1){
						for ( count_check =0 ; count_check < 3200 ; count_check ++){
								if ( ACPT2_fix >= float2fixed(200,10) || ACPT2_fix <= float2fixed(-200,10)){
									count_check = 3200;
									check_ac_voltage = 1;
								}
							}

						if (check_ac_voltage == 0){
							fault_count = 6;
							kode_fault =3;
							}
						else{
							sprintf(kal2,"Finished!");
							check_ac_voltage = 0;
							flag_invalid_K2=0;
							}
					}
					else if (time_activate_deactivate == (time_finish_voltage_ac-6) && timeout_state ==1){
						sprintf(kal1,"Connect to Grid");
						sprintf(kal2,"Droop Activated!");

						Close_K2(); // ini buat debug pll close k2 dimatiin dulu
						flag_interrupt = 1;
						flag_MPPT =1;

						// matikan proteksi sementara
						NVIC_DisableIRQ(ERU0_0_IRQn);
						NVIC_DisableIRQ(ERU0_1_IRQn);

						//NVIC_DisableIRQ(ERU0_2_IRQn);
						//NVIC_DisableIRQ(ERU0_3_IRQn);

						// flag switch
						flag_K2 = 1; // close

						}
					else if (time_activate_deactivate == (time_finish_voltage_ac-10) && timeout_state ==1){
						sprintf(kal1,"MPPT MODE");
						sprintf(kal2,"Activated!");
						MODE =2 ;
					}
					else if (time_activate_deactivate == (time_finish_voltage_ac-12) && timeout_state ==1){
						flag_increase_power=1;
						power_reference =float2fixed(Pset,10);
						Plimit = float2fixed(500,10);
						sprintf(kal1,"Power Limit");
						sprintf(kal2,"Output %.0f W",fixed2float(Plimit,10));

					}
					else if( flag_increase_power == 1 && timeout_state == 1){
						Plimit = Plimit + float2fixed(200,10);
						if (Plimit >= power_reference){
						Plimit = power_reference;
						flag_increase_power = 0;
						time_finish_power = time_activate_deactivate;
						power_reference= float2fixed(500,10);
						}
						sprintf(kal2,"Output %.0f W",fixed2float(Plimit,10));
					}
					else if (time_activate_deactivate == (time_finish_power-2) && timeout_state ==1){
						flag_activation=0;
						time_activate_deactivate = 0;

						state_fsm = 4;
					}

					if (timeout_state==1){
						if (fault_count <= 2){
							fault_count = 0;
						}
						timeout_state=0;
						time_activate_deactivate -= 1;
						flag_timer_nobuzz=0;
						if(frekuensi_grid >= 320280) { //342422978) { ini cobain pake 51 // 51*6.28*1000
							fault_count = 6;
							kode_fault = 6; // kode over frekuensi
							}
						if (frekuensi_grid < 301597){//309497692) { ini cobain pake 48.025 // 48.1*6.28*1000
							fault_count =6;
							kode_fault = 7; // kode underfrekuensi
							}


						if ( (voac_peak < 294 && voac_peak > 277)){ //277)){
							flag_fault_voltage_under =1;
							}
						else{
							timeout_fault=0;
							flag_timer_fault=0;
							flag_fault_voltage_under=0;
							}


						if ( (voac_peak <= 277)){ //277)){
							fault_count = 6;
							kode_fault = 4;
							}

						if ( (voac_peak > 326 && voac_peak < 339)){ //277)){
							flag_fault_voltage_over =1;
							}
						else{
							timeout_fault=0;
							flag_timer_fault=0;
							flag_fault_voltage_over =0;
							}

						if ( (voac_peak >= 339)){
							fault_count = 6;
							kode_fault = 2;
							}
						//uart_xmc_request(1);


					}

				}
				else if (flag_deactivation==1 && flag_activation == 0){
					timer_state_nobuzz(1);
					if ( button[2] == '1' && button[3] == '1'){
						fault_count=6;
						kode_fault =5;
						}
					if (time_activate_deactivate == -1){
						time_activate_deactivate = 120;
						timeout_state ==1;
						time_finish_voltage_dc=0;
						time_finish_voltage_ac=0;
						time_finish_power=0;
					}
					else if (time_activate_deactivate == 119 && timeout_state ==1){
						sprintf(kal1,"Power Limit");
						sprintf(kal2,"Output %.0f W",fixed2float(Plimit,10));
						flag_increase_power=1;
						power_reference =float2fixed(500,10);
						}
					else if( flag_increase_power == 1 && timeout_state == 1){
						Plimit = Plimit - float2fixed(200,10);
						if (Plimit <= power_reference){
							Plimit = power_reference;
							flag_increase_power = 0;
							time_finish_power = time_activate_deactivate;
							power_reference= float2fixed(500,10);
						}
						sprintf(kal2,"Output %.0f W",fixed2float(Plimit,10));
					}
					else if(time_activate_deactivate >= time_finish_power-3 && timeout_state ==1){
						sprintf(kal1,"Deactivate MPPT");
						sprintf(kal2,"");
						MODE = 1;
						flag_invalid_K2=2;
						flag_update_send_xmc = 1;

					}
					else if(time_activate_deactivate == time_finish_power-8 && timeout_state ==1){
						sprintf(kal1,"Deactivate GTI");
						sprintf(kal2,"Disconect Grid");
						Open_K2();
						// flag switch
						flag_K2=0; // open

						flag_interrupt = 1;


						// matikan proteksi sementara
						NVIC_DisableIRQ(ERU0_0_IRQn);
						NVIC_DisableIRQ(ERU0_1_IRQn);
						//NVIC_DisableIRQ(ERU0_2_IRQn);
						//NVIC_DisableIRQ(ERU0_3_IRQn);
						//un comment ketika akan droop

						uart_xmc_request(4); // matikan droop
						flag_test == 0;
					}
					else if (time_activate_deactivate == time_finish_power-13 && timeout_state ==1){
						sprintf(kal1,"Stop PWM DC-AC");
						sprintf(kal2,"synchron off");
						uart_xmc_request(10);
					}
					else if (time_activate_deactivate == time_finish_power -16 && timeout_state ==1){
						sprintf(kal2,"PLL Off");
						uart_xmc_request(6);
						voltage_reference = 10; // tegangan untuk nanti dimatikan
						flag_increase_voltage_ac=1;
					}
					else if(flag_increase_voltage_ac == 1 && timeout_state == 1){
						Vreff_inv = Vreff_inv - 10;
						if (Vreff_inv <= voltage_reference){
							Vreff_inv = voltage_reference;
							flag_increase_voltage_ac=0;
							time_finish_voltage_ac = time_activate_deactivate;
							voltage_reference = 0;
						}
						sprintf(kal2,"Output %d Vp",Vreff_inv);
						flag_update_send_xmc = 1;
					}
					else if (time_activate_deactivate == (time_finish_voltage_ac-5) && timeout_state ==1){
						uart_xmc_request(2);
					}

					else if (time_activate_deactivate == (time_finish_voltage_ac-10) && timeout_state ==1){
						sprintf(kal1,"STOP PWM DC-DC");
						sprintf(kal2,"");
						restart_control();
						control_mode =0;

					}
					else if (time_activate_deactivate == (time_finish_voltage_ac-10) && timeout_state ==1){
						GST_LOW();

					}
					else if (time_activate_deactivate == (time_finish_voltage_ac-12) && timeout_state ==1){
						sprintf(kal1,"Close HK2 & HK1");
						sprintf(kal2,"");
						Close_HK2();
						flag_HK2 = 0;
						Close_HK1();
						flag_HK1 = 0;
						}
					else if (time_activate_deactivate == (time_finish_voltage_ac-15) && timeout_state ==1){
						flag_deactivation=0;
						time_activate_deactivate =0;
						state_fsm = 1;

						}
					if (timeout_state==1){
						if (fault_count <= 2){
							fault_count = 0;
						}
						timeout_state=0;
						time_activate_deactivate -= 1;
						flag_timer_nobuzz=0;
						if(frekuensi_grid >= 320280) { //342422978) { ini cobain pake 51 // 51*6.28*1000
							fault_count = 6;
							kode_fault = 6; // kode over frekuensi
							}
						if (frekuensi_grid < 301597){//309497692) { ini cobain pake 48.025 // 48.1*6.28*1000
							fault_count =6;
							kode_fault = 7; // kode underfrekuensi
							}


						if ( (voac_peak < 294 && voac_peak > 277)){ //277)){
							flag_fault_voltage_under =1;
							}
						else{
							timeout_fault=0;
							flag_timer_fault=0;
							flag_fault_voltage_under=0;
							}


						if ( (voac_peak <= 277)){ //277)){
							fault_count = 6;
							kode_fault = 4;
							}

						if ( (voac_peak > 326 && voac_peak < 339)){ //277)){
							flag_fault_voltage_over =1;
							}
						else{
							timeout_fault=0;
							flag_timer_fault=0;
							flag_fault_voltage_over =0;
							}

						if ( (voac_peak >= 339)){ //277)){
							fault_count = 6;
							kode_fault = 2;
							}
						//uart_xmc_request(1);
					}
				}
				else {
					timeout_state=0;
					time_activate_deactivate = -1;
					time_finish_voltage_dc=0;
					time_finish_voltage_ac=0;
					time_finish_power=0;
					flag_increase_voltage_dc =0;
					flag_increase_voltage_ac =0;
					flag_increase_power = 0;
				}



}

void print_lcd(void){

	int flag_refresh_1=0;
	int flag_refresh_2=0;
	int i;
	for(i=0;i<15;i++){
		if (kal1_old[i] != kal1[i]){
			flag_refresh_1 =1;
		}
		if (kal2_old[i] != kal2[i]){
			flag_refresh_2 =1;
		}
		if (flag_refresh_1 == 1 || flag_refresh_2 == 1){
			i=15;
		}
	}
	if ( flag_refresh_1 == 1){
	lcd_gotoxy(0,0);
	lcd_puts("                ");
	lcd_gotoxy(0,0);
	lcd_puts(kal1);
	for(i=0;i<15;i++){
		kal1_old[i] = kal1[i];
		}
	}
	if ( flag_refresh_2 == 1){
	lcd_gotoxy(0,1);
	lcd_puts("                ");
	lcd_gotoxy(0,1);
	lcd_puts(kal2);
	for(i=0;i<15;i++){
		kal2_old[i] = kal2[i];
		}
	}
}

void menu_standby(){
	volatile char temp_stat[16];

	if (state_menu == 0){
		temp = control_mode;
		}
	else if (state_menu == 1) {
		temp = Qset;
		}
	else if (state_menu == 2) {
		temp = Pset;
		}
	else if ( state_menu == 3 ){
		temp = state_adc;
		}

	if (button[2] == '1'&& flag_push[2]=='0'){
		kal1[0]= 0b00000111;
			if ((button[0] == '1' )&&(flag_push[0] == '0')){
				temp=temp-100;
				flag_push[0]='1';
			}
			if (( button[1] == '1')&&(flag_push[1] == '0')){
				temp=temp+100;
				flag_push[1]='1';
			}

		if (state_menu == 0){
			if (temp <= OFF){
				temp = 0;
				}
			else if (temp > ON){
				temp = 1;
				}
			control_mode = temp;
			flag_update_send_xmc=1;
			if (temp == 1){
			sprintf (temp_stat,"ON");
			}
			else {inisiasi_control();sprintf (temp_stat,"OFF");}

			sprintf(kal2,"       %s",temp_stat);
			}
		else if (state_menu == 1) {
			if (temp <= min_QREF){
				temp = min_QREF;
				}
			else if (temp > max_QREF){
				temp = max_QREF;
				}
			Qset=temp;
			power_inv=temp;
			flag_update_send_xmc=1;
			sprintf(kal2,"    %4.0d VAR",temp);
			}
		else if (state_menu == 2) {
			if (temp <= min_PREF){
				temp = min_PREF;
				}
			else if (temp > max_PREF){
				temp = max_PREF;
				}
			Pset=temp;
			sprintf(kal2,"    %4.0d Watt",temp);
			}
		else if ( state_menu == 3 ){
			if (temp <= min_ADC){
				temp = min_ADC;
				}
			else if (temp > max_ADC){
				temp = max_ADC;
				}

			state_adc=temp;
			cek_adc();
			}

	}
	else if (button[2] == '0'){

		if ((button[0] == '1' )&&(flag_push[0] == '0')){
			state_menu=state_menu-1;
			flag_push[0]='1';
			if (state_menu <= min_menu_standby){
				state_menu = min_menu_standby;
				}
		}
		if (( button[1] == '1')&&(flag_push[1] == '0')){
			state_menu=state_menu+1;
			flag_push[1]='1';
			if (state_menu > max_menu_standby){
				state_menu = max_menu_standby;
				}
		}


		if (state_menu == 0){
			temp = control_mode;
			if (temp == 1){
			sprintf (temp_stat,"ON");
			}
			else {sprintf (temp_stat,"OFF");}

			sprintf(kal2,"       %s",temp_stat);

			sprintf(kal1," CONTROL STATUS");
			}
		else if (state_menu == 1) {
			temp = Qset;
			sprintf(kal1,"    SET  QREF");
			sprintf(kal2,"    %4.0d VAR  ",temp);
			}
		else if (state_menu == 2) {
			temp = Pset;
			sprintf(kal1,"    SET  PREF");
			sprintf(kal2,"    %4.0d Watt ",temp);
			}
		else if ( state_menu == 3 ){
			cek_adc();
			}
	}
}

void menu_operation(){

	if (state_menu == 0){
		temp = control_mode;
		}
	else if (state_menu == 1) {
		temp = Qset;
		}
	else if (state_menu == 2) {
		temp = Pset;
		}
	else if ( state_menu == 3){
		temp = state_adc;
		}

	volatile char temp_stat[16];

	if (button[2] == '1'&& flag_push[2]=='0'){
		kal1[0]= 0b00000111;
			if ((button[0] == '1' )&&(flag_push[0] == '0')){
				temp=temp-100;
				flag_push[0]='1';
			}
			if (( button[1] == '1')&&(flag_push[1] == '0')){
				temp=temp+100;
				flag_push[1]='1';
			}

		if (state_menu == 0){
			sprintf(kal2," CANNOT  CHANGE ");
			}
		else if (state_menu == 1) {
			if (temp <= min_QREF){
				temp = min_QREF;
				}
			else if (temp > max_QREF){
				temp = max_QREF;
				}
			Qset=temp;
			power_inv=temp;
			flag_update_send_xmc=1;
			sprintf(kal2,"    %4.0d VAR",temp);
			}
		else if (state_menu == 2) {
			if (temp <= min_PREF){
				temp = min_PREF;
				}
			else if (temp > max_PREF){
				temp = max_PREF;
				}
			Pset=temp;
			sprintf(kal2,"    %4.0d Watt",temp);
			}
		else if ( state_menu == 3 ){
			if (temp <= min_ADC){
				temp = min_ADC;
				}
			else if (temp > max_ADC){
				temp = max_ADC;
				}

			state_adc=temp;
			cek_adc();
			}

	}
	else if (button[2] == '0'){

		if ((button[0] == '1' )&&(flag_push[0] == '0')){
			state_menu=state_menu-1;
			flag_push[0]='1';
			if (state_menu <= min_menu_operation){
				state_menu = min_menu_operation;
				}
		}
		if (( button[1] == '1')&&(flag_push[1] == '0')){
			state_menu=state_menu+1;
			flag_push[1]='1';
			if (state_menu > max_menu_operation){
				state_menu = max_menu_operation;
				}
		}


		if (state_menu == 0){
			temp = control_mode;
			if (temp == 1){
			sprintf (temp_stat,"ON");
			}
			else {sprintf (temp_stat,"OFF");}

			sprintf(kal2,"       %s",temp_stat);
			sprintf(kal1," CONTROL STATUS");
			}
		else if (state_menu == 2) {
			temp = Pset;
			sprintf(kal1,"    SET  PREF");
			sprintf(kal2,"    %4.0d Watt  ",temp);
			}
		else if ( state_menu == 3 ){
			cek_adc();
			}
	}
}

volatile int data_send_count = 0;
volatile int count = 0;
volatile int increment = 0;
volatile int update_data_rabbit = 1;
volatile int data_rabbit_ready = 0;

void update_data(void){
	char buf[16],buf_out[16];

	if (flag_update_send_xmc == 1){
		data_uart_xmc[0]= comparepwm4;//
		data_uart_xmc[1]= comparepwm5;//
		data_uart_xmc[2]= Qset;//
		//data_uart_xmc[3]= 0; //
		data_uart_xmc[4]= MODE;
		data_uart_xmc[5]= Vreff_inv;
		//data_uart_xmc[6]= 0;
		//data_uart_xmc[9]= 0;
		//data_uart_xmc[8]= 0;
		//data_uart_xmc[7]= 0;
		uart_xmc_send();
		flag_update_send_xmc=0;
	}
	if(flag_update_receive_xmc == 1){
		uart_xmc_receive();
		string_read_xmc_increment=0;
		comparepwm4= data_uart_xmc[0];
		comparepwm5= data_uart_xmc[1];
		Qset= data_uart_xmc[2];

		frekuensi_grid = data_uart_xmc[9];
		DCPT2_infineon_2 = data_uart_xmc[10];
		voac_peak = data_uart_xmc[8];
		CT5_infineon_2 = data_uart_xmc[11];//CT5_fix;//CT5_Result*10;ACPT2_AIN_Result*10;
		CT7_infineon_2 = data_uart_xmc[12];//CT7_fix;//CT7_Result*10;GRPT2_AIN_Result*10;//
		ACPT2_infineon_2 = data_uart_xmc[6];//CT5_Result*10;//
		//fault_frekuensi = data_uart_xmc[7];//CT7_Result*10;//
		P_Filt = data_uart_xmc[13];
		Q_Filt = data_uart_xmc[14];

		flag_update_receive_xmc = 0;
	}



	if (flag_update_send_rabbit == 1){
		if (data_send_count == 0 && count == 1){
			data_rabbit();
		}
		if ( count == 2){
			uart_rabbit_send(data_send_count);
			data_send_count ++;
			count = 0;
		}

		if (data_send_count >= 26){

			NVIC_EnableIRQ(USIC1_0_IRQn);
			start_scan =1;

		flag_update_send_rabbit=0;
		data_send_count = 0;
		for ( i = 0; i< 31; i++ ){
			data_uart_rabbit_update[i]=0;

		}

		}

		count ++;
	}
	if(flag_update_receive_rabbit == 1){
		int i;
		char buf[30];
/*
		if(increment == 25){
			//UART001_WaitWriteDataMultiple(UART_RS232,string_read_rabbit,strlen(string_read_rabbit));

			for ( i = 0; i< 50; i++ ){
				sprintf(buf,"%d. %d\r\n",i,data_uart_rabbit_update[i]);
				UART001_WaitWriteDataMultiple(UART_RS232,buf,strlen(buf));
				data_uart_rabbit_update[i]=0;

			}
			uart_rabbit_receive();
		}

*/
	//	UART001_WaitWriteDataMultiple(UART_RS232,string_read_rabbit,strlen(string_read_rabbit));

		uart_rabbit_receive(increment);
		sprintf(buf,"%d. %d\r\n",increment,data_uart_rabbit_update[increment]);
		UART001_WaitWriteDataMultiple(UART_RS232,buf,strlen(buf));

/*
		uart_rabbit_receive();

				for ( int i = 0; i< 12; i++ ){

					sprintf(buf,"%d. %d\r\n",i,data_uart_rabbit_update[i]);
					UART001_WaitWriteDataMultiple(UART_RS232,buf,strlen(buf));

					}
*/
		if ( increment >= 11){
			increment = -1;
			flag_update_send_rabbit=1;
			flag_update_receive_rabbit = 0;

			/*

			if (flag_reset_time == 0){

				temp_date = data_uart_rabbit_update[10];
				tanggal = temp_date*0.0001;
				temp_date= data_uart_rabbit_update[10];
				bulan = (temp_date-(tanggal*10000))*0.01;
				temp_date=data_uart_rabbit_update[10];
				tahun = (temp_date-(tanggal*10000)-(bulan*100));
				temp_date = data_uart_rabbit_update[11];
				jam = temp_date*0.0001;
				temp_date= data_uart_rabbit_update[11];
				menit = (temp_date-(jam*10000))*0.01;
				temp_date=data_uart_rabbit_update[11];
				detik = (temp_date-(jam*10000)-( menit*100));

				RTC_Init(1,tanggal,bulan,tahun,jam,menit,detik);
				flag_reset_time = 1;
			}

*/
			/*   //di ilangin buat sekarang
			if ( data_uart_rabbit_update[8] == 1 && flag_invalid_K2 == 0){
				timeout_state=0;
				flag_close_rabbit = 1;
				MODE =1 ;
				flag_test == 0;
				}
			else if (data_uart_rabbit_update[8] == 2  && flag_invalid_K2 == 0){
				Close_K2();
				flag_interrupt = 1;
				flag_MPPT =1;
				//flag_droop = 1;

				// matikan proteksi sementara
				NVIC_DisableIRQ(ERU0_0_IRQn);
				NVIC_DisableIRQ(ERU0_1_IRQn);
				//NVIC_DisableIRQ(ERU0_2_IRQn);
				//NVIC_DisableIRQ(ERU0_3_IRQn);

				// flag switch
				flag_K2 = 1; // close
				}
			*/
		}
		increment++;

		}

}

void cek_adc(void){
	int32_t voac_rms;
	fixed_32 adc_fixed;
	fixed_32 adc_fixed_2;
	float adc_float;
	float adc_float_2;
	adc_fixed.fract_bits = 10;
	sprintf(kal1," ");

		if (state_adc == 0){
			adc_fixed.nilai = DCPT1_fix;
			adc_float = fixed2float(adc_fixed.nilai,10);
			strcat(kal1,"  PV Voltage");
			sprintf(kal2,"   %+.2f Volt ",adc_float);

			}
		else if ( state_adc == 200){
			adc_float = frekuensi_grid*0.001;
			adc_float = adc_float*0.159;
			strcat(kal1,"  Grid Freq");
			sprintf(kal2,"  %.4f Hz ",adc_float);
		}

		else if ( state_adc == 300 ){
			adc_fixed.nilai = ACPT2_fix;
			adc_fixed_2.nilai = GRPT2_fix;
			adc_float = fixed2float(adc_fixed.nilai,10);
			adc_float_2 = fixed2float(adc_fixed_2.nilai,10);
			strcat(kal1," Sync Stat");
			sprintf(kal2,"   %+.3f Volt ",(adc_float-adc_float_2));
			}


		else if ( state_adc == 100){
			voac_rms = voac_peak*0.71;
			strcat(kal1,"  Grid Voltage");
			sprintf(kal2," %d Vp %d Vrms ",voac_peak,voac_rms);
		}
}

void timer_reff(int count_num){
	int count;
	if (flag_reff==0){
		RTC_get_time(&hour,&minute_start_reff,&second_start_reff);
		flag_reff=1;
	}
		RTC_get_time(&hour,&minute_reff,&second_reff);
		count = (minute_reff*60+second_reff) - (minute_start_reff*60+second_start_reff);
		if (count >= count_num ){
			timeout_reff=1;
		}
	}


void preparation (void){
	int i;
	char kal[32];

	control_mode = 0;

	state_adc = 0;

	comparepwm1 = 3745;
	comparepwm2 = 3745;

	PWM_Boost_Init();
	PWM_Boost_Start();

	My_Delay_s(10);


	for(i=0;i<10;i++) {
	My_Delay_ms(100);
	ACPT1_OFFSET += ACPT1_AIN_Result;
	ACPT3_OFFSET += ACPT3_AIN_Result;
	GRPT1_OFFSET += GRPT1_AIN_Result;
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

	ACPT1_OFFSET = ACPT1_OFFSET*0.1;
	ACPT3_OFFSET = ACPT3_OFFSET*0.1;
	GRPT1_OFFSET = GRPT1_OFFSET*0.1;
	GRPT3_OFFSET = GRPT3_OFFSET*0.1;
	CT1_OFF = CT1_OFF*0.1;
	CT2_OFF = CT2_OFF*0.1;
	CT3_OFF = CT3_OFF*0.1;
	CT4_OFF = CT4_OFF*0.1;
	CT5_OFF = CT5_OFF*0.1;
	CT6_OFF = CT6_OFF*0.1;
	CT7_OFF = CT7_OFF*0.1;
	CT8_OFF = CT8_OFF*0.1;



	flag_data_receive_xmc =0;

	lcd_gotoxy(0,0);
	lcd_puts("                 ");
	lcd_gotoxy(0,0);
	sprintf(kal,"Check Com XMC");
	lcd_puts(kal);



	while (flag_data_receive_xmc == 0){
	uart_xmc_request(0); // request update for initialization and adc offset renewal
	for(int i=0;i<10;i++) My_Delay_ms(100);
	string_read_xmc_increment=0;
	}

	lcd_gotoxy(0,0);
	lcd_puts("                 ");
	lcd_gotoxy(0,0);
	sprintf(kal,"Update Data");
	lcd_puts(kal);

	while (comparepwm4==100 || comparepwm5==0 ){
	uart_xmc_request(1); // request update for initialization
	for(int i=0;i<10;i++) My_Delay_ms(100);
	uart_xmc_receive();
	string_read_xmc_increment=0;
	comparepwm4= data_uart_xmc[0];
	comparepwm5= data_uart_xmc[1];
	flag_update_receive_xmc = 0;
	}


	flag_prep = 0;
	finish_prep = 0;
	flag_timer = 0;
	flag_menu = 0;
	timeout = 0;

	// inisiasi switch
	flag_K1=0;
	flag_K2=0;
	flag_HK1=0;
	flag_HK2=0;

	RTC_Init(3,11,2,17,1,40,0);

	inisiasi_control(); // inisiasi kontrol closed loop untuk converter

	lcd_gotoxy(0,1);
	lcd_puts("                 ");
	lcd_gotoxy(0,1);
	sprintf(kal,"Inverter 5 kW");
	lcd_puts(kal);

	for(i=0;i<10;i++) My_Delay_ms(100);

	state=1;
	previous_state=0;
	finish_prep = 1;

	increment_adc = 0;

	state_menu = min_menu_standby ;

	// untuk bisa ngecek frekuensi dan peak tegangan, langsung nyalain PLL
	uart_xmc_request(7);

	lcd_clear();
	GST_LOW();
}

