/*
 * CAN_app.h
 *
 *  Created on: Mar 25, 2014
 *      Author: angga
 */

#ifndef CAN_APP_H_
#define CAN_APP_H_

//Debug
#include <stdio.h>
#include "peripheral/UART_app.h"

extern volatile char data_can[8];

CAN001_MessageHandleType CanRecMsgObj;

uint32_t SendCanMessage
(
		const CAN001_HandleType* Handle,
		uint8_t MsgObjnr,
		uint8_t DataLength,
		const uint8_t* DataPtr
);

#endif /* CAN_APP_H_ */
