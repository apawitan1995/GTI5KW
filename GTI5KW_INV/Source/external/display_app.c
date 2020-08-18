/*
 * display_app.c
 *
 *  Created on: Apr 8, 2014
 *      Author: angga
 */

#include "external/display_app.h"

void Display_Init(void)
{
	//DO_Display_Init(); //Initialization is done at IO_Init()
	lcd_init();
	lcd_clear();
	lcd_gotoxy(0,0);
	Button_Init();
}


