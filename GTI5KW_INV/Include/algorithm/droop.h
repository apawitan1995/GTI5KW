/*
 * droop.h
 *
 *  Created on: May 5, 2014
 *      Author: adjikharisma
 */

#ifndef DROOP_H_
#define DROOP_H_

#include <DAVE3.h>
#include "common/global.h"
#include "algorithm/inverter.h"
//#include "converter.h"
#include "algorithm/PLL.h"

#include <stdint.h>


#define b0P 9.6250471E-007
#define b1P 1.9250094E-006
#define b2P 9.6250471E-007
#define a1P -1.997251
#define a2P 0.99725488

#define b0f 0.0019596476
#define b1f 0.0019596476
#define a1f -0.9960807

#define m 0.00001
#define n_droop 0.004

extern float P, Q, deltaP;
float Pripple, Qripple;
float xf[2];
float yf[2];
float Vo[1500];
float xP[3];
float yP[3];
float xQ[3];
float yQ[3];
extern volatile float Pset;
extern volatile float Qset;
extern volatile int32_t PV_voltage;
extern volatile int32_t PV_current;
extern volatile int MODE;


void FrequencyLPF(void);
void PowerCalculation(void);
void PowerFilter(void);
void DroopControl(void);
int32_t DroopControl_P(void);
void DroopControl_Q(void);
void DroopDisable(void);

#endif /* DROOP_H_ */
