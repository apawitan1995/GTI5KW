/*
 * my_multiplexer.h
 *
 *  Created on: Sep 24, 2014
 *      Author: angga
 */

#ifndef MY_MULTIPLEXER_H_
#define MY_MULTIPLEXER_H_

#include <DAVE3.h>

#define Check_Byte(reg,pos) (reg & (1<<pos))

void My_DAVE_MUX_Init(void);

#endif /* MY_MULTIPLEXER_H_ */
