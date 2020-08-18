/*
 * ISR_app.c
 *
 *  Created on: Sep 26, 2014
 *      Author: angga
 */

#include "peripheral/ISR_app.h"


volatile bool PLL_status = FALSE;
volatile bool SYNC_status = FALSE;
volatile bool pre_droop = FALSE;
volatile long long temp_voac_peak;
volatile int32_t temp_voac_peak_pll;

volatile long long temp_frekuensi_pll;
volatile int32_t frekuensi_pll=0;
volatile int32_t temp_frekuensi_pll_avg;
volatile int16_t counter320=0;
volatile int32_t teta_manual=1024;
volatile int32_t delta_voltage = 0;
volatile int32_t DCPT2_fix_old = 0;
volatile int mppt_enable = 0;
volatile int32_t errdeltaVcor_fix;
volatile int32_t counter4=0;

volatile int32_t preVDCin_z1_fix=0;
volatile int32_t preVDCin_fix = 0;
volatile int32_t VDCin_Filt_z1_fix = 0;
volatile int32_t VDCin_Filt_fix = 0;
volatile int32_t errVDCin_fix = 0;
volatile int32_t integVDCin_fix= 0;
volatile int32_t Pdccomp_fix = 0;
volatile int32_t Ppv_fix;
volatile float Ppv;
//volatile int32_t Plimit = 3072000; // 3000 Q10
//volatile int32_t PV_voltage = 0;
//volatile int32_t PV_current = 0;
//volatile int32_t dV = 0; // 10
//volatile int32_t temp_pv_volt = 0; //10
//volatile int32_t dI = 0; // 10
//volatile int32_t temp_pv_curr = 0; //10
//volatile int32_t PV_power = 0; // 3+7-10 = 10
//volatile int32_t dP = 0; // 10
//volatile int32_t temp_pv_power = 0; // 10
//volatile int32_t Iref = 0;
//volatile int32_t dIref = 3; // 0.05 *51 Q10
//volatile int i = 0;

void UART_XMC_Handler(void)
{
	char data;

	data=UART_U2->OUTR;

	string_read_xmc[string_read_xmc_increment]= data;

	if(data == 'T'){
		flag_update_receive_xmc=1;
		string_read_xmc_increment=-1;
	}
	else if(data == 'I'){
		uart_xmc_request(1);
	
		string_read_xmc_increment=-1;
	}
	else if(data == 'X'){
		flag_update_send_xmc=1;
		string_read_xmc_increment=-1;
	}
	else if(data == 'Y'){ // matikan inverter , perintahnya g
		inverter_enable=0;
		string_read_xmc_increment=-1;
		PLL_status = FALSE; // Toggle Status PLL
		SYNC_status = FALSE;

	}
	else if(data == 'Z'){ // nyalakan inverter, nalain G
		inverter_enable=1;
		string_read_xmc_increment=-1;
	}
	else if(data == 'A'){ // droop off ,
		droop_control_enable=0;
		pll_voltage_enable = 0;
		mppt_enable =0;
		//PLL_status = TRUE; // Toggle Status PLL
		//SYNC_status = TRUE;
		l_impedance = 10;
		string_read_xmc_increment=-1;
	}
	else if(data == 'B'){ // droop on // masih dimatiin
		droop_control_enable=1;

		mppt_enable = 1;
		//pll_voltage_enable=1;
		l_impedance = 10;
		//PLL_status = FALSE; // Toggle Status PLL
		//SYNC_status = FALSE;
		string_read_xmc_increment=-1;
	}
	else if(data == 'C'){ // matikan PLL, perintahny x
		PLL_status = FALSE; // Toggle Status PLL

		string_read_xmc_increment=-1;
	}
	else if(data == 'D'){ // nyalakan PLL, perintahnya X
		PLL_status = TRUE; // Toggle Status PLL

		string_read_xmc_increment=-1;
	}
	
	else if(data == 'E'){ // matikan sinkronisasi
		SYNC_status = FALSE;


		string_read_xmc_increment=-1;
	}
	else if(data == 'F'){ // nyalakan sinkronisasi
		SYNC_status = TRUE;

		string_read_xmc_increment=-1;
	}

	else if(data == 'G'){

		//PLL_status = TRUE; // Toggle Status PLL
		//SYNC_status = TRUE;
				string_read_xmc_increment=-1;
	}
	else if(data == 'g'){

		//PLL_status = FALSE; // Toggle Status PLL
		//SYNC_status = FALSE;
		//droop_control_enable=1;
		l_impedance = 10;
		string_read_xmc_increment=-1;
	}

	else if(data == 'H'){
		pll_voltage_enable=0;
		string_read_xmc_increment=-1;
	}
	else if(data == 'h'){
		pll_voltage_enable=1;
		string_read_xmc_increment=-1;
	}

	else if(data == 'J'){
		mppt_enable = 1;
		//teta_manual=teta_manual+10240;
		string_read_xmc_increment=-1;
	}
	else if(data == 'j'){
		mppt_enable =0;
		//teta_manual=teta_manual-10240;
		string_read_xmc_increment=-1;
	}

	else if(data == 'K'){
		//voac_peak_ref_fix+=1024;
			//if(voac_peak_ref_fix>337920) //337920 //307200)
			//{voac_peak_ref_fix = 337920;} //337920 //307200;}
		teta_manual=teta_manual-10240;


		string_read_xmc_increment=-1;

	}
	else if(data == 'k'){
	//	voac_peak_ref_fix-=1024;
		//if(voac_peak_ref_fix<10240)
		//{voac_peak_ref_fix = 10240;}

		teta_manual=teta_manual+10240;

		string_read_xmc_increment=-1;
	}

	else if(data=='['){ // perintahnya ,
		//voac_peak_ref_fix+=10240;
		//	if(voac_peak_ref_fix>337920) //337920 //307200)
		//	{voac_peak_ref_fix = 337920;} //337920 //307200;}

		string_read_xmc_increment=-1;
	}

	else if (data == ']'){ // perintahnya .
		//voac_peak_ref_fix-=10240;
		//if(voac_peak_ref_fix<10240)
		//{voac_peak_ref_fix = 10240;}

		string_read_xmc_increment=-1;
	}
	string_read_xmc_increment++;

	SET_BIT(UART_U2->TRBSCR,USIC_CH_TRBSCR_CSRBI_Pos);
}

void PWM4A4B_IRQ_Handler(void)	//interrupt handler PWM_1A_1B CCU81.SR3
{
	comparepwm4=3749;
	comparepwm5=3749;

	static int32_t ind = 0;
	static int8_t counter32=0;
	static int8_t j=0;
	static int8_t counter_dac=0;
	static char kal[32];

	while(CheckBit(VADC_G0->RESD[DCPT1_AIN_Result_Register],VADC_G_RESD_VF_Pos)==0);
		DCPT1_AIN_Result = VADC_G0->RES[DCPT1_AIN_Result_Register] & VADC_G_RES_RESULT_Msk;
		//DCPT1_AIN_Result =  DCPT1_Filter(DCPT1_AIN_Result);
		DCPT1_fix = ((int32_t)DCPT1_AIN_Result-DCPT1_OFFSET)*DCPT1_GRAD_FIX;
		//DCPT1 = ((int32_t)DCPT1_AIN_Result-DCPT1_OFFSET)*DCPT1_GRAD;
		//PV_voltage=DCPT1_fix; //Q10

	while(CheckBit(VADC_G0->RESD[DCPT2_AIN_Result_Register],VADC_G_RESD_VF_Pos)==0);
	DCPT2_AIN_Result = VADC_G0->RES[DCPT2_AIN_Result_Register] & VADC_G_RES_RESULT_Msk;
	//DCPT2_AIN_Result =  DCPT2_Filter(DCPT2_AIN_Result);
	DCPT2_fix = ((int32_t)DCPT2_AIN_Result-DCPT2_OFFSET)*DCPT2_GRAD_FIX;

	while(CheckBit(VADC_G0->RESD[CT1_Result_Register],VADC_G_RESD_VF_Pos)==0);
	CT1_Result = VADC_G0->RES[CT1_Result_Register] & VADC_G_RES_RESULT_Msk;
	CT1_fix = -((int32_t)CT1_Result-CT1_OFF)*CT1_GRAD_FIX;
	//CT1 = ((int32_t)CT1_Result-CT1_OFF)*CT1_GRAD;
	//PV_current=CT1_fix; // Q10

	//SET_BIT(PORT1->OUT,10);
	//SET_BIT(LCDDAT_PORT->OUT,D4);

	// Set Flag for Time Slot Checking
	SET_BIT(PORT0->OUT,10);


	//-------------  MPPT  --------------//

	/*
	// LFP====
	preVDCin_fix = (DCPT2_fix>>10)*1; //Q0,  P before filtering, max val 2k
	VDCin_Filt_fix = preVDCin_fix*2055 + 2055*preVDCin_z1_fix+1020*(VDCin_Filt_z1_fix>>10); //b0 = b1 =  0.0019596476; Q20
	preVDCin_z1_fix = preVDCin_fix;
	VDCin_Filt_z1_fix =VDCin_Filt_fix;	//after filtering

	 */
	/*
	dV = DCPT1_fix - temp_pv_volt; // 10
	temp_pv_volt = DCPT1_fix; //10

	dI = CT1_fix - temp_pv_curr; // 10
	temp_pv_curr = CT1_fix; //10


	PV_power = ((DCPT1_fix>>7)*(CT1_fix>>3)); // 3+7-10 = 10
	dP = PV_power - temp_pv_power; // 10
	temp_pv_power = PV_power; // 10
*/

	if (mppt_enable == 1){
		/*i++;
	    if(i==5)
	    {	i = 0;
	    	if (dV  == 0 )
	    	{
	    		if (dI > 0)
	    		{
	    		Iref = Iref - dIref; // 10
	    		}

	    		else if (dI < 0)
	    		{
	    		Iref = Iref + dIref; // 10
	    		}
	    	}

	    	else if (dV != 0)
	    	{
	    		if ( dP*dV > 0)  // dapat juga menggunakan dP/dV
	    		{
	    		Iref = Iref - dIref; // 10
	    		}
	    		else if ( dP*dV  < 0 && PV_power <Plimit)
	    		{
	    		Iref = Iref + dIref;
	    		}
	    		else if ( dP*dV  < 0 && PV_power > Plimit)  //Limit to 3k
	    		{
	    		Iref = Iref -dIref;
	    		}
	    	}
	    }

    	if (Iref  > float2fixed(100,10)){ Iref = float2fixed(100,10);}
    	else if ( Iref < float2fixed(0.5,10)) { Iref = float2fixed(0.5,10);}
		*/
    	Ppv_fix = ((CT1_fix)*(DCPT1_fix>>10)); // Q10

    	Ppv = fixed2float(Ppv_fix,10);

		errVDCin_fix =  DCPT2_fix - 399360; //(VDCin_Filt_fix>>10) - 399360;	//Q10 , 390

		integVDCin_fix += (errVDCin_fix>>3)*5;	// 1024/1600 = 0.64, Q20 GTI1 5.12

		//if (integVDCin_fix < -float2fixed(100,10)){ integVDCin_fix = -float2fixed(100,10);} // ini cuma ada di GTI 1



		Pdccomp_fix = (errVDCin_fix)*26 + (integVDCin_fix>>10);	// Q10 * 10 // GTI3 26, GTI2 25, GTI1 75

		Pset = fixed2float(Pdccomp_fix,10)+ (Ppv) ;

		/*
		if (DCPT2_fix > 419840 ) // 410 << 10
				{
				Pset = Pset + 50;
				}
			else if ( DCPT2_fix < 337920) // 330 << 10
				{S
				Pset = Pset - 50;
			}
			*/
	}
	else {
		integVDCin_fix = 0;
		//Iref = float2fixed(2,10);
	}

	//	if (Pset < 0){Pset = 0;}


	//-------------MPPT END--------------//



	//-------------PLL--------------//

		// AC voltage sensed from ACPT2 for PLL
		VacPLL_fix = GRPT2_fix/VAC_REF; //Q10
		Vac_self_fix = ACPT2_fix/VAC_REF; //Q10


		// PLL active only when the Flag is set
		if(PLL_status == TRUE) {
			//temp_voac_peak_pll
			temp_voac_peak_pll = PLL_fix();
			//PLL_Self_fix();
	//		sin_theta_pll_fix = sinTeta_PLL_fix; // Q11, added to print theta calc from PLL
			if (counter4 < 320){
						counter4++;
						temp_voac_peak= temp_voac_peak+ temp_voac_peak_pll;
						temp_frekuensi_pll = temp_frekuensi_pll + wPLL_fix;
					}
					else {
						voac_peak_pll = temp_voac_peak;
						frekuensi_pll = (temp_frekuensi_pll*0.003125);
						temp_voac_peak =0;
						temp_frekuensi_pll=0;
						counter4=0;
					}
		}
		else {
			integ_Vac_q_fix = 0;
			integ1_fix = 0;
			integ2_fix = 0;
			//integ_Vac_q_self_fix=0;
			//integ1_self_fix = 0;
			//integ2_self_fix = 0;
			counter4=0;
			temp_voac_peak = 0;
			temp_frekuensi_pll=0;
		}

		/* ---------------------- END OF PLL ------------------------*/

		/* ---------------------- CHECK FAULT -----------------------*/
		//if(fault_count>4) {
		//	inverter_enable = 0;
		//}
		/* ------------------END OF CHECK FAULT ---------------------*/

		if(inverter_enable)
		{
			SET_BIT(PORT0->OUT,9);

			if(SYNC_status == TRUE) {
	//			Synchronization_Loop(1);
				Tetaref_fix = Teta_PLL+deltaw_fix+teta_manual;
			}
			else {
	//			Synchronization_Loop(0);
				Tetaref_Generator();
			}

	//		Tetaref_Generator();

			//if(pre_droop == TRUE){
			//	counter320++;
			//	if(counter320 == 1600){
			//		SYNC_status = FALSE;
			//		droop_control_enable = 1;
			//		pre_droop = FALSE;
			//		sprintf(kal,"DROOP ON\r\n");
			//		UART001_WaitWriteDataMultiple(UART_U2,kal, strlen(kal));
			//	}
			//}

			if(droop_control_enable)
			{
				//power_calculation(MODE);
				//if(counter4 == 0)
				//{
				DroopControl_P(); // output deltaw
				//DroopControl_Q(); // output voac_peak_fix
				//w_fix = wPLL_fix + deltaw_fix; //Q20
				//}

				//w=wref+n(p-pref)
			}
			else
			{
				DroopDisable();
				//voac_peak_fix = voac_peak_ref_fix;

				w_fix = wref_fix;
				deltaw_fix=0; //Q20
			}



			if(pll_voltage_enable)
			{
				//voac_peak_fix = voac_peak_fix;
				//if ( j >= 320){
					if (Q_Filt<Qset){errdeltaVcor_fix += 1;}
					else if (Q_Filt>Qset){errdeltaVcor_fix -= 1;}
				//	j=0;
			//	}
				//j++;
				}
			else
			{
				errdeltaVcor_fix=0;
				//voac_peak_fix = voac_peak_ref_fix;
			}

			voac_peak_fix = voac_peak_ref_fix + errdeltaVcor_fix;



			//Trigonometric Function in Fixed-Point
			TetaSine_fix = Tetaref_fix + TetaSynch;	// 20 using Tetaref_fix as an input for trigo. func.
			if(TetaSine_fix > 3294206) {TetaSine_fix -= 6588413;}
			else if (TetaSine_fix < -3294206) {TetaSine_fix+=6588413;}

	//		Tetaref_fix = Tetaref_fix + TetaSynch; //20 bit
	//		if(Tetaref_fix > 3294206) {Tetaref_fix -= 6588413;}
	//		else if (Tetaref_fix < -3294206) {Tetaref_fix+=6588413;}

	//		TetaSine_fix = Tetaref_fix;
			Sin_Cos_App_Fixed_Q10(); // Calculate Sine and Cosine for Control Loop

	//		if(Vsync_loop == TRUE)
	//		{
	//			VoACreff_fix = (voac_peak_fix+(Vsynch>>10))*sinTeta_fix; //Q:10+10=20
	//			temp_voac_peak = voac_peak_fix+(Vsynch>>10);
	//
	//		}else
	//		{
				VoACreff_fix = (voac_peak_fix)*sinTeta_fix; //Q:10+10=20
	//			voac_peak_ref_fix = temp_voac_peak;
	//		}

			Virtual_Impedance();
			Voltage_Loop();
			Inner_Current_Loop();

	//		PWMInv_fix = (ILreffAC_fix >> 10);

			if(PWMInv_fix > LIM_PWM_INV_FIXQ20){PWMInv_fix = LIM_PWM_INV_FIXQ20;}
			if(PWMInv_fix < -LIM_PWM_INV_FIXQ20){PWMInv_fix = -LIM_PWM_INV_FIXQ20;}

			comparepwm4 =  (((int32_t)((-PWMInv_fix>>9)*1.83))+3749)>>1; //20-9=11
			comparepwm5 =  (((int32_t)((PWMInv_fix>>9)*1.83))+3749)>>1; //20-9=11

			PWM_Inverter_Enable();

			//-------------power calculation (including filter)--------------//
			//frekuens1 4 kHz / 1/4 kali frekuensi switching
			//if(counter4 == 0)
			//{
			//	counter4=4;
			//	SET_BIT(PORT0->OUT,1);
				//Power_Filter_16k();
			//	Power_Filter();
			//	CLR_BIT(PORT0->OUT,1);
			//}
			//counter4--;
			//-------------END-power calculation (including filter)--------------//

			Power_Filter_16k();

			CLR_BIT(PORT0->OUT,9);

			}
		else
		{
			PWM_Inverter_Disable();
			Tetaref=0;
			ILreffAC_fix = 0;
			errIL_fix = 0;
			integ_errIL_fix = 0;
			integ_errVoAC_d_fix = 0;
			integ_errVoAC_q_fix = 0;

			// Reset Integrator PLL
			integ_Vac_q_fix = 0;

			TetaSynch = 0;
	//		Vsynch = 0;
			//integ_Vg_Vpll_Filt_f = 0;
	//		integ_errVsynch = 0;



	//		integ_errIL = 0;
	//		integ_errVoAC_d = 0;
	//		integ_errVoAC_q = 0;
	//		ind=0;
	//		PWMInverter_Set_Compare(3749,3749);
		}

		//CLR_BIT(LCDDAT_PORT->OUT,D4);
		//CLR_BIT(PORT1->OUT,10);
		CLR_BIT(PORT0->OUT,10);


	PWM4A4B_Set_Compare(comparepwm4,comparepwm4);


	//Clear Interrupt
	SET_BIT(PWM_4A_4B->SWR,CCU8_CC8_SWR_RCM1D_Pos);
	SET_BIT(PWM_4A_4B->SWR,CCU8_CC8_SWR_RCM2D_Pos);
}

void PWM5A5B_IRQ_Handler(void)	//interrupt handler PWM_2A_2B CCU81.SR1
{

	PWM5A5B_Set_Compare(comparepwm5,comparepwm5);
	//PWM6A6B_Set_Compare(3749,3749);

	//Clear Interrupt
	SET_BIT(PWM_5A_5B->SWR,CCU8_CC8_SWR_RCM1D_Pos);
	SET_BIT(PWM_5A_5B->SWR,CCU8_CC8_SWR_RCM2D_Pos);
}

void PWM6A6B_IRQ_Handler(void)	//interrupt handler PWM_3A_3B CCU81.SR2
{


	while(CheckBit(VADC_G2->RESD[CT5_Result_Register],VADC_G_RESD_VF_Pos)==0);
	CT5_Result = VADC_G2->RES[CT5_Result_Register] & VADC_G_RES_RESULT_Msk;
	CT5_fix = ((int32_t)CT5_Result-CT5_OFF)*CT5_GRAD_FIX; // Q10


	while(CheckBit(VADC_G1->RESD[CT7_Result_Register],VADC_G_RESD_VF_Pos)==0);
	CT7_Result = VADC_G1->RES[CT7_Result_Register] & VADC_G_RES_RESULT_Msk;
	CT7_fix = ((int32_t)CT7_Result-CT7_OFF)*CT7_GRAD_FIX; // Q10


	while(CheckBit(VADC_G2->RESD[ACPT2_AIN_Result_Register],VADC_G_RESD_VF_Pos)==0);
	ACPT2_AIN_Result = VADC_G2->RES[ACPT2_AIN_Result_Register] & VADC_G_RES_RESULT_Msk;
	//ACPT2_AIN_Result = ACPT2_Filter(ACPT2_AIN_Result);
	ACPT2_fix = ((int32_t)ACPT2_AIN_Result-ACPT2_OFFSET)*ACPT2_GRAD_FIX; //Q10


	while(CheckBit(VADC_G2->RESD[GRPT2_AIN_Result_Register],VADC_G_RESD_VF_Pos)==0);
	GRPT2_AIN_Result = VADC_G2->RES[GRPT2_AIN_Result_Register] & VADC_G_RES_RESULT_Msk;
	//GRPT2_AIN_Result = GRPT2_Filter(GRPT2_AIN_Result);
	GRPT2_fix = ((int32_t)GRPT2_AIN_Result-GRPT2_OFFSET)*GRPT2_GRAD_FIX; //Q10

	PWM6A6B_Set_Compare(3749,3749);

	//Clear Interrupt
	SET_BIT(PWM_6A_6B->SWR,CCU8_CC8_SWR_RCM1D_Pos);
	SET_BIT(PWM_6A_6B->SWR,CCU8_CC8_SWR_RCM2D_Pos);
}
