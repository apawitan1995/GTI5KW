/*
 * AD5328_app.h
 *
 *  Created on: Feb 14, 2014
 *      Author: Hanif
 */

#ifndef AD5328_APP_H_
#define AD5328_APP_H_

#include <DAVE3.h>
#include "common/my_multiplexer.h"
#include "common/delay.h"

//counter for clock 100KHz
#define CLOCK 5

#define DCPT1_REF 0
#define CT1_REF 1
#define CT234_REF 2
#define DCPT2REF 3
#define CT56_REF 4
#define ACPT_REF 5
#define CT78_REF 6
#define GRPT_REF 7

#define DATA_PORT PORT0
//Serial clock SCLK_DAC P0.11
#define SCLK 11
//Chip select SS_DAC P0.6
#define CS 6
//Serial data MOSI_DAC P0.5
#define MOSI 5
//DAC update LDAC P0.4
#define LDAC 4

#define CS_HIGH() SET_BIT(DATA_PORT->OUT,CS)
#define CS_LOW() CLR_BIT(DATA_PORT->OUT,CS)

#define SCLK_HIGH() SET_BIT(DATA_PORT->OUT,SCLK)
#define SCLK_LOW() CLR_BIT(DATA_PORT->OUT,SCLK)
#define TOGGLE_SCLK() DATA_PORT->OUT^=((uint32_t)1<<SCLK)

#define MOSI_HIGH() SET_BIT(DATA_PORT->OUT,MOSI)
#define MOSI_LOW() CLR_BIT(DATA_PORT->OUT,MOSI)
#define WRITE_MOSI(n) (n ? MOSI_HIGH() : MOSI_LOW())

#define LDAC_HIGH() SET_BIT(DATA_PORT->OUT,LDAC)
#define LDAC_LOW() CLR_BIT(DATA_PORT->OUT,LDAC)

#define ADREF 2.483f

void AD5328_Init(void);
void AD5328_Send_Data(int32_t dac,int32_t data);
void AD5328_Set_Value_Volt(int32_t dac,float volt);

#endif /* AD5328_APP_H_ */
