
/*
 * inverter.h
 *
 *  Created on: 2 Jan 2015
 *      Author: Dell
 */

#ifndef INVERTER_H_
#define INVERTER_H_

#include <DAVE3.h>
#include <arm_math.h>
#include "common/global.h"
#include "peripheral/ADC_app.h"
#include "peripheral/PWM_app.h"
#include "algorithm/PLL.h"

#include "peripheral/DAC_app.h"


extern volatile int l_impedance;

//=============Controller Gain=======================================//
//Outer Loop
// KP and KI for Inverter
#define KpVI 0.1	//2,5 10-2, 26
#define KiVI 0.03125 // KcVI = 500/16k  //0,0625, 64

//Inner Loop
#define KpCI 0.01	// 10
#define KiCI 0.00625	// 10
//===================================================================//

//=============Integration Limiter====================================//
#define LimMaxInv 100
#define LimMinInv -100
//====================================================================//

#define LIM_INTV_INV_FIX 266338304 // 127<<21 532676608 //(127<<22)
#define LIM_INTV_INV_FIX20 2097152000 // 133169152 // 127<<21 532676608 //(127<<22)
#define LIM_INT_IL_FIX  52428800 // 6291456 //(3<<21) 1610612736 //(3<<29)
#define LIM_PWM_INV_FIX 2097152 // 1<<21 536870912 //(1<<29)
#define LIM_PWM_INV_FIXQ20 1048576 // 1<<20

#define VoAC_PEAK_FIX 51200 //102400 //30720 //318464 //311*1024
#define VAC_REF 320

//tambahan untuk droop
extern volatile int32_t b0_Ld_fix;
extern volatile int32_t a1_Ld_fix;	//Q10
extern volatile float deltafrad;
extern volatile float deltaw;
extern volatile int32_t deltaw_fix;
extern volatile long long IacLd_fix, IacLd_y_fix, IacLd_yz_fix, IacLd_yz1_fix, IacLd_z1_fix, IacLdo_fix, IacLdo_z1_fix;
extern volatile int32_t w_fix;
extern volatile float w_droop;
extern volatile float wPLL;

extern volatile int32_t ki;
extern volatile int32_t kp;

extern volatile int32_t ki_il;
extern volatile int32_t kp_il;

// Data Acquistion
extern volatile int32_t ACPT1_fix;
extern volatile int32_t ACPT2_fix;
extern volatile int32_t CT5_fix;
extern volatile int32_t CT6_fix;

extern volatile int32_t vreff_inv;

extern volatile float VoAC;
extern volatile float ILout;
extern volatile float VoACreff;

extern volatile int32_t VoAC_fix;
extern volatile int32_t ILout_fix;
extern volatile int32_t VoACreff_fix;

extern volatile int32_t VoAC_Filt_fix;
extern volatile int32_t VoAC_Filt_z1_fix;
extern volatile int32_t VoAC_z1_fix;
extern volatile int32_t ILout_Filt_fix;
extern volatile int32_t ILout_Filt_z1_fix;
extern volatile int32_t ILout_z1_fix;

// Edit Open Loop
extern volatile int32_t VoACreff_OpenLoop_fix;
extern volatile int32_t voac_peak_openloop_fix;
extern volatile int32_t voac_reff_peak_openloop_fix; // Q10
extern volatile bool Closed_Loop;
extern volatile bool Vsync_loop;

/*========== PI Voltage/Current outer Loop =============*/
// reference and feedback on dq frame
extern volatile float VoAC_d;
extern volatile float VoAC_q;
extern volatile float VoACreff_d;
extern volatile float VoACreff_q;

// error, integration, and output as a reference for inner loop
extern volatile float errVoAC_d;
extern volatile float integ_errVoAC_d;
extern volatile float ILreffAC_d;

extern volatile float errVoAC_q;
extern volatile float integ_errVoAC_q;
extern volatile float ILreffAC_q;

extern volatile float ILreffAC;

extern volatile int32_t errVoAC_fix;
extern volatile int32_t integ_errVoAC_fix;
extern volatile int32_t ILreffAC_fix;

extern volatile int32_t errVoAC_d_fix, errVoAC_q_fix;
extern volatile int32_t integ_errVoAC_d_fix, integ_errVoAC_q_fix;
extern volatile int32_t ILreffAC_d_fix, ILreffAC_q_fix;


/*========== PI Current inner Loop =============*/
extern volatile float errIL;
extern volatile float integ_errIL;
extern volatile float PWMInv;

extern volatile int32_t errIL_fix;
extern volatile int32_t integ_errIL_fix;
extern volatile int32_t PWMInv_fix;

extern volatile float cos_theta;
extern volatile float sin_theta;

extern volatile int32_t cos_theta_fix;
extern volatile int32_t sin_theta_fix;

// Edit to Print Teta from PLL
extern volatile int32_t sin_theta_pll_fix;

extern volatile float theta_deg_master;
extern volatile float theta;
extern volatile int32_t count_theta;

extern volatile uint8_t inverter_enable;
extern volatile uint8_t droop_control_enable;
extern volatile uint8_t pll_voltage_enable;
extern volatile uint8_t SIGNAL_OK;

extern volatile int32_t Iac_fix;
extern volatile int32_t FIRCoef_Fix[81];
extern volatile int32_t acc_fix;
extern volatile int32_t accQ_fix;
extern volatile int32_t In_FiltBuff_fix[81];
extern volatile int32_t In_Filt_fix;
extern volatile int32_t In_FiltBuffQ_fix[81];
extern volatile int32_t In_FiltQ_fix;
extern volatile int32_t P_fix, Q_fix;

extern volatile float fsys;
extern volatile int32_t Nsamp;
extern volatile float fsamp;

extern volatile int32_t VoBuff[320];
extern volatile int32_t Vo90;
extern volatile int32_t p;
extern volatile int32_t q;

extern volatile float prePow;
extern volatile float Pow_Filt;
extern volatile float Pow_Filt_test; // Dummy Variable, for Testing only
extern volatile float Q_Filt;
extern volatile float preQ;
extern volatile float b0;
extern volatile float a1;

extern volatile int32_t voac_peak_fix;
extern volatile int32_t voac_peak_ref_fix;
extern volatile int32_t voac_pll_avg;
extern volatile int32_t voac_peak_pll;

extern volatile int32_t TetaSine_fix, TetaCos_fix, integSine_fix, integCos_fix;
extern volatile int32_t sinTeta_fix, cosTeta_fix;
extern volatile int32_t Tetaref_fix;
extern volatile float Tetaref;
extern volatile float Teta_delta;

// Synchronization Loop
extern volatile int32_t Vsynch;
extern volatile int32_t TetaSynch;
extern volatile float integ_Vg_Vpll_Filt_f;
extern int32_t RMS1;
extern int32_t RMS2;

void Tetaref_Generator(void);
void Inverter_Control(void);
void Inverter_Control_Q20(void);
void Sin_Cos_App_Fixed_Q10(void);
void Power_Filter(void);
void Power_Filter_16k(void);
void Synchronization_Loop(int ENABLE);
void Inner_Current_Loop(void);
void Voltage_Loop(void);
void Virtual_Impedance(void);

#endif /* INVERTER_H_ */
