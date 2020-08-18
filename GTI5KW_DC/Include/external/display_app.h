/*
 * display_app.h
 *
 *  Created on: Apr 8, 2014
 *      Author: angga
 */

#ifndef DISPLAY_APP_H_
#define DISPLAY_APP_H_

#include "external/LCD_app.h"
#include "external/button_app.h"
#include "peripheral/IO_app.h"

#define LED1_BUZZ 14 	//P2.14
#define LED2 15 		//P2.15

void DO_Display_Init(void);
void Display_Init(void);

#endif /* DISPLAY_APP_H_ */
