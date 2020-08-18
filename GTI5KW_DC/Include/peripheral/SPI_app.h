/*
 * SPI_app.h
 *
 *  Created on: 5 Jan 2015
 *      Author: asus
 */

#ifndef SPI_APP_H_
#define SPI_APP_H_

#include <DAVE3.h>

#define DUMMY 0x00

extern volatile uint16_t tes_spi_uc[];
extern volatile uint16_t tes_spi_data;

void SPI_uC_Write_1Byte_Receive_3Byte(uint32_t transmit_data, uint16_t* p_spi_uc);
uint16_t SPI_uC_Read_Data(uint32_t addr);

#endif /* SPI_APP_H_ */
