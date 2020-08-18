/*
 * PLL.c
 *
 *  Created on: 2 Jan 2014
 *      Author: asus
 */
#include "algorithm/PLL.h"

volatile float vac;
volatile float w_PLL;
volatile float theta_PLL;

float theta_deg_PLL;
float cos_theta_PLL=0, sin_theta_PLL=0;
float integ_vac_q=0;
float vac_b;
float integ_a, integ_b;

volatile int32_t xf_PLL[2];
volatile int32_t yf_PLL[2];
volatile int32_t fault_frekuensi;

volatile int32_t Tes_Dummy;
volatile int32_t VacPLL_fix,Vac_b_fix, Vac_tes_fix, Vac_self_fix,Vac_b_self_fix;
volatile int32_t TetaSine_PLL_fix,TetaCos_PLL_fix;
volatile int32_t TetaSine_PLLS_fix,TetaCos_PLLS_fix;
volatile int32_t Teta_PLL,Teta_Self;
volatile int32_t integSine_PLL_fix,integCos_PLL_fix;
volatile int32_t integSine_PLLS_fix,integCos_PLLS_fix;
volatile int32_t sinTeta_PLL_fix,cosTeta_PLL_fix;
volatile int32_t sinTeta_PLLS_fix,cosTeta_PLLS_fix;
volatile int32_t Vac_d_fix, Vac_q_fix;
volatile int32_t integ_Vac_q_fix,integ_Vac_q_self_fix;
volatile int32_t delta_w_fix;
volatile int32_t wPLL_fix,wPLLS_fix; // wPLLS_fix dipakai buat ngirim frekuensi
volatile int32_t wref_fix = 329420636;//50 HZ //329420636;
volatile int32_t Vacw_fix,Vacw_self_fix;
volatile int32_t integ1_fix, integ2_fix;
volatile int32_t integ1_self_fix, integ2_self_fix;

volatile int flag_check_frekuensi =0;

//int32_t Vnom_fix;
//int32_t theta_PLL_fix;
//int32_t theta_deg_PLL_fix;
//int32_t Vd_fix,Vq_fix;
//int32_t cos_theta_PLL_fix, sin_theta_PLL_fix;
//int32_t Err_PLL_fix;
//int32_t Int_Vac_PLL_fix;
//int32_t Pro_Vac_PLL_fix;
//int32_t w_fix;

int32_t PLL_fix()
{
	//static uint16_t index_dummy=0;

	//Vac = in[0]/314;
	//Vac_fix = (long int) (Vac*1024); // 10 bit err, max 1024
	//Vac_fix = ((ACPT2_AIN_Result-ACPT2_OFFSET)*ACPT2_GRAD_FIX)/311;
	//Vac_fix = ((ACPT2_DUMMY[index_dummy]-ACPT2_OFFSET)*ACPT2_GRAD_FIX)/80;

	//Trigonomethric Function in Fixed-Point
	TetaSine_PLL_fix = Teta_PLL;	// 20
	//compute sine
	if( TetaSine_PLL_fix < 0)
	{
		integSine_PLL_fix = 1304*(TetaSine_PLL_fix>>10)+(415*(TetaSine_PLL_fix>>10)>>10)*(TetaSine_PLL_fix>>10); // 20 bit
		if (integSine_PLL_fix <0) {integSine_PLL_fix = 261*(((integSine_PLL_fix>>10)*(-integSine_PLL_fix>>10)-integSine_PLL_fix)>>10)+integSine_PLL_fix;}
		else {integSine_PLL_fix = 261*(((integSine_PLL_fix>>10)*(integSine_PLL_fix>>10)-integSine_PLL_fix)>>10)+integSine_PLL_fix;}
	}
	else
	{
		integSine_PLL_fix = 1304*(TetaSine_PLL_fix>>10)-(415*(TetaSine_PLL_fix>>10)>>10)*(TetaSine_PLL_fix>>10); // 20 bit
		if (integSine_PLL_fix <0) {integSine_PLL_fix = 261*(((integSine_PLL_fix>>10)*(-integSine_PLL_fix>>10)-integSine_PLL_fix)>>10)+integSine_PLL_fix;}
		else {integSine_PLL_fix = 261*(((integSine_PLL_fix>>10)*(integSine_PLL_fix>>10)-integSine_PLL_fix)>>10)+integSine_PLL_fix;}
	}

	//compute cosine
	TetaCos_PLL_fix = TetaSine_PLL_fix + 1647100;	// 20
	if (TetaCos_PLL_fix > 3294206 ) {TetaCos_PLL_fix -= 6588413;}

	if( TetaCos_PLL_fix < 0)
	{
		integCos_PLL_fix = 1304*(TetaCos_PLL_fix>>10)+(415*(TetaCos_PLL_fix>>10)>>10)*(TetaCos_PLL_fix>>10); // 20 bit
		if (integCos_PLL_fix <0) {integCos_PLL_fix = 261*(((integCos_PLL_fix>>10)*(-integCos_PLL_fix>>10)-integCos_PLL_fix)>>10)+integCos_PLL_fix;}
		else {integCos_PLL_fix = 261*(((integCos_PLL_fix>>10)*(integCos_PLL_fix>>10)-integCos_PLL_fix)>>10)+integCos_PLL_fix;}
	}
	else
	{
		integCos_PLL_fix = 1304*(TetaCos_PLL_fix>>10)-(415*(TetaCos_PLL_fix>>10)>>10)*(TetaCos_PLL_fix>>10); // 20 bit
		if (integCos_PLL_fix <0) {integCos_PLL_fix = 261*(((integCos_PLL_fix>>10)*(-integCos_PLL_fix>>10)-integCos_PLL_fix)>>10)+integCos_PLL_fix;}
		else {integCos_PLL_fix = 261*(((integCos_PLL_fix>>10)*(integCos_PLL_fix>>10)-integCos_PLL_fix)>>10)+integCos_PLL_fix;}
	}
	// Trigonomethric Approximation END Here

	sinTeta_PLL_fix = integSine_PLL_fix>>10; //20-10
	cosTeta_PLL_fix = integCos_PLL_fix>>10;

	// ab to dq transform
	Vac_d_fix = VacPLL_fix*sinTeta_PLL_fix - (Vac_b_fix>>10)*cosTeta_PLL_fix;  //20 bit err, 1048576
	Vac_q_fix = VacPLL_fix*cosTeta_PLL_fix + (Vac_b_fix>>10)*sinTeta_PLL_fix;

	//integ_Vac_q = integ_Vac_q + Ki_pll*Vac_q;  // max -+60

	integ_Vac_q_fix = integ_Vac_q_fix + Ki_pll_fix*(Vac_q_fix>>10);  // 20 bit err, vs Ki_pll_fix*(Vac_q_fix>>10)
	delta_w_fix = integ_Vac_q_fix + Kp_pll_fix*(Vac_q_fix>>10);


	wPLL_fix = wref_fix + delta_w_fix; //20 bit err, max 400M

	// Limit frequency of PLL Upper = 55 Hz Lower 45 Hz
	if(wPLL_fix > 362362700) {
		wPLL_fix = 362362700;
	}
	else if (wPLL_fix < 296478572) {
		wPLL_fix = 296478572;
		}


	Teta_PLL = Teta_PLL + (wPLL_fix/16000); //20 bit
	if(Teta_PLL > 3294206) {Teta_PLL -= 6588413;}
	else if (Teta_PLL < -3294206) {Teta_PLL+=6588413;}

	//SOGI fixed point
	Vacw_fix = VacPLL_fix*(wPLL_fix>>10); // 10+(20-10) =20
	integ1_fix = integ1_fix + Vacw_fix/16000; // 20 bit err
	Vac_b_fix = integ1_fix - (VacPLL_fix<<10) - integ2_fix;  // 20
	integ2_fix = integ2_fix + (Vac_b_fix>>10)*(wPLL_fix>>10)/16000; //max 2M
	return ((Vac_d_fix));
}

void PLL_Self_fix(void)
{
	//Trigonomethric Function in Fixed-Point
	TetaSine_PLLS_fix = Teta_Self;	// 20
	//compute sine
	if( TetaSine_PLLS_fix < 0)
	{
		integSine_PLLS_fix = 1304*(TetaSine_PLLS_fix>>10)+(415*(TetaSine_PLLS_fix>>10)>>10)*(TetaSine_PLLS_fix>>10); // 20 bit
		if (integSine_PLLS_fix <0) {integSine_PLLS_fix = 261*(((integSine_PLLS_fix>>10)*(-integSine_PLLS_fix>>10)-integSine_PLLS_fix)>>10)+integSine_PLLS_fix;}
		else {integSine_PLLS_fix = 261*(((integSine_PLLS_fix>>10)*(integSine_PLLS_fix>>10)-integSine_PLLS_fix)>>10)+integSine_PLLS_fix;}
	}
	else
	{
		integSine_PLLS_fix = 1304*(TetaSine_PLLS_fix>>10)-(415*(TetaSine_PLLS_fix>>10)>>10)*(TetaSine_PLLS_fix>>10); // 20 bit
		if (integSine_PLLS_fix <0) {integSine_PLLS_fix = 261*(((integSine_PLLS_fix>>10)*(-integSine_PLLS_fix>>10)-integSine_PLLS_fix)>>10)+integSine_PLLS_fix;}
		else {integSine_PLLS_fix = 261*(((integSine_PLLS_fix>>10)*(integSine_PLLS_fix>>10)-integSine_PLLS_fix)>>10)+integSine_PLLS_fix;}
	}

	//compute cosine
	TetaCos_PLLS_fix = TetaSine_PLLS_fix + 1647100;	// 20
	if (TetaCos_PLLS_fix > 3294206 ) {TetaCos_PLLS_fix -= 6588413;}

	if( TetaCos_PLLS_fix < 0)
	{
		integCos_PLLS_fix = 1304*(TetaCos_PLLS_fix>>10)+(415*(TetaCos_PLLS_fix>>10)>>10)*(TetaCos_PLLS_fix>>10); // 20 bit
		if (integCos_PLLS_fix <0) {integCos_PLLS_fix = 261*(((integCos_PLLS_fix>>10)*(-integCos_PLLS_fix>>10)-integCos_PLLS_fix)>>10)+integCos_PLLS_fix;}
		else {integCos_PLLS_fix = 261*(((integCos_PLLS_fix>>10)*(integCos_PLLS_fix>>10)-integCos_PLLS_fix)>>10)+integCos_PLLS_fix;}
	}
	else
	{
		integCos_PLLS_fix = 1304*(TetaCos_PLLS_fix>>10)-(415*(TetaCos_PLLS_fix>>10)>>10)*(TetaCos_PLLS_fix>>10); // 20 bit
		if (integCos_PLLS_fix <0) {integCos_PLLS_fix = 261*(((integCos_PLLS_fix>>10)*(-integCos_PLLS_fix>>10)-integCos_PLLS_fix)>>10)+integCos_PLLS_fix;}
		else {integCos_PLLS_fix = 261*(((integCos_PLLS_fix>>10)*(integCos_PLLS_fix>>10)-integCos_PLLS_fix)>>10)+integCos_PLLS_fix;}
	}
	// Trigonomethric Approximation END Here

	sinTeta_PLLS_fix = integSine_PLLS_fix>>10; //20-10
	cosTeta_PLLS_fix = integCos_PLLS_fix>>10;

	// ab to dq transform
	//Vac_d_fix = Vac_fix*sinTeta_PLL_fix - (Vac_b_fix>>10)*cosTeta_PLL_fix;  //20 bit err, 1048576
	Vac_q_fix = Vac_self_fix*cosTeta_PLLS_fix + (Vac_b_self_fix>>10)*sinTeta_PLLS_fix;

	//integ_Vac_q = integ_Vac_q + Ki_pll*Vac_q;  // max -+60

	integ_Vac_q_self_fix = integ_Vac_q_self_fix + Ki_pll_fix*(Vac_q_fix>>10);  // 20 bit err, vs Ki_pll_fix*(Vac_q_fix>>10)
	delta_w_fix = integ_Vac_q_self_fix + Kp_pll_fix*(Vac_q_fix>>10);

	wPLLS_fix = wref_fix + delta_w_fix; //20 bit err, max 400M

	// Limit frequency of PLL Upper = 55 Hz Lower 45 Hz


	if(wPLLS_fix > 362362700) {wPLLS_fix = 362362700;}
	else if (wPLLS_fix < 296478572) {wPLLS_fix = 296478572;}

	Teta_Self = Teta_Self + (wPLLS_fix/16000); //20 bit
	if(Teta_Self > 3294206) {Teta_Self -= 6588413;}
	else if (Teta_Self < -3294206) {Teta_Self+=6588413;}

	//SOGI fixed point
	Vacw_self_fix = Vac_self_fix*(wPLLS_fix>>10); // 10+(20-10) =20
	integ1_self_fix = integ1_self_fix + Vacw_self_fix/16000; // 20 bit err
	Vac_b_self_fix = integ1_self_fix - (Vac_self_fix<<10) - integ2_self_fix;  // 20
	integ2_self_fix = integ2_self_fix + (Vac_b_self_fix>>10)*(wPLLS_fix>>10)/16000; //max 2M


}

void PLL(void)
{
	float vac_q;
	//float vac_d;

	//dq transform teta + 90 deg
	//vac_d = vac*sin_theta_PLL - vac_b*cos_theta_PLL;
	vac = ADC_ACPTtoHV((float)ACPT1_AIN_Result);
	vac_q = vac*cos_theta_PLL + vac_b*sin_theta_PLL;

	//PI error = vac_q
	integ_vac_q = integ_vac_q + KI_PLL*vac_q;
	w_PLL = 314.16 + integ_vac_q + KP_PLL*vac_q;
	theta_PLL = theta_PLL + w_PLL*TS;

	if (theta_PLL > TWOPI ) {theta_PLL -= TWOPI;}
	else if (theta_PLL < -TWOPI) {theta_PLL += TWOPI; }

	theta_deg_PLL = R2D*theta_PLL;
  	arm_sin_cos_f32(theta_deg_PLL, &sin_theta_PLL, &cos_theta_PLL);

	//SOGI
	integ_a = integ_a +  vac*w_PLL*TS;
	vac_b = integ_a - vac - integ_b;
	integ_b = integ_b + vac_b*w_PLL*TS;
}


