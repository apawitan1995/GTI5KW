/*
 * my_multiplexer.c
 *
 *  Created on: Sep 24, 2014
 *      Author: Angga
 */

#include "common/my_multiplexer.h"

void My_DAVE_MUX_Init(void)
{

	  /*VADC GROUP0 Mux Related SFR/Bitfields Configurations*/
//	  WR_REG(VADC_G0->ALIAS, VADC_G_ALIAS_ALIAS0_Msk, VADC_G_ALIAS_ALIAS0_Pos,0);
//	  WR_REG(VADC_G0->ALIAS, VADC_G_ALIAS_ALIAS1_Msk, VADC_G_ALIAS_ALIAS1_Pos,1);

	  //Multiplexing ADC (prevent aliasing by dave)
//	  WR_REG(VADC_G0->CHCTR[0], VADC_G_CHCTR_RESREG_Msk, VADC_G_CHCTR_RESREG_Pos,15);

//	  WR_REG(VADC_G0->CHCTR[1], VADC_G_CHCTR_RESREG_Msk, VADC_G_CHCTR_RESREG_Pos,4);

//	  WR_REG(VADC_G0->CHCTR[6], VADC_G_CHCTR_RESREG_Msk, VADC_G_CHCTR_RESREG_Pos,5);


					  /*VADC GROUP1 Mux Related SFR/Bitfields Configurations*/


//	  WR_REG(VADC_G1->CHCTR[3], VADC_G_CHCTR_RESREG_Msk, VADC_G_CHCTR_RESREG_Pos,3);

//	  WR_REG(VADC_G1->CHCTR[4], VADC_G_CHCTR_RESREG_Msk, VADC_G_CHCTR_RESREG_Pos,15);


					  /*VADC GROUP2 Mux Related SFR/Bitfields Configurations*/
//	  WR_REG(VADC_G2->ALIAS, VADC_G_ALIAS_ALIAS0_Msk, VADC_G_ALIAS_ALIAS0_Pos,0);
//	  WR_REG(VADC_G2->ALIAS, VADC_G_ALIAS_ALIAS1_Msk, VADC_G_ALIAS_ALIAS1_Pos,1);

//	  WR_REG(VADC_G2->CHCTR[0], VADC_G_CHCTR_RESREG_Msk, VADC_G_CHCTR_RESREG_Pos,0);

//	  WR_REG(VADC_G2->CHCTR[1], VADC_G_CHCTR_RESREG_Msk, VADC_G_CHCTR_RESREG_Pos,1);


					  /*VADC GROUP3 Mux Related SFR/Bitfields Configurations*/


//	  WR_REG(VADC_G3->CHCTR[6], VADC_G_CHCTR_RESREG_Msk, VADC_G_CHCTR_RESREG_Pos,4);

//	  WR_REG(VADC_G3->CHCTR[7], VADC_G_CHCTR_RESREG_Msk, VADC_G_CHCTR_RESREG_Pos,9);


   /*VADC GLOBAL RESULT Mux Related SFR/Bitfields Configurations*/
   /*VADC BACKGROUND Source Mux Related SFR/Bitfields Configurations*/
}



