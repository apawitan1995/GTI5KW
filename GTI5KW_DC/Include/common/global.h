/*
 * global.h
 *
 *  Created on: 2 Jan 2014
 *      Author: angga
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <DAVE3.h>

#define TS 1/16000 // Ts = 1/16000 has not already included on PI algorithm GAIN

#define CheckBit(reg,pos) ((reg)&(1<<pos))

#define R2D 57.29577f
//#define Pi 3.141592653f
#define TWOPI 6.28318f

//extern volatile float DCPT2,DCPT1;	// intermediate voltage value, input voltage
//extern volatile float CT1;
//extern volatile float CT2;
//extern volatile float CT3;
//extern volatile float CT4;
//extern volatile float CT5;
//extern volatile float CT6;
//extern volatile float ACPT1,ACPT2; // output, load

#endif /* GLOBAL_H_ */
