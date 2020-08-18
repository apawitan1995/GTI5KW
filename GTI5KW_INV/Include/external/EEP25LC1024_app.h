/*
 * EEP25LC1024_app.h
 *
 *  Created on: Feb 28, 2014
 *      Author: Hanif
 */

#ifndef EEP25LC1024_APP_H_
#define EEP25LC1024_APP_H_

#include <DAVE3.h>
#include "common/my_multiplexer.h"
#include "common/delay.h"

//EEP Clock
//100KHz
#define EEP_CLOCK 5 //in us

//I/O Definition
#define EEP_PORT PORT1
#define SI 0 		// MISO P5.0
#define CS_EEP 0	// CS P1.0
#define SO 7 		// MOSI P1.7
#define SCK 6 		//SCK P1.6

#define CS_EEP_HIGH() SET_BIT(EEP_PORT->OUT,CS_EEP)
#define CS_EEP_LOW() CLR_BIT(EEP_PORT->OUT,CS_EEP)

#define SCK_HIGH() SET_BIT(EEP_PORT->OUT,SCK)
#define SCK_LOW() CLR_BIT(EEP_PORT->OUT,SCK)
#define TOGGLE_SCK() EEP_PORT->OUT^=((uint32_t)1<<SCK)

#define SO_HIGH() SET_BIT(EEP_PORT->OUT,SO)
#define SO_LOW() CLR_BIT(EEP_PORT->OUT,SO)
#define WRITE_SO(n) (n ? SO_HIGH() : SO_LOW())

#define READ_SI() (PORT5->IN & (1<<SI))

//Instruction Code
#define eep_READ 0x03
#define eep_WRITE 0x02
#define eep_WREN 0x06
#define eep_WRDI 0x04
#define eep_RDSR 0x05
#define eep_WRSR 0x01
#define eep_PE 0x42
#define eep_SE 0xD8
#define eep_CE 0xC7
#define eep_RDID 0xAB
#define eep_DPD 0xB9

void EEP25LC_Init(void);
void EEP25LC_Write_Data(int32_t addr,int32_t data);
int8_t EEP25LC_Read_Data(int32_t addr);
void EEP25LC_Write_Multiple_Data(int32_t addr,int8_t* data);

#endif /* EEP25LC1024_APP_H_ */
