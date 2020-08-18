/*
 * ADC_app.h
 *
 *  Created on: Dec 12, 2013
 *      Author: Hanif
 */

#ifndef ADC_APP_H_
#define ADC_APP_H_

#include <DAVE3.h>
#include "common/global.h"

//Definisi macro untuk DAVE ADC API
#define ADC_G0_Queue_API (ADC002_HandleType*)&ADC002_Handle0
#define ADC_G1_Scan_API (ADC003_HandleType*)&ADC003_Handle0
#define ADC_G2_Scan_API (ADC003_HandleType*)&ADC003_Handle1

#define ADC_G0CH0_CT1_Handler ADCCH001_Handle0
#define ADC_G0CH1_CT2_Handler ADCCH001_Handle1
#define ADC_G0CH2_CT3_Handler ADCCH001_Handle2
#define ADC_G0CH3_CT4_Handler ADCCH001_Handle3
#define ADC_G0CH6_DCPT1_AIN_Handler ADCCH001_Handle4
#define ADC_G0CH7_DCPT2_AIN_Handler ADCCH001_Handle5
#define ADC_G1CH4_CT7_Handler ADCCH001_Handle6
#define ADC_G1CH5_CT8_Handler ADCCH001_Handle7
#define ADC_G2CH0_CT5_Handler ADCCH001_Handle8
#define ADC_G2CH1_CT6_Handler ADCCH001_Handle9
#define ADC_G2CH2_ACPT1_AINHandler ADCCH001_Handle10
#define ADC_G2CH3_ACPT2_AINHandler ADCCH001_Handle11
#define ADC_G2CH4_ACPT3_AINHandler ADCCH001_Handle12
#define ADC_G2CH5_GRPT1_AINHandler ADCCH001_Handle13
#define ADC_G2CH6_GRPT2_AINHandler ADCCH001_Handle14
#define ADC_G2CH7_GRPT3_AINHandler ADCCH001_Handle15

#define CT1_Result_Register 0
#define CT2_Result_Register 1
#define CT3_Result_Register 2
#define CT4_Result_Register 3
#define DCPT1_AIN_Result_Register 6
#define DCPT2_AIN_Result_Register 7
#define CT7_Result_Register 4
#define CT8_Result_Register 5
#define CT5_Result_Register 0
#define CT6_Result_Register 1
#define ACPT1_AIN_Result_Register 2
#define ACPT2_AIN_Result_Register 3
#define ACPT3_AIN_Result_Register 4
#define GRPT1_AIN_Result_Register 5
#define GRPT2_AIN_Result_Register 6
#define GRPT3_AIN_Result_Register 7
#define TQ1_AIN_Result_Register 6
#define TQ2_AIN_Result_Register 7
#define AIS1_AIN_Result_Register 6
#define AIS2_AIN_Result_Register 7

#define VADC_REF 3.235f
#define ADCtoAnaVolt VADC_REF/4095.0f
#define AnaVolttoADC 4095.0f/VADC_REF

#define ADC_DCPTtoHV(x) (x-523)/5.5f
#define ADC_CTtoHV(x) (x-1724)/13.8f
#define ADC_ACPTtoHV(x) (x-1600)/3.03f

/*

// GTI#2 infineon 1


#define	DCPT1_OFFSET 2438//638
#define DCPT2_OFFSET 2448//2449

#define	DCPT1_GRAD 2.8916
#define	DCPT1_GRAD_FIX 354 //380
#define DCPT2_GRAD  2.8876
#define	DCPT2_GRAD_FIX   355

//#define  ACPT1_OFFSET 2062
#define  ACPT1_GRAD 1.853
#define  ACPT1_GRAD_FIX 1062

#define  ACPT2_OFFSET 2113//2161//2128
#define  ACPT2_GRAD 3.1332
#define  ACPT2_GRAD_FIX 327//326


//#define  ACPT3_OFFSET 2061
#define  ACPT3_GRAD 1.853
#define  ACPT3_GRAD_FIX 1062

//#define GRPT1_OFFSET 2062
#define GRPT1_GRAD 3.198
#define GRPT1_GRAD_FIX 320

#define GRPT2_OFFSET 2113//2161//2129
#define GRPT2_GRAD 3.1327
#define GRPT2_GRAD_FIX 327//326

//#define GRPT3_OFFSET 2062
#define GRPT3_GRAD 3.198
#define GRPT3_GRAD_FIX 320

//#define  CT1_OFF 1761
#define  CT1_GRAD 14.525
#define  CT1_GRAD_FIX 70

//#define  CT2_OFF 1628
#define  CT2_GRAD 27
#define  CT2_GRAD_FIX 38

//#define  CT3_OFF 1628
#define  CT3_GRAD 26.725
#define  CT3_GRAD_FIX 38

//#define  CT4_OFF 1748
#define  CT4_GRAD 14.23
#define  CT4_GRAD_FIX 72

//#define  CT5_OFF 1619
#define  CT5_GRAD 13.112
#define  CT5_GRAD_FIX  78

//#define  CT6_OFF 1742
#define  CT6_GRAD 14.316
#define  CT6_GRAD_FIX 312

//#define CT7_OFF 1757
#define CT7_GRAD 14.498
#define CT7_GRAD_FIX 71

//#define CT8_OFF 1738
#define CT8_GRAD 14.348
#define CT8_GRAD_FIX 71


*/



// GTI#1 infineon 1


#define	DCPT1_OFFSET 2427
#define DCPT2_OFFSET 2455

#define	DCPT1_GRAD 2.862
#define	DCPT1_GRAD_FIX 358
#define DCPT2_GRAD  2.884
#define	DCPT2_GRAD_FIX   355

//#define  ACPT1_OFFSET 2062
#define  ACPT1_GRAD 1.853
#define  ACPT1_GRAD_FIX 1062

#define  ACPT2_OFFSET 2105
#define  ACPT2_GRAD 3.1395
#define  ACPT2_GRAD_FIX 327


//#define  ACPT3_OFFSET 2061
#define  ACPT3_GRAD 1.853
#define  ACPT3_GRAD_FIX 1062

//#define GRPT1_OFFSET 2062
#define GRPT1_GRAD 3.198
#define GRPT1_GRAD_FIX 320

#define GRPT2_OFFSET 2105
#define GRPT2_GRAD 3.1764
#define GRPT2_GRAD_FIX 327


//#define GRPT3_OFFSET 2062
#define GRPT3_GRAD 3.198
#define GRPT3_GRAD_FIX 320

//#define  CT1_OFF 1765
#define  CT1_GRAD 14.292
#define  CT1_GRAD_FIX 68

//#define  CT2_OFF 1650
#define  CT2_GRAD 26.561
#define  CT2_GRAD_FIX 39

//#define  CT3_OFF 1646
#define  CT3_GRAD 26.236
#define  CT3_GRAD_FIX 38

//#define  CT4_OFF 1748
#define  CT4_GRAD 14.23
#define  CT4_GRAD_FIX 72

//#define  CT5_OFF 1637
#define  CT5_GRAD 12.97
#define  CT5_GRAD_FIX  78

//#define  CT6_OFF 1742
#define  CT6_GRAD 14.316
#define  CT6_GRAD_FIX 72

//#define CT7_OFF 1778
#define CT7_GRAD 14.545
#define CT7_GRAD_FIX 72

//#define CT8_OFF 1738
#define CT8_GRAD 14.348
#define CT8_GRAD_FIX 71



/*

// GTI#3 infineon 1


#define	DCPT1_OFFSET 2468//638
#define DCPT2_OFFSET 2474//2449

#define	DCPT1_GRAD 2.9207//2.6836
#define	DCPT1_GRAD_FIX 351//381
#define DCPT2_GRAD  2.8816
#define	DCPT2_GRAD_FIX   355

//#define  ACPT1_OFFSET 2062
#define  ACPT1_GRAD 1.853
#define  ACPT1_GRAD_FIX 1062

#define  ACPT2_OFFSET 2134
#define  ACPT2_GRAD 3.1784
#define  ACPT2_GRAD_FIX 322//1294


//#define  ACPT3_OFFSET 2061
#define  ACPT3_GRAD 1.853
#define  ACPT3_GRAD_FIX 1062

//#define GRPT1_OFFSET 2062
#define GRPT1_GRAD 3.198
#define GRPT1_GRAD_FIX 320

#define GRPT2_OFFSET 2134//2102
#define GRPT2_GRAD 3.156
#define GRPT2_GRAD_FIX 324//1301


//#define GRPT3_OFFSET 2062
#define GRPT3_GRAD 3.198
#define GRPT3_GRAD_FIX 320

//#define  CT1_OFF 1777
#define  CT1_GRAD 14.525
#define  CT1_GRAD_FIX 70

//#define  CT2_OFF 1649
#define  CT2_GRAD 27
#define  CT2_GRAD_FIX 38

//#define  CT3_OFF 1642
#define  CT3_GRAD 26.725
#define  CT3_GRAD_FIX 38

//#define  CT4_OFF 1748
#define  CT4_GRAD 14.23
#define  CT4_GRAD_FIX 72

//#define  CT5_OFF 1638
#define  CT5_GRAD 13.112
#define  CT5_GRAD_FIX  78

//#define  CT6_OFF 1742
#define  CT6_GRAD 14.316
#define  CT6_GRAD_FIX 72

//#define CT7_OFF 1776
#define CT7_GRAD 14.498
#define CT7_GRAD_FIX 71

//#define CT8_OFF 1738
#define CT8_GRAD 14.348
#define CT8_GRAD_FIX 71
*/


#define TQ1_GRAD 1
#define TQ2_GRAD 1
#define AIS1_GRAD 1
#define AIS2_GRAD 1


extern volatile int32_t DCPT1_AIN_Result;
extern volatile int32_t DCPT2_AIN_Result;
extern volatile int32_t ACPT1_AIN_Result;
extern volatile int32_t ACPT2_AIN_Result;
extern volatile int32_t ACPT3_AIN_Result;
extern volatile int32_t GRPT1_AIN_Result;
extern volatile int32_t GRPT2_AIN_Result;
extern volatile int32_t GRPT3_AIN_Result;
extern volatile int32_t CT1_Result;
extern volatile int32_t CT2_Result;
extern volatile int32_t CT3_Result;
extern volatile int32_t CT4_Result;
extern volatile int32_t CT5_Result;
extern volatile int32_t CT6_Result;
extern volatile int32_t CT7_Result;
extern volatile int32_t CT8_Result;
extern volatile int32_t TQ1_AIN_Result;
extern volatile int32_t TQ2_AIN_Result;
extern volatile int32_t AIS1_AIN_Result;
extern volatile int32_t AIS2_AIN_Result;

//extern volatile int32_t DCPT1_OFFSET;
//extern volatile int32_t DCPT2_OFFSET;
extern volatile int32_t ACPT1_OFFSET;
//extern volatile int32_t ACPT2_OFFSET;
extern volatile int32_t ACPT3_OFFSET;
extern volatile int32_t GRPT1_OFFSET;
//extern volatile int32_t GRPT2_OFFSET;
extern volatile int32_t GRPT3_OFFSET;
extern volatile int32_t CT1_OFF;
extern volatile int32_t CT2_OFF;
extern volatile int32_t CT3_OFF;
extern volatile int32_t CT4_OFF;
extern volatile int32_t CT5_OFF;
extern volatile int32_t CT6_OFF;
extern volatile int32_t CT7_OFF;
extern volatile int32_t CT8_OFF;
extern volatile int32_t TQ1_OFF;
extern volatile int32_t TQ2_OFF;
extern volatile int32_t AIS1_OFF;
extern volatile int32_t AIS2_OFF;


extern volatile int32_t DCPT1_fix;
extern volatile int32_t DCPT2_fix;
extern volatile int32_t ACPT1_fix;
extern volatile int32_t ACPT2_fix;
extern volatile int32_t ACPT3_fix;
extern volatile int32_t GRPT1_fix;
extern volatile int32_t GRPT2_fix;
extern volatile int32_t GRPT3_fix;
extern volatile int32_t CT1_fix;
extern volatile int32_t CT2_fix;
extern volatile int32_t CT3_fix;
extern volatile int32_t CT4_fix;
extern volatile int32_t CT5_fix;
extern volatile int32_t CT6_fix;
extern volatile int32_t CT7_fix;
extern volatile int32_t CT8_fix;
extern volatile int32_t TQ1_fix;
extern volatile int32_t TQ2_fix;
extern volatile int32_t AIS1_fix;
extern volatile int32_t AIS2_fix;

void ADC_Init(void);
void ADC_G0_Init(void);
void ADC_G1_Init(void);
void ADC_G2_Init(void);

void ADC_G0_Queue_Init(void);
void ADC_G1_Scan_Init(void);
void ADC_G2_Scan_Init(void);
void ADC_Background_Init(void);

void ADC_SWQueue_Conv(ADC002_HandleType HandlePtr);
void ADC_SWScan_Conv(ADC003_HandleType HandlePtr);
void ADC_SWBackground_Conv(void);

int DCPT1_Filter( int data );
int DCPT2_Filter( int data);
int ACPT2_Filter(int data);
int GRPT2_Filter(int data);

#endif /* ADC_APP_H_ */
