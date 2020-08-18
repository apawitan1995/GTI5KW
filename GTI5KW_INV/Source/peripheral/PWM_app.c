/*
 * PWM_app.c
 *
 *  Created on: Dec 4, 2013
 *      Author: Hanif
 */

#include "peripheral/PWM_app.h"

volatile int comparepwm4;
volatile int comparepwm5;


volatile int flag_pwm;

void CCU8_Init(void)
{
	//De-Assert reset signal trough
}

void PWM_DAVE_Init_app(void)
{
	//PWM Dave apps Init
	//  Initialization of app 'CCU8GLOBAL'
	CCU8GLOBAL_Init();

	//  Initialization of app 'PWMSP002'
	PWMSP002_Init();

	//  Initialization of app 'CCU4GLOBAL'
//	CCU4GLOBAL_Init();

	//  Initialization of app 'PWMSP001'
//	PWMSP001_Init();

}

void PWM4A4B_Init(void)
{
/************************************************************************/
	//Init PWM4A P5.6 (CCU81.OUT03) dan PWM4B P5.7 (CCU81.OUT02)
	//Set passive level low P5.6
	CLR_BIT(PWM_4A_4B->PSL,CCU8_CC8_PSL_PSL22_Pos);
	//Set passive level low P5.7
	CLR_BIT(PWM_4A_4B->PSL,CCU8_CC8_PSL_PSL21_Pos);

	//Set output selector non inverted output to CCU81.OUT03
	SET_BIT(PWM_4A_4B->CHC,CCU8_CC8_CHC_OCS4_Pos);
	//Set output selector inverted output to CCU81.OUT02
	SET_BIT(PWM_4A_4B->CHC,CCU8_CC8_CHC_OCS3_Pos);

	//Enable Dead Time for Channel 2
	SET_BIT(PWM_4A_4B->DTC,CCU8_CC8_DTC_DTE2_Pos);

	//Dead Time enabled for non inverted output CCU81.OUT03
	SET_BIT(PWM_4A_4B->DTC,CCU8_CC8_DTC_DCEN3_Pos);
	//Dead Time enabled for inverted output CCU81.OUT02
	SET_BIT(PWM_4A_4B->DTC,CCU8_CC8_DTC_DCEN4_Pos);

	//Set dead time channel 2 value for Rising edge
	WR_REG(PWM_4A_4B->DC2R,CCU8_CC8_DC2R_DT2R_Msk,CCU8_CC8_DC2R_DT2R_Pos,PWM_4A_4B_Handle.kRisingDeadTime);
	//Set dead time channel 2 value for Falling edge
	WR_REG(PWM_4A_4B->DC2R,CCU8_CC8_DC2R_DT2F_Msk,CCU8_CC8_DC2R_DT2F_Pos,PWM_4A_4B_Handle.kFallingDeadTime);

	//Select P5.6 Driver Pad
	WR_REG(PORT5->PDR0,PORT5_PDR0_PD6_Msk,PORT5_PDR0_PD6_Pos,0);
	//Select P5.7 Driver Pad
	WR_REG(PORT5->PDR0,PORT5_PDR0_PD7_Msk,PORT5_PDR0_PD7_Pos,0);
/************************************************************************/
}

void PWM5A5B_Init(void)
{
	//Init PWM5A P5.4 (CCU81.OUT13) dan PWM5B P5.5 (CCU81.OUT12)
	//Set passive level low P5.4
	CLR_BIT(PWM_5A_5B->PSL,CCU8_CC8_PSL_PSL22_Pos);
	//Set passive level low P5.5
	CLR_BIT(PWM_5A_5B->PSL,CCU8_CC8_PSL_PSL21_Pos);

	//Set output selector non inverted output to CCU81.OUT13
	SET_BIT(PWM_5A_5B->CHC,CCU8_CC8_CHC_OCS4_Pos);
	//Set output selector inverted output to CCU81.OUT12
	SET_BIT(PWM_5A_5B->CHC,CCU8_CC8_CHC_OCS3_Pos);

	//Enable Dead Time for Channel 2
	SET_BIT(PWM_5A_5B->DTC,CCU8_CC8_DTC_DTE2_Pos);

	//Dead Time enabled for non inverted output CCU81.OUT13
	SET_BIT(PWM_5A_5B->DTC,CCU8_CC8_DTC_DCEN3_Pos);
	//Dead Time enabled for inverted output CCU81.OUT12
	SET_BIT(PWM_5A_5B->DTC,CCU8_CC8_DTC_DCEN4_Pos);

	//Set dead time channel 2 value for Rising edge
	WR_REG(PWM_5A_5B->DC2R,CCU8_CC8_DC2R_DT2R_Msk,CCU8_CC8_DC2R_DT2R_Pos,PWM_5A_5B_Handle.kRisingDeadTime);
	//Set dead time channel 2 value for Falling edge
	WR_REG(PWM_5A_5B->DC2R,CCU8_CC8_DC2R_DT2F_Msk,CCU8_CC8_DC2R_DT2F_Pos,PWM_5A_5B_Handle.kFallingDeadTime);

	//Select P5.4 Driver Pad
	WR_REG(PORT5->PDR0,PORT5_PDR0_PD4_Msk,PORT5_PDR0_PD4_Pos,0);
	//Select P5.5 Driver Pad
	WR_REG(PORT5->PDR0,PORT5_PDR0_PD5_Msk,PORT5_PDR0_PD5_Pos,0);
/************************************************************************/
	//Init PWM5A_5B_ADC (CCU41_CC41)
	//Set Passive Low Level signal
	//CLR_BIT(PWM_5A_5B_ADC->PSL,CCU4_CC4_PSL_PSL_Pos);
}

void PWM6A6B_Init(void)
{
	//Init PWM6A P5.2 (CCU81.OUT23) dan PWM6B P5.3 (CCU81.OUT22)
	//Set passive level low P5.2
	CLR_BIT(PWM_6A_6B->PSL,CCU8_CC8_PSL_PSL22_Pos);
	//Set passive level low P5.3
	CLR_BIT(PWM_6A_6B->PSL,CCU8_CC8_PSL_PSL21_Pos);

	//Set output selector non inverted output to CCU81.OUT23
	SET_BIT(PWM_6A_6B->CHC,CCU8_CC8_CHC_OCS4_Pos);
	//Set output selector inverted output to CCU81.OUT22
	CLR_BIT(PWM_6A_6B->CHC,CCU8_CC8_CHC_OCS3_Pos);

	//Enable Dead Time for Channel 2
	//SET_BIT(PWM_6A_6B->DTC,CCU8_CC8_DTC_DTE2_Pos);

	//Dead Time enabled for non inverted output CCU81.OUT23
	//SET_BIT(PWM_6A_6B->DTC,CCU8_CC8_DTC_DCEN3_Pos);
	//Dead Time enabled for inverted output CCU81.OUT22
	//SET_BIT(PWM_6A_6B->DTC,CCU8_CC8_DTC_DCEN4_Pos);

	//Set dead time channel 2 value for Rising edge
	//WR_REG(PWM_6A_6B->DC2R,CCU8_CC8_DC2R_DT2R_Msk,CCU8_CC8_DC2R_DT2R_Pos,PWM_6A_6B_Handle.kRisingDeadTime);
	//Set dead time channel 2 value for Falling edge
	//WR_REG(PWM_6A_6B->DC2R,CCU8_CC8_DC2R_DT2F_Msk,CCU8_CC8_DC2R_DT2F_Pos,PWM_6A_6B_Handle.kFallingDeadTime);

	//Select P5.2 Driver Pad
	WR_REG(PORT5->PDR0,PORT5_PDR0_PD2_Msk,PORT5_PDR0_PD2_Pos,0);
	//Select P5.3 Driver Pad
	WR_REG(PORT5->PDR0,PORT5_PDR0_PD3_Msk,PORT5_PDR0_PD3_Pos,0);
}

void PWM4A4B_Trig_ADC_Init(void)
{
	//Inisialisasi Compare Match CCU81_CC80 trigger untuk ADC G0 Queue
	//Enable CCU81_CC80 compare match 2 while counting down (CCCM2_D) Interrupt
	SET_BIT(PWM_4A_4B_Handle.CC8yRegsPtr->INTE,CCU8_CC8_INTE_CMD2E_Pos);
	//Connect CCU81_CC80 compare match 2 (CCCM2) Interrupt to CCU81.CC80SR3 --> CCU81_3_IRQHandler
	WR_REG(PWM_4A_4B_Handle.CC8yRegsPtr->SRS,CCU8_CC8_SRS_CM2SR_Msk,CCU8_CC8_SRS_CM2SR_Pos,0x03);
}

void PWM5A5B_Trig_ADC_Init(void)
{
	//Inisialisasi Compare Match CCU41_CC41 trigger untuk ADC Scan Source
	//Enable CCU41_CC41 compare match while counting down (CCCM_D) Interrupt
	//SET_BIT(PWM_5A_5B_ADC_Handle.CC4yRegsPtr->INTE,CCU4_CC4_INTE_CMDE_Pos);
	//Connect CCU41_CC41 compare match (CCCM) Interrupt to CCU41.SR2 --> CCU41_2_IRQHandler
	//WR_REG(PWM_5A_5B_ADC_Handle.CC4yRegsPtr->SRS,CCU4_CC4_SRS_CMSR_Msk,CCU4_CC4_SRS_CMSR_Pos,0x02);

/************************************************************************/
	//Enable CCU81_CC81 compare match 2 while counting down (CCCM2_D) Interrupt
	SET_BIT(PWM_5A_5B_Handle.CC8yRegsPtr->INTE,CCU8_CC8_INTE_CMD2E_Pos);
	//Connect CCU81_CC81 compare match 2 (CCCM2) Interrupt to CCU81.CC81SR1 --> CCU81_1_IRQHandler
	WR_REG(PWM_5A_5B_Handle.CC8yRegsPtr->SRS,CCU8_CC8_SRS_CM2SR_Msk,CCU8_CC8_SRS_CM2SR_Pos,0x01);
}

void PWM6A6B_Trig_ADC_Init(void)
{
	//Inisialisasi Compare Match CCU81_CC82 trigger untuk ADC G3
	//Enable CCU81_CC82 compare match 2  while counting down (CCCM2_D) Interrupt
	SET_BIT(PWM_6A_6B_Handle.CC8yRegsPtr->INTE,CCU8_CC8_INTE_CMD2E_Pos);
	//Connect CCU81_CC82 compare match 2 (CCCM2) Interrupt to CCU81.CC82SR2 --> CCU81_2_IRQHandler
	WR_REG(PWM_6A_6B_Handle.CC8yRegsPtr->SRS,CCU8_CC8_SRS_CM2SR_Msk,CCU8_CC8_SRS_CM2SR_Pos,0x02);
}

uint32_t PWMSP002_Calculate_Compare_Value(const PWMSP002_HandleType* HandlePtr,float DC)
{
	uint32_t period;

	PWMSP002_GetPeriodReg(HandlePtr,&period);

	if((DC>100.0f) || (DC<0.0f))
	{
		DC=0.0f;
	}
	return ((uint32_t)((1.0f-(DC/100.0f))*(float)period));
}

uint32_t PWMSP002_Calculate_Compare_Value_Float(const PWMSP002_HandleType* HandlePtr,float DC)
{
	uint32_t period;

	PWMSP002_GetPeriodReg(HandlePtr,&period);

	return ((uint32_t)((1.0f-DC)*(float)period));
}



void PWM4A4B_Set_Compare(uint32_t compare1,uint32_t compare2)
{
	//PWM_1A_1B CC81_CC80
	//Set value to compare shadow register

	PWM_4A_4B->CR1S=compare1;
	PWM_4A_4B->CR2S=compare2;

	//Trigger shadow register transfer
	CCU81->GCSS |=((uint32_t)(1 << CCU8_GCSS_S0SE_Pos) &CCU8_GCSS_S0SE_Msk);
}

void PWM5A5B_Set_Compare(uint32_t compare1,uint32_t compare2)
{
	//PWM_2A_2B CC81_CC81
	//Set value to compare shadow register
	PWM_5A_5B->CR1S=compare1;
	PWM_5A_5B->CR2S=compare2;

//	PWM_2A_2B_ADC->CRS=compare2;

	//Trigger shadow register transfer
	CCU81->GCSS |=((uint32_t)(1 << CCU8_GCSS_S1SE_Pos) &CCU8_GCSS_S1SE_Msk);
	//Trigger shadow register transfer
//	CCU41->GCSS |=((uint32_t)(1 << CCU4_GCSS_S1SE_Pos) &CCU4_GCSS_S1SE_Msk);
}

void PWM6A6B_Set_Compare(uint32_t compare1,uint32_t compare2)
{
	//PWM_3A_3B CC81_CC82
	//Set value to compare shadow register
	PWM_6A_6B->CR1S=compare1;
	PWM_6A_6B->CR2S=compare2;

	//Trigger shadow register transfer
	CCU81->GCSS |=((uint32_t)(1 << CCU8_GCSS_S2SE_Pos) &CCU8_GCSS_S2SE_Msk);
}


void PWM_Inverter_Enable(void)
{
	//Enable P5.6 as Push-Pull Output and Connect CCU81.OUT03 to P5.6
	WR_REG(PORT5->IOCR4,PORT5_IOCR4_PC6_Msk,PORT5_IOCR4_PC6_Pos,0x13);
	//Enable P5.7 as Push-Pull Output and Connect CCU81.OUT02 to P5.7
	WR_REG(PORT5->IOCR4,PORT5_IOCR4_PC7_Msk,PORT5_IOCR4_PC7_Pos,0x13);

	//Enable P5.4 as Push-Pull Output and Connect CCU81.OUT13 to P5.4
	WR_REG(PORT5->IOCR4,PORT5_IOCR4_PC4_Msk,PORT5_IOCR4_PC4_Pos,0x13);
	//Enable P5.5 as Push-Pull Output and Connect CCU81.OUT12 to P5.5
	WR_REG(PORT5->IOCR4,PORT5_IOCR4_PC5_Msk,PORT5_IOCR4_PC5_Pos,0x13);

	//Enable P5.2 as Push-Pull Output and Connect CCU81.OUT23 to P5.2
	WR_REG(PORT5->IOCR0,PORT5_IOCR0_PC2_Msk,PORT5_IOCR0_PC2_Pos,0x13);
	//Enable P5.3 as Push-Pull Output and Connect CCU81.OUT22 to P5.3
	WR_REG(PORT5->IOCR0,PORT5_IOCR0_PC3_Msk,PORT5_IOCR0_PC3_Pos,0x13);
}

void PWM_Inverter_Disable(void)
{
	//Enable P5.6 as Direct I/O Push-Pull Output
	WR_REG(PORT5->IOCR4,PORT5_IOCR4_PC6_Msk,PORT5_IOCR4_PC6_Pos,0x10);
	CLR_BIT(PORT5->OUT,PORT5_OUT_P6_Pos);
	//Enable P5.7 as Direct I/O Push-Pull Output
	WR_REG(PORT5->IOCR4,PORT5_IOCR4_PC7_Msk,PORT5_IOCR4_PC7_Pos,0x10);
	CLR_BIT(PORT5->OUT,PORT5_OUT_P7_Pos);

	//Enable P5.4 as Direct I/O Push-Pull Output
	WR_REG(PORT5->IOCR4,PORT5_IOCR4_PC4_Msk,PORT5_IOCR4_PC4_Pos,0x10);
	CLR_BIT(PORT5->OUT,PORT5_OUT_P4_Pos);
	//Enable P5.5 as Direct I/O Push-Pull Output
	WR_REG(PORT5->IOCR4,PORT5_IOCR4_PC5_Msk,PORT5_IOCR4_PC5_Pos,0x10);
	CLR_BIT(PORT5->OUT,PORT5_OUT_P5_Pos);

	//Enable P5.2 as Direct I/O Push-Pull Output
	WR_REG(PORT5->IOCR0,PORT5_IOCR0_PC2_Msk,PORT5_IOCR0_PC2_Pos,0x10);
	CLR_BIT(PORT5->OUT,PORT5_OUT_P2_Pos);
	//Enable P5.3 as Direct I/O Push-Pull Output
	WR_REG(PORT5->IOCR0,PORT5_IOCR0_PC3_Msk,PORT5_IOCR0_PC3_Pos,0x10);
	CLR_BIT(PORT5->OUT,PORT5_OUT_P3_Pos);
}

void PWM_Inverter_Init(void)
{
	PWM4A4B_Init();
/************************************************************************/
	PWM5A5B_Init();
/************************************************************************/
	PWM6A6B_Init();
/************************************************************************/

	PWM4A4B_Trig_ADC_Init();
	PWM5A5B_Trig_ADC_Init();
	PWM6A6B_Trig_ADC_Init();
}

void PWM_Inverter_Start(void)
{
	uint32_t compare_value;

	//PWM_Inverter_Enable(); //diprojek yg lama baris ini dioffkan

	compare_value=PWMSP002_Calculate_Compare_Value(PWM_4A_4B_API,0.0f);
	PWM4A4B_Set_Compare(compare_value,compare_value);

	compare_value=PWMSP002_Calculate_Compare_Value(PWM_5A_5B_API,0.0f);
	PWM5A5B_Set_Compare(compare_value,compare_value);

	compare_value=PWMSP002_Calculate_Compare_Value(PWM_6A_6B_API,0.0f);
	PWM6A6B_Set_Compare(compare_value,compare_value);

	//PWM_App_Run_Stat=1;
	PWMSP002_Start(PWM_4A_4B_API);
	PWMSP002_Start(PWM_5A_5B_API);
	PWMSP002_Start(PWM_6A_6B_API);
}

void PWM_Inverter_Stop(void)
{
	PWMSP002_Stop(PWM_4A_4B_API);
	PWMSP002_Stop(PWM_5A_5B_API);
	PWMSP002_Stop(PWM_6A_6B_API);

	PWM_Inverter_Disable();
}

//dibawah belum diedit

void PWM_Enable_Interrupt_NVIC(void)
{
	//Set Interrupt Priority
	NVIC_SetPriority(CCU81_3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),58,0));
	//Enable CCU81_0_IRQHandler (PWM_4A_4B) interrupt
	NVIC_EnableIRQ(CCU81_3_IRQn);

	//Set Interrupt Priority
	NVIC_SetPriority(CCU81_1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),58,0));
	//Enable CCU81_1_IRQHandler (PWM_5A_5B) interrupt
	NVIC_EnableIRQ(CCU81_1_IRQn);

	//Set Interrupt Priority
	NVIC_SetPriority(CCU81_2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),58,0));
	//Enable CCU81_2_IRQHandler (PWM_6A_6B) interrupt
	NVIC_EnableIRQ(CCU81_2_IRQn);
}

void PWM_Disable_Interrupt_NVIC(void)
{
	//Enable CCU81_0_IRQHandler (PWM_4A_4B) interrupt
	NVIC_DisableIRQ(CCU81_3_IRQn);
	//Enable CCU81_1_IRQHandler (PWM_5A_5B) interrupt
	NVIC_DisableIRQ(CCU81_1_IRQn);
	//Enable CCU81_2_IRQHandler (PWM_6A_6B) interrupt
	NVIC_DisableIRQ(CCU81_2_IRQn);
}
