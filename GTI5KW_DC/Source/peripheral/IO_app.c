/*
 * IO_app.c
 *
 *  Created on: Sep 25, 2014
 *      Author: angga
 */

#include "peripheral/IO_app.h"

void DI_PMST_Init(void)
{
	//Set direct input no pull-up DI_PMST1 P0.12
	WR_REG(PORT0->IOCR12,PORT0_IOCR12_PC12_Msk,PORT0_IOCR12_PC12_Pos,0x00);
	//Set direct input no pull-up DI_PMST2 P0.2
	WR_REG(PORT0->IOCR0,PORT0_IOCR0_PC2_Msk,PORT0_IOCR0_PC2_Pos,0x00);
}

void DIS_Init(void)
{
	//Set direct input no pull-up DIS1 P3.4
	WR_REG(PORT3->IOCR4,PORT3_IOCR4_PC4_Msk,PORT3_IOCR4_PC4_Pos,0x00);
	//Set direct input no pull-up DIS2 P3.3
	WR_REG(PORT3->IOCR0,PORT3_IOCR0_PC3_Msk,PORT3_IOCR0_PC3_Pos,0x00);
	//Set direct input no pull-up DIS3 P3.15
	WR_REG(PORT3->IOCR12,PORT3_IOCR12_PC15_Msk,PORT3_IOCR12_PC15_Pos,0x00);
}

void DI_Init(void)
{
	//DI_PMST is not used on new system minimum
	DI_PMST_Init();
	DIS_Init();
}

void DO_Relay_Init(void)
{
	//Set DO_HK as output
	WR_REG(PORT1->IOCR12,PORT1_IOCR12_PC12_Msk,PORT1_IOCR12_PC12_Pos,0x10);
	//Set DO_HK level low
	CLR_BIT(PORT1->OUT,DO_HK1);

	//Set DO_HK2 as output
	WR_REG(PORT1->IOCR12,PORT1_IOCR12_PC13_Msk,PORT1_IOCR12_PC13_Pos,0x10);
	//Set DO_HK2 level low
	CLR_BIT(PORT1->OUT,DO_HK2);

	//Set DO_K1 as output
	WR_REG(PORT1->IOCR8,PORT1_IOCR8_PC10_Msk,PORT1_IOCR8_PC10_Pos,0x10);
	//Set DO_K1 level low
	CLR_BIT(PORT1->OUT,DO_K1);

	//Set DO_K2 as output
	WR_REG(PORT1->IOCR8,PORT1_IOCR8_PC11_Msk,PORT1_IOCR8_PC11_Pos,0x10);
	//Set DO_K2 level low
	CLR_BIT(PORT1->OUT,DO_K2);
}

void DOS_Init(void)
{
	//Set DOS1 as output, Low
	WR_REG(PORT2->IOCR8,PORT2_IOCR8_PC11_Msk,PORT2_IOCR8_PC11_Pos,0x10);
	CLR_BIT(PORT2->OUT,DOS1);
	//Set DOS2 as output, Low
	WR_REG(PORT2->IOCR12,PORT2_IOCR12_PC12_Msk,PORT2_IOCR12_PC12_Pos,0x10);
	CLR_BIT(PORT2->OUT,DOS2);
}

void DO_Enable_Init(void)
{
	//Set output FSW high
	WR_REG(PORT0->IOCR12,PORT0_IOCR12_PC14_Msk,PORT0_IOCR12_PC14_Pos,0x10);
	SET_BIT(PORT0->OUT,14);
	//Set output GST low
	WR_REG(PORT0->IOCR12,PORT0_IOCR12_PC13_Msk,PORT0_IOCR12_PC13_Pos,0x10);
	CLR_BIT(PORT0->OUT,13);
}

void DO_Init(void)
{
	DO_Relay_Init();
	DOS_Init();
	DO_Enable_Init();
	//DO_Display_Init();
}

void IO_Status_Init(void)
{
	//C1STAT P2.9 - Output from XMC1 to HPS
	WR_REG(PORT2->IOCR8,PORT2_IOCR8_PC9_Msk,PORT2_IOCR8_PC9_Pos,0x10);
	SET_BIT(PORT2->OUT,C1STAT);

	//C2STAT1 P2.10 - OUTPUT from XMC1 to XMC2
	WR_REG(PORT2->IOCR8,PORT2_IOCR8_PC10_Msk,PORT2_IOCR8_PC10_Pos,0x10);
	SET_BIT(PORT2->OUT,C2STAT1);

	//C2STAT2 P2.8 INPUT from XMC2
	WR_REG(PORT2->IOCR8,PORT2_IOCR8_PC8_Msk,PORT2_IOCR8_PC8_Pos,0x00);
}

void IO_Init(void)
{
	DI_Init();
	DO_Init();
	IO_Status_Init();
	IO_COM_Init();
}

void IO_COM_Init(void)
{
	//COM1 P4.0
	WR_REG(PORT4->IOCR0,PORT4_IOCR0_PC0_Msk,PORT4_IOCR0_PC0_Pos,0x10);
	SET_BIT(PORT4->OUT,0);
	//COM2 P4.1
	WR_REG(PORT4->IOCR0,PORT4_IOCR0_PC1_Msk,PORT4_IOCR0_PC1_Pos,0x10);
	SET_BIT(PORT4->OUT,1);
	//COM3 P4.2
	WR_REG(PORT4->IOCR0,PORT4_IOCR0_PC2_Msk,PORT4_IOCR0_PC2_Pos,0x10);
	SET_BIT(PORT4->OUT,2);
	//COM4 P4.3
	WR_REG(PORT4->IOCR0,PORT4_IOCR0_PC3_Msk,PORT4_IOCR0_PC3_Pos,0x10);
	SET_BIT(PORT4->OUT,3);
	//COM5 P4.4
	WR_REG(PORT4->IOCR4,PORT4_IOCR4_PC4_Msk,PORT4_IOCR4_PC4_Pos,0x10);
	SET_BIT(PORT4->OUT,4);
	//COM6 P4.5
	WR_REG(PORT4->IOCR4,PORT4_IOCR4_PC5_Msk,PORT4_IOCR4_PC5_Pos,0x10);
	SET_BIT(PORT4->OUT,5);
	//COM7 P4.6
	WR_REG(PORT4->IOCR4,PORT4_IOCR4_PC6_Msk,PORT4_IOCR4_PC6_Pos,0x10);
	SET_BIT(PORT4->OUT,6);
	//COM8 P4.7
	WR_REG(PORT4->IOCR4,PORT4_IOCR4_PC7_Msk,PORT4_IOCR4_PC7_Pos,0x10);
	SET_BIT(PORT4->OUT,7);
}
