/*
 * ERU_app.c
 *
 *  Created on: Jan 9, 2014
 *      Author: Hanif
 */

#include "peripheral/ERU_app.h"
#include "peripheral/PWM_app.h"

char kal[32];

volatile int kode_fault;

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
		WR_REG(PORT0->IOCR0,PORT0_IOCR0_PC2_Msk,PORT0_IOCR0_PC2_Pos,0x00);
		// P0.2 DI_GD2S -> GD2 Fault
		WR_REG(PORT0->IOCR0,PORT0_IOCR0_PC3_Msk,PORT0_IOCR0_PC3_Pos,0x00);

		// P2.3 DCPT2_F
		WR_REG(PORT2->IOCR0,PORT2_IOCR0_PC3_Msk,PORT2_IOCR0_PC3_Pos,0x00);
		// P2.4 ACPT1_F
		WR_REG(PORT2->IOCR0,PORT2_IOCR4_PC4_Msk,PORT0_IOCR0_PC3_Pos,0x00);
		// P2.7 ACPT3_F
		WR_REG(PORT2->IOCR4,PORT2_IOCR4_PC7_Msk,PORT2_IOCR4_PC7_Pos,0x00);
		// P1.1 GRPT1_F
		WR_REG(PORT1->IOCR0,PORT1_IOCR0_PC1_Msk,PORT1_IOCR0_PC1_Pos,0x00);
		// P1.2 GRPT2_F
		WR_REG(PORT1->IOCR0,PORT1_IOCR0_PC2_Msk,PORT1_IOCR0_PC2_Pos,0x00);
		// P1.3 GRPT3_F
		WR_REG(PORT1->IOCR0,PORT1_IOCR0_PC3_Msk,PORT1_IOCR0_PC3_Pos,0x00);

		//Connect  CT234_F P2.5 to ERU0.0A2
		WR_REG(ERU0->EXISEL,ERU_EXISEL_EXS0A_Msk,ERU_EXISEL_EXS0A_Pos,0x02);
		//Connect  CT56_F P0.9 to ERU0.1B0
		WR_REG(ERU0->EXISEL,ERU_EXISEL_EXS1B_Msk,ERU_EXISEL_EXS1B_Pos,0x00);
		//Connect  DI_GD1S P0.12 to ERU0.2B2
		WR_REG(ERU0->EXISEL,ERU_EXISEL_EXS2B_Msk,ERU_EXISEL_EXS2B_Pos,0x02);
		//Connect  DI_GD2S P0.2 to ERU0.3B3
		WR_REG(ERU0->EXISEL,ERU_EXISEL_EXS3B_Msk,ERU_EXISEL_EXS3B_Pos,0x03);

		//Connect  P2.4 ACPT1_F to ERU0.0B2
		WR_REG(ERU0->EXISEL,ERU_EXISEL_EXS0B_Msk,ERU_EXISEL_EXS0B_Pos,0x02);
		//Connect  P2.3 DCPT2_F to ERU0.1A2
		WR_REG(ERU0->EXISEL,ERU_EXISEL_EXS1A_Msk,ERU_EXISEL_EXS1A_Pos,0x02);
		//Connect  P1.1 GRPT1_F to ERU0.3A0
		WR_REG(ERU0->EXISEL,ERU_EXISEL_EXS3A_Msk,ERU_EXISEL_EXS3A_Pos,0x00);
		//Connect  P2.7 ACPT3_F to ERU1.1B0
		WR_REG(ERU1->EXISEL,ERU_EXISEL_EXS1B_Msk,ERU_EXISEL_EXS1B_Pos,0x00);
		//Connect  P1.3 GRPT3_F to ERU1.2A0
		WR_REG(ERU1->EXISEL,ERU_EXISEL_EXS2A_Msk,ERU_EXISEL_EXS2A_Pos,0x00);
		//Connect  P1.2 GRPT2_F to ERU1.2B0
		WR_REG(ERU1->EXISEL,ERU_EXISEL_EXS2B_Msk,ERU_EXISEL_EXS2B_Pos,0x00);

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

		for(i=0;i<4;i++)
		{
			//Use input directly for ERU1.0x ~ ERU1.1x
			CLR_BIT(ERU1->EXICON[i],ERU_EXICON_NA_Pos);
			CLR_BIT(ERU1->EXICON[i],ERU_EXICON_NB_Pos);
			//Enable falling edge detection
			SET_BIT(ERU1->EXICON[i],ERU_EXICON_FE_Pos);
			//Enable trigger pulse output
			SET_BIT(ERU1->EXICON[i],ERU_EXICON_PE_Pos);
			//Setting Gating Output
			WR_REG(ERU1->EXOCON[i],ERU_EXOCON_GP_Msk,ERU_EXOCON_GP_Pos,0x01);
		}

		//Select A OR B input for ERU0.0A2 (CT234_F) OR ERU0.0B2 (ACPT1_OV)
		WR_REG(ERU0->EXICON[0],ERU_EXICON_SS_Msk,ERU_EXICON_SS_Pos,0x02);
		//Select A OR B input for ERU0.1B0 (CT56_F) OR ERU0.1A2 (DCPT2_F)
		WR_REG(ERU0->EXICON[1],ERU_EXICON_SS_Msk,ERU_EXICON_SS_Pos,0x00);
		//Select B input for ERU0.2B2 (GD1_F)
		WR_REG(ERU0->EXICON[2],ERU_EXICON_SS_Msk,ERU_EXICON_SS_Pos,0x01);
		//Select A OR B input for ERU0.3B3 (GD2_F) OR ERU0.3A0 (GRPT1_OV)
		WR_REG(ERU0->EXICON[3],ERU_EXICON_SS_Msk,ERU_EXICON_SS_Pos,0x01);
		//Select A OR B input for ERU0.3B3 (GD2_F) OR ERU0.3A0 (GRPT1_OV)
		//WR_REG(ERU0->EXICON[3],ERU_EXICON_SS_Msk,ERU_EXICON_SS_Pos,0x02);
		//Select B input for ERU1.1B0 (ACPT3_OV)
		WR_REG(ERU1->EXICON[1],ERU_EXICON_SS_Msk,ERU_EXICON_SS_Pos,0x01);
		//Select A OR B input for ERU1.2A0 (GRPT3_OV) OR ERU1.2B0 (GRPT2_OV)
		WR_REG(ERU1->EXICON[2],ERU_EXICON_SS_Msk,ERU_EXICON_SS_Pos,0x02);

		//Select trigger pulse output for ERU0.0
		WR_REG(ERU0->EXICON[0],ERU_EXICON_OCS_Msk,ERU_EXICON_OCS_Pos,0x00);
		//Select trigger pulse output for ERU0.1
		WR_REG(ERU0->EXICON[1],ERU_EXICON_OCS_Msk,ERU_EXICON_OCS_Pos,0x01);
		//Select trigger pulse output for ERU0.2
		WR_REG(ERU0->EXICON[2],ERU_EXICON_OCS_Msk,ERU_EXICON_OCS_Pos,0x02);
		//Select trigger pulse output for ERU0.3
		WR_REG(ERU0->EXICON[3],ERU_EXICON_OCS_Msk,ERU_EXICON_OCS_Pos,0x03);
		//Select trigger pulse output for ERU1.1
		WR_REG(ERU1->EXICON[1],ERU_EXICON_OCS_Msk,ERU_EXICON_OCS_Pos,0x01);
		//Select trigger pulse output for ERU1.2
		WR_REG(ERU1->EXICON[2],ERU_EXICON_OCS_Msk,ERU_EXICON_OCS_Pos,0x02);

		//Enable NVIC Interrupt for corresponded ERU.OUT
		//Set Interrupt Priority for NVIC 64 Node App Instance 0

		NVIC_SetPriority(ERU0_0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),57,0));
		NVIC_EnableIRQ(ERU0_0_IRQn);

		NVIC_SetPriority(ERU0_1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),57,0));
		NVIC_EnableIRQ(ERU0_1_IRQn);

		NVIC_SetPriority(ERU0_2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),57,0));
		NVIC_EnableIRQ(ERU0_2_IRQn);

		NVIC_SetPriority(ERU0_3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),57,0));
		NVIC_EnableIRQ(ERU0_3_IRQn);

		NVIC_SetPriority(ERU1_1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),57,0));
		NVIC_EnableIRQ(ERU1_1_IRQn);

		NVIC_SetPriority(ERU1_2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),57,0));
		NVIC_EnableIRQ(ERU1_2_IRQn);

		//disable interrupt untuk GD1 dan GD2
		NVIC_DisableIRQ(ERU0_2_IRQn);
		NVIC_DisableIRQ(ERU0_3_IRQn);
}

void ERU0_0_IRQHandler(void)
{
	//C005 (CT2) OR C007 (CT4) Interrupt
	//sprintf(kal,"CT2 OR CT4 Fault\r\n");

	//C005 (CT2_F) OR C007 (CT3_CT4_F) Interrupt
	fault_count++;
	if(fault_count>2) {
		CLR_BIT(PORT0->OUT,13); // GST_LOW
		NVIC_DisableIRQ(ERU0_0_IRQn);
		NVIC_DisableIRQ(ERU0_1_IRQn);
		NVIC_DisableIRQ(ERU0_2_IRQn);
		NVIC_DisableIRQ(ERU0_3_IRQn);
		//flag_interrupt = 1;
		}
	sprintf(kal,"CT2_CT3_CT4_F OR ACPT2_F\r\n");
	UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
	kode_fault = 0;

}


void ERU0_1_IRQHandler(void)
{
	//C008 (CT3) OR FHW Interrupt
	//sprintf(kal,"CT3 OR FHW Fault\r\n");
	fault_count++;
	if(fault_count>2) {
		CLR_BIT(PORT0->OUT,13);
		NVIC_DisableIRQ(ERU0_1_IRQn);
		NVIC_DisableIRQ(ERU0_0_IRQn);
		NVIC_DisableIRQ(ERU0_2_IRQn);
		NVIC_DisableIRQ(ERU0_3_IRQn);
		//flag_interrupt = 1;
		}
	sprintf(kal,"CT5_CT6_F OR DCPT2_F\r\n");
	UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
	kode_fault = 1;
}

void ERU0_2_IRQHandler(void)
{
	//C004 (DCPT2) Interrupt
	//PWM_Boost_Disable();
	//PWM_Disable_Interrupt_NVIC();
	//sprintf(kal,"DCPT2 Fault\r\n");
	fault_count++;
	if(fault_count>2) {
		CLR_BIT(PORT0->OUT,13);
		NVIC_DisableIRQ(ERU0_2_IRQn);
		NVIC_DisableIRQ(ERU0_1_IRQn);
		NVIC_DisableIRQ(ERU0_0_IRQn);
		NVIC_DisableIRQ(ERU0_3_IRQn);
		//flag_interrupt = 1;
		}
	sprintf(kal,"GD1_F\r\n");
	UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
}

void ERU0_3_IRQHandler(void)
{
	//C001 (CT6) Interrupt
	//sprintf(kal,"CT6 Fault\r\n");
	fault_count++;
	if(fault_count>2) {
		CLR_BIT(PORT0->OUT,13);
		NVIC_DisableIRQ(ERU0_3_IRQn);
		NVIC_DisableIRQ(ERU0_1_IRQn);
		NVIC_DisableIRQ(ERU0_2_IRQn);
		NVIC_DisableIRQ(ERU0_0_IRQn);

		//flag_interrupt = 1;
		}
	sprintf(kal,"GD2_F\r\n");
	UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
}

//void ERU1_0_IRQHandler(void)
//{
//	sprintf(kal,"AC1_AC_F\r\n");
//	UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
//	fault_count++;
//}

void ERU1_1_IRQHandler(void)
{
	//C006 (CT5) Interrupt
	fault_count++;
	//if(fault_count>2) {
		//CLR_BIT(PORT0->OUT,13);
		//NVIC_DisableIRQ(ERU0_2_IRQn);
		//NVIC_DisableIRQ(ERU0_1_IRQn);
		//NVIC_DisableIRQ(ERU0_0_IRQn);
		//NVIC_DisableIRQ(ERU0_3_IRQn);
		//flag_interrupt = 1;
		//}
	sprintf(kal,"ACPT3_F\r\n");
	UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
	//fault_count++;
}

void ERU1_2_IRQHandler(void)
{
	//C003 (DCPT1) Interrupt
	fault_count++;
	//if(fault_count>2) {
		//CLR_BIT(PORT0->OUT,13);
		//NVIC_DisableIRQ(ERU0_2_IRQn);
		//NVIC_DisableIRQ(ERU0_1_IRQn);
		//NVIC_DisableIRQ(ERU0_0_IRQn);
		//NVIC_DisableIRQ(ERU0_3_IRQn);
		//flag_interrupt = 1;
		//}
	sprintf(kal,"GRPT2_F OR GRPT3_F\r\n");
	UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
	kode_fault = 2;
	//fault_count++;
}

//void ERU1_3_IRQHandler(void)
//{
//	//C002 (CT1) Interrupt
//	sprintf(kal,"CT1_F\r\n");
//	UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
//	fault_count++;
//}
