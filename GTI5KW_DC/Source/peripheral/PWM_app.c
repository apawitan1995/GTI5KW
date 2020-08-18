/*
 * PWM_app.c
 *
 *  Created on: Dec 4, 2013
 *   Update on: Jan 13, 2016
 *      Author: Hanif, Arif
 */

#include "peripheral/PWM_app.h"

volatile int comparepwm1;
volatile int comparepwm2;

volatile int32_t testpwmfixed = 3749;
volatile int32_t pwmol = 3649;

volatile int32_t EN_CV = 0;

// flag switch
volatile int flag_K1;
volatile int flag_K2;
volatile int flag_HK1;
volatile int flag_HK2;

// data-data komunikasi dari dan ke uart XMC 2
volatile int comparepwm4;
volatile int comparepwm5;

volatile float ILreff_s;

volatile int increment_adc;

// flag nyalakan kembali interrupt
volatile int flag_interrupt;


// flag nyalakan MPPT
volatile int flag_MPPT;


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
	CCU4GLOBAL_Init();

	//  Initialization of app 'PWMSP001'
	PWMSP001_Init();

}

void PWM1A1B_Init(void)
{
/************************************************************************/
	//Init PWM1A P5.6 (CCU81.OUT03) dan PWM1B P5.7 (CCU81.OUT02)
	//Set passive level low P5.6
	CLR_BIT(PWM_1A_1B->PSL,CCU8_CC8_PSL_PSL22_Pos);
	//Set passive level low P5.7
	CLR_BIT(PWM_1A_1B->PSL,CCU8_CC8_PSL_PSL21_Pos);

	//Set output selector non inverted output to CCU81.OUT03
	SET_BIT(PWM_1A_1B->CHC,CCU8_CC8_CHC_OCS4_Pos);

	//Set output selector non inverted output to CCU81.OUT02
	CLR_BIT(PWM_1A_1B->CHC,CCU8_CC8_CHC_OCS3_Pos);
	//Set output selector non inverted output to CCU81.OUT02
	//CLR_BIT(CCU81_CC80->CHC,CCU8_CC8_CHC_OCS3_Pos);

//	//Enable Dead Time for Channel 2
//	SET_BIT(PWM_1A_1B->DTC,CCU8_CC8_DTC_DTE2_Pos);
//
//	//Dead Time enabled for non inverted output CCU81.OUT03
//	SET_BIT(PWM_1A_1B->DTC,CCU8_CC8_DTC_DCEN3_Pos);
//	//Dead Time enabled for inverted output CCU81.OUT02
//	SET_BIT(PWM_1A_1B->DTC,CCU8_CC8_DTC_DCEN4_Pos);

//	//Set dead time channel 2 value for Rising edge
//	WR_REG(PWM_1A_1B->DC2R,CCU8_CC8_DC2R_DT2R_Msk,CCU8_CC8_DC2R_DT2R_Pos,PWM_1A_1B_Handle.kRisingDeadTime);
//	//Set dead time channel 2 value for Falling edge
//	WR_REG(PWM_1A_1B->DC2R,CCU8_CC8_DC2R_DT2F_Msk,CCU8_CC8_DC2R_DT2F_Pos,PWM_1A_1B_Handle.kFallingDeadTime);

	//Select P5.6 Driver Pad
	WR_REG(PORT5->PDR0,PORT5_PDR0_PD6_Msk,PORT5_PDR0_PD6_Pos,0);

	//Select P5.7 Driver Pad
	WR_REG(PORT5->PDR0,PORT5_PDR0_PD7_Msk,PORT5_PDR0_PD7_Pos,0);
/************************************************************************/
}

void PWM2A2B_Init(void)
{
	//Init PWM2A P5.4 (CCU81.OUT13) dan PWM2B P5.5 (CCU81.OUT12)
	//Set passive level low P5.4
	CLR_BIT(PWM_2A_2B->PSL,CCU8_CC8_PSL_PSL22_Pos);
	//Set passive level low P5.5
	CLR_BIT(PWM_2A_2B->PSL,CCU8_CC8_PSL_PSL21_Pos);

	//Set output selector non inverted output to CCU81.OUT13
	SET_BIT(PWM_2A_2B->CHC,CCU8_CC8_CHC_OCS4_Pos);
	//Set output selector non inverted output to CCU81.OUT12
	CLR_BIT(PWM_2A_2B->CHC,CCU8_CC8_CHC_OCS3_Pos);
	//Set output selector non inverted output to CCU81.OUT12
	//CLR_BIT(CCU81_CC81->CHC,CCU8_CC8_CHC_OCS3_Pos);

//	//Enable Dead Time for Channel 2
//	SET_BIT(PWM_2A_2B->DTC,CCU8_CC8_DTC_DTE2_Pos);
//
//	//Dead Time enabled for non inverted output CCU81.OUT13
//	SET_BIT(PWM_2A_2B->DTC,CCU8_CC8_DTC_DCEN3_Pos);
//	//Dead Time enabled for inverted output CCU81.OUT12
//	SET_BIT(PWM_2A_2B->DTC,CCU8_CC8_DTC_DCEN4_Pos);

//	//Set dead time channel 2 value for Rising edge
//	WR_REG(PWM_2A_2B->DC2R,CCU8_CC8_DC2R_DT2R_Msk,CCU8_CC8_DC2R_DT2R_Pos,PWM_2A_2B_Handle.kRisingDeadTime);
//	//Set dead time channel 2 value for Falling edge
//	WR_REG(PWM_2A_2B->DC2R,CCU8_CC8_DC2R_DT2F_Msk,CCU8_CC8_DC2R_DT2F_Pos,PWM_2A_2B_Handle.kFallingDeadTime);

	//Select P5.4 Driver Pad
	WR_REG(PORT5->PDR0,PORT5_PDR0_PD4_Msk,PORT5_PDR0_PD4_Pos,0);

	//Select P5.5 Driver Pad
	WR_REG(PORT5->PDR0,PORT5_PDR0_PD5_Msk,PORT5_PDR0_PD5_Pos,0);
/************************************************************************/
	//Init PWM2A_2B_ADC (CCU41_CC41)
	//Set Passive Low Level signal
	//CLR_BIT(PWM_2A_2B_ADC->PSL,CCU4_CC4_PSL_PSL_Pos);
}

void PWM3A3B_Init(void)
{
	//Init PWM3A P5.2 (CCU81.OUT23) dan PWM3B P5.3 (CCU81.OUT22)
	//Set passive level low P5.2
	CLR_BIT(PWM_3A_3B->PSL,CCU8_CC8_PSL_PSL22_Pos);
	//Set passive level low P5.3
	CLR_BIT(PWM_3A_3B->PSL,CCU8_CC8_PSL_PSL21_Pos);

	//Set output selector non inverted output to CCU81.OUT23
	SET_BIT(PWM_3A_3B->CHC,CCU8_CC8_CHC_OCS4_Pos);
	//Set output selector non inverted output to CCU81.OUT22
	CLR_BIT(PWM_3A_3B->CHC,CCU8_CC8_CHC_OCS3_Pos);
	//Set output selector non inverted output to CCU81.OUT22
	//CLR_BIT(CCU81_CC82->CHC,CCU8_CC8_CHC_OCS3_Pos);

//	//Enable Dead Time for Channel 2
//	SET_BIT(PWM_3A_3B->DTC,CCU8_CC8_DTC_DTE2_Pos);
//
//	//Dead Time enabled for non inverted output CCU81.OUT23
//	SET_BIT(PWM_3A_3B->DTC,CCU8_CC8_DTC_DCEN3_Pos);
//	//Dead Time enabled for inverted output CCU81.OUT22
//	SET_BIT(PWM_3A_3B->DTC,CCU8_CC8_DTC_DCEN4_Pos);
//
//	//Set dead time channel 2 value for Rising edge
//	WR_REG(PWM_3A_3B->DC2R,CCU8_CC8_DC2R_DT2R_Msk,CCU8_CC8_DC2R_DT2R_Pos,PWMSP002_Handle1.kRisingDeadTime);
//	//Set dead time channel 2 value for Falling edge
//	WR_REG(PWM_3A_3B->DC2R,CCU8_CC8_DC2R_DT2F_Msk,CCU8_CC8_DC2R_DT2F_Pos,PWMSP002_Handle1.kFallingDeadTime);

	//Select P5.2 Driver Pad
	WR_REG(PORT5->PDR0,PORT5_PDR0_PD2_Msk,PORT5_PDR0_PD2_Pos,0);

	//Select P5.3 Driver Pad
	WR_REG(PORT5->PDR0,PORT5_PDR0_PD3_Msk,PORT5_PDR0_PD3_Pos,0);
}

void PWM1A1B_Trig_ADC_Init(void)
{
	//Inisialisasi Compare Match CCU81_CC80 trigger untuk ADC G0 Queue
	//Enable CCU81_CC80 compare match 2 while counting down (CCCM2_D) Interrupt
	SET_BIT(PWM_1A_1B_Handle.CC8yRegsPtr->INTE,CCU8_CC8_INTE_CMD2E_Pos);
	//Connect CCU81_CC80 compare match 2 (CCCM2) Interrupt to CCU81.CC80SR3 --> CCU81_3_IRQHandler
	WR_REG(PWM_1A_1B_Handle.CC8yRegsPtr->SRS,CCU8_CC8_SRS_CM2SR_Msk,CCU8_CC8_SRS_CM2SR_Pos,0x03);
}

void PWM2A2B_Trig_ADC_Init(void)
{
	//Inisialisasi Compare Match CCU41_CC41 trigger untuk ADC Scan Source
	//Enable CCU41_CC41 compare match while counting down (CCCM_D) Interrupt
	//SET_BIT(PWM_2A_2B_ADC_Handle.CC4yRegsPtr->INTE,CCU4_CC4_INTE_CMDE_Pos);
	//Connect CCU41_CC41 compare match (CCCM) Interrupt to CCU41.SR2 --> CCU41_2_IRQHandler
	//WR_REG(PWM_2A_2B_ADC_Handle.CC4yRegsPtr->SRS,CCU4_CC4_SRS_CMSR_Msk,CCU4_CC4_SRS_CMSR_Pos,0x02);

/************************************************************************/
	//Enable CCU81_CC81 compare match 2 while counting down (CCCM2_D) Interrupt
	SET_BIT(PWM_2A_2B_Handle.CC8yRegsPtr->INTE,CCU8_CC8_INTE_CMD2E_Pos);
	//Connect CCU81_CC81 compare match 2 (CCCM2) Interrupt to CCU81.CC81SR1 --> CCU81_1_IRQHandler
	WR_REG(PWM_2A_2B_Handle.CC8yRegsPtr->SRS,CCU8_CC8_SRS_CM2SR_Msk,CCU8_CC8_SRS_CM2SR_Pos,0x01);
}

void PWM3A3B_Trig_ADC_Init(void)
{
	//Inisialisasi Compare Match CCU81_CC82 trigger untuk ADC G3
	//Enable CCU81_CC82 compare match 2  while counting down (CCCM2_D) Interrupt
	SET_BIT(PWM_3A_3B_Handle.CC8yRegsPtr->INTE,CCU8_CC8_INTE_CMD2E_Pos);
	//Connect CCU81_CC82 compare match 2 (CCCM2) Interrupt to CCU81.CC82SR2 --> CCU81_2_IRQHandler
	WR_REG(PWM_3A_3B_Handle.CC8yRegsPtr->SRS,CCU8_CC8_SRS_CM2SR_Msk,CCU8_CC8_SRS_CM2SR_Pos,0x02);
}

//void PWM4A4B_Trig_ADC_Init(void)
//{
	//Inisialisasi Compare Match CCU81_CC83 trigger untuk ADC G0,G1,G2, dan G3
	//Enable CCU81_CC83 compare match 1  while counting down (CCCM1_D) Interrupt
	//SET_BIT(PWM_4A_4B_5A_5B_Handle.CC8yRegsPtr->INTE,CCU8_CC8_INTE_CMD1E_Pos);
	//Connect CCU81_CC83 compare match 1 (CCM1) interrupt to CCU81.CC83SR0 --> CCU81_0_IRQHandler
	//WR_REG(PWM_4A_4B_5A_5B_Handle.CC8yRegsPtr->SRS,CCU8_CC8_SRS_CM1SR_Msk,CCU8_CC8_SRS_CM1SR_Pos,0x00);
//}

void PWM_Buck_Boost_Init(void)
{
	PWM1A1B_Init();
/************************************************************************/
	PWM2A2B_Init();
/************************************************************************/
	PWM3A3B_Init();
/************************************************************************/
	//Inisialisasi start PWM untuk konfigurasi external start trigger
	PWMSP002_Start(PWM_1A_1B_API);
	PWMSP002_Start(PWM_2A_2B_API);
	PWMSP002_Start(PWM_3A_3B_API);
	PWMSP001_Start(PWM_2A_2B_PS_API);
	PWMSP001_Start(PWM_3A_3B_PS_API);
//	PWMSP001_Start(PWM_2A_2B_ADC_API);
//	PWMSP002_Start(PWM_5A_5B_4A_4B_API);
/************************************************************************/
	//Inisialisasi Global external trigger untuk PWM_1A_1B (CCU81_CC80)
	//External start function CCU81_CC80 triggered by input event 0
	WR_REG(PWM_1A_1B_Handle.CC8yRegsPtr->CMC,CCU8_CC8_CMC_STRTS_Msk,CCU8_CC8_CMC_STRTS_Pos,0x01);
	//Connect input event 0 CCU81_CC80 to SCU Global Start Control CCU81
	WR_REG(PWM_1A_1B_Handle.CC8yRegsPtr->INS,CCU8_CC8_INS_EV0IS_Msk,CCU8_CC8_INS_EV0IS_Pos,0x07);
	//Set signal active on rising edge
	WR_REG(PWM_1A_1B_Handle.CC8yRegsPtr->INS,CCU8_CC8_INS_EV0EM_Msk,CCU8_CC8_INS_EV0EM_Pos,0x01);
	//Signal active on high level
	CLR_BIT(PWM_1A_1B_Handle.CC8yRegsPtr->INS,CCU8_CC8_INS_EV0LM_Pos);
/************************************************************************/
	//Inisialisasi Global external trigger untuk PWM_2A_2B_PS (CCU41_CC40)
	//External start function CCU41_CC40 triggered by input event 0
	WR_REG(PWM_2A_2B_PS_Handle.CC4yRegsPtr->CMC,CCU4_CC4_CMC_STRTS_Msk,CCU4_CC4_CMC_STRTS_Pos,0x01);
	//Connect input event 0 CCU41_CC40 to SCU Global Start Control CCU41
	WR_REG(PWM_2A_2B_PS_Handle.CC4yRegsPtr->INS,CCU4_CC4_INS_EV0IS_Msk,CCU4_CC4_INS_EV0IS_Pos,0x08);
	//Set signal active on rising edge
	WR_REG(PWM_2A_2B_PS_Handle.CC4yRegsPtr->INS,CCU4_CC4_INS_EV0EM_Msk,CCU4_CC4_INS_EV0EM_Pos,0x01);
	//Signal active on high level
	CLR_BIT(PWM_2A_2B_PS_Handle.CC4yRegsPtr->INS,CCU4_CC4_INS_EV0LM_Pos);
/************************************************************************/
	//Inisialisasi Global external trigger untuk PWM_3A_3B_PS (CCU42_CC40)
	//External start function CCU42_CC40 triggered by input event 0
	WR_REG(PWM_3A_3B_PS_Handle.CC4yRegsPtr->CMC,CCU4_CC4_CMC_STRTS_Msk,CCU4_CC4_CMC_STRTS_Pos,0x01);
	//Connect input event 0 CCU42_CC40 to SCU Global Start Control CCU42
	WR_REG(PWM_3A_3B_PS_Handle.CC4yRegsPtr->INS,CCU4_CC4_INS_EV0IS_Msk,CCU4_CC4_INS_EV0IS_Pos,0x08);
	//Set signal active on rising edge
	WR_REG(PWM_3A_3B_PS_Handle.CC4yRegsPtr->INS,CCU4_CC4_INS_EV0EM_Msk,CCU4_CC4_INS_EV0EM_Pos,0x01);
	//Signal active on high level
	CLR_BIT(PWM_3A_3B_PS_Handle.CC4yRegsPtr->INS,CCU4_CC4_INS_EV0LM_Pos);
/************************************************************************/
	//Inisialisasi Compare Match PWM2A_2B_PS (CCU41_CC40) Start trigger untuk PWM_2A_2B (CCU81_CC81) dan PWM_2A_2B_ADC (CCU41_CC41)
	//External start function CCU81_CC81 triggered by input event 0
	WR_REG(PWM_2A_2B_Handle.CC8yRegsPtr->CMC,CCU8_CC8_CMC_STRTS_Msk,CCU8_CC8_CMC_STRTS_Pos,0x01);
	//Connect input event 0 CCU81_CC81 to CCU41.SR3
	WR_REG(PWM_2A_2B_Handle.CC8yRegsPtr->INS,CCU8_CC8_INS_EV0IS_Msk,CCU8_CC8_INS_EV0IS_Pos,0x0A);
	//Set signal active on rising edge
	WR_REG(PWM_2A_2B_Handle.CC8yRegsPtr->INS,CCU8_CC8_INS_EV0EM_Msk,CCU8_CC8_INS_EV0EM_Pos,0x01);
	//Signal active on high level
	CLR_BIT(PWM_2A_2B_Handle.CC8yRegsPtr->INS,CCU8_CC8_INS_EV0LM_Pos);

	//External start function PWM_2A_2B_ADC (CCU41_CC41) triggered by input event 0
	//WR_REG(PWM_2A_2B_ADC_Handle.CC4yRegsPtr->CMC,CCU4_CC4_CMC_STRTS_Msk,CCU4_CC4_CMC_STRTS_Pos,0x01);
	//Connect input event 0 CCU41_CC41 to CCU41_CC40_ST
	//WR_REG(PWM_2A_2B_ADC_Handle.CC4yRegsPtr->INS,CCU4_CC4_INS_EV0IS_Msk,CCU4_CC4_INS_EV0IS_Pos,0x0C);
	//Set signal active on rising edge
	//WR_REG(PWM_2A_2B_ADC_Handle.CC4yRegsPtr->INS,CCU4_CC4_INS_EV0EM_Msk,CCU4_CC4_INS_EV0EM_Pos,0x01);
	//Signal active on high level
	//CLR_BIT(PWM_2A_2B_ADC_Handle.CC4yRegsPtr->INS,CCU4_CC4_INS_EV0LM_Pos);

	//Enable CCU41_CC40 compare match event (CCCM_U) Interrupt
	SET_BIT(PWM_2A_2B_PS_Handle.CC4yRegsPtr->INTE,CCU4_CC4_INTE_CMUE_Pos);
	//Connect CCU41_CC40 compare match event (CCCM_U) Interrupt to CCU41.SR3
	WR_REG(PWM_2A_2B_PS_Handle.CC4yRegsPtr->SRS,CCU4_CC4_SRS_CMSR_Msk,CCU4_CC4_SRS_CMSR_Pos,0x03);

/************************************************************************/
	//Inisialisasi Compare Match CCU42_CC40 Start trigger untuk PWM_3A_3B (CCU81_CC82)
	//External start function CCU81_CC82 triggered by input event 0
	WR_REG(PWM_3A_3B_Handle.CC8yRegsPtr->CMC,CCU8_CC8_CMC_STRTS_Msk,CCU8_CC8_CMC_STRTS_Pos,0x01);
	//Connect input event 0 CCU81_CC82 to CCU42.SR3
	WR_REG(PWM_3A_3B_Handle.CC8yRegsPtr->INS,CCU8_CC8_INS_EV0IS_Msk,CCU8_CC8_INS_EV0IS_Pos,0x0A);
	//Set signal active on rising edge
	WR_REG(PWM_3A_3B_Handle.CC8yRegsPtr->INS,CCU8_CC8_INS_EV0EM_Msk,CCU8_CC8_INS_EV0EM_Pos,0x01);
	//Signal active on high level
	CLR_BIT(PWM_3A_3B_Handle.CC8yRegsPtr->INS,CCU8_CC8_INS_EV0LM_Pos);

	//Enable CCU42_CC40 compare match event (CCCM_U) Interrupt
	SET_BIT(PWM_3A_3B_PS_Handle.CC4yRegsPtr->INTE,CCU4_CC4_INTE_CMUE_Pos);
	//Connect CCU42_CC40 compare match event (CCCM_U) Interrupt to CCU42.SR3
	WR_REG(PWM_3A_3B_PS_Handle.CC4yRegsPtr->SRS,CCU4_CC4_SRS_CMSR_Msk,CCU4_CC4_SRS_CMSR_Pos,0x03);

//	//Untuk Testing
//	//Inisialisasi Global external trigger untuk PWM_4A_4B (CCU81_CC83)
//	//External start function CCU81_CC83 triggered by input event 0
//	WR_REG(PWM_4A_4B_5A_5B_Handle.CC8yRegsPtr->CMC,CCU8_CC8_CMC_STRTS_Msk,CCU8_CC8_CMC_STRTS_Pos,0x01);
//	//Connect input event 0 CCU81_CC83 to SCU Global Start Control CC81
//	WR_REG(PWM_4A_4B_5A_5B_Handle.CC8yRegsPtr->INS,CCU8_CC8_INS_EV0IS_Msk,CCU8_CC8_INS_EV0IS_Pos,0x07);
//	//Set signal active on rising edge
//	WR_REG(PWM_4A_4B_5A_5B_Handle.CC8yRegsPtr->INS,CCU8_CC8_INS_EV0EM_Msk,CCU8_CC8_INS_EV0EM_Pos,0x01);
//	//Signal active on high level
//	CLR_BIT(PWM_4A_4B_5A_5B_Handle.CC8yRegsPtr->INS,CCU8_CC8_INS_EV0LM_Pos);
}

//void PWM_Inverter_Init(void)
//{
	//Init PWM5A P5.0 (CCU81.OUT33) dan PWM5B P5.1 (CCU81.OUT32)
	//Set passive level low P5.0
//	CLR_BIT(PWM_Inverter->PSL,CCU8_CC8_PSL_PSL21_Pos);
	//Set passive level low P5.1
//	CLR_BIT(PWM_Inverter->PSL,CCU8_CC8_PSL_PSL22_Pos);

	//Set output selector non inverted output to CCU81.OUT33
//	SET_BIT(PWM_Inverter->CHC,CCU8_CC8_CHC_OCS4_Pos);
	//Set output selector inverted output to CCU81.OUT32
//	SET_BIT(PWM_Inverter->CHC,CCU8_CC8_CHC_OCS3_Pos);

	//Enable Dead Time for Channel 2
//	SET_BIT(PWM_Inverter->DTC,CCU8_CC8_DTC_DTE2_Pos);

	//Dead Time enabled for non inverted output CCU81.OUT23
//	SET_BIT(PWM_Inverter->DTC,CCU8_CC8_DTC_DCEN3_Pos);
	//Dead Time enabled for inverted output CCU81.OUT22
//	SET_BIT(PWM_Inverter->DTC,CCU8_CC8_DTC_DCEN4_Pos);

	//Set dead time channel 2 value for Rising edge
//	WR_REG(PWM_Inverter->DC2R,CCU8_CC8_DC2R_DT2R_Msk,CCU8_CC8_DC2R_DT2R_Pos,PWMSP002_Handle0.kRisingDeadTime);
	//Set dead time channel 2 value for Falling edge
//	WR_REG(PWM_Inverter->DC2R,CCU8_CC8_DC2R_DT2F_Msk,CCU8_CC8_DC2R_DT2F_Pos,PWMSP002_Handle0.kFallingDeadTime);

	//Select P5.0 Driver Pad
//	WR_REG(PORT5->PDR0,PORT5_PDR0_PD0_Msk,PORT5_PDR0_PD0_Pos,0);

	//Select P5.1 Driver Pad
//	WR_REG(PORT5->PDR0,PORT5_PDR0_PD1_Msk,PORT5_PDR0_PD1_Pos,0);
/************************************************************************/
	//Init PWM4A P6.0 (CCU81.OUT31) dan PWM4B P6.1 (CCU81.OUT30)
	//Set passive level low P6.0
//	CLR_BIT(PWM_Inverter->PSL,CCU8_CC8_PSL_PSL11_Pos);
	//Set passive level low P6.1
//	CLR_BIT(PWM_Inverter->PSL,CCU8_CC8_PSL_PSL12_Pos);

	//Set output selector non inverted output to CCU81.OUT31
//	SET_BIT(PWM_Inverter->CHC,CCU8_CC8_CHC_OCS2_Pos);
	//Set output selector inverted output to CCU81.OUT30
//	SET_BIT(PWM_Inverter->CHC,CCU8_CC8_CHC_OCS1_Pos);

	//Enable Dead Time for Channel 1
//	SET_BIT(PWM_Inverter->DTC,CCU8_CC8_DTC_DTE1_Pos);

	//Dead Time enabled for non inverted output CCU81.OUT31
//	SET_BIT(PWM_Inverter->DTC,CCU8_CC8_DTC_DCEN1_Pos);
	//Dead Time enabled for inverted output CCU81.OUT30
//	SET_BIT(PWM_Inverter->DTC,CCU8_CC8_DTC_DCEN2_Pos);

	//Set dead time channel 2 value for Rising edge
//	WR_REG(PWM_Inverter->DC1R,CCU8_CC8_DC1R_DT1R_Msk,CCU8_CC8_DC1R_DT1R_Pos,PWMSP002_Handle0.kRisingDeadTime);
	//Set dead time channel 2 value for Falling edge
//	WR_REG(PWM_Inverter->DC1R,CCU8_CC8_DC1R_DT1F_Msk,CCU8_CC8_DC1R_DT1F_Pos,PWMSP002_Handle0.kFallingDeadTime);

	//Select P6.0 Driver Pad
//	WR_REG(PORT6->PDR0,PORT6_PDR0_PD0_Msk,PORT6_PDR0_PD0_Pos,0);

	//Select P6.1 Driver Pad
//	WR_REG(PORT6->PDR0,PORT6_PDR0_PD1_Msk,PORT6_PDR0_PD1_Pos,0);
/************************************************************************/
//	PWM4A4B_Trig_ADC_Init();
//}

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

uint32_t PWMSP001_Calculate_Compare_Value(const PWMSP001_HandleType* HandlePtr,float DC)
{
	uint32_t period;

	PWMSP001_GetPeriodReg(HandlePtr,&period);

	if((DC>100.0f) || (DC<0.0f))
	{
		DC=0.0f;
	}
	return ((uint32_t)((1.0f-(DC/100.0f))*(float)period));
}

uint32_t PWMSP001_Calculate_Compare_Value_Float(const PWMSP001_HandleType* HandlePtr,float DC)
{
	uint32_t period;

	PWMSP001_GetPeriodReg(HandlePtr,&period);

	return ((uint32_t)((1.0f-DC)*(float)period));
}

void PWM_Buck_Disable(void)
{
	//Disable output PWM untuk PWM1A, PWM2A, dan PWM3A
	//Enable P5.6 as Direct I/O Push-Pull Output
	WR_REG(PORT5->IOCR4,PORT5_IOCR4_PC6_Msk,PORT5_IOCR4_PC6_Pos,0x10);
	CLR_BIT(PORT5->OUT,PORT5_OUT_P6_Pos);
	//Enable P5.4 as Direct I/O Push-Pull Output
	WR_REG(PORT5->IOCR4,PORT5_IOCR4_PC4_Msk,PORT5_IOCR4_PC4_Pos,0x10);
	CLR_BIT(PORT5->OUT,PORT5_OUT_P4_Pos);
	//Enable P5.2 as Direct I/O Push-Pull Output
	WR_REG(PORT5->IOCR0,PORT5_IOCR0_PC2_Msk,PORT5_IOCR0_PC2_Pos,0x10);
	CLR_BIT(PORT5->OUT,PORT5_OUT_P2_Pos);
}

void PWM_Buck_Enable(void)
{
	//Enable output PWM untuk PWM1A, PWM2A, dan PWM3A
	//Enable P5.6 as Push-Pull Output and Connect CCU81.OUT03 to P5.6
	WR_REG(PORT5->IOCR4,PORT5_IOCR4_PC6_Msk,PORT5_IOCR4_PC6_Pos,0x13);
	//Enable P5.4 as Push-Pull Output and Connect CCU81.OUT13 to P5.4
	WR_REG(PORT5->IOCR4,PORT5_IOCR4_PC4_Msk,PORT5_IOCR4_PC4_Pos,0x13);
	//Enable P5.2 as Push-Pull Output and Connect CCU81.OUT23 to P5.2
	//WR_REG(PORT5->IOCR0,PORT5_IOCR0_PC2_Msk,PORT5_IOCR0_PC2_Pos,0x13);
}

void PWM_Boost_Disable(void)
{
	//Disable output PWM untuk PWM1B, PWM2B, dan PWM3B
	//Enable P5.7 as Direct I/O Push-Pull Output
	WR_REG(PORT5->IOCR4,PORT5_IOCR4_PC7_Msk,PORT5_IOCR4_PC7_Pos,0x10);
	CLR_BIT(PORT5->OUT,PORT5_OUT_P7_Pos);
	//Enable P5.5 as Direct I/O Push-Pull Output
	WR_REG(PORT5->IOCR4,PORT5_IOCR4_PC5_Msk,PORT5_IOCR4_PC5_Pos,0x10);
	CLR_BIT(PORT5->OUT,PORT5_OUT_P5_Pos);
	//Enable P5.3 as Direct I/O Push-Pull Output
	WR_REG(PORT5->IOCR0,PORT5_IOCR0_PC3_Msk,PORT5_IOCR0_PC3_Pos,0x10);
	CLR_BIT(PORT5->OUT,PORT5_OUT_P3_Pos);
}

void PWM_Boost_Enable(void)
{
	//Enable output PWM untuk PWM1B, PWM2B, dan PWM3B
	//Enable P5.7 as Push-Pull Output and Connect CCU81.OUT02 to P5.7
	WR_REG(PORT5->IOCR4,PORT5_IOCR4_PC7_Msk,PORT5_IOCR4_PC7_Pos,0x13);
	//Enable P5.5 as Push-Pull Output and Connect CCU81.OUT12 to P5.5
	WR_REG(PORT5->IOCR4,PORT5_IOCR4_PC5_Msk,PORT5_IOCR4_PC5_Pos,0x13);
	//Enable P5.3 as Push-Pull Output and Connect CCU81.OUT22 to P5.3
	//WR_REG(PORT5->IOCR0,PORT5_IOCR0_PC3_Msk,PORT5_IOCR0_PC3_Pos,0x13);
}

void PWM_Buck_Boost_Stop(void)
{
	PWMSP002_Stop(PWM_1A_1B_API);
	PWMSP002_Stop(PWM_2A_2B_API);
	PWMSP002_Stop(PWM_3A_3B_API);
	PWM_Buck_Disable();
	PWM_Boost_Disable();
}

void PWM1A1B_Set_Compare(uint32_t compare1,uint32_t compare2)
{
	//PWM_1A_1B CC81_CC80
	//Set value to compare shadow register

	PWM_1A_1B->CR1S=compare1;
	PWM_1A_1B->CR2S=compare2;

	//Trigger shadow register transfer
	CCU81->GCSS |=((uint32_t)(1 << CCU8_GCSS_S0SE_Pos) &CCU8_GCSS_S0SE_Msk);
}

void PWM2A2B_Set_Compare(uint32_t compare1,uint32_t compare2)
{
	//PWM_2A_2B CC81_CC81
	//Set value to compare shadow register
	PWM_2A_2B->CR1S=compare1;
	PWM_2A_2B->CR2S=compare2;

//	PWM_2A_2B_ADC->CRS=compare2;

	//Trigger shadow register transfer
	CCU81->GCSS |=((uint32_t)(1 << CCU8_GCSS_S1SE_Pos) &CCU8_GCSS_S1SE_Msk);
	//Trigger shadow register transfer
	CCU41->GCSS |=((uint32_t)(1 << CCU4_GCSS_S1SE_Pos) &CCU4_GCSS_S1SE_Msk);
}

void PWM3A3B_Set_Compare(uint32_t compare1,uint32_t compare2)
{
	//PWM_3A_3B CC81_CC82
	//Set value to compare shadow register
	PWM_3A_3B->CR1S=compare1;
	PWM_3A_3B->CR2S=compare2;

	//Trigger shadow register transfer
	CCU81->GCSS |=((uint32_t)(1 << CCU8_GCSS_S2SE_Pos) &CCU8_GCSS_S2SE_Msk);
}

//void PWMInverter_Set_Compare(uint32_t compare1, uint32_t compare2)
//{
	//PWM_3A_3B CC81_CC82
	//Set value to compare shadow register
//	PWM_Inverter->CR1S= compare1;
//	PWM_Inverter->CR2S= compare2; //2812;

	//Trigger shadow register transfer
//	CCU81->GCSS |=((uint32_t)(1 << CCU8_GCSS_S3SE_Pos) &CCU8_GCSS_S3SE_Msk);
//}

void PWM_Boost_Init(void)
{
	uint32_t compare_value;

	PWM_Buck_Boost_Stop();
	PWM_Buck_Boost_Init();
	PWM_Buck_Disable();
	PWM_Boost_Enable();

	PWM1A1B_Trig_ADC_Init();
	PWM2A2B_Trig_ADC_Init();
	PWM3A3B_Trig_ADC_Init();

	compare_value=PWMSP002_Calculate_Compare_Value(PWM_1A_1B_API,0.0f);
	PWM1A1B_Set_Compare(compare_value,compare_value);

	compare_value=PWMSP002_Calculate_Compare_Value(PWM_2A_2B_API,0.0f);
	PWM2A2B_Set_Compare(compare_value,compare_value);

	compare_value=PWMSP002_Calculate_Compare_Value(PWM_3A_3B_API,0.0f);
	PWM3A3B_Set_Compare(compare_value,compare_value);

	compare_value=PWMSP001_Calculate_Compare_Value(PWM_2A_2B_PS_API,0.0f);
	PWMSP001_SetCompare(PWM_2A_2B_PS_API,compare_value);
	CCU41->GCSS |= (uint32_t) ((1 << CCU4_GCSS_S0SE_Pos) &CCU4_GCSS_S0SE_Msk);

	compare_value=PWMSP001_Calculate_Compare_Value(PWM_3A_3B_PS_API,0.0f);
	PWMSP001_SetCompare(PWM_3A_3B_PS_API,compare_value);
	CCU42->GCSS |= (uint32_t) ((1 << CCU4_GCSS_S0SE_Pos) &CCU4_GCSS_S0SE_Msk);
}

void PWM_Boost_Start(void)
{

	//PWM_App_Run_Stat=1;
	//Start Global CCUCON Register untuk CC81, CC42, dan CC41
	SCU_GENERAL->CCUCON |=(uint32_t)
			(((uint32_t) 1 << SCU_GENERAL_CCUCON_GSC81_Pos) &SCU_GENERAL_CCUCON_GSC81_Msk)|\
			(((uint32_t) 1 << SCU_GENERAL_CCUCON_GSC42_Pos) &SCU_GENERAL_CCUCON_GSC42_Msk)|\
			(((uint32_t) 1 << SCU_GENERAL_CCUCON_GSC41_Pos) &SCU_GENERAL_CCUCON_GSC41_Msk);
}

void PWM_Buck_Init(void)
{
	uint32_t compare_value;

	PWM_Buck_Boost_Stop();
	PWM_Buck_Boost_Init();
	PWM_Boost_Disable();
	PWM_Buck_Enable();

	PWM1A1B_Trig_ADC_Init();
	PWM2A2B_Trig_ADC_Init();
	PWM3A3B_Trig_ADC_Init();

	compare_value=PWMSP002_Calculate_Compare_Value(PWM_1A_1B_API,0.0f);
	PWM1A1B_Set_Compare(compare_value,compare_value);

	compare_value=PWMSP002_Calculate_Compare_Value(PWM_2A_2B_API,0.0f);
	PWM2A2B_Set_Compare(compare_value,compare_value);

	compare_value=PWMSP002_Calculate_Compare_Value(PWM_3A_3B_API,0.0f);
	PWM3A3B_Set_Compare(compare_value,compare_value);

	compare_value=PWMSP001_Calculate_Compare_Value(PWM_2A_2B_PS_API,0.0f);
	PWMSP001_SetCompare(PWM_2A_2B_PS_API,compare_value);
	CCU41->GCSS |= (uint32_t) ((1 << CCU4_GCSS_S0SE_Pos) &CCU4_GCSS_S0SE_Msk);

	compare_value=PWMSP001_Calculate_Compare_Value(PWM_3A_3B_PS_API,0.0f);
	PWMSP001_SetCompare(PWM_3A_3B_PS_API,compare_value);
	CCU42->GCSS |= (uint32_t) ((1 << CCU4_GCSS_S0SE_Pos) &CCU4_GCSS_S0SE_Msk);
}

void PWM_Buck_Start(void)
{
	//PWM_App_Run_Stat=1;
	//Start Global CCUCON Register untuk CC81, CC42, dan CC41
	SCU_GENERAL->CCUCON |=(uint32_t)
			(((uint32_t) 1 << SCU_GENERAL_CCUCON_GSC81_Pos) &SCU_GENERAL_CCUCON_GSC81_Msk)|\
			(((uint32_t) 1 << SCU_GENERAL_CCUCON_GSC42_Pos) &SCU_GENERAL_CCUCON_GSC42_Msk)|\
			(((uint32_t) 1 << SCU_GENERAL_CCUCON_GSC41_Pos) &SCU_GENERAL_CCUCON_GSC41_Msk);
}

//void PWM_Inverter_Enable(void)
//{
	//Enable P5.0 as Push-Pull Output and Connect CCU81.OUT33 to P5.0
//	WR_REG(PORT5->IOCR0,PORT5_IOCR0_PC0_Msk,PORT5_IOCR0_PC0_Pos,0x13);
	//Enable P5.1 as Push-Pull Output and Connect CCU81.OUT32 to P5.1
//	WR_REG(PORT5->IOCR0,PORT5_IOCR0_PC1_Msk,PORT5_IOCR0_PC1_Pos,0x13);

	//Enable P6.0 as Push-Pull Output and Connect CCU81.OUT31 to P6.0
//	WR_REG(PORT6->IOCR0,PORT6_IOCR0_PC0_Msk,PORT6_IOCR0_PC0_Pos,0x13);
	//Enable P6.1 as Push-Pull Output and Connect CCU81.OUT30 to P6.1
//	WR_REG(PORT6->IOCR0,PORT6_IOCR0_PC1_Msk,PORT6_IOCR0_PC1_Pos,0x13);
//}

//void PWM_Inverter_Disable(void)
//{
	//Enable P5.0 as Push-Pull Output and Connect CCU81.OUT33 to P5.0
//	WR_REG(PORT5->IOCR0,PORT5_IOCR0_PC0_Msk,PORT5_IOCR0_PC0_Pos,0x10);
//	CLR_BIT(PORT5->OUT,PORT5_OUT_P0_Pos);
	//Enable P5.1 as Push-Pull Output and Connect CCU81.OUT32 to P5.1
//	WR_REG(PORT5->IOCR0,PORT5_IOCR0_PC1_Msk,PORT5_IOCR0_PC1_Pos,0x10);
//	CLR_BIT(PORT5->OUT,PORT5_OUT_P1_Pos);
	//Enable P6.0 as Push-Pull Output and Connect CCU81.OUT31 to P6.0
//	WR_REG(PORT6->IOCR0,PORT6_IOCR0_PC0_Msk,PORT6_IOCR0_PC0_Pos,0x10);
//	CLR_BIT(PORT6->OUT,PORT6_OUT_P0_Pos);
	//Enable P6.1 as Push-Pull Output and Connect CCU81.OUT30 to P6.1
//	WR_REG(PORT6->IOCR0,PORT6_IOCR0_PC1_Msk,PORT6_IOCR0_PC1_Pos,0x10);
//	CLR_BIT(PORT6->OUT,PORT6_OUT_P1_Pos);
//}

//void PWM_Inverter_Start(void)
//{
//	PWM_Inverter_Enable();
//	PWMInverter_Set_Compare(PWMSP002_Calculate_Compare_Value(PWM_5A_5B_4A_4B_API,0.0f),PWMSP002_Calculate_Compare_Value(PWM_5A_5B_4A_4B_API,0.0f));
	//PWM_App_Run_Stat=1;
//	PWMSP002_Start(PWM_5A_5B_4A_4B_API);
//}

//void PWM_Inverter_Stop(void)
//{
//	PWMSP002_Stop(PWM_5A_5B_4A_4B_API);
//	PWM_Inverter_Disable();
//}

void PWM_Enable_Interrupt_NVIC(void)
{
	//Set Interrupt Priority
	NVIC_SetPriority(CCU81_3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),58,0));
	//Enable CCU81_0_IRQHandler (PWM_1A_1B) interrupt
	NVIC_EnableIRQ(CCU81_3_IRQn);

	//Set Interrupt Priority
	NVIC_SetPriority(CCU81_1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),58,0));
	//Enable CCU81_1_IRQHandler (PWM_2A_2B) interrupt
	NVIC_EnableIRQ(CCU81_1_IRQn);

	//Set Interrupt Priority
	NVIC_SetPriority(CCU81_2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),58,0));
	//Enable CCU81_2_IRQHandler (PWM_3A_3B) interrupt
	NVIC_EnableIRQ(CCU81_2_IRQn);

	//Set Interrupt Priority
//	NVIC_SetPriority(CCU81_0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),59,0));
	//Enable CCU81_3_IRQHandler (PWM_Inverter) interrupt
//	NVIC_EnableIRQ(CCU81_0_IRQn);
}

void PWM_Disable_Interrupt_NVIC(void)
{
	//Enable CCU81_0_IRQHandler (PWM_1A_1B) interrupt
	NVIC_DisableIRQ(CCU81_3_IRQn);
	//Enable CCU81_1_IRQHandler (PWM_2A_2B) interrupt
	NVIC_DisableIRQ(CCU81_1_IRQn);
	//Enable CCU81_2_IRQHandler (PWM_3A_3B) interrupt
	NVIC_DisableIRQ(CCU81_2_IRQn);
	//Enable CCU81_3_IRQHandler (PWM_Inverter) interrupt
	//NVIC_DisableIRQ(CCU81_0_IRQn);
}

