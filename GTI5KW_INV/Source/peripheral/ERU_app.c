/*
 * ERU_app.c
 *
 *  Created on: Jan 9, 2014
 *      Author: Hanif
 */

#include "peripheral/ERU_app.h"
#include "peripheral/PWM_app.h"

char kal[32];

volatile uint32_t fault_count = 0;

void ERU_Init(void)
{
	int8_t i;

	//ERU0 Initialization, ERU1 by dave apps

	//Select IO Function as Direct Input

	// P2.5 CT234_F -> CT234 Fault
	WR_REG(PORT2->IOCR8,PORT2_IOCR8_PC11_Msk,PORT2_IOCR8_PC11_Pos,0x00);
	// P0.9 CT56_F -> CT56 Fault
	WR_REG(PORT2->IOCR12,PORT2_IOCR12_PC12_Msk,PORT2_IOCR12_PC12_Pos,0x00);
	// P0.12 DI_GD1S -> GD1 Fault
	WR_REG(PORT0->IOCR0,PORT0_IOCR0_PC2_Msk,PORT0_IOCR0_PC2_Pos,0x01);
	// P0.2 DI_GD2S -> GD2 Fault
	WR_REG(PORT0->IOCR0,PORT0_IOCR0_PC3_Msk,PORT0_IOCR0_PC3_Pos,0x01);

	//Connect  CT234_F P2.5 to ERU0.0A2
	WR_REG(ERU0->EXISEL,ERU_EXISEL_EXS0A_Msk,ERU_EXISEL_EXS0A_Pos,0x02);
	//Connect  CT56_F P0.9 to ERU0.1B0
	WR_REG(ERU0->EXISEL,ERU_EXISEL_EXS1B_Msk,ERU_EXISEL_EXS1B_Pos,0x00);
	//Connect  DI_GD1S P0.12 to ERU0.2B2
	WR_REG(ERU0->EXISEL,ERU_EXISEL_EXS2B_Msk,ERU_EXISEL_EXS2B_Pos,0x02);
	//Connect  DI_GD2S P0.2 to ERU0.3B3
	WR_REG(ERU0->EXISEL,ERU_EXISEL_EXS3B_Msk,ERU_EXISEL_EXS3B_Pos,0x03);

	for(i=0;i<4;i++)
	{
		//Use input directly for ERU0.0x ~ ERU0.1x
		CLR_BIT(ERU0->EXICON[i],ERU_EXICON_NA_Pos);
		CLR_BIT(ERU0->EXICON[i],ERU_EXICON_NB_Pos);

		//Enable falling edge detection
		SET_BIT(ERU0->EXICON[i],ERU_EXICON_FE_Pos);

		//Enable trigger pulse output
		SET_BIT(ERU0->EXICON[i],ERU_EXICON_PE_Pos);

		//Setting Gating Output
		WR_REG(ERU0->EXOCON[i],ERU_EXOCON_GP_Msk,ERU_EXOCON_GP_Pos,0x01);
	}

//	for(i=2;i<4;i++)
//	{
		//Use input directly for ERU0.2x ~ ERU0.3x
//		CLR_BIT(ERU0->EXICON[i],ERU_EXICON_NA_Pos);
		//CLR_BIT(ERU0->EXICON[i],ERU_EXICON_NB_Pos);

		//Enable rising edge detection
//		SET_BIT(ERU0->EXICON[i],ERU_EXICON_RE_Pos);

		//Enable trigger pulse output
//		SET_BIT(ERU0->EXICON[i],ERU_EXICON_PE_Pos);

		//Setting Gating Output
//		WR_REG(ERU0->EXOCON[i],ERU_EXOCON_GP_Msk,ERU_EXOCON_GP_Pos,0x01);
//	}

	//Select A input for ERU0.0A2
	WR_REG(ERU0->EXICON[0],ERU_EXICON_SS_Msk,ERU_EXICON_SS_Pos,0x00);
	//Select A input for ERU0.1B0
	WR_REG(ERU0->EXICON[1],ERU_EXICON_SS_Msk,ERU_EXICON_SS_Pos,0x01);
	//Select A input for ERU0.2B2
	WR_REG(ERU0->EXICON[2],ERU_EXICON_SS_Msk,ERU_EXICON_SS_Pos,0x01);
	//Select A input for ERU0.3B3
	WR_REG(ERU0->EXICON[3],ERU_EXICON_SS_Msk,ERU_EXICON_SS_Pos,0x01);


	//Select trigger pulse output for ERU0.0
	WR_REG(ERU0->EXICON[0],ERU_EXICON_OCS_Msk,ERU_EXICON_OCS_Pos,0x00);
	//Select trigger pulse output for ERU0.1
	WR_REG(ERU0->EXICON[1],ERU_EXICON_OCS_Msk,ERU_EXICON_OCS_Pos,0x01);
	//Select trigger pulse output for ERU0.2
	WR_REG(ERU0->EXICON[2],ERU_EXICON_OCS_Msk,ERU_EXICON_OCS_Pos,0x02);
	//Select trigger pulse output for ERU0.3
	WR_REG(ERU0->EXICON[3],ERU_EXICON_OCS_Msk,ERU_EXICON_OCS_Pos,0x03);


	//Enable NVIC Interrupt for corresponded ERU.OUT
	/* Set Interrupt Priority for NVIC 64 Node App Instance 0  */
	NVIC_SetPriority(ERU0_0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),57,0));
	NVIC_EnableIRQ(ERU0_0_IRQn);

	NVIC_SetPriority(ERU0_1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),57,0));
	NVIC_EnableIRQ(ERU0_1_IRQn);

	NVIC_SetPriority(ERU0_2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),57,0));
	NVIC_EnableIRQ(ERU0_2_IRQn);

	NVIC_SetPriority(ERU0_3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),57,0));
	NVIC_EnableIRQ(ERU0_3_IRQn);


	//disable interrupt untuk GD1 dan GD2
	NVIC_DisableIRQ(ERU0_0_IRQn);
		NVIC_DisableIRQ(ERU0_1_IRQn);
	NVIC_DisableIRQ(ERU0_2_IRQn);
	NVIC_DisableIRQ(ERU0_3_IRQn);
}

void ERU0_0_IRQHandler(void)
{
	//C005 (CT2) OR C007 (CT4) Interrupt
	//sprintf(kal,"CT2 OR CT4 Fault\r\n");

	//C005 (CT2_F) OR C007 (CT3_CT4_F) Interrupt
	fault_count++;
	sprintf(kal,"CT2_CT3_CT4_F\r\n");
	//UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
	//fault_count++;
}


void ERU0_1_IRQHandler(void)
{
	//C008 (CT3) OR FHW Interrupt
	//sprintf(kal,"CT3 OR FHW Fault\r\n");
	fault_count++;
	sprintf(kal,"CT5_CT6_F\r\n");
	//UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
	//fault_count++;
}

void ERU0_2_IRQHandler(void)
{
	fault_count++;
	//C004 (DCPT2) Interrupt
	//PWM_Boost_Disable();
	//PWM_Disable_Interrupt_NVIC();
	//sprintf(kal,"DCPT2 Fault\r\n");

	sprintf(kal,"GD1_F\r\n");
	//UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
	//fault_count++;
}

void ERU0_3_IRQHandler(void)
{
	fault_count++;
	//C001 (CT6) Interrupt
	//sprintf(kal,"CT6 Fault\r\n");
	sprintf(kal,"GD2_F\r\n");
	//UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
	//fault_count++;
}

//void ERU1_0_IRQHandler(void)
//{
//	sprintf(kal,"AC1_AC_F\r\n");
//	UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
//	fault_count++;
//}

//void ERU1_1_IRQHandler(void)
//{
//	//C006 (CT5) Interrupt
//	sprintf(kal,"DC1_F\r\n");
//	UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
//	fault_count++;
//}

//void ERU1_2_IRQHandler(void)
//{
//	//C003 (DCPT1) Interrupt
//	sprintf(kal,"DC2_F\r\n");
//	UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
//	fault_count++;
//}

//void ERU1_3_IRQHandler(void)
//{
//	//C002 (CT1) Interrupt
//	sprintf(kal,"CT1_F\r\n");
//	UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
//	fault_count++;
//}
