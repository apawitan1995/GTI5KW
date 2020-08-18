/*
 * DAC_app.h
 *
 *  Created on: Feb 14, 2014
 *      Author: Hanif
 */

#ifndef DAC_APP_H_
#define DAC_APP_H_

#include <DAVE3.h>
#include "peripheral/UART_app.h"

void DAC_Init(void);
void DAC0_Output(uint32_t val);
void DAC1_Output(uint32_t val);
void DAC0_Output_Volt(float val);
void DAC1_Output_Volt(float val);

#endif /* DAC_APP_H_ */
