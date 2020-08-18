/*
 * ISR_app.h
 *
 *  Created on: Sep 26, 2014
 *   Update on: Jan 13, 2016
 *      Author: angga, arif
 */

#ifndef ISR_APP_H_
#define ISR_APP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "algorithm/fixed_operation.h"
#include <DAVE3.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)

#include "common/delay.h"
#include "common/global.h"
#include "common/look_up_table.h"
#include "common/my_multiplexer.h"

#include "peripheral/IO_app.h"
#include "peripheral/UART_app.h"
#include "peripheral/ADC_app.h"
#include "peripheral/PWM_app.h"
#include "peripheral/DAC_app.h"
#include "peripheral/ERU_app.h"
#include "peripheral/I2C_SW_app.h"
#include "peripheral/SPI_app.h"
#include "peripheral/CAN_app.h"

#include "external/EEP25LC1024_app.h"
#include "external/display_app.h"
#include "external/AD5328_app.h"
#include "external/MCP79410_app.h"
#include "external/button_app.h"
#include "external/relay_app.h"

extern volatile int32_t power_inv;
extern volatile int32_t Pset;
extern volatile int32_t Qset;


extern volatile float P_Filt;
extern volatile float Q_Filt;

extern volatile int flag_fault;

extern volatile int flag_activation;
extern volatile int flag_deactivation;

#include "algorithm/converter.h"

#endif /* ISR_APP_H_ */
