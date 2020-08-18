/*
 * ISR_app.h
 *
 *  Created on: Sep 26, 2014
 *      Author: angga
 */

#ifndef ISR_APP_H_
#define ISR_APP_H_


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
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

#include "external/EEP25LC1024_app.h"
#include "external/display_app.h"
#include "external/AD5328_app.h"
#include "external/MCP79410_app.h"
#include "external/button_app.h"
#include "external/relay_app.h"

//#include "algorithm/converter.h"
#include "algorithm/inverter.h"
#include "algorithm/PLL.h"
#include "algorithm/droop.h"

//extern volatile uint32_t DCPT2_array_plot[1280];
//extern volatile uint32_t ACPT1_array_plot[1280];
extern volatile uint32_t DCPT2_filter;
extern volatile uint32_t ACPT1_filter;
extern volatile uint32_t DCPT2_ind;
extern volatile uint32_t ACPT1_ind;
extern volatile int32_t frekuensi_pll;

//extern volatile int32_t voac_peak_fix;
//
//extern volatile int32_t TetaSine_fix, TetaCos_fix, integSine_fix, integCos_fix;
//extern volatile int32_t sinTeta_fix, cosTeta_fix;
//extern volatile int32_t Tetaref_fix;
//extern volatile float Tetaref;

#endif /* ISR_APP_H_ */
