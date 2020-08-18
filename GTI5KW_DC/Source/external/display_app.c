/*
 * display_app.c
 *
 *  Created on: Apr 8, 2014
 *      Author: angga
 */

#include "external/display_app.h"

void Display_Init(void)
{
	DO_Display_Init(); //Initialization is not done at IO_Init()
	lcd_init();
	lcd_clear();
	lcd_gotoxy(0,0);
	Button_Init();
}

void DO_Display_Init(void)
{
	//Set as output pin LED2 2.15
	WR_REG(PORT2->IOCR12,PORT2_IOCR12_PC15_Msk,PORT2_IOCR12_PC15_Pos,0x10);
	SET_BIT(PORT2->OUT,LED2);
	//Set as output pin LED1_BUZZ 2.14
	WR_REG(PORT2->IOCR12,PORT2_IOCR12_PC14_Msk,PORT2_IOCR12_PC14_Pos,0x10);
	CLR_BIT(PORT2->OUT,LED1_BUZZ);
}
