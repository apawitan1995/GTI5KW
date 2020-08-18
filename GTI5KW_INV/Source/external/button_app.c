/*
 * BUTTON_app.c
 *
 *  Created on: Apr 11, 2014
 *      Author: angga
 */


#include "external/button_app.h"

void Button_Init(void)
{
	//Set Direct Input pull-up PB1 5.8
	WR_REG(PORT5->IOCR8,PORT5_IOCR8_PC8_Msk,PORT5_IOCR8_PC8_Pos,0x02);

	//Set Direct Input pull-up PB2 5.9
	WR_REG(PORT5->IOCR8,PORT5_IOCR8_PC9_Msk,PORT5_IOCR8_PC9_Pos,0x02);

	//Set Direct Input pull-up PB3 5.10
	WR_REG(PORT5->IOCR8,PORT5_IOCR8_PC10_Msk,PORT5_IOCR8_PC10_Pos,0x02);

	//Set Direct Input pull-up PB4 5.11
	WR_REG(PORT5->IOCR8,PORT5_IOCR8_PC11_Msk,PORT5_IOCR8_PC11_Pos,0x02);
}

unsigned char Button_Read(void)
{

    char button;
    if(_NOT_PRESSED)
	{
		button = '0';
	}
    if(_UP_PRESSED) {button = '1';}// else {BTN = 0;}//   LATDbits.LATD0 = 1; LATDbits.LATD4 = 1; LATDbits.LATD5 = 0; } // Sensor4
    if(_DWN_PRESSED) {button = '2';}// else {BTN = 0;}//   LATDbits.LATD1 = 1; LATDbits.LATD4 = 1; LATDbits.LATD5 = 0; } // Sensor5
    if(_ESC_PRESSED) {button = '3';}// else {BTN = 0;}//   LATDbits.LATD2 = 1; LATDbits.LATD4 = 1; LATDbits.LATD5 = 0; } // Sensor6
    if(_SET_PRESSED) {button = '4';}// else {BTN = 0;}//   LATDbits.LATD3 = 1; LATDbits.LATD4 = 1; LATDbits.LATD5 = 0; } // Sensor7
    return button;
}
