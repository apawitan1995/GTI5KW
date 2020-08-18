/*
 * PWM_app.h
 *
 *  Created on: Dec 4, 2013
 *   Update on: Jan 13, 2016
 *      Author: Hanif, Arif
 */

#ifndef PWM_APP_H_
#define PWM_APP_H_

#include <DAVE3.h>
#include <stdio.h>
#include "common/look_up_table.h"
#include "peripheral/UART_app.h" //Debug
#include "peripheral/ADC_app.h" //Trigger by PWM

//Definisi macro untuk DAVE PWM API

#define PWM_1A_1B_API (PWMSP002_HandleType*)&PWMSP002_Handle0
#define PWM_2A_2B_API (PWMSP002_HandleType*)&PWMSP002_Handle1
#define PWM_3A_3B_API (PWMSP002_HandleType*)&PWMSP002_Handle2
//#define PWM_5A_5B_4A_4B_API (PWMSP002_HandleType*)&PWMSP002_Handle3
#define PWM_2A_2B_PS_API (PWMSP001_HandleType*)&PWMSP001_Handle0
#define PWM_3A_3B_PS_API (PWMSP001_HandleType*)&PWMSP001_Handle1
//#define PWM_2A_2B_ADC_API (PWMSP001_HandleType*)&PWMSP001_Handle2

#define PWM_1A_1B CCU81_CC80
#define PWM_2A_2B CCU81_CC81
#define PWM_3A_3B CCU81_CC82
//#define PWM_Inverter CCU81_CC83

#define PWM_2A_2B_PS CCU41_CC40
#define PWM_3A_3B_PS CCU42_CC40
//#define PWM_2A_2B_ADC CCU41_CC41

#define PWM_1A_1B_Handle PWMSP002_Handle0
#define PWM_2A_2B_Handle PWMSP002_Handle1
#define PWM_3A_3B_Handle PWMSP002_Handle2
//#define PWM_4A_4B_5A_5B_Handle PWMSP002_Handle3

#define PWM_2A_2B_PS_Handle PWMSP001_Handle0
#define PWM_3A_3B_PS_Handle PWMSP001_Handle1
//#define PWM_2A_2B_ADC_Handle PWMSP001_Handle2

#define PWM1A1B_IRQ_Handler CCU81_3_IRQHandler
#define PWM2A2B_IRQ_Handler CCU81_1_IRQHandler
#define PWM3A3B_IRQ_Handler CCU81_2_IRQHandler
//#define PWM_Inverter_IRQ_Handler CCU81_0_IRQHandler

void PWM_DAVE_Init_app(void);
void PWM1A1B_Init(void);
void PWM2A2B_Init(void);
void PWM3A3B_Init(void);
void PWM1A1B_Trig_ADC_Init(void);
void PWM2A2B_Trig_ADC_Init(void);
void PWM3A3B_Trig_ADC_Init(void);
void PWM_Buck_Boost_Init(void);
void PWM_Buck_Enable(void);
void PWM_Boost_Enable(void);
void PWM_Buck_Disable(void);
void PWM_Boost_Disable(void);
void PWM_Boost_Init(void);
void PWM_Boost_Start(void);
void PWM_Buck_Init(void);
void PWM_Buck_Start(void);
void PWM_Buck_Boost_Stop(void);
void PWM_Enable_Interrupt_NVIC(void);
void PWM_Disable_Interrupt_NVIC(void);

void PWM1A1B_Set_Compare(uint32_t compare1,uint32_t compare2);
void PWM2A2B_Set_Compare(uint32_t compare1,uint32_t compare2);
void PWM3A3B_Set_Compare(uint32_t compare1,uint32_t compare2);

uint32_t PWMSP002_Calculate_Compare_Value(const PWMSP002_HandleType* HandlePtr,float DC);
uint32_t PWMSP002_Calculate_Compare_Value_Float(const PWMSP002_HandleType* HandlePtr,float DC);
uint32_t PWMSP001_Calculate_Compare_Value(const PWMSP001_HandleType* HandlePtr,float DC);
uint32_t PWMSP001_Calculate_Compare_Value_Float(const PWMSP001_HandleType* HandlePtr,float DC);

void CCU8_Init(void);
void CCU4_Init(void);

extern volatile int comparepwm1;
extern volatile int comparepwm2;
extern volatile int comparepwm4;
extern volatile int comparepwm5;
extern volatile float ILreff_s;
extern volatile int increment_adc;

// flag switch
extern volatile int flag_K1;
extern volatile int flag_K2;
extern volatile int flag_HK1;
extern volatile int flag_HK2;

extern volatile int flag_interrupt;
extern volatile int flag_MPPT;

//variable to test PWM CONV and INV
extern volatile int32_t pwmol;
extern volatile int32_t testpwmfixed;
extern volatile int32_t testpwmfixedinv;

extern volatile float testpwm;
extern volatile float testpwmINV;
extern volatile float pwmtestinverter1;
extern volatile float pwmtestinverter2;

extern volatile int32_t EN_CV;
extern volatile uint32_t lcd_refresh_counter;
extern volatile uint32_t en_lcd_print;
extern volatile uint32_t offset_rate;


#endif /* PWM_APP_H_ */
