/*
 * inverter.c
 *
 *  Created on: 2 Jan 2014
 *      Author: asus, BIM2
 */

#include "algorithm/inverter.h"

volatile int32_t vreff_inv=10;

volatile int32_t ki = 0;
volatile int32_t kp = 0;

volatile int32_t ki_il = 0;
volatile int32_t kp_il = 0;
volatile int32_t voac_pll_avg =0;

//tambahan untuk droop
volatile int32_t b0_Ld_fix = 168;
volatile int32_t a1_Ld_fix = -687;	//Q10
volatile float deltafrad = 0;
volatile float deltaw = 0;
volatile int32_t deltaw_fix = 0;
volatile long long IacLd_fix = 0, IacLd_y_fix = 0,IacLd_yz_fix = 0, IacLd_yz1_fix = 0, IacLd_z1_fix = 0, IacLdo_fix = 0, IacLdo_z1_fix = 0;
volatile int32_t w_fix = 0;
volatile float w_droop = 314.16;
volatile float wPLL = 0;

// Data acquisition
volatile int32_t ACPT1_fix = 0;
volatile int32_t ACPT2_fix = 0;
volatile int32_t CT5_fix = 0;
volatile int32_t CT6_fix = 0;

volatile float VoAC;
volatile float ILout;
volatile float VoACreff;

volatile int32_t VoAC_fix;
volatile int32_t ILout_fix;
volatile int32_t VoACreff_fix;

volatile int32_t VoAC_Filt_fix;
volatile int32_t VoAC_Filt_z1_fix;
volatile int32_t VoAC_z1_fix;
volatile int32_t ILout_Filt_fix;
volatile int32_t ILout_Filt_z1_fix;
volatile int32_t ILout_z1_fix;

/*========== PI Voltage/Current outer Loop =============*/
// reference and feedback on dq frame

volatile float VoAC_d = 0;
volatile float VoAC_q = 0;
volatile float VoACreff_d = 0;
volatile float VoACreff_q = 0;

volatile float errVoAC_d = 0;
volatile float integ_errVoAC_d = 0;
volatile float ILreffAC_d = 0;

volatile float errVoAC_q = 0;
volatile float integ_errVoAC_q = 0;
volatile float ILreffAC_q = 0;

volatile float ILreffAC = 0;

volatile int32_t errVoAC_fix = 0;
volatile int32_t integ_errVoAC_fix = 0;
volatile int32_t ILreffAC_fix = 0;

volatile int32_t errVoAC_d_fix, errVoAC_q_fix;
volatile int32_t integ_errVoAC_d_fix, integ_errVoAC_q_fix;
volatile int32_t ILreffAC_d_fix, ILreffAC_q_fix;

/*========== PI Current inner Loop =============*/
volatile float errIL = 0;
volatile float integ_errIL = 0;
volatile float PWMInv = 0;

volatile int32_t errIL_fix = 0;
volatile int32_t integ_errIL_fix = 0;
volatile int32_t PWMInv_fix = 0;

volatile int32_t PWMInv_Filt_fix = 0;
volatile int32_t PWMInv_Filt_z1_fix = 0;
volatile int32_t PWMInv_z1_fix = 0;

volatile float cos_theta;
volatile float sin_theta;
volatile int32_t cos_theta_fix;
volatile int32_t sin_theta_fix;

// Edit to Print Teta from PLL
volatile int32_t sin_theta_pll_fix;

volatile float theta_deg_master = 0;
volatile float theta;
volatile int32_t count_theta=0;

volatile uint8_t inverter_enable=0;
volatile uint8_t droop_control_enable=0;
volatile uint8_t pll_voltage_enable=0;
volatile uint8_t SIGNAL_OK = 0;

//Power Filter
volatile int x,z;
volatile int32_t Iac_fix;
volatile int32_t FIRCoef_Fix[81] = {2,     2,     2,     2,     2,     3,     3,     3,     4,     5,     5,     6,     6,     7,     8,     9,     9,    10,    11,    12,    13,    14,    14,    15,    16,    17,    18,    18,  19,    20,    20,    21 ,  22 ,   22,    23 ,   23,    23 ,   23,    24,    24,    24,    24,   24,    23,    23,    23,    23,    22,    22,    21,    20,    20,    19,    18,    18,    17,    16,    15 ,   14,    14,    13,    12,    11,    10,     9,     9,     8,     7,     6,     6,     5,     5,     4,     3,     3,     3,     2,     2,     2,     2,     2};  // Q10;
volatile int32_t acc_fix;
volatile int32_t accQ_fix;
volatile int32_t In_FiltBuff_fix[81];
volatile int32_t In_Filt_fix;
volatile int32_t In_FiltBuffQ_fix[81];
volatile int32_t In_FiltQ_fix;
volatile int32_t P_fix, Q_fix;

volatile float fsys = 50;
volatile int32_t Nsamp = 0;
volatile float fsamp = 4000;

volatile int32_t VoBuff[320];
volatile int32_t Vo90;
volatile int32_t p = 0;
volatile int32_t q = 0;

volatile float prePow;
volatile float Pow_Filt;
volatile float Pow_Filt_test; // Test Variable Only
volatile float Q_Filt;
volatile float preQ;
volatile float b0 = 0.0015683328;
volatile float a1 = -0.99686333;

volatile int32_t prePow_fix;
volatile int32_t Pow_Filt_fix;
volatile int32_t Q_Filt_fix;
volatile int32_t preQ_fix;

volatile int32_t voac_peak_fix = 10240;

volatile int32_t voac_peak_pll = 0;
volatile int32_t voac_peak_ref_fix = 10240;

volatile int32_t TetaSine_fix, TetaCos_fix, integSine_fix, integCos_fix;
volatile int32_t sinTeta_fix, cosTeta_fix;
volatile int32_t Tetaref_fix;
volatile float Tetaref;
volatile float Teta_delta;

// Edit Open Loop
volatile int32_t VoACreff_OpenLoop_fix; // Q21
volatile int32_t voac_peak_openloop_fix = 0; // Q10
volatile int32_t voac_reff_peak_openloop_fix = 10; // Q10

volatile bool Closed_Loop = FALSE;
volatile bool Vsync_loop = FALSE;

// ------------ Current Control -------------- //
volatile int32_t Iopref = 0, Ioref = 0;
volatile int32_t errId = 0;
volatile int32_t errIq = 0;
volatile int32_t integ_errId = 0;
volatile int32_t integ_errIq = 0;
volatile int32_t Iref_d = 0;
volatile int32_t Iref_q = 0;
//volatile int32_t Iref = 0;
volatile int32_t errIac = 0;
volatile int32_t integ_errIac = 0;

// Synchronization Loop
int N;
int32_t preVg_Vpll;
int32_t Vg_Vpll_Filt;
int32_t preVg_Vpll_z1;
int32_t Vg_Vpll_Filt_z1;
int32_t integ_Vg_Vpll_Filt;
int32_t Peak_Val1;
int32_t Peak_Val2;
int32_t RMS1;
int32_t RMS2;
int32_t errVsynch;
int32_t integ_errVsynch;
volatile int32_t Vsynch;
volatile int32_t TetaSynch;
volatile float preVg_Vpll_f;
volatile float Vg_Vpll_Filt_f;
volatile float preVg_Vpll_z1_f;
volatile float Vg_Vpll_Filt_z1_f;
volatile float integ_Vg_Vpll_Filt_f;
volatile float TetaSynch_f;

volatile int l_impedance = 10;

void Tetaref_Generator(void)
{
	// ----- Teta Reference Generator ------
	Tetaref_fix = Tetaref_fix + (w_fix/16000); //20 bit
	if(Tetaref_fix > 3294206) {Tetaref_fix -= 6588413;}
	else if (Tetaref_fix < -3294206) {Tetaref_fix+=6588413;}
}

void Virtual_Impedance(void)
{

	/*

	IacLd_fix = (CT7_fix)*1024;//l_impedance; //Q20 = 10+10 //Lvir = 5mH
	//IacLd_y_fix = (IacLd_fix - IacLd_z1_fix)*16000; //Q20
	//IacLdo_fix = b0_Ld_fix*IacLd_y_fix + b0_Ld_fix*IacLd_yz1_fix - a1_Ld_fix*IacLdo_z1_fix; //Q30

	IacLd_y_fix = b0_Ld_fix*IacLd_fix + b0_Ld_fix*IacLd_z1_fix - a1_Ld_fix*IacLd_yz1_fix; //Q30


	IacLd_z1_fix = IacLd_fix;
	IacLd_yz1_fix = IacLd_y_fix>>10;
	IacLdo_fix = ((IacLd_y_fix>>10) - IacLd_yz_fix)*16000; //Q20
	IacLd_yz_fix = (IacLd_y_fix>>10);

	//IacLdo_z1_fix = (IacLdo_fix>>10);

	VoACreff_fix -= (IacLdo_fix>>10)*l_impedance;

	*/



	IacLd_fix = (CT7_fix)*l_impedance; //Q20 = 10+10 //Lvir = 5mH
	IacLd_y_fix = (((IacLd_fix) - (IacLd_z1_fix))*16000); //Q20
	IacLdo_fix = b0_Ld_fix*IacLd_y_fix + b0_Ld_fix*IacLd_yz1_fix - a1_Ld_fix*IacLdo_z1_fix; //Q30

	IacLd_z1_fix = IacLd_fix;
	IacLd_yz1_fix = IacLd_y_fix;
	IacLdo_z1_fix = (IacLdo_fix>>10);

	VoACreff_fix -= (IacLdo_fix>>10);



	/*
	IacLd_fix = (CT7_fix)*l_impedance; //Q20
	IacLd_y_fix = ((((IacLd_fix) - (IacLd_z1_fix)))*4000); //Q18
	IacLdo_fix = b0_Ld_fix*IacLd_y_fix + b0_Ld_fix*IacLd_yz1_fix - a1_Ld_fix*IacLdo_z1_fix; //Q28

	IacLd_z1_fix = IacLd_fix; // Q20
	IacLd_yz1_fix = IacLd_y_fix;
	IacLdo_z1_fix = (IacLdo_fix>>10);

	VoACreff_fix -= (IacLdo_fix>>8);

	*/

}

void Voltage_Loop(void)
{
	VoAC_fix = ACPT2_fix; //10

	/* ---------------- DQ transform -------------*/
	errVoAC_d_fix = ((VoACreff_fix>>10) - VoAC_fix)*cosTeta_fix; //Q20	 = 10+10
	integ_errVoAC_d_fix = integ_errVoAC_d_fix + ((errVoAC_d_fix>>10)*32);//96); //1000 Q20
	if(integ_errVoAC_d_fix > LIM_INTV_INV_FIX20){integ_errVoAC_d_fix = LIM_INTV_INV_FIX20;}
	if(integ_errVoAC_d_fix < -LIM_INTV_INV_FIX20){integ_errVoAC_d_fix = -LIM_INTV_INV_FIX20;}
	ILreffAC_d_fix = integ_errVoAC_d_fix + (errVoAC_d_fix>>10)*1;

	errVoAC_q_fix = -((VoACreff_fix>>10) - VoAC_fix)*sinTeta_fix;
	integ_errVoAC_q_fix = integ_errVoAC_q_fix + ((errVoAC_q_fix>>10)*32);//96); // 1000
	if(integ_errVoAC_q_fix > LIM_INTV_INV_FIX20){integ_errVoAC_q_fix = LIM_INTV_INV_FIX20;}
	if(integ_errVoAC_q_fix < -LIM_INTV_INV_FIX20){integ_errVoAC_q_fix = -LIM_INTV_INV_FIX20;}
	ILreffAC_q_fix = integ_errVoAC_q_fix + (errVoAC_q_fix>>10)*1;

	ILreffAC_fix = (ILreffAC_d_fix>>10)*cosTeta_fix - (ILreffAC_q_fix>>10)*sinTeta_fix;
	//	20 = 10+10


	// DAC0_Output_Volt(((fixed2float(ILreffAC_fix,20)*0.008)+1));

//	if(ILreffAC_fix > 183500800){ILreffAC_fix = 183500800;} //+-50
//	if(ILreffAC_fix < -183500800){ILreffAC_fix = -183500800;}

	/* ------------------- End of DQ transform --------------- */
}

void Inner_Current_Loop(void)
{
	errIac = ILreffAC_fix - (CT5_fix<<10); //Q20
	integ_errIac += ((errIac>>10)*ki_il); //Q20
	if (integ_errIac > 13107200 ){integ_errIac = 13107200;}		// Limit integrator to +- 12.5f
	else if (integ_errIac < -13107200){integ_errIac = -13107200;}

//	PWMInv_fix = 1*integ_errIac + ((errIac>>10)*kp_il); //Q20
	PWMInv_fix = 0*integ_errIac + ((errIac>>10)*3); //Q20
//	PWMInv_fix = Iref/5;
//	if (PWMInv_fix > 1048576){PWMInv_fix = 1048576;}		// Limit PWM to +- 1
//	else if (PWMInv_fix <-1048576){PWMInv_fix = -1048576;}
}

void Inverter_Control_Q20(void)
{
	static char kal[32]; // Print For Debugging

	VoAC_fix = ACPT1_fix; //10

	// ---------------- LPF ---------------- //
//	VoAC_Filt_fix = (VoAC_fix+VoAC_z1_fix)*450-(-123)*VoAC_Filt_z1_fix; //Q20, fc = 4 kHz
//	VoAC_z1_fix = VoAC_fix;	//Q10
//	VoAC_Filt_z1_fix = VoAC_Filt_fix>>10; //Q10

//	ILout_Filt_fix = (ILout_fix+ILout_z1_fix)*450-(-123)*ILout_Filt_z1_fix; //Q20, fc = 4 kHz
//	ILout_z1_fix = ILout_fix;
//	ILout_Filt_z1_fix = ILout_Filt_fix>>10;
	// --------------- END LPF ------------- //

	/* ---------------- DQ transform -------------*/
	errVoAC_d_fix = ((VoACreff_fix>>10) - VoAC_fix)*cosTeta_fix; //Q20	 = 10+10
	integ_errVoAC_d_fix = integ_errVoAC_d_fix + ((errVoAC_d_fix>>10)*96); //Q20
	if(integ_errVoAC_d_fix > LIM_INTV_INV_FIX20){integ_errVoAC_d_fix = LIM_INTV_INV_FIX20;}
	if(integ_errVoAC_d_fix < -LIM_INTV_INV_FIX20){integ_errVoAC_d_fix = -LIM_INTV_INV_FIX20;}
	ILreffAC_d_fix = integ_errVoAC_d_fix + (errVoAC_d_fix>>10)*1;

	errVoAC_q_fix = -((VoACreff_fix>>10) - VoAC_fix)*sinTeta_fix;
	integ_errVoAC_q_fix = integ_errVoAC_q_fix + ((errVoAC_q_fix>>10)*96);
	if(integ_errVoAC_q_fix > LIM_INTV_INV_FIX20){integ_errVoAC_q_fix = LIM_INTV_INV_FIX20;}
	if(integ_errVoAC_q_fix < -LIM_INTV_INV_FIX20){integ_errVoAC_q_fix = -LIM_INTV_INV_FIX20;}
	ILreffAC_q_fix = integ_errVoAC_q_fix + (errVoAC_q_fix>>10)*1;

	ILreffAC_fix = (ILreffAC_d_fix>>10)*cosTeta_fix - (ILreffAC_q_fix>>10)*sinTeta_fix;

//	20 = 10+10

	/* ------------------- End of DQ transform --------------- */

	// -------------------- PI Voltage Control Loop -------------- //
//	errVoAC_fix = VoACreff_fix - (VoAC_fix<<10); //(20) - (10+10)

//	if(ki<0)
//	{integ_errVoAC_fix = integ_errVoAC_fix + ((errVoAC_fix>>4)>>(abs(ki)));} // (21)+4-4=21
//	else
//	{integ_errVoAC_fix = integ_errVoAC_fix + ((errVoAC_fix>>4)<<ki);}

//	integ_errVoAC_fix = integ_errVoAC_fix + ((errVoAC_fix>>10)*64);

//	if(integ_errVoAC_fix > LIM_INTV_INV_FIX20){integ_errVoAC_fix = LIM_INTV_INV_FIX20;} //127<<21
//	if(integ_errVoAC_fix < -LIM_INTV_INV_FIX20){integ_errVoAC_fix = -LIM_INTV_INV_FIX20;}

//	if(kp<0)
//	{ILreffAC_fix = integ_errVoAC_fix + (((errVoAC_fix>>10)*26)>>(abs(kp)));} //(21) + 21+5-4, 21-10+10=21
//	else
//	{ILreffAC_fix = integ_errVoAC_fix + (((errVoAC_fix>>10)*26)<<kp);} //21

//	ILreffAC_fix = integ_errVoAC_fix + ((errVoAC_fix>>10)*26);
	// -------------------- PI Voltage Control Loop -------------- //

	/* ---------------- Current Control -------------*/
	ILout_fix = CT5_fix; //10

//	errIL = ILreffAC - ILout;
	errIL_fix = ILreffAC_fix - (ILout_fix<<10); // 20, 10+10=20

//	integ_errIL = integ_errIL + KiCI*errIL;
	integ_errIL_fix = integ_errIL_fix + ((errIL_fix*10)/16000);

	// Check if Integrator reaches its Limit +-50
	if(integ_errIL_fix > LIM_INT_IL_FIX){
		integ_errIL_fix = LIM_INT_IL_FIX;
	}
	if(integ_errIL_fix < -LIM_INT_IL_FIX){
		integ_errIL_fix = -LIM_INT_IL_FIX;
	}

//	PWMInv = integ_errIL + KpCI*errIL;

	//integ_errIL_fix = 0; // added to disable current loop integrator

//	if(kp_il<0)
//	{PWMInv_fix = integ_errIL_fix + (((errIL_fix>>10)*10)>>(abs(kp_il)));} //21, 21-10+10=21,
//	else
//	{PWMInv_fix = integ_errIL_fix + (((errIL_fix>>10)*10)<<kp_il);}

//	if(kp_il<0)
//	{PWMInv_fix = integ_errIL_fix + (((errIL_fix>>10)*10)>>(abs(kp_il))) + (VoACreff_fix/100);} //21, 21-10+10=21,
//	else
//	{PWMInv_fix = integ_errIL_fix + (((errIL_fix>>10)*10)<<kp_il) + (VoACreff_fix/100);}
	/* ---------------- END - Current Control -------------*/

	// ------------- Bypass Current Loop ------------- //
	PWMInv_fix = (ILreffAC_fix >> 10) + (VoACreff_fix/200);
//	PWMInv_fix = (ILreffAC_fix >> 10);
	// ------------- End of Current Loop Bypass ------ //

	// -------------- PWM LPF --------------------- //
//	PWMInv_Filt_fix = ((PWMInv_fix>>10)+(PWMInv_z1_fix>>10))*20-(-985)*(PWMInv_Filt_z1_fix>>10); //Q20, fc = 100;
//	PWMInv_z1_fix = PWMInv_fix;
//	PWMInv_Filt_z1_fix = PWMInv_Filt_fix;
//
//	PWMInv_fix = PWMInv_Filt_fix; // Filtered PWM
	// -------------- END - PWM LPF --------------------- //

	//	if(PWMInv > 1){PWMInv = 1;}
	//	if(PWMInv < -1){PWMInv = -1;}
	if(PWMInv_fix > LIM_PWM_INV_FIXQ20){PWMInv_fix = LIM_PWM_INV_FIXQ20;}
	if(PWMInv_fix < -LIM_PWM_INV_FIXQ20){PWMInv_fix = -LIM_PWM_INV_FIXQ20;}

}

void Inverter_Control_Open(void)
{
	static char kal[32]; // Print For Debugging

//	//Small Voltage Testing, Still Floating point calculation
//	VoAC=((float)ACPT1_AIN_Result-ACPT1_OFFSET)/ACPT1_GRAD;
//	VoACreff = 30*sin_theta;
//	ILout = ((float)CT5_Result-CT5_OFF)/CT5_GRAD;

	VoAC_fix = ((int32_t)ACPT1_AIN_Result-ACPT1_OFFSET)*ACPT1_GRAD_FIX; //10
	// ---------- Sensor Limiter ----------- //
//	if(VoAC_fix > (90<<10)) VoAC_fix=90<<10;
//	if(VoAC_fix < ((-90)<<10)) VoAC_fix=(-90)<<10;
	// ---------- END - Sensor Limiter ----------- //

	ILout_fix = ((int32_t)CT5_Result-CT5_OFF)*CT5_GRAD_FIX; //

	PWMInv_fix = (VoACreff_fix/100);//Q20

	//	if(PWMInv > 1){PWMInv = 1;}
	//	if(PWMInv < -1){PWMInv = -1;}
	if(PWMInv_fix > LIM_PWM_INV_FIXQ20){PWMInv_fix = LIM_PWM_INV_FIXQ20;}
	if(PWMInv_fix < -LIM_PWM_INV_FIXQ20){PWMInv_fix = -LIM_PWM_INV_FIXQ20;}

	// To Short the Full Bridge
	//PWMInv_fix = LIM_PWM_INV_FIX;

}

void Synchronization_Loop(int ENABLE)
{
	if (ENABLE ==1)
 {
		preVg_Vpll_f = ((float)(Vac_self_fix) / 1024) * ((float)(Vac_b_fix) / 1048576);
		Vg_Vpll_Filt_f = preVg_Vpll*b0 + b0*preVg_Vpll_z1_f - a1*Vg_Vpll_Filt_z1_f;

		preVg_Vpll_z1_f = preVg_Vpll_f;
		Vg_Vpll_Filt_z1_f = Vg_Vpll_Filt_f;

		integ_Vg_Vpll_Filt_f += Vg_Vpll_Filt_f*0.00003125f;  //default 100/16k
		if(integ_Vg_Vpll_Filt_f > 5){integ_Vg_Vpll_Filt_f = 5;}
		if(integ_Vg_Vpll_Filt_f < -5){integ_Vg_Vpll_Filt_f = -5;}

		TetaSynch_f = integ_Vg_Vpll_Filt_f + Vg_Vpll_Filt_f*0.05f;
		TetaSynch = (int32_t)(TetaSynch_f*1048576);


		N++;
		//Sum1 += VacPLL*VacPLL;
		//Sum2 += Vo*Vo/96100;
		//if (N>=320) {RMS1 = 310*sqrt(Sum1)/310;RMS2 = 320*sqrt(Sum2)/320; N = 0; Sum1 = 0; Sum2 = 0;}
		if (Peak_Val1<abs(ACPT1_fix)) Peak_Val1 = abs(ACPT1_fix);
		if (Peak_Val2<abs(ACPT2_fix)) Peak_Val2 = abs(ACPT2_fix);
		if (N>=320)
		{
			RMS1 = Peak_Val1*724; //Q20
			RMS2 = Peak_Val2*724; //Q20
			N=0;

			errVsynch = (RMS2 - RMS1)/VAC_REF; //Q20
			integ_errVsynch += (errVsynch>>10)*680; // 1680 //Q20 // *10.50*2500/16000;
			Vsynch = integ_errVsynch + (errVsynch>>10)*15;

			Peak_Val1 = 0;
			Peak_Val2 = 0;

		}

		//rms_calc();
//		errVsynch = (RMS1 - RMS2)/VAC_REF; //Q20
//		integ_errVsynch += (errVsynch>>10)*1680; //Q20 // *10.50*2500/16000;
//		Vsynch = integ_errVsynch + (errVsynch>>10)*15;
 }
	else
		{
//		TetaSynch = 0;
		Vsynch = 0;
//		integ_Vg_Vpll_Filt_f = 0;
		integ_errVsynch = 0;
		}
}

void Sin_Cos_App_Fixed_Q10(void)
{
	//compute sine
	if( TetaSine_fix < 0)
	{
		integSine_fix = 1304*(TetaSine_fix>>10)+(415*(TetaSine_fix>>10)>>10)*(TetaSine_fix>>10); // 20 bit
		if (integSine_fix <0) {integSine_fix = 261*(((integSine_fix>>10)*(-integSine_fix>>10)-integSine_fix)>>10)+integSine_fix;}
		else {integSine_fix = 261*(((integSine_fix>>10)*(integSine_fix>>10)-integSine_fix)>>10)+integSine_fix;}
	}
	else
	{
		integSine_fix = 1304*(TetaSine_fix>>10)-(415*(TetaSine_fix>>10)>>10)*(TetaSine_fix>>10); // 20 bit
		if (integSine_fix <0) {integSine_fix = 261*(((integSine_fix>>10)*(-integSine_fix>>10)-integSine_fix)>>10)+integSine_fix;}
		else {integSine_fix = 261*(((integSine_fix>>10)*(integSine_fix>>10)-integSine_fix)>>10)+integSine_fix;}
	}

	//compute cosine
	TetaCos_fix = TetaSine_fix + 1647100;	// 20
	if (TetaCos_fix > 3294206 ) {TetaCos_fix -= 6588413;}

	if( TetaCos_fix < 0)
	{
		integCos_fix = 1304*(TetaCos_fix>>10)+(415*(TetaCos_fix>>10)>>10)*(TetaCos_fix>>10); // 20 bit
		if (integCos_fix <0) {integCos_fix = 261*(((integCos_fix>>10)*(-integCos_fix>>10)-integCos_fix)>>10)+integCos_fix;}
		else {integCos_fix = 261*(((integCos_fix>>10)*(integCos_fix>>10)-integCos_fix)>>10)+integCos_fix;}
	}

	else
	{
		integCos_fix = 1304*(TetaCos_fix>>10)-(415*(TetaCos_fix>>10)>>10)*(TetaCos_fix>>10); // 20 bit
		if (integCos_fix <0) {integCos_fix = 261*(((integCos_fix>>10)*(-integCos_fix>>10)-integCos_fix)>>10)+integCos_fix;}
		else {integCos_fix = 261*(((integCos_fix>>10)*(integCos_fix>>10)-integCos_fix)>>10)+integCos_fix;}
	}

	sinTeta_fix = integSine_fix>>10; //20-10
	cosTeta_fix = integCos_fix>>10;
}

void Power_Filter(void)
{
//	int32_t z=0;
//	int32_t x=79;

	//float VoAC;
	//float IoAC;

	//static float prePow_z1;
	//static float Pow_Filt_z1;
	//static float Q_Filt_z1;
	//static float preQ_z1;

	//VoAC = fixed2float(ACPT2_fix,10);
	//IoAC = fixed2float(CT7_fix,10);

	// P Calculation  FIR Fixed Point
		acc_fix = 0; z = 0; x = 79;
		In_Filt_fix =(ACPT2_fix>>10)*CT7_fix; // 10
		In_FiltBuff_fix[0] = In_Filt_fix;
		for (z = 0; z < 81; z++)
		{
			acc_fix += (FIRCoef_Fix[z]*In_FiltBuff_fix[z])>>10; //10, Max 15M
		}
		for (x = 79; x>= 0;x--)
		{
			In_FiltBuff_fix[x+1] = In_FiltBuff_fix[x];
		}


	//prePow = VoAC * IoAC; // P before filtering
	//Pow_Filt = prePow*b0 + b0*prePow_z1- a1*Pow_Filt_z1;
	//prePow_z1 = prePow;
	//Pow_Filt_z1 = Pow_Filt;	//after filtering

	//Time Delay to Generate Phase Shifting
	// Penggeseran fasa 90 deg

	//Nsamp = (int32_t)(fsamp/fsys);
	//Nsamp = 80; // 16kHz -> 320 ; 4kHz -> 80
	//if(p>= Nsamp) {p = 0; q = 0;}

	//VoBuff[p] = ACPT2_fix; //Q10

	//if (q < (Nsamp/4)){Vo90 = VoBuff[((3*Nsamp)/4)+q];} //Q10
	//else if (q >= (Nsamp/4)){Vo90 = VoBuff[q-(Nsamp/4)];} //Q10
	//p++;q++;

	//preQ = ((float)(Vo90)/1024) * IoAC; // Q before filtering
	//Q_Filt = preQ*b0 + b0*preQ_z1- a1*Q_Filt_z1;
	//preQ_z1 = preQ;
	//Q_Filt_z1 = Q_Filt;	//after filtering
}

void Power_Filter_16k(void)
{

	//float VoAC;
	//float VoAC_90 = 0;
	//float IoAC;
	//static float SOGI_int1,SOGI_int2  = 0;
	//static float SOGI_int1_z1,SOGI_int2_z1 = 0;
/*
	static float prePow_z1;
	static float Pow_Filt_z1;
	*/
	//static float Q_Filt_z1;
	//static float preQ_z1;



	static int32_t prePow_z1_fix;
	static int32_t Pow_Filt_z1_fix;
	static int32_t Q_Filt_z1_fix;
	static int32_t preQ_z1_fix;




	//VoAC = fixed2float(ACPT2_fix,10);
	//IoAC = fixed2float(CT7_fix,10);

	// ---- SOGI ----- // to generate 90 deg delayed voltage
	//SOGI_int1 = SOGI_int1 + (0.0000625 * fixed2float(ACPT2_fix,10)  * 314.16);
	//SOGI_int1 = SOGI_int1 + (0.0000625 * fixed2float(ACPT2_fix,10)  * ((float)(w_fix) / 1048576) );
	//VoAC_90 = SOGI_int1 - fixed2float(ACPT2_fix,10)  - SOGI_int2;
	//SOGI_int2 = SOGI_int2 + (0.0000625 * VoAC_90 * 314.16);
	//SOGI_int2 = SOGI_int2 + (0.0000625 * VoAC_90 * ((float)(w_fix) / 1048576) );
	//SOGI_int1_z1 = SOGI_int1;
	//SOGI_int2_z1 = SOGI_int2;
	// ------- End SOGI ------- //


prePow_fix = ((ACPT2_fix>>10)*CT7_fix)>>2; //Q8
Pow_Filt_fix = prePow_fix*1.0043 + 1.0043*prePow_z1_fix+1021.99*Pow_Filt_z1_fix; //Q18, b0 = 0.0015683328, a1 = -0.99686333
Pow_Filt = fixed2float(Pow_Filt_fix,18);
prePow_z1_fix = prePow_fix; //Q8
Pow_Filt_z1_fix = Pow_Filt_fix>>10; //Q8	//after filtering , fc = 5 Hz, p to  8 kilo, fsamp 16kHz

preQ_fix = ((Vac_b_fix>>14)*CT7_fix*5)>>2; //Q8
Q_Filt_fix = preQ_fix*1.0043 + 1.0043*preQ_z1_fix+1021.99*Q_Filt_z1_fix; //Q18, b0 = 0.0015683328, a1 = -0.99686333
Q_Filt = fixed2float(Q_Filt_fix,18);
preQ_z1_fix = preQ_fix; //Q8
Q_Filt_z1_fix = Q_Filt_fix>>10; //Q8	//after filtering , fc = 5 Hz, Q to  8 kilo, fsamp 16kHz



	/*
	prePow = fixed2float(ACPT2_fix,10) * fixed2float(CT7_fix,10);; // P before filtering
	Pow_Filt = prePow*0.00098078482 + 0.00098078482*prePow_z1 + 0.99803843*Pow_Filt_z1;
	prePow_z1 = prePow;
	Pow_Filt_z1 = Pow_Filt;	//after filtering
	 */
	//preQ = VoAC_90 * fixed2float(CT7_fix,10); // Q before filtering
	//Q_Filt = preQ*0.00098078482 + 0.00098078482*preQ_z1 + 0.99803843*Q_Filt_z1;
	//preQ_z1 = preQ;
	//Q_Filt_z1 = Q_Filt;	//after filtering


}

