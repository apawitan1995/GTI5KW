/*
 * droop.c
 *
 *  Created on: May 5, 2014
 *      Author: adjikharisma
 */

#include "algorithm/droop.h"

float P, Q, deltaP;
float Pripple = 0, Qripple = 0;
float xf[2];
float yf[2];
float Vo[1500];
float xP[3];
float yP[3];
float xQ[3];
float yQ[3];

float theta_ref = 0.2;
float w = 314.16;
float P_set;
float P_set_comp;
float vo_reff = 220;
float Q_set;
float ld = 0.00035;
float kL =  0.000004;
float delta_w;

int32_t error_pset=0;
int32_t Pset_fix=0;
int32_t integ_error_pset ;
int32_t power =0;

int32_t deltafrad_fix;

volatile int32_t PV_voltage=0;
volatile int32_t PV_current = 0;
volatile int MODE =1;



volatile float integ_Q_Filt = 0;
volatile int32_t Ppv=0;
volatile float Pset = 100; // set nilai minimal, biar gak ada power yang masuk
volatile float Qset = 0;

//VoAC = ACPT1;
////fset = in[3];  // deactivated when operate in islading mode
//VoDC = DCPT2;
//VinDC= DCPT1;
//IL1 = CT2;
//IL2 = CT3;
//IL3 = CT4;
//Ibatt = -CT1;

void FrequencyLPF(void)
{
//  LPF, fc = 10 Hz, to filter high frequency ripple on frequency calculation
	xf[1] = xf[0];
	xf[0] = w_PLL/TWOPI;     // calculate wPLL in Hz
	yf[1] = yf[0];
	yf[0] = b0f*xf[0] + b1f*xf[1] - a1f*yf[1];
}

void PowerCalculation(void)
{
	float  Vo90;
	static int i, k;
	float Tsys;
	float il_out;

	Tsys = floor(16000/yf[0]);
	if (yf[0] > 35)
	{
		if (i > Tsys)
		{
			i = 0;
			k = 0;
		}
		//Vo[i] = ADC_ACPTtoHV((float)ACPT1_AIN_Result); //VoAC
		Vo[i] = vac;

		if (k < (int)floor(Tsys/4))
		{Vo90 = Vo[(int)floor(3*Tsys/4)+k];}
		else if (k >= (int)floor(Tsys/4))
		{Vo90 = Vo[k-(int)floor(Tsys/4)];}

		i++;
		k++;
		//ILout = CT6;
		il_out = ADC_CTtoHV((float)CT6_Result);

		Pripple = il_out*Vo[i];
		Qripple = il_out*Vo90;
	}
}

//Filtering Pripple and Qripple
void PowerFilter(void)
{
	xP[2] = xP[1];
	xP[1] = xP[0];
	xP[0] = Pripple;

	yP[2] = yP[1];
	yP[1] = yP[0];
	yP[0] = b0P*xP[0] + b1P*xP[1] + b2P*xP[2] - a1P*yP[1] - a2P*yP[2];

	xQ[2] = xQ[1];
	xQ[1] = xQ[0];
	xQ[0] = Qripple;

	yQ[2] = yQ[1];
	yQ[1] = yQ[0];
	yQ[0] = b0P*xQ[0] + b1P*xQ[1] + b2P*xQ[2] - a1P*yQ[1] - a2P*yQ[2];

	P = yP[0];
	Q = yQ[0];
	deltaP = yP[0]-yP[1];

	if (Q >= 2000) {Q = 2000;}  // Need to check the effect if it's ignored
	else if (Q <= -2000) {Q = -2000;}
}


void power_calculation(int MODE){

	Ppv = ((PV_voltage>>7)*(PV_current>>3)); // 3+7 = 10

	if ( MODE == 2){
		error_pset =  DCPT2_fix - float2fixed(400,10); // 10

		integ_error_pset = integ_error_pset + (1*(error_pset>>5)); // (11 +5) = 16

		power =( integ_error_pset >>6)+ (error_pset*5);  // 16
	}
	else {
		power = 0;
	}

	Pset_fix = power + Ppv;

	Pset = fixed2float(Pset_fix,10);

}

int32_t DroopControl_P(void)
{
//	wPLL = (float)(wPLL_fix)/1048576;
//	w_droop = wPLL - 0.0000956f*Pow_Filt;
//	w_droop = 314.16f - 0.00005664*(Pow_Filt-0);
//	w_fix = (int32_t)(w_droop*1048576);

//	deltafrad += (Pow_Filt - Pset )*0.00000001;
//	deltaw = -deltafrad - 0.000005*(Pow_Filt - Pset);

//	deltaw = 6.2832 * 0.000062832 * (Pset - Pow_Filt);
	deltaw = 0.00062832 * (Pset-Pow_Filt); //0.00062832
	deltafrad_fix = float2fixed(deltaw,20);
//	Pow_Filt_temp = (Pow_Filt - Pset);
	deltaw_fix = deltaw_fix + (deltafrad_fix/16000); //20 bit, nanti coba di kali 7, sekarang blom
	if(deltaw_fix > 3294206) {deltaw_fix -= 6588413;}
	else if (deltaw_fix < -3294206) {deltaw_fix+=6588413;}

	return (deltaw_fix);
}

void DroopControl_Q(void)
{
//	integ_Q_Filt += (Q_Filt-Qset)*0.0000051;
//	voac_peak_fix = (int32_t)((VAC_REF - integ_Q_Filt - 0.00151*(Q_Filt-Qset))*1024);
//	voac_peak_fix = (int32_t)(((voac_peak_ref_fix/1024) - integ_Q_Filt - 0.00151*(Q_Filt-Qset))*1024);
//	voac_peak_fix = (int32_t)((VAC_REF - 0.0051f*(Q_Filt-0))*1024);
//	voac_peak_fix = (int32_t)(((voac_peak_ref_fix/1024) + 0.0025*(Qset - Q_Filt))*1024);
	voac_peak_fix = (voac_peak_ref_fix + (int32_t)(3.1744*(Qset - Q_Filt))); //3.1744

//	voac_peak_fix = ( (voac_peak_fix <= VAC_REF*1126) ? voac_peak_fix : (VAC_REF*1126) );
//	voac_peak_fix = ( (voac_peak_fix >= VAC_REF*921) ? voac_peak_fix : (VAC_REF*921) );
	voac_peak_fix = ( (voac_peak_fix <= voac_peak_ref_fix*1.2) ? voac_peak_fix : (voac_peak_ref_fix*1.2) );
	voac_peak_fix = ( (voac_peak_fix >= voac_peak_ref_fix*0.8) ? voac_peak_fix : (voac_peak_ref_fix*0.8) );

}

void DroopDisable(void)
{
	// Reset Integrator in Droop Control
	deltafrad = 0;
	integ_Q_Filt = 0;
}
