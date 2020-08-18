/*
 * IO_app.h
 *
 *  Created on: Sep 25, 2014
 *      Author: adjikharisma
 */

#ifndef IO_APP_H_
#define IO_APP_H_

#include <DAVE3.h>

//IO Port
//#define IO_PORT PORT0

//Relay discharging 		DO_HK1 P1.12
#define DO_HK1	12
//Relay 					DO_HK2 P1.13
#define DO_HK2	13
//Relay bypass charging		DO_K1S P1.10
#define DO_K1	10
//Relay AC line 			DO_K2S P1.11
#define DO_K2	11
//Digital output 1 spare 	DOS1 P2.11
#define DOS1	11
//Digital output 2 spare 	DOS2 P2.12
#define DOS2	12

//Digital input spare 1 	DIS1 P3.4
#define DIS1	4
//Digital input spare 2 	DIS2 P3.3
#define DIS2	3
//Digital input spare 3 	DIS3 P3.15
#define DIS3	15
//Status gate driver 1 		GD1F P0.12
#define DI_PMST1	12
//Status gate driver 		GD2F 2 P0.2
#define DI_PMST2	2

//DO Display di Mikro 1
#define LED1 14 //P2.14
#define LED2 15 //P2.15
#define BUZZER 14 //P1.14

//#define C1STAT 9 // P2.9 di Mikro 1?
#define C2STAT1 2 //P1.2 di Mikro 2?
#define C2STAT2 3 //P1.3 di Mikro 2?

//DO Enable GST P0.13
#define GST_HIGH() SET_BIT(PORT0->OUT,13)
#define GST_LOW() CLR_BIT(PORT0->OUT,13)

//DO FSW P0.14
#define FSW_HIGH() SET_BIT(PORT0->OUT,14)
#define FSW_LOW() CLR_BIT(PORT0->OUT,14)

void DI_PMST_Init(void);
void DIS_Init(void);
void DI_Init(void);
void DO_Relay_Init(void);
void DOS_Init(void);
void DO_Enable_Init(void);
void DO_Init(void);
void DO_Display_Init(void);
void DO_Status_Init(void);

void DO_Profiling_Init(void);
void IO_Slave_Status_Init(void);
void IO_COM_Init(void);

void IO_Init(void);


#endif /* IO_APP_H_ */
