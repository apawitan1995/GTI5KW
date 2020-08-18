/*
 * PLL.h
 *
 *  Created on: 2 Jan 2014
 *      Author: asus
 */

#ifndef PLL_H_
#define PLL_H_

#include <DAVE3.h>
#include <arm_math.h>
#include <math.h>
#include "common/global.h"
#include "common/look_up_table.h"
#include "peripheral/ADC_app.h"
#include "peripheral/UART_app.h"

#define KP_PLL 40
#define KI_PLL 0.335

#define TWOPI_21 13176794

#define LIM_F_MAX 70
#define LIM_F_MIN 30

#define LIM_W_MAX 439.8226
#define LIM_W_MIN 188.4954

#define Ki_pll_fix 343
#define Kp_pll_fix 40960

extern volatile float vac;
extern volatile float w_PLL;
extern volatile float theta_PLL;

float theta_deg_PLL;
float cos_theta_PLL, sin_theta_PLL;
float integ_vac_q;
float vac_b;
float integ_a, integ_b;

extern volatile int32_t fault_frekuensi;

extern volatile int32_t Tes_Dummy;
extern volatile int32_t VacPLL_fix,Vac_b_fix, Vac_tes_fix, Vac_self_fix,Vac_b_self_fix;
extern volatile int32_t TetaSine_PLL_fix,TetaCos_PLL_fix;
extern volatile int32_t TetaSine_PLLS_fix,TetaCos_PLLS_fix;
extern volatile int32_t Teta_PLL,Teta_Self;
extern volatile int32_t integSine_PLL_fix,integCos_PLL_fix;
extern volatile int32_t integSine_PLLS_fix,integCos_PLLS_fix;
extern volatile int32_t sinTeta_PLL_fix,cosTeta_PLL_fix;
extern volatile int32_t sinTeta_PLLS_fix,cosTeta_PLLS_fix;
extern volatile int32_t Vac_d_fix, Vac_q_fix;
extern volatile int32_t integ_Vac_q_fix,integ_Vac_q_self_fix;
extern volatile int32_t delta_w_fix;
extern volatile int32_t wPLL_fix,wPLLS_fix;
extern volatile int32_t wref_fix;
extern volatile int32_t Vacw_fix,Vacw_self_fix;
extern volatile int32_t integ1_fix, integ2_fix;
extern volatile int32_t integ1_self_fix, integ2_self_fix;
extern volatile int flag_check_frekuensi;

void PLL(void);
int32_t PLL_fix();
void PLL_fixed(void);

extern const long int sin_fixed[321];
extern const long int cos_fixed[321];

#endif /* PLL_H_ */

//extern int32_t Vnom_fix;
//extern int32_t theta_PLL_fix;
//extern int32_t theta_deg_PLL_fix;
//extern int32_t Vd_fix,Vq_fix;
//extern int32_t cos_theta_PLL_fix, sin_theta_PLL_fix;
//extern int32_t Err_PLL_fix;
//extern int32_t Int_Vac_PLL_fix;
//extern int32_t Pro_Vac_PLL_fix;
//extern int32_t w_fix;
