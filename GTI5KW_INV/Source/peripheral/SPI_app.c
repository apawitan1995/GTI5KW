/*
 * SPI_app.c
 *
 *  Created on: Jan 5, 2015
 *      Author: adjikharisma
 */

//#include "peripheral/SPI_app.h"
//
//SPI001_ConfigType SPI001_Config0;
//
//void SPI_Init(void)
//{
//	/* Slave Mode at USIC0_CH0
//     * P2.14 = mrST(U1C0.DOUT0), ALT2
//     * P2.15 = mtSR(U1C0.DX0C),
//     * P5.8 = CLKIN(U1C0.DX1B),
//     * P5.9 = CS(U1C0.DX2B)
//     */
//
////    /* Disable P1.7 as transmit pin (assigned by solver) */
////    PORT1->IOCR4 = (PORT1->IOCR4 & ~0xF8000000) | (16 << 27);
////    PORT1->PDR0 = (PORT1->PDR0 & ~0x70000000) | (2 << 28);
//
////    /* Set P1.7 to Low */
////    PORT1->OMR = 0x00800000;
//
//    /* Disable USIC Channel */
//    USIC1_CH0->CCR &= ~0x000F;
//
//    /* Configure P1.4, mtSR (U0C0.DX0B) */
//    /* Configure P2.15, mtSR (U1C0.DX0C) */
//    USIC1_CH0->DX0CR =
//            (2 << USIC_CH_DX0CR_DSEL_Pos)    |    // DX0B is selected
//            (1 << USIC_CH_DX0CR_INSW_Pos)    |
//            (0 << USIC_CH_DX0CR_DFEN_Pos)    |
//            (0 << USIC_CH_DX0CR_DSEN_Pos)    |
//            (0 << USIC_CH_DX0CR_DPOL_Pos)    |
//            (0 << USIC_CH_DX0CR_SFSEL_Pos)    |
//            (0 << USIC_CH_DX0CR_CM_Pos)        |
//            (0 << USIC_CH_DX0CR_DXS_Pos)    ;
//
//    /* Configure P1.1, CLKIN (U0C0.DX1A) */
//    /* Configure P5.8, CLKIN (U1C0.DX1B) */
//    USIC1_CH0->DX1CR =
//            (1 << USIC_CH_DX0CR_DSEL_Pos)    |    // DX1A is selected
//            (1 << USIC_CH_DX0CR_INSW_Pos)    |
//            (0 << USIC_CH_DX0CR_DFEN_Pos)    |
//            (0 << USIC_CH_DX0CR_DSEN_Pos)    |
//            (0 << USIC_CH_DX0CR_DPOL_Pos)    |
//            (0 << USIC_CH_DX0CR_SFSEL_Pos)    |
//            (0 << USIC_CH_DX0CR_CM_Pos)        |
//            (0 << USIC_CH_DX0CR_DXS_Pos)    ;
//
//
//    /* Configure P0.7, CS (U0C0.DX2B) */
//    /* Configure P5.9, CS (U1C0.DX2B) */
//    USIC1_CH0->DX2CR =
//            (1 << USIC_CH_DX0CR_DSEL_Pos)    |    // DX2B is selected
//            (1 << USIC_CH_DX0CR_INSW_Pos)    |
//            (0 << USIC_CH_DX0CR_DFEN_Pos)    |
//            (0 << USIC_CH_DX0CR_DSEN_Pos)    |
//            (0 << USIC_CH_DX0CR_DPOL_Pos)    |    // Polarity is inverted
//            (0 << USIC_CH_DX0CR_SFSEL_Pos)    |
//            (0 << USIC_CH_DX0CR_CM_Pos)        |
//            (0 << USIC_CH_DX0CR_DXS_Pos)    ;
//
//    /* Switch off fractional divider and baud rate generator */
//    USIC1_CH0->FDR=0;
//    USIC1_CH0->BRG=0;
//
//    /* Configure PCR register. Disable MSLSEN, SELCTR, SELINV, etc... */
//    SPI001_Handle0.USICRegs->PCR_SSCMode =
//            (0 << USIC_CH_PCR_SSCMode_MSLSEN_Pos)    |
//            (0 << USIC_CH_PCR_SSCMode_SELCTR_Pos)    |
//            (0 << USIC_CH_PCR_SSCMode_SELINV_Pos)    |
//            (0 << USIC_CH_PCR_SSCMode_CTQSEL1_Pos)    |
//            (0 << USIC_CH_PCR_SSCMode_PCTQ1_Pos)    |
//            (0 << USIC_CH_PCR_SSCMode_MSLSIEN_Pos)    |
//            (0 << USIC_CH_PCR_SSCMode_SELO_Pos)        |
//            (0 << USIC_CH_PCR_SSCMode_TIWEN_Pos)    ;
//
////    /* Control P0.7 as Input pin, Strong drive strength */
////    PORT0->IOCR4 = (PORT0->IOCR4 & ~0xF8000000) | (0 << 27);
////    PORT0->PDR0 = (PORT0->PDR0 & ~0x70000000) | (2 << 28);
//    /* Control P5.9 as Input pin, Strong drive strength */
//    WR_REG(PORT5->IOCR8,PORT5_IOCR8_PC9_Msk,PORT5_IOCR8_PC9_Pos,0x00);
//    WR_REG(PORT5->PDR1,PORT5_PDR1_PD9_Msk,PORT5_PDR1_PD9_Pos,0x02);
//
////    /* Control P1.1 as Input pin, Strong drive strength */
////    PORT1->IOCR0 = (PORT1->IOCR0 & ~0x0000F800) | (0 << 11);
////    PORT1->PDR0 = (PORT1->PDR0 & ~0x00000070) | (2 << 4);
//    /* Control P5.8 as Input pin, Strong drive strength */
//    WR_REG(PORT5->IOCR8,PORT5_IOCR8_PC8_Msk,PORT5_IOCR8_PC8_Pos,0x00);
//    WR_REG(PORT5->PDR1,PORT5_PDR1_PD8_Msk,PORT5_PDR1_PD8_Pos,0x02);
//
////    /* Control P1.4 as Input pin, Strong drive strength */
////    PORT1->IOCR4 = (PORT1->IOCR4 & ~0x000000F8) | (0 << 3);
////    PORT1->PDR0 = (PORT1->PDR0 & ~0x00070000) | (2 << 16);
//    /* Control P2.15 as Input pin, Strong drive strength */
//	WR_REG(PORT2->IOCR12,PORT2_IOCR12_PC15_Msk,PORT2_IOCR12_PC15_Pos,0x00);
//	WR_REG(PORT2->PDR1,PORT2_PDR1_PD15_Msk,PORT2_PDR1_PD15_Pos,0x02);
//
//    SPI001_Config0.FrameLen = 32;
//
//    SPI001_Configure(&SPI001_Handle0,&SPI001_Config0);
//
//    /* Enable USIC Channel in SPI mode */
//    USIC1_CH0->CCR |= 0x0001;
//
////    /* Set P1.5 as mrST pin (Solver assign P1.7 as MTsr pin). We are interested in the Transmit capability of the pin. (Remember, this implementation use SPI Master App) */
////    PORT1->IOCR4 = (PORT1->IOCR4 & ~0x0000F800) | (18 << 11);
////    PORT1->PDR0 = (PORT1->PDR0 & ~0x00700000) | (2 << 20);
//
//
//}
