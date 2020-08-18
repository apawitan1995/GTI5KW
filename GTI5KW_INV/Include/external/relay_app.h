/*
 * relay_app.h
 *
 *  Created on: Sep 23, 2014
 *      Author: angga
 */

#ifndef RELAY_APP_H_
#define RELAY_APP_H_

#include <DAVE3.h>
#include "peripheral/IO_app.h"

#define Open_HK1() SET_BIT(PORT1->OUT,DO_HK1)
#define Open_HK2() SET_BIT(PORT1->OUT,DO_HK2)
#define Open_K1() CLR_BIT(PORT1->OUT,DO_K1)
#define Open_K2() CLR_BIT(PORT1->OUT,DO_K2)

#define Close_HK1() CLR_BIT(PORT1->OUT,DO_HK1)
#define Close_HK2() CLR_BIT(PORT1->OUT,DO_HK2)
#define Close_K1() SET_BIT(PORT1->OUT,DO_K1)
#define Close_K2() SET_BIT(PORT1->OUT,DO_K2)

void Relay_Init(void);

#endif /* RELAY_APP_H_ */
