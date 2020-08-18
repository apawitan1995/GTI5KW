/*
 * delay.h
 *
 *  Created on: Apr 8, 2014
 *      Author: angga
 */

#ifndef DELAY_H_
#define DELAY_H_

#include <DAVE3.h>
//Debugging purposes
#include "peripheral/UART_app.h"

void My_Delay_us(int16_t n);
void My_Delay_ms(int8_t n);
void My_Delay_s(int16_t n);

#endif /* DELAY_H_ */
