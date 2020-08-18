/*
 * ADC_app.c
 *
 *  Created on: Dec 12, 2013
 *      Author: Hanif
 */

#include "peripheral/ADC_app.h"

volatile int32_t DCPT1_fix;
volatile int32_t DCPT2_fix;
volatile int32_t ACPT1_fix;
volatile int32_t ACPT2_fix;
volatile int32_t ACPT3_fix;
volatile int32_t GRPT1_fix;
volatile int32_t GRPT2_fix;
volatile int32_t GRPT3_fix;
volatile int32_t CT1_fix;
volatile int32_t CT2_fix;
volatile int32_t CT3_fix;
volatile int32_t CT4_fix;
volatile int32_t CT5_fix;
volatile int32_t CT6_fix;
volatile int32_t CT7_fix;
volatile int32_t CT8_fix;
volatile int32_t TQ1_fix;
volatile int32_t TQ2_fix;
volatile int32_t AIS1_fix;
volatile int32_t AIS2_fix;


//volatile int32_t DCPT1_OFFSET;
//volatile int32_t DCPT2_OFFSET;
volatile int32_t ACPT1_OFFSET;
//volatile int32_t ACPT2_OFFSET;
volatile int32_t ACPT3_OFFSET;
volatile int32_t GRPT1_OFFSET;
//volatile int32_t GRPT2_OFFSET;
volatile int32_t GRPT3_OFFSET;
volatile int32_t CT1_OFF;
volatile int32_t CT2_OFF;
volatile int32_t CT3_OFF;
volatile int32_t CT4_OFF;
volatile int32_t CT5_OFF;
volatile int32_t CT6_OFF;
volatile int32_t CT7_OFF;
volatile int32_t CT8_OFF;


volatile int32_t CT1_Result=0;
volatile int32_t CT2_Result=0; // 30 A
volatile int32_t CT3_Result=0;
volatile int32_t CT4_Result=0;
volatile int32_t CT5_Result=0;
volatile int32_t CT6_Result=0;
volatile int32_t CT7_Result=0;
volatile int32_t CT8_Result=0;
volatile int32_t TQ1_Result=0;
volatile int32_t TQ2_Result=0;

volatile int32_t DCPT1_AIN_Result=0;
volatile int32_t DCPT2_AIN_Result=0;
volatile int32_t ACPT1_AIN_Result=0;
volatile int32_t ACPT2_AIN_Result=0;
volatile int32_t ACPT3_AIN_Result=0;
volatile int32_t GRPT1_AIN_Result=0;
volatile int32_t GRPT2_AIN_Result=0;
volatile int32_t GRPT3_AIN_Result=0;

volatile int32_t TQ1_AIN_Result=0;
volatile int32_t TQ2_AIN_Result=0;
volatile int32_t AIS1_AIN_Result=0;
volatile int32_t AIS2_AIN_Result=0;

void ADC_G0_Queue_Init(void)
{
	//Clear all input queue first
	SET_BIT(VADC_G0->QMR0,VADC_G_QMR0_FLUSH_Pos);
	//Select input Queue
	//Channel number
	//Automatic refill
	//Disable source interrupt
	//Enable external trigger
	VADC_G0->QINR0=(uint32_t)
		 ((((uint32_t)0\
			   << VADC_G_QINR0_REQCHNR_Pos) &VADC_G_QINR0_REQCHNR_Msk ) |\
		 (((uint32_t)1\
			   << VADC_G_QINR0_RF_Pos) &VADC_G_QINR0_RF_Msk)|\
		 (((uint32_t)0\
			   << VADC_G_QINR0_ENSI_Pos) &VADC_G_QINR0_ENSI_Msk )     |\
		 (((uint32_t)1\
			   << VADC_G_QINR0_EXTR_Pos) & VADC_G_QINR0_EXTR_Msk));

	VADC_G0->QINR0=(uint32_t)
			 ((((uint32_t)1\
				   << VADC_G_QINR0_REQCHNR_Pos) &VADC_G_QINR0_REQCHNR_Msk ) |\
			 (((uint32_t)1\
				   << VADC_G_QINR0_RF_Pos) &VADC_G_QINR0_RF_Msk)|\
			 (((uint32_t)0\
				   << VADC_G_QINR0_ENSI_Pos) &VADC_G_QINR0_ENSI_Msk )     |\
			 (((uint32_t)0\
				   << VADC_G_QINR0_EXTR_Pos) & VADC_G_QINR0_EXTR_Msk));

	VADC_G0->QINR0=(uint32_t)
			 ((((uint32_t)2\
				   << VADC_G_QINR0_REQCHNR_Pos) &VADC_G_QINR0_REQCHNR_Msk ) |\
			 (((uint32_t)1\
				   << VADC_G_QINR0_RF_Pos) &VADC_G_QINR0_RF_Msk)|\
			 (((uint32_t)0\
				   << VADC_G_QINR0_ENSI_Pos) &VADC_G_QINR0_ENSI_Msk )     |\
			 (((uint32_t)0\
				   << VADC_G_QINR0_EXTR_Pos) & VADC_G_QINR0_EXTR_Msk));

	VADC_G0->QINR0=(uint32_t)
			 ((((uint32_t)3\
				   << VADC_G_QINR0_REQCHNR_Pos) &VADC_G_QINR0_REQCHNR_Msk ) |\
			 (((uint32_t)1\
				   << VADC_G_QINR0_RF_Pos) &VADC_G_QINR0_RF_Msk)|\
			 (((uint32_t)0\
				   << VADC_G_QINR0_ENSI_Pos) &VADC_G_QINR0_ENSI_Msk )     |\
			 (((uint32_t)0\
				   << VADC_G_QINR0_EXTR_Pos) & VADC_G_QINR0_EXTR_Msk));

	VADC_G0->QINR0=(uint32_t)
			 ((((uint32_t)6\
				   << VADC_G_QINR0_REQCHNR_Pos) &VADC_G_QINR0_REQCHNR_Msk ) |\
			 (((uint32_t)1\
				   << VADC_G_QINR0_RF_Pos) &VADC_G_QINR0_RF_Msk)|\
			 (((uint32_t)0\
				   << VADC_G_QINR0_ENSI_Pos) &VADC_G_QINR0_ENSI_Msk )     |\
			 (((uint32_t)0\
				   << VADC_G_QINR0_EXTR_Pos) & VADC_G_QINR0_EXTR_Msk));

	VADC_G0->QINR0=(uint32_t)
			 ((((uint32_t)7\
				   << VADC_G_QINR0_REQCHNR_Pos) &VADC_G_QINR0_REQCHNR_Msk ) |\
			 (((uint32_t)1\
				   << VADC_G_QINR0_RF_Pos) &VADC_G_QINR0_RF_Msk)|\
			 (((uint32_t)0\
				   << VADC_G_QINR0_ENSI_Pos) &VADC_G_QINR0_ENSI_Msk )     |\
			 (((uint32_t)0\
				   << VADC_G_QINR0_EXTR_Pos) & VADC_G_QINR0_EXTR_Msk));


	//Inisialisasi Queue Entry, refill, dan External Trigger enable
	//ADC002_InitializeQueue(ADC_G0_Queue_API);

	//Koneksi External trigger start untuk ADC G0 Queue
	//CCU81.SR3 (PWM1A/1B) trigger
	VADC_G0->QCTRL0 |= (uint32_t)
			(((uint32_t) 1 << VADC_G_QCTRL0_XTWC_Pos) &VADC_G_QCTRL0_XTWC_Msk)|\
			(((uint32_t)  0x0B<< VADC_G_QCTRL0_XTSEL_Pos) &VADC_G_QCTRL0_XTSEL_Msk);

	//Trigger upon falling edge
	VADC_G0->QCTRL0 |= (uint32_t)
			(((uint32_t) 1 << VADC_G_QCTRL0_XTWC_Pos) &VADC_G_QCTRL0_XTWC_Msk)|\
			(((uint32_t)  0x01<< VADC_G_QCTRL0_XTMODE_Pos) &VADC_G_QCTRL0_XTMODE_Msk);

	//Enable external trigger
	SET_BIT(VADC_G0->QMR0,VADC_G_QMR0_ENTR_Pos);
	//Enable always Gate signal
	WR_REG(VADC_G0->QMR0,VADC_G_QMR0_ENGT_Msk,VADC_G_QMR0_ENGT_Pos,0x01);


	//Route result register assignment

	//Clear bit RESTBS to allow store result in source register
	CLR_BIT(VADC_G0->CHCTR[0],VADC_G_CHCTR_RESTBS_Pos);
	//Store result ADC G0CH0 in RESULT 0 Register
	WR_REG(VADC_G0->CHCTR[0],VADC_G_CHCTR_RESREG_Msk,VADC_G_CHCTR_RESREG_Pos,0x00);

	//Clear bit RESTBS to allow store result in source register
	CLR_BIT(VADC_G0->CHCTR[1],VADC_G_CHCTR_RESTBS_Pos);
	//Store result ADC G0CH1 in RESULT 1 Register
	WR_REG(VADC_G0->CHCTR[1],VADC_G_CHCTR_RESREG_Msk,VADC_G_CHCTR_RESREG_Pos,0x01);

	//Clear bit RESTBS to allow store result in source register
	CLR_BIT(VADC_G0->CHCTR[2],VADC_G_CHCTR_RESTBS_Pos);
	//Store result ADC G0CH2 in RESULT 2 Register
	WR_REG(VADC_G0->CHCTR[2],VADC_G_CHCTR_RESREG_Msk,VADC_G_CHCTR_RESREG_Pos,0x02);

	//Clear bit RESTBS to allow store result in source register
	CLR_BIT(VADC_G0->CHCTR[3],VADC_G_CHCTR_RESTBS_Pos);
	//Store result ADC G0CH3 in RESULT 3 Register
	WR_REG(VADC_G0->CHCTR[3],VADC_G_CHCTR_RESREG_Msk,VADC_G_CHCTR_RESREG_Pos,0x03);

	//Clear bit RESTBS to allow store result in source register
	CLR_BIT(VADC_G0->CHCTR[6],VADC_G_CHCTR_RESTBS_Pos);
	//Store result ADC G0CH6 in RESULT 6 Register
	WR_REG(VADC_G0->CHCTR[6],VADC_G_CHCTR_RESREG_Msk,VADC_G_CHCTR_RESREG_Pos,0x06);

	//Clear bit RESTBS to allow store result in source register
	CLR_BIT(VADC_G0->CHCTR[7],VADC_G_CHCTR_RESTBS_Pos);
	//Store result ADC G0CH7 in RESULT 7 Register
	WR_REG(VADC_G0->CHCTR[7],VADC_G_CHCTR_RESREG_Msk,VADC_G_CHCTR_RESREG_Pos,0x07);


/************************************************************************/
	//Activate result 6 register interrupt
	//Enable service result generation
	//SET_BIT(VADC_G0->RCR[6],VADC_G_RCR_SRGEN_Pos);
	//Route ADC_G0_RES06 to VADC.G0.SR3
	//WR_REG(VADC_G0->REVNP0,VADC_G_REVNP0_REV6NP_Msk,VADC_G_REVNP0_REV6NP_Pos,0x03);

//	//Set Interrupt Priority
//	NVIC_SetPriority(VADC0_G0_0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),63,0));
//	//Enable VADC.G0.SR0 interrupt
//	NVIC_EnableIRQ(VADC0_G0_0_IRQn);
/************************************************************************/
	//Enable arbiter for queue source control
	SET_BIT(VADC_G0->ARBPR,VADC_G_ARBPR_ASEN0_Pos);
/************************************************************************/
}

//void ADC_G0_Scan_Init(void)
//{
	//Select channel for ADC G0 Scan request
	//clear all channel
	//VADC_G0->ASSEL=0;
	//Select G0CH1
	//SET_BIT(VADC_G0->ASSEL,VADC_G_ASSEL_CHSEL1_Pos);

	//Koneksi External trigger start untuk ADC G0 Scan
	//CCU41.SR2 (PWM2A/2B) trigger
	//VADC_G0->ASCTRL |= (uint32_t)
	//				(((uint32_t) 1 << VADC_G_ASCTRL_XTWC_Pos) &VADC_G_ASCTRL_XTWC_Msk)|\
	//				(((uint32_t)  0x02<< VADC_G_ASCTRL_XTSEL_Pos) &VADC_G_ASCTRL_XTSEL_Msk);

	//Trigger upon faling edge
	//VADC_G0->ASCTRL |= (uint32_t)
	//					(((uint32_t) 1 << VADC_G_ASCTRL_XTWC_Pos) &VADC_G_ASCTRL_XTWC_Msk)|\
	//					(((uint32_t)  0x01<< VADC_G_ASCTRL_XTMODE_Pos) &VADC_G_ASCTRL_XTMODE_Msk);

	//Enable External trigger
	//SET_BIT(VADC_G0->ASMR,VADC_G_ASMR_ENTR_Pos);
	//Enable Gate signal
	//WR_REG(VADC_G0->ASMR,VADC_G_ASMR_ENGT_Msk,VADC_G_ASMR_ENGT_Pos,0x01);

	//Route result register assignment
	//Clear bit RESTBS to allow store result in source register
	//CLR_BIT(VADC_G0->CHCTR[1],VADC_G_CHCTR_RESTBS_Pos);
	//Store result ADC G0CH1 in RESULT 1 Register
	//WR_REG(VADC_G0->CHCTR[1],VADC_G_CHCTR_RESREG_Msk,VADC_G_CHCTR_RESREG_Pos,0x01);
	//Configure Result interrupt for RESG0_CH1
/************************************************************************/
//	//Enable service result generation
//	SET_BIT(VADC_G0->RCR[1],VADC_G_RCR_SRGEN_Pos);
//	//Route ADC_G0_RES01 to VADC.G0.SR1
//	WR_REG(VADC_G0->REVNP0,VADC_G_REVNP0_REV1NP_Msk,VADC_G_REVNP0_REV1NP_Pos,0x01);

//	//Set Interrupt Priority
//	NVIC_SetPriority(VADC0_G0_1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),63,0));
//	//Enable VADC.G0.SR0 interrupt
//	NVIC_EnableIRQ(VADC0_G0_1_IRQn);
/************************************************************************/
	//Enable arbiter for scan source control
	//SET_BIT(VADC_G0->ARBPR,VADC_G_ARBPR_ASEN1_Pos);
//}

void ADC_G0_Init(void)
{
	//Select channel for ADC G0
	//G0CH0
	SET_BIT(VADC_G0->CHASS,VADC_G_CHASS_ASSCH0_Pos);
	//G0CH1
	SET_BIT(VADC_G0->CHASS,VADC_G_CHASS_ASSCH1_Pos);
	//G0CH2
	SET_BIT(VADC_G0->CHASS,VADC_G_CHASS_ASSCH2_Pos);
	//G0CH3
	SET_BIT(VADC_G0->CHASS,VADC_G_CHASS_ASSCH3_Pos);
	//G0CH6
	SET_BIT(VADC_G0->CHASS,VADC_G_CHASS_ASSCH6_Pos);
	//G0CH7
	SET_BIT(VADC_G0->CHASS,VADC_G_CHASS_ASSCH7_Pos);
/************************************************************************/
	ADC_G0_Queue_Init();
/************************************************************************/
	//ADC_G0_Scan_Init();
}

//void ADC_G1_Queue_Init(void)
//{
	//Clear all input queue first
//	SET_BIT(VADC_G1->QMR0,VADC_G_QMR0_FLUSH_Pos);
	//Select input Queue
	//Channel number
	//Automatic refill
	//Disable source interrupt
	//Enable external trigger
//	VADC_G1->QINR0=(uint32_t)
//		 ((((uint32_t)2\
			   << VADC_G_QINR0_REQCHNR_Pos) &VADC_G_QINR0_REQCHNR_Msk ) |\
		 (((uint32_t)1\
			   << VADC_G_QINR0_RF_Pos) &VADC_G_QINR0_RF_Msk)|\
		 (((uint32_t)0\
			   << VADC_G_QINR0_ENSI_Pos) &VADC_G_QINR0_ENSI_Msk )     |\
		 (((uint32_t)1\
			   << VADC_G_QINR0_EXTR_Pos) & VADC_G_QINR0_EXTR_Msk));

	//Inisialisasi Queue Entry, refill, dan External Trigger enable
	//ADC002_InitializeQueue(ADC_G1_Queue_API);
	//Koneksi External trigger start untuk ADC G1 Queue
	//CCU81.SR3 (PWM1A/1B) trigger
//	VADC_G1->QCTRL0 |= (uint32_t)
//			(((uint32_t) 1 << VADC_G_QCTRL0_XTWC_Pos) &VADC_G_QCTRL0_XTWC_Msk)|\
			(((uint32_t)  0x0B<< VADC_G_QCTRL0_XTSEL_Pos) &VADC_G_QCTRL0_XTSEL_Msk);

	//Trigger upon falling edge
//	VADC_G1->QCTRL0 |= (uint32_t)
//				(((uint32_t) 1 << VADC_G_QCTRL0_XTWC_Pos) &VADC_G_QCTRL0_XTWC_Msk)|\
				(((uint32_t)  0x01<< VADC_G_QCTRL0_XTMODE_Pos) &VADC_G_QCTRL0_XTMODE_Msk);
	//Enable external trigger
//	SET_BIT(VADC_G1->QMR0,VADC_G_QMR0_ENTR_Pos);
	//Enable always Gate signal
//	WR_REG(VADC_G1->QMR0,VADC_G_QMR0_ENGT_Msk,VADC_G_QMR0_ENGT_Pos,0x01);

	//Route result register assignment
	//Clear bit RESTBS to allow store result in source register
//	CLR_BIT(VADC_G1->CHCTR[2],VADC_G_CHCTR_RESTBS_Pos);
	//Store result ADC G1CH2 in RESULT 2 Register
//	WR_REG(VADC_G1->CHCTR[2],VADC_G_CHCTR_RESREG_Msk,VADC_G_CHCTR_RESREG_Pos,0x02);
/************************************************************************/
	//Activate result 2 register interrupt
	//Enable service result generation
//	SET_BIT(VADC_G1->RCR[2],VADC_G_RCR_SRGEN_Pos);
	//Route ADC_G1_RES02 to VADC.G1.SR3
//	WR_REG(VADC_G1->REVNP0,VADC_G_REVNP0_REV2NP_Msk,VADC_G_REVNP0_REV2NP_Pos,0x03);
/************************************************************************/
	//Enable arbiter for queue source control
//	SET_BIT(VADC_G1->ARBPR,VADC_G_ARBPR_ASEN0_Pos);
//}

void ADC_G1_Scan_Init(void)
{
	//Select channel for ADC G1 Scan request
	//clear all channel
	VADC_G1->ASSEL=0;
	//Select G1CH4
	SET_BIT(VADC_G1->ASSEL,VADC_G_ASSEL_CHSEL4_Pos);
	//Select G1CH5
	SET_BIT(VADC_G1->ASSEL,VADC_G_ASSEL_CHSEL5_Pos);

	//Koneksi External trigger start untuk ADC G1 Scan
	//CCU81.SR2 (PWM3A/3B) trigger
	VADC_G1->ASCTRL |= (uint32_t)
				(((uint32_t) 1 << VADC_G_ASCTRL_XTWC_Pos) &VADC_G_ASCTRL_XTWC_Msk)|\
				(((uint32_t)  0x0A<< VADC_G_ASCTRL_XTSEL_Pos) &VADC_G_ASCTRL_XTSEL_Msk);
//				(((uint32_t) 1 << VADC_G_ASCTRL_GTWC_Pos) &VADC_G_ASCTRL_GTWC_Msk)|\
//				(((uint32_t)  0x06<< VADC_G_ASCTRL_GTSEL_Pos) &VADC_G_ASCTRL_GTSEL_Msk);

	//Trigger upon faling edge
	VADC_G1->ASCTRL |= (uint32_t)
						(((uint32_t) 1 << VADC_G_ASCTRL_XTWC_Pos) &VADC_G_ASCTRL_XTWC_Msk)|\
						(((uint32_t)  0x01<< VADC_G_ASCTRL_XTMODE_Pos) &VADC_G_ASCTRL_XTMODE_Msk);

	//Enable External trigger
	SET_BIT(VADC_G1->ASMR,VADC_G_ASMR_ENTR_Pos);
	//Enable Gate signal
	WR_REG(VADC_G1->ASMR,VADC_G_ASMR_ENGT_Msk,VADC_G_ASMR_ENGT_Pos,0x01);


	//Route result register assignment

	//Clear bit RESTBS to allow store result in source register
	CLR_BIT(VADC_G1->CHCTR[4],VADC_G_CHCTR_RESTBS_Pos);
	//Store result ADC G1CH4 in RESULT 4 Register
	WR_REG(VADC_G1->CHCTR[4],VADC_G_CHCTR_RESREG_Msk,VADC_G_CHCTR_RESREG_Pos,0x04);

	//Clear bit RESTBS to allow store result in source register
	CLR_BIT(VADC_G1->CHCTR[5],VADC_G_CHCTR_RESTBS_Pos);
	//Store result ADC G1CH5 in RESULT 5 Register
	WR_REG(VADC_G1->CHCTR[5],VADC_G_CHCTR_RESREG_Msk,VADC_G_CHCTR_RESREG_Pos,0x05);
/************************************************************************/
	//Activate result 4 register interrupt
	//Enable service result generation
	//SET_BIT(VADC_G1->RCR[4],VADC_G_RCR_SRGEN_Pos);
	//Route ADC_G1_RES03 to VADC.G1.SR0
	//WR_REG(VADC_G1->REVNP0,VADC_G_REVNP0_REV3NP_Msk,VADC_G_REVNP0_REV3NP_Pos,0x00);
/************************************************************************/
	//Enable arbiter for scan source control
	SET_BIT(VADC_G1->ARBPR,VADC_G_ARBPR_ASEN1_Pos);
}

void ADC_G1_Init(void)
{
	//Select channel for ADC G1
	//G1CH2
	//SET_BIT(VADC_G1->CHASS,VADC_G_CHASS_ASSCH2_Pos);
	//G1CH3
	//SET_BIT(VADC_G1->CHASS,VADC_G_CHASS_ASSCH3_Pos);

	//G1CH4
	SET_BIT(VADC_G1->CHASS,VADC_G_CHASS_ASSCH4_Pos);
	//G1CH5
	SET_BIT(VADC_G1->CHASS,VADC_G_CHASS_ASSCH5_Pos);

/************************************************************************/
	//ADC_G1_Queue_Init();
/************************************************************************/
	ADC_G1_Scan_Init();
/************************************************************************/
}

//void ADC_G2_Queue_Init(void)
//{
	//Clear all input queue first
//	SET_BIT(VADC_G2->QMR0,VADC_G_QMR0_FLUSH_Pos);
	//Select input Queue
	//Channel number
	//Automatic refill
	//Disable source interrupt
	//Enable external trigger
//	VADC_G2->QINR0=(uint32_t)
//		 ((((uint32_t)1\
			   << VADC_G_QINR0_REQCHNR_Pos) &VADC_G_QINR0_REQCHNR_Msk ) |\
		 (((uint32_t)1\
			   << VADC_G_QINR0_RF_Pos) &VADC_G_QINR0_RF_Msk)|\
		 (((uint32_t)0\
			   << VADC_G_QINR0_ENSI_Pos) &VADC_G_QINR0_ENSI_Msk )     |\
		 (((uint32_t)1\
			   << VADC_G_QINR0_EXTR_Pos) & VADC_G_QINR0_EXTR_Msk));

	//Koneksi External trigger start untuk ADC G2 Queue
	//CCU81.SR3 (PWM1A/1B) trigger
//	VADC_G2->QCTRL0 |= (uint32_t)
//			(((uint32_t) 1 << VADC_G_QCTRL0_XTWC_Pos) &VADC_G_QCTRL0_XTWC_Msk)|\
			(((uint32_t)  0x0B<< VADC_G_QCTRL0_XTSEL_Pos) &VADC_G_QCTRL0_XTSEL_Msk);

	//Trigger upon falling edge
//	VADC_G2->QCTRL0 |= (uint32_t)
//				(((uint32_t) 1 << VADC_G_QCTRL0_XTWC_Pos) &VADC_G_QCTRL0_XTWC_Msk)|\
				(((uint32_t)  0x01<< VADC_G_QCTRL0_XTMODE_Pos) &VADC_G_QCTRL0_XTMODE_Msk);
	//Enable external trigger
//	SET_BIT(VADC_G2->QMR0,VADC_G_QMR0_ENTR_Pos);
	//Enable always Gate signal
//	WR_REG(VADC_G2->QMR0,VADC_G_QMR0_ENGT_Msk,VADC_G_QMR0_ENGT_Pos,0x01);

	//Inisialisasi Queue Entry, refill, dan External Trigger enable
	//ADC002_InitializeQueue(ADC_G2_Queue_API);
	//Route result register assignment
	//Clear bit RESTBS to allow store result in source register
//	CLR_BIT(VADC_G2->CHCTR[1],VADC_G_CHCTR_RESTBS_Pos);
	//Store result ADC G2CH1 in RESULT 1 Register
//	WR_REG(VADC_G2->CHCTR[1],VADC_G_CHCTR_RESREG_Msk,VADC_G_CHCTR_RESREG_Pos,0x01);
/************************************************************************/
	//Activate result 1 register interrupt
	//Enable service result generation
//	SET_BIT(VADC_G2->RCR[1],VADC_G_RCR_SRGEN_Pos);
	//Route ADC_G2_RES01 to VADC.G2.SR1
//	WR_REG(VADC_G2->REVNP0,VADC_G_REVNP0_REV1NP_Msk,VADC_G_REVNP0_REV1NP_Pos,0x01);
/************************************************************************/
	//Enable arbiter for queue source control
//	SET_BIT(VADC_G2->ARBPR,VADC_G_ARBPR_ASEN0_Pos);
//}

void ADC_G2_Scan_Init(void)
{
	//Select channel for ADC G2 Scan request
	//clear all channel
	VADC_G2->ASSEL=0;
	//Select G2CH0
	SET_BIT(VADC_G2->ASSEL,VADC_G_ASSEL_CHSEL0_Pos);
	//Select G2CH1
	SET_BIT(VADC_G2->ASSEL,VADC_G_ASSEL_CHSEL1_Pos);
	//Select G2CH2
	SET_BIT(VADC_G2->ASSEL,VADC_G_ASSEL_CHSEL2_Pos);
	//Select G2CH3
	SET_BIT(VADC_G2->ASSEL,VADC_G_ASSEL_CHSEL3_Pos);
	//Select G2CH4
	SET_BIT(VADC_G2->ASSEL,VADC_G_ASSEL_CHSEL4_Pos);
	//Select G2CH5
	SET_BIT(VADC_G2->ASSEL,VADC_G_ASSEL_CHSEL5_Pos);
	//Select G2CH6
	SET_BIT(VADC_G2->ASSEL,VADC_G_ASSEL_CHSEL6_Pos);
	//Select G2CH7
	SET_BIT(VADC_G2->ASSEL,VADC_G_ASSEL_CHSEL7_Pos);


	//Koneksi External trigger start untuk ADC G2 Scan
	//CCU81.SR2 (PWM3A/3B) trigger
	VADC_G2->ASCTRL |= (uint32_t)
				(((uint32_t) 1 << VADC_G_ASCTRL_XTWC_Pos) &VADC_G_ASCTRL_XTWC_Msk)|\
				(((uint32_t)  0x0A<< VADC_G_ASCTRL_XTSEL_Pos) &VADC_G_ASCTRL_XTSEL_Msk);
//				(((uint32_t) 1 << VADC_G_ASCTRL_GTWC_Pos) &VADC_G_ASCTRL_GTWC_Msk)|\
//				(((uint32_t)  0x06<< VADC_G_ASCTRL_GTSEL_Pos) &VADC_G_ASCTRL_GTSEL_Msk);

	//Trigger upon faling edge
	VADC_G2->ASCTRL |= (uint32_t)
						(((uint32_t) 1 << VADC_G_ASCTRL_XTWC_Pos) &VADC_G_ASCTRL_XTWC_Msk)|\
						(((uint32_t)  0x01<< VADC_G_ASCTRL_XTMODE_Pos) &VADC_G_ASCTRL_XTMODE_Msk);

	//Enable External trigger
	SET_BIT(VADC_G2->ASMR,VADC_G_ASMR_ENTR_Pos);
	//Enable Gate signal
	WR_REG(VADC_G2->ASMR,VADC_G_ASMR_ENGT_Msk,VADC_G_ASMR_ENGT_Pos,0x01);


	//Route result register assignment

	//Clear bit RESTBS to allow store result in source register
	CLR_BIT(VADC_G2->CHCTR[0],VADC_G_CHCTR_RESTBS_Pos);
	//Store result ADC G2CH0 in RESULT 0 Register
	WR_REG(VADC_G2->CHCTR[0],VADC_G_CHCTR_RESREG_Msk,VADC_G_CHCTR_RESREG_Pos,0x00);

	//Clear bit RESTBS to allow store result in source register
	CLR_BIT(VADC_G2->CHCTR[1],VADC_G_CHCTR_RESTBS_Pos);
	//Store result ADC G2CH1 in RESULT 1 Register
	WR_REG(VADC_G2->CHCTR[1],VADC_G_CHCTR_RESREG_Msk,VADC_G_CHCTR_RESREG_Pos,0x01);

	//Clear bit RESTBS to allow store result in source register
	CLR_BIT(VADC_G2->CHCTR[2],VADC_G_CHCTR_RESTBS_Pos);
	//Store result ADC G2CH2 in RESULT 2 Register
	WR_REG(VADC_G2->CHCTR[2],VADC_G_CHCTR_RESREG_Msk,VADC_G_CHCTR_RESREG_Pos,0x02);

	//Clear bit RESTBS to allow store result in source register
	CLR_BIT(VADC_G2->CHCTR[3],VADC_G_CHCTR_RESTBS_Pos);
	//Store result ADC G2CH3 in RESULT 3 Register
	WR_REG(VADC_G2->CHCTR[3],VADC_G_CHCTR_RESREG_Msk,VADC_G_CHCTR_RESREG_Pos,0x03);

	//Clear bit RESTBS to allow store result in source register
	CLR_BIT(VADC_G2->CHCTR[4],VADC_G_CHCTR_RESTBS_Pos);
	//Store result ADC G2CH4 in RESULT 4 Register
	WR_REG(VADC_G2->CHCTR[4],VADC_G_CHCTR_RESREG_Msk,VADC_G_CHCTR_RESREG_Pos,0x04);

	//Clear bit RESTBS to allow store result in source register
	CLR_BIT(VADC_G2->CHCTR[5],VADC_G_CHCTR_RESTBS_Pos);
	//Store result ADC G2CH5 in RESULT 5 Register
	WR_REG(VADC_G2->CHCTR[5],VADC_G_CHCTR_RESREG_Msk,VADC_G_CHCTR_RESREG_Pos,0x05);

	//Clear bit RESTBS to allow store result in source register
	CLR_BIT(VADC_G2->CHCTR[6],VADC_G_CHCTR_RESTBS_Pos);
	//Store result ADC G2CH6 in RESULT 6 Register
	WR_REG(VADC_G2->CHCTR[6],VADC_G_CHCTR_RESREG_Msk,VADC_G_CHCTR_RESREG_Pos,0x06);

	//Clear bit RESTBS to allow store result in source register
	CLR_BIT(VADC_G2->CHCTR[7],VADC_G_CHCTR_RESTBS_Pos);
	//Store result ADC G2CH7 in RESULT 7 Register
	WR_REG(VADC_G2->CHCTR[7],VADC_G_CHCTR_RESREG_Msk,VADC_G_CHCTR_RESREG_Pos,0x07);

/************************************************************************/
	//Activate result 0 register interrupt
	//Enable service result generation
//	SET_BIT(VADC_G2->RCR[0],VADC_G_RCR_SRGEN_Pos);
	//Route ADC_G2_RES00 to VADC.G2.SR0
//	WR_REG(VADC_G2->REVNP0,VADC_G_REVNP0_REV0NP_Msk,VADC_G_REVNP0_REV0NP_Pos,0x00);
/************************************************************************/
	//Enable arbiter for scan source control
	SET_BIT(VADC_G2->ARBPR,VADC_G_ARBPR_ASEN1_Pos);
}

void ADC_G2_Init(void)
{
	//Select channel for ADC G2
	//G2CH0
	SET_BIT(VADC_G2->CHASS,VADC_G_CHASS_ASSCH0_Pos);
	//G2CH1
	SET_BIT(VADC_G2->CHASS,VADC_G_CHASS_ASSCH1_Pos);
	//G2CH2
	SET_BIT(VADC_G2->CHASS,VADC_G_CHASS_ASSCH2_Pos);
	//G2CH3
	SET_BIT(VADC_G2->CHASS,VADC_G_CHASS_ASSCH3_Pos);
	//G2CH4
	SET_BIT(VADC_G2->CHASS,VADC_G_CHASS_ASSCH4_Pos);
	//G2CH5
	SET_BIT(VADC_G2->CHASS,VADC_G_CHASS_ASSCH5_Pos);
	//G2CH6
	SET_BIT(VADC_G2->CHASS,VADC_G_CHASS_ASSCH6_Pos);
	//G2CH7
	SET_BIT(VADC_G2->CHASS,VADC_G_CHASS_ASSCH7_Pos);
/************************************************************************/
	//ADC_G2_Queue_Init();
/************************************************************************/
	ADC_G2_Scan_Init();
/************************************************************************/
}

//void ADC_G3_Queue_Init(void)
//{
	//Clear all input queue first
//	SET_BIT(VADC_G3->QMR0,VADC_G_QMR0_FLUSH_Pos);
	//Select input Queue
	//Channel number
	//Automatic refill
	//Disable source interrupt
	//Enable external trigger
//	VADC_G3->QINR0=(uint32_t)
//		 ((((uint32_t)6\
			   << VADC_G_QINR0_REQCHNR_Pos) &VADC_G_QINR0_REQCHNR_Msk ) |\
		 (((uint32_t)1\
			   << VADC_G_QINR0_RF_Pos) &VADC_G_QINR0_RF_Msk)|\
		 (((uint32_t)0\
			   << VADC_G_QINR0_ENSI_Pos) &VADC_G_QINR0_ENSI_Msk )     |\
		 (((uint32_t)1\
			   << VADC_G_QINR0_EXTR_Pos) & VADC_G_QINR0_EXTR_Msk));

	//Koneksi External trigger start untuk ADC G3 Queue
	//CCU81.SR2 (PWM3A/3B) trigger
//	VADC_G3->QCTRL0 |= (uint32_t)
//			(((uint32_t) 1 << VADC_G_QCTRL0_XTWC_Pos) &VADC_G_QCTRL0_XTWC_Msk)|\
			(((uint32_t)  0x0A<< VADC_G_QCTRL0_XTSEL_Pos) &VADC_G_QCTRL0_XTSEL_Msk);

	//Trigger upon falling edge
//	VADC_G3->QCTRL0 |= (uint32_t)
//				(((uint32_t) 1 << VADC_G_QCTRL0_XTWC_Pos) &VADC_G_QCTRL0_XTWC_Msk)|\
				(((uint32_t)  0x01<< VADC_G_QCTRL0_XTMODE_Pos) &VADC_G_QCTRL0_XTMODE_Msk);
	//Enable external trigger
//	SET_BIT(VADC_G3->QMR0,VADC_G_QMR0_ENTR_Pos);
	//Enable always Gate signal
//	WR_REG(VADC_G3->QMR0,VADC_G_QMR0_ENGT_Msk,VADC_G_QMR0_ENGT_Pos,0x01);

	//Inisialisasi Queue Entry, refill, dan External Trigger enable
	//ADC002_InitializeQueue(ADC_G3_Queue_API);
	//Route result register assignment
	//Clear bit RESTBS to allow store result in source register
//	CLR_BIT(VADC_G3->CHCTR[6],VADC_G_CHCTR_RESTBS_Pos);
	//Store result ADC G3CH6 in RESULT 6 Register
//	WR_REG(VADC_G3->CHCTR[6],VADC_G_CHCTR_RESREG_Msk,VADC_G_CHCTR_RESREG_Pos,0x06);

	//Activate result register 6 interrupt
	//Enable service result generation
//	SET_BIT(VADC_G3->RCR[6],VADC_G_RCR_SRGEN_Pos);
	//Route ADC_G3_RES06 to VADC.G3.SR0
//	WR_REG(VADC_G3->REVNP0,VADC_G_REVNP0_REV6NP_Msk,VADC_G_REVNP0_REV6NP_Pos,0x00);

//	//Set Interrupt Priority
//	NVIC_SetPriority(VADC0_G3_0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),63,0));
//	//Enable VADC.G0.SR0 interrupt
//	NVIC_EnableIRQ(VADC0_G3_0_IRQn);

	//Enable arbiter for queue source control
//	SET_BIT(VADC_G3->ARBPR,VADC_G_ARBPR_ASEN0_Pos);
//}

//void ADC_G3_Scan_Init(void)
//{
	//Select channel for ADC G3 Scan request
	//clear all channel
//	VADC_G3->ASSEL=0;
	//Select G3CH7
//	SET_BIT(VADC_G3->ASSEL,VADC_G_ASSEL_CHSEL7_Pos);

	//Koneksi External trigger start untuk ADC G3 Scan
	//CCU81.ST3B (PWM4A/4B) trigger
//	VADC_G3->ASCTRL |= (uint32_t)
//				(((uint32_t) 1 << VADC_G_ASCTRL_XTWC_Pos) &VADC_G_ASCTRL_XTWC_Msk)|\
				(((uint32_t)  0x0F<< VADC_G_ASCTRL_XTSEL_Pos) &VADC_G_ASCTRL_XTSEL_Msk)|\
				(((uint32_t) 1 << VADC_G_ASCTRL_GTWC_Pos) &VADC_G_ASCTRL_GTWC_Msk)|\
				(((uint32_t)  0x06<< VADC_G_ASCTRL_GTSEL_Pos) &VADC_G_ASCTRL_GTSEL_Msk);

	//Trigger upon faling edge
//	VADC_G3->ASCTRL |= (uint32_t)
//						(((uint32_t) 1 << VADC_G_ASCTRL_XTWC_Pos) &VADC_G_ASCTRL_XTWC_Msk)|\
						(((uint32_t)  0x01<< VADC_G_ASCTRL_XTMODE_Pos) &VADC_G_ASCTRL_XTMODE_Msk);

	//Enable External trigger
//	SET_BIT(VADC_G3->ASMR,VADC_G_ASMR_ENTR_Pos);
	//Enable Gate signal
//	WR_REG(VADC_G3->ASMR,VADC_G_ASMR_ENGT_Msk,VADC_G_ASMR_ENGT_Pos,0x01);

	//Route result register assignment
	//Clear bit RESTBS to allow store result in source register
//	CLR_BIT(VADC_G3->CHCTR[7],VADC_G_CHCTR_RESTBS_Pos);
	//Store result ADC G3CH7 in RESULT 7 Register
//	WR_REG(VADC_G3->CHCTR[7],VADC_G_CHCTR_RESREG_Msk,VADC_G_CHCTR_RESREG_Pos,0x07);

	//Enable arbiter for scan source control
//	SET_BIT(VADC_G3->ARBPR,VADC_G_ARBPR_ASEN1_Pos);
//}

//void ADC_G3_Init(void)
//{
	//Select channel for ADC G3
	//G3CH6
//	SET_BIT(VADC_G3->CHASS,VADC_G_CHASS_ASSCH6_Pos);
	//G3CH7
//	SET_BIT(VADC_G3->CHASS,VADC_G_CHASS_ASSCH7_Pos);
/************************************************************************/
//	ADC_G3_Queue_Init();
/************************************************************************/
//	ADC_G3_Scan_Init();
/************************************************************************/
//}

void ADC_Background_Init(void)
{
	//Select channel for background source
	//G1CH6 TQ1
	CLR_BIT(VADC_G1->CHASS,VADC_G_CHASS_ASSCH6_Pos);
	//G1CH7 TQ2
	CLR_BIT(VADC_G1->CHASS,VADC_G_CHASS_ASSCH7_Pos);
	//G2CH3 AIS1
	CLR_BIT(VADC_G3->CHASS,VADC_G_CHASS_ASSCH6_Pos);
	//G2CH2 AIS2
	CLR_BIT(VADC_G3->CHASS,VADC_G_CHASS_ASSCH7_Pos);
/************************************************************************/
	//Enable Background source channel G1CH6
	SET_BIT(VADC->BRSSEL[1],VADC_BRSSEL_CHSELG6_Pos);
	//Enable Background source channel G1CH7
	SET_BIT(VADC->BRSSEL[1],VADC_BRSSEL_CHSELG7_Pos);

	//Clear bit RESTBS to allow store result in source register
	CLR_BIT(VADC_G1->CHCTR[6],VADC_G_CHCTR_RESTBS_Pos);
	//Store result ADC G0CH1 in RESULT 1 Register
	WR_REG(VADC_G1->CHCTR[6],VADC_G_CHCTR_RESREG_Msk,VADC_G_CHCTR_RESREG_Pos,0x06);

	//Clear bit RESTBS to allow store result in source register
	CLR_BIT(VADC_G1->CHCTR[7],VADC_G_CHCTR_RESTBS_Pos);
	//Store result ADC G0CH1 in RESULT 1 Register
	WR_REG(VADC_G1->CHCTR[7],VADC_G_CHCTR_RESREG_Msk,VADC_G_CHCTR_RESREG_Pos,0x07);

	//Enable arbiter for background source control
	SET_BIT(VADC_G1->ARBPR,VADC_G_ARBPR_ASEN2_Pos);
/************************************************************************/
	//Enable Background source channel G2CH3
	SET_BIT(VADC->BRSSEL[3],VADC_BRSSEL_CHSELG6_Pos);
	//Enable Background source channel G2CH2
	SET_BIT(VADC->BRSSEL[3],VADC_BRSSEL_CHSELG7_Pos);

	//Clear bit RESTBS to allow store result in source register
	CLR_BIT(VADC_G3->CHCTR[6],VADC_G_CHCTR_RESTBS_Pos);
	//Store result ADC G3CH6 in RESULT 6 Register
	WR_REG(VADC_G3->CHCTR[6],VADC_G_CHCTR_RESREG_Msk,VADC_G_CHCTR_RESREG_Pos,0x06);

	//Clear bit RESTBS to allow store result in source register
	CLR_BIT(VADC_G3->CHCTR[7],VADC_G_CHCTR_RESTBS_Pos);
	//Store result ADC G3CH7 in RESULT 7 Register
	WR_REG(VADC_G3->CHCTR[7],VADC_G_CHCTR_RESREG_Msk,VADC_G_CHCTR_RESREG_Pos,0x07);

	//Enable arbiter for background source control
	SET_BIT(VADC_G3->ARBPR,VADC_G_ARBPR_ASEN2_Pos);
/************************************************************************/
}

void ADC_Init(void)
{
	ADC_G0_Init();
	ADC_G1_Init();
	ADC_G2_Init();
	//ADC_G3_Init();
	ADC_Background_Init();
}

void ADC_SWQueue_Conv(ADC002_HandleType HandlePtr)
{
	SET_BIT(HandlePtr.VADCGroupPtr->QMR0,VADC_G_QMR0_TREV_Pos);
}

void ADC_SWScan_Conv(ADC003_HandleType HandlePtr)
{
	SET_BIT(HandlePtr.VADCGroupPtr->ASMR,VADC_G_ASMR_LDEV_Pos);
}

void ADC_SWBackground_Conv(void)
{
	SET_BIT(VADC->BRSMR,VADC_BRSMR_LDEV_Pos);
}

void VADC0_G0_0_IRQHandler(void)
{
	//Result interrupt handler for CT2 (G0CH1)
	//Check ADC Result
	if(CheckBit(VADC_G0->RESD[CT2_Result_Register],VADC_G_RESD_VF_Pos))
	{
		//Valid Result
		CT2_Result = VADC_G0->RES[CT2_Result_Register] & VADC_G_RESD_RESULT_Msk;
	}
}

void VADC0_G0_1_IRQHandler(void)
{
	//Result interrupt handler for CT3 (G0CH2)
	//Check ADC Result
	if(CheckBit(VADC_G0->RESD[CT3_Result_Register],VADC_G_RESD_VF_Pos))
	{
		//Valid Result
		CT3_Result = VADC_G0->RES[CT3_Result_Register] & VADC_G_RES_RESULT_Msk;
	}
}

void VADC0_G0_2_IRQHandler(void)
{
	//Result interrupt handler for CT4 (G0CH3)
	//Check ADC Result
	if(CheckBit(VADC_G0->RESD[CT4_Result_Register],VADC_G_RESD_VF_Pos))
	{
		//Valid Result
		CT4_Result = VADC_G0->RES[CT4_Result_Register] & VADC_G_RES_RESULT_Msk;
	}
}

volatile int buffer_dcpt1[20];
volatile int buffer_dcpt2[20];
volatile int buffer_acpt2[10];
volatile int buffer_grpt2[20];


int DCPT1_Filter(int data){
	int sum=0;
	for (int i = 19; i>=1;i--){
		buffer_dcpt1[i]=buffer_dcpt1[i-1];
	}
	buffer_dcpt1[0]=data;

	for (int i = 0; i<20;i++){
		sum=sum+buffer_dcpt1[i];
	}
	return sum*0.05;
}

int DCPT2_Filter(int data){
	int sum=0;
	for (int i = 19; i>=1;i--){
		buffer_dcpt2[i]=buffer_dcpt2[i-1];
	}
	buffer_dcpt2[0]=data;

	for (int i = 0; i<20;i++){
		sum=sum+buffer_dcpt2[i];
	}
	return sum*0.05;
}

int ACPT2_Filter(int data){
	int sum=0;
	for (int i = 9; i>=1;i--){
		buffer_acpt2[i]=buffer_acpt2[i-1];
	}
	buffer_acpt2[0]=data;

	for (int i = 0; i<10;i++){
		sum=sum+buffer_acpt2[i];
	}
	return sum*0.1;
}

int GRPT2_Filter(int data){
	int sum=0;
	for (int i = 19; i>=1;i--){
		buffer_grpt2[i]=buffer_grpt2[i-1];
	}
	buffer_grpt2[0]=data;

	for (int i = 0; i<20;i++){
		sum=sum+buffer_grpt2[i];
	}
	return sum*0.05;
}
