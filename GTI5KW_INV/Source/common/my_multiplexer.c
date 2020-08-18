/*
 * my_multiplexer.c
 *
 *  Created on: Sep 24, 2014
 *      Author: Angga
 */

#include "common/my_multiplexer.h"

void My_DAVE_MUX_Init(void)
{
	  /*	PORT Macro definitions for IOCR_OE, IOCR_PCR & HWSEL_HW     */
//	  WR_REG(PORT1->IOCR8, 0xb80000U, PORT_IOCR_PC2_PCR_Pos, 0x13U);                /*P1.10 : PORT1_IOCR8_PC10_PCR and PORT1_IOCR8_PC10_OE */
//
//	  WR_REG(PORT1->IOCR8, 0xb8000000U, PORT_IOCR_PC3_PCR_Pos, 0x13U);                /*P1.11 : PORT1_IOCR8_PC11_PCR and PORT1_IOCR8_PC11_OE */
//
//	  WR_REG(PORT1->IOCR12, 0xb8U, PORT_IOCR_PC0_PCR_Pos, 0x13U);                /*P1.12 : PORT1_IOCR12_PC12_PCR and PORT1_IOCR12_PC12_OE */
//
//	  WR_REG(PORT1->IOCR12, 0xb800U, PORT_IOCR_PC1_PCR_Pos, 0x13U);                /*P1.13 : PORT1_IOCR12_PC13_PCR and PORT1_IOCR12_PC13_OE */
//
//	  WR_REG(PORT1->IOCR12, 0xb80000U, PORT_IOCR_PC2_PCR_Pos, 0x13U);                /*P1.14 : PORT1_IOCR12_PC14_PCR and PORT1_IOCR12_PC14_OE */
//
//	  WR_REG(PORT1->IOCR12, 0xb8000000U, PORT_IOCR_PC3_PCR_Pos, 0x13U);                /*P1.15 : PORT1_IOCR12_PC15_PCR and PORT1_IOCR12_PC15_OE */
//
//	  WR_REG(PORT2->IOCR12, 0xb80000U, PORT_IOCR_PC2_PCR_Pos, 0x12U);                /*P2.14 : PORT2_IOCR12_PC14_PCR and PORT2_IOCR12_PC14_OE */
//
//	  WR_REG(PORT3->IOCR0, 0xb8U, PORT_IOCR_PC0_PCR_Pos, 0x12U);                /*P3.0 : PORT3_IOCR0_PC0_PCR and PORT3_IOCR0_PC0_OE */
//
//	  WR_REG(PORT3->IOCR12, 0xb8U, PORT_IOCR_PC0_PCR_Pos, 0x12U);                /*P3.12 : PORT3_IOCR12_PC12_PCR and PORT3_IOCR12_PC12_OE */
//
//	  WR_REG(PORT3->IOCR12, 0xb800U, PORT_IOCR_PC1_PCR_Pos, 0x12U);                /*P3.13 : PORT3_IOCR12_PC13_PCR and PORT3_IOCR12_PC13_OE */
//
//	  WR_REG(PORT5->IOCR8, 0xb8U, PORT_IOCR_PC0_PCR_Pos, 0x12U);                /*P5.8 : PORT5_IOCR8_PC8_PCR and PORT5_IOCR8_PC8_OE */
//
//	  WR_REG(PORT5->IOCR8, PORT_IOCR_PC3_OE_Msk, PORT_IOCR_PC3_OE_Pos, PORT_IOCR_OE1);                /*    P5.11 : PORT5_IOCR8_PC11_OE */
//
//	  WR_REG(PORT6->IOCR0, 0xb8U, PORT_IOCR_PC0_PCR_Pos, 0x13U);                /*P6.0 : PORT6_IOCR0_PC0_PCR and PORT6_IOCR0_PC0_OE */
//
//	  WR_REG(PORT6->IOCR0, 0xb800U, PORT_IOCR_PC1_PCR_Pos, 0x13U);                /*P6.1 : PORT6_IOCR0_PC1_PCR and PORT6_IOCR0_PC1_OE */

	  //********* Capture/Compare Unit 8 (CAPCOM8) CONFIGURATIONS *************************


	  /*VADC GROUP0 Mux Related SFR/Bitfields Configurations*/
	  WR_REG(VADC_G0->ALIAS, VADC_G_ALIAS_ALIAS0_Msk, VADC_G_ALIAS_ALIAS0_Pos,0);
	  WR_REG(VADC_G0->ALIAS, VADC_G_ALIAS_ALIAS1_Msk, VADC_G_ALIAS_ALIAS1_Pos,1);

	  //Multiplexing ADC (prevent aliasing by dave)
	  WR_REG(VADC_G0->CHCTR[0], VADC_G_CHCTR_RESREG_Msk, VADC_G_CHCTR_RESREG_Pos,15);

	  WR_REG(VADC_G0->CHCTR[1], VADC_G_CHCTR_RESREG_Msk, VADC_G_CHCTR_RESREG_Pos,4);

	  WR_REG(VADC_G0->CHCTR[6], VADC_G_CHCTR_RESREG_Msk, VADC_G_CHCTR_RESREG_Pos,5);


					  /*VADC GROUP1 Mux Related SFR/Bitfields Configurations*/


	  WR_REG(VADC_G1->CHCTR[3], VADC_G_CHCTR_RESREG_Msk, VADC_G_CHCTR_RESREG_Pos,3);

	  WR_REG(VADC_G1->CHCTR[4], VADC_G_CHCTR_RESREG_Msk, VADC_G_CHCTR_RESREG_Pos,15);


					  /*VADC GROUP2 Mux Related SFR/Bitfields Configurations*/
	  //WR_REG(VADC_G2->ALIAS, VADC_G_ALIAS_ALIAS0_Msk, VADC_G_ALIAS_ALIAS0_Pos,1);

	  WR_REG(VADC_G2->ALIAS, VADC_G_ALIAS_ALIAS0_Msk, VADC_G_ALIAS_ALIAS0_Pos,0);
  	  WR_REG(VADC_G2->ALIAS, VADC_G_ALIAS_ALIAS1_Msk, VADC_G_ALIAS_ALIAS1_Pos,1);

	  WR_REG(VADC_G2->CHCTR[1], VADC_G_CHCTR_RESREG_Msk, VADC_G_CHCTR_RESREG_Pos,1);


					  /*VADC GROUP3 Mux Related SFR/Bitfields Configurations*/


	  WR_REG(VADC_G3->CHCTR[6], VADC_G_CHCTR_RESREG_Msk, VADC_G_CHCTR_RESREG_Pos,4);

	  WR_REG(VADC_G3->CHCTR[7], VADC_G_CHCTR_RESREG_Msk, VADC_G_CHCTR_RESREG_Pos,9);


   /*VADC GLOBAL RESULT Mux Related SFR/Bitfields Configurations*/
   /*VADC BACKGROUND Source Mux Related SFR/Bitfields Configurations*/
}



