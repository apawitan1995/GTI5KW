/*
 * PWM_app.h
 *
 *  Created on: Dec 4, 2013
 *      Author: Hanif
 */

#ifndef PWM_APP_H_
#define PWM_APP_H_

#include <DAVE3.h>
#include <stdio.h>
#include "common/look_up_table.h"
#include "peripheral/UART_app.h" //Debug
#include "peripheral/ADC_app.h" //Trigger by PWM
#include "algorithm/fixed_operation.h"

//variable to test PWM CONV and INV
extern volatile int32_t pwmol;
extern volatile int32_t testpwmfixed;
extern volatile int32_t testpwmfixedinv;


extern volatile int comparepwm4;
extern volatile int comparepwm5;

extern volatile int flag_pwm;;

extern volatile float testpwm;
extern volatile float testpwmINV;
extern volatile float pwmtestinverter1;
extern volatile float pwmtestinverter2;

extern volatile int32_t EN_CV;
extern volatile uint32_t lcd_refresh_counter;
extern volatile uint32_t en_lcd_print;
extern volatile uint32_t offset_rate;
extern volatile uint32_t counter_ref;

//Definisi macro untuk DAVE PWM API

#define PWM_4A_4B_API (PWMSP002_HandleType*)&PWMSP002_Handle0
#define PWM_5A_5B_API (PWMSP002_HandleType*)&PWMSP002_Handle1
#define PWM_6A_6B_API (PWMSP002_HandleType*)&PWMSP002_Handle2
//#define PWM_5A_5B_PS_API (PWMSP001_HandleType*)&PWMSP001_Handle0
//#define PWM_6A_6B_PS_API (PWMSP001_HandleType*)&PWMSP001_Handle1
//#define PWM_5A_5B_ADC_API (PWMSP001_HandleType*)&PWMSP001_Handle2

#define PWM_4A_4B CCU81_CC80
#define PWM_5A_5B CCU81_CC81
#define PWM_6A_6B CCU81_CC82
//#define PWM_5A_5B_PS CCU41_CC40
//#define PWM_6A_6B_PS CCU42_CC40
//#define PWM_5A_5B_ADC CCU41_CC41

#define PWM_4A_4B_Handle PWMSP002_Handle0
#define PWM_5A_5B_Handle PWMSP002_Handle1
#define PWM_6A_6B_Handle PWMSP002_Handle2
//#define PWM_5A_5B_PS_Handle PWMSP001_Handle0
//#define PWM_6A_6B_PS_Handle PWMSP001_Handle1
//#define PWM_5A_5B_ADC_Handle PWMSP001_Handle2

#define PWM4A4B_IRQ_Handler CCU81_3_IRQHandler
#define PWM5A5B_IRQ_Handler CCU81_1_IRQHandler
#define PWM6A6B_IRQ_Handler CCU81_2_IRQHandler

void CCU8_Init(void);
//void CCU4_Init(void);

void PWM_DAVE_Init_app(void);
void PWM4A4B_Init(void);
void PWM5A5B_Init(void);
void PWM6A6B_Init(void);

void PWM4A4B_Trig_ADC_Init(void);
void PWM5A5B_Trig_ADC_Init(void);
void PWM6A6B_Trig_ADC_Init(void);

uint32_t PWMSP002_Calculate_Compare_Value(const PWMSP002_HandleType* HandlePtr,float DC);
uint32_t PWMSP002_Calculate_Compare_Value_Float(const PWMSP002_HandleType* HandlePtr,float DC);
//uint32_t PWMSP001_Calculate_Compare_Value(const PWMSP001_HandleType* HandlePtr,float DC);
//uint32_t PWMSP001_Calculate_Compare_Value_Float(const PWMSP001_HandleType* HandlePtr,float DC);

void PWM4A4B_Set_Compare(uint32_t compare1,uint32_t compare2);
void PWM5A5B_Set_Compare(uint32_t compare1,uint32_t compare2);
void PWM6A6B_Set_Compare(uint32_t compare1,uint32_t compare2);

void PWM_Boost_Start(void);
void PWM_Inverter_Enable(void);
void PWM_Inverter_Disable(void);
void PWM_Inverter_Init(void);
void PWM_Inverter_Start(void);
void PWM_Inverter_Stop(void);

void PWM_Enable_Interrupt_NVIC(void);
void PWM_Disable_Interrupt_NVIC(void);

#endif /* PWM_APP_H_ */
