/*
 * DAC_app.c
 *
 *  Created on: Feb 14, 2014
 *      Author: Hanif
 */

#include "peripheral/DAC_app.h"

void DAC_Init(void)
{
	//De-assert DAC module
	SET_BIT(SCU_RESET->PRCLR1,SCU_RESET_PRCLR1_DACRS_Pos);
	//Configure Single Value Mode
	WR_REG(DAC->DAC0CFG0,DAC_DAC0CFG0_MODE_Msk,DAC_DAC0CFG0_MODE_Pos,1);
	WR_REG(DAC->DAC1CFG0,DAC_DAC1CFG0_MODE_Msk,DAC_DAC1CFG0_MODE_Pos,1);

	//Enable DAC
	SET_BIT(DAC->DAC0CFG1,DAC_DAC0CFG1_ANAEN_Pos);
	SET_BIT(DAC->DAC1CFG1,DAC_DAC1CFG1_ANAEN_Pos);
}

void DAC0_Output(uint32_t val)
{
	DAC->DAC0DATA=val;
}

void DAC1_Output(uint32_t val)
{
	DAC->DAC1DATA=val;
}

void DAC0_Output_Volt(float volt)
{
	char kal[32];
	uint32_t bit_val;
	uint32_t volt_int;

	bit_val = (uint32_t)(4095.0f*((volt-0.3074f)/2.1363f));

	if(bit_val>4095)
	{bit_val=4095;}
	else if(bit_val<0)
	{bit_val=0;}

	DAC->DAC0DATA=bit_val;
}

void DAC1_Output_Volt(float volt)
{
	char kal[32];
	uint32_t bit_val;
	uint32_t volt_int;

	bit_val = (uint32_t)(4095.0f*((volt-0.3082f)/2.1366f));

	if(bit_val>4095)
	{bit_val=4095;}
	else if(bit_val<0)
	{bit_val=0;}

	DAC->DAC1DATA=bit_val;
}

