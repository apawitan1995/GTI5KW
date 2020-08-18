/*
 * delay.c
 *
 *  Created on: Apr 8, 2014
 *      Author: angga
 */
#include "common/delay.h"

void My_Delay_us(int16_t n)
{
	int32_t delay;

	delay=120*n;
	//Use SysTick Timer to generate timing
	//Program reload value counter
	WR_REG(PPB->SYST_RVR,PPB_SYST_RVR_RELOAD_Msk,PPB_SYST_RVR_RELOAD_Pos,16777215);
	//Clear current value
	WR_REG(PPB->SYST_CVR,PPB_SYST_CVR_CURRENT_Msk,PPB_SYST_CVR_CURRENT_Pos,0);
	//Disable SysTick exception request
	CLR_BIT(PPB->SYST_CSR,PPB_SYST_CSR_TICKINT_Pos);
	//Select fCPU clock source
	SET_BIT(PPB->SYST_CSR,PPB_SYST_CSR_CLKSOURCE_Pos);
	//Enable Timer
	SET_BIT(PPB->SYST_CSR,PPB_SYST_CSR_ENABLE_Pos);

	while((16777215-PPB->SYST_CVR)<delay);

	//Disable Timer
	CLR_BIT(PPB->SYST_CSR,PPB_SYST_CSR_ENABLE_Pos);
}

void My_Delay_ms(int8_t n)
{
	//Maximum of 138 ms
	int32_t delay;

	delay=120000*n;
	//Use SysTick Timer to generate timing
	//Program reload value counter
	WR_REG(PPB->SYST_RVR,PPB_SYST_RVR_RELOAD_Msk,PPB_SYST_RVR_RELOAD_Pos,16777215);
	//Clear current value
	WR_REG(PPB->SYST_CVR,PPB_SYST_CVR_CURRENT_Msk,PPB_SYST_CVR_CURRENT_Pos,0);
	//Disable SysTick exception request
	CLR_BIT(PPB->SYST_CSR,PPB_SYST_CSR_TICKINT_Pos);
	//Select fCPU clock source
	SET_BIT(PPB->SYST_CSR,PPB_SYST_CSR_CLKSOURCE_Pos);
	//Enable Timer
	SET_BIT(PPB->SYST_CSR,PPB_SYST_CSR_ENABLE_Pos);

	while((16777215-PPB->SYST_CVR)<delay);

	//Disable Timer
	CLR_BIT(PPB->SYST_CSR,PPB_SYST_CSR_ENABLE_Pos);
}

void My_Delay_s(int16_t n)
{
	int32_t j=0;

	for(j=0;j<10*n;j++)
	{
		My_Delay_ms(100);
	}
}
