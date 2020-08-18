/*
 * I2C_sw_xmc.h
 *
 *  Created on: Dec 23, 2013
 *      Author: Hanif
 */

#ifndef I2C_SW_XMC_H_
#define I2C_SW_XMC_H_

#include <XMC4500.h>
#include "common/my_multiplexer.h"

////Debug
//#include <stdio.h>
//#include "UART_app.h"

#define CPU_CLK 120 //in MHz
//I2C Clock 100KHz
#define DELAY_CLK 30 //in us

#define SDA 5 //Define MCU Pin connected as SDA
#define SCL 6 //Define MCU Pin connected as SCL
#define PortOut PORT3->OUT
#define PortIn PORT3->IN
#define PortSDACtrl PORT3->IOCR4
#define PortSDAMsk PORT3_IOCR4_PC5_Msk
#define PortSDAPos PORT3_IOCR4_PC5_Pos
#define PortSCLCtrl PORT3->IOCR4
#define PortSCLMsk PORT3_IOCR4_PC6_Msk
#define PortSCLPos PORT3_IOCR4_PC6_Pos

#define WriteBit(rreg,rpos,wreg,wpos) (Check_Byte(rreg,rpos) ? SET_BIT(wreg,wpos) : CLR_BIT(wreg,wpos))

#define BusIdle() PortOut|=((uint32_t)(1 << SCL) | (1<<SDA))

#define SDA_H() SET_BIT(PortOut,SDA)
#define SDA_L() CLR_BIT(PortOut,SDA)
#define WRITE_SDA(n) (n ? SDA_H() : SDA_L())

#define READ_SDA() (PortIn & (1<<SDA))
#define SCL_H() PortOut|=((uint32_t)1<<SCL)
#define SCL_L() PortOut&=(~(uint32_t)(1<<SCL))
#define ToggleSCL() PortOut^=((uint32_t)1<<SCL)
#define SET_SDA_IN() WR_REG(PortSDACtrl,PortSDAMsk,PortSDAPos,0x00) //Input no pull-up
#define SET_SDA_OUT() WR_REG(PortSDACtrl,PortSDAMsk,PortSDAPos,0x18)//Output no pull-up
#define SET_SCL_IN() WR_REG(PortSCLCtrl,PortSCLMsk,PortSCLPos,0x00) //Input no pull-up
#define SET_SCL_OUT() WR_REG(PortSCLCtrl,PortSCLMsk,PortSCLPos,0x18)//Output no pull-up

void I2C_Init(void);
void I2C_Disable(void);
int8_t I2C_start_cond(void);
int8_t I2C_wryte_byte(uint8_t byte);
int8_t I2C_write_data(uint8_t addr,uint8_t addreg,uint8_t* data,uint8_t bytes);
int8_t I2C_read_byte(uint8_t* data,uint8_t index,uint8_t bytes);
int8_t I2C_read_data(uint8_t addr,uint8_t addreg,uint8_t* data,uint8_t bytes);

#endif /* I2C_SW_XMC_H_ */
