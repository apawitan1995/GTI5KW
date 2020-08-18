
#include "algorithm/converter.h"

/* punya bima */

//extern variable untuk mengatur DC-DC di fungsi ISR, coba sambil di crosscheck di isr.c
volatile uint32_t enable_CV=0;
volatile uint32_t enable_voltage_update=0;
volatile int32_t vodc_reff_boost_fix = 153600 ; // 112640 ; //307200; 153600 // Starting Set Point for Closed Loop


// ini variabel untuk melimit power yang akan dikeluarkan oleh GTI, Q10 ya ini
volatile int32_t Plimit = 512000;//3072000; //3000 Q10 819200;//




/*variabel yang dapat dirubah berdasarkan kondisi sebelumnya */

	volatile int flag_count_pwm;
	volatile int i;

	volatile int32_t PV_voltage, PV_current, PV_power;
	volatile int32_t dV, dP, dI;
	volatile int32_t temp_pv_volt = 0.0, temp_pv_curr = 0.0, temp_pv_power =0.0;
	volatile int32_t Vref = 153600;// Q10
	volatile int32_t Iref = 512; // Q10
	volatile int32_t dVref = 51;//Q10 // 0.05*1024;
	volatile int32_t dIref =  3;//0.5*51; // Q10 //0.05*1024;
	volatile int32_t Kic = 3;//Q18 // 0.000011875*1024*256; // Kic = 0.019/16k
	volatile int32_t VoDC = 0.0, VinDC = 0.0 , Ipv = 0.0;
	volatile int32_t errVoDC = 0.0 , integ_errVoDC =0, errVinDC = 0, integ_errVinDC = 0, errIbatt = 0, integ_errIbatt = 0;
	volatile int32_t ILreff = 0;
	volatile int32_t Kpv = 1024; // Q10 1*1024////2*1024; //2.5;
	volatile int32_t Kcv = 2; // Q13 //0.000122*1024*8; //00015625; // Kcv = 10/0.4/16k *16

	volatile int32_t   integ_errIL1 = 0, PWM1 = 0;
	volatile int32_t   integ_errIL2 = 0, PWM2 = 0;

	volatile int32_t errIL1 = 0 , IL1 = 0 , integ_errIL1buck = 0, integ_errIL1boost = 0  ;
	volatile int32_t errIL2 = 0 , IL2 = 0 , integ_errIL2buck = 0, integ_errIL2boost = 0 ;
	volatile int32_t errIL3 = 0 , IL3 = 0 , integ_errIL3buck = 0, integ_errIL3boost = 0 ;

	volatile int32_t Kpc; // Q10//0.0019*1024*0.5;
	volatile int32_t Kcc; // Q14// //0.000122; // 0.0001875*1024*16; Kcc = 0.00019/0.001/16k

	volatile int MODE;

	volatile float Vreff;
	volatile int Vreff_inv; // untuk test algoritma inverter

    //data yang bisa dirubah
    volatile int control_mode;
    volatile int Prefered_Mode;
    volatile int data_in[12];

    //data output
    volatile int data_out[12];

	/*end of variable declaration */

    // buat kalibrasi

    volatile int DCPT1_infineon_2;
    volatile int DCPT2_infineon_2;
    volatile int CT1_infineon_2;
    volatile int CT5_infineon_2;
    volatile int CT7_infineon_2;
    volatile int ACPT2_infineon_2;
    volatile int GRPT2_infineon_2;

    volatile int flag_test = 0;

    volatile int32_t current_test=1048576; // 1 Q 20




void inisiasi_control (void) {


	// variabel nilai iterasi pada operasi kontrol mode 2 dan 3
    dVref = 51;
    dIref = 3;  // nilai iterasi tidak akan diubah ubah

    // variabel untuk konstanta pengendali
    Kcv = 1; // nilai Konstanta integral voltage //0.000122
    Kpv = 1024; // nilai konstanta proporsional voltage // 1

    Kcc = 2; // nilai konstantta integral current
    Kpc = 1*5; // nilai konstanta proporsional current // khusus untuk GTI 1 kali 5

    Kic = 3;     //konstanta untuk loop control mppt (mode 2)

    // bagian ini hanya diinisiasi 1 KALI SAJA karena
    // pada kondisi sebenarnya nilai ini akan terus dirubah dan diiterasikan
    // ========================================================//
    temp_pv_volt    = 0;
    temp_pv_curr    = 0;
    temp_pv_power   = 0;

    enable_CV   = 0;
    MODE        = 1;

    integ_errVoDC = 0;
    integ_errIL1 = 0 ;
    integ_errIL2 = 0;

    integ_errIL1boost = 0;
    integ_errIL2boost= 0;
    Plimit = 512000;

    Vref = 153600;
    Iref = 2048;
    Pset = 4500;


    Vreff = 0;
    Vreff_inv= 0;

    data_out[0]=3749;
    data_out[1]=3749;
    PWM1 = 0;
    PWM2 = 0;


    //=========================================================//
}

void restart_control(void ){
	// setiap controller DC-DC dimatikan, maka variabel dibawah ini harus di inisiasi ulang
    // bagian ini hanya diinisiasi 1 KALI SAJA karena
    // pada kondisi sebenarnya nilai ini akan terus dirubah dan diiterasikan
    // ========================================================//
    temp_pv_volt    = 0;
    temp_pv_curr    = 0;
    temp_pv_power   = 0;
    flag_test =0;

    enable_CV   = 0;
    MODE        = 1;

    integ_errVoDC = 0;
    integ_errIL1 = 0 ;
    integ_errIL2 = 0;

    integ_errIL1boost = 0;
    integ_errIL2boost= 0;

    Vref = 153600;
    Iref = 2048;
    Plimit = 512000;


    Vreff = 0;

    data_out[0]=3749;
    data_out[1]=3749;
    PWM1 = 0;
    PWM2 = 0;

}

// PWM untuk DC-DC , dia ada dua loop, loop tegangan dan loop arus, loop tegangan nanti dipakai untuk mendapatkan referensi arus
// referensi arus akan dipakai untuk mengontrol loop arus di 2 buah leg induktor
// loop utamanya itu tegangan jadi dia akan berusaha mencapai tegangan kapasitor intermediate yang diinginkan dengan mengontrol arus yang masuk ke induktor.

// di dalem sini juga ada algoritma "MPPT", kalo di MPPT loop controlnya ada 2 , MPPT untuk dapet referensi arus dan loop arusnya.
// controllnya lebih kayak sumber arus jadi referensi arus akan terus dinaikkan sampai tegangan input dari PV turun ke tegangan yang tidak efektif ( liat tabel PV-I dari solar panel )
void calculate_pwm(){


    VinDC   		=   data_in[0];
    VoDC    		=   data_in[1];
    Ipv     		=   data_in[2];
    IL1     		=   data_in[3];
    IL2     		=   data_in[4];

	PV_voltage = VinDC;
	PV_current = Ipv;
	PV_power = ((PV_voltage>>7)*(PV_current>>3)); // 3+7 = 10


        if( MODE == 1)	//Constant Voltage Mode
        {
    		/*========== PI Voltage/Current outer Loop =============*/
    		errVoDC = float2fixed(Vreff,10) - VoDC; // 10
     		integ_errVoDC = integ_errVoDC + (Kcv*(errVoDC>>7)); // 17+3 = 20
    		 if (integ_errVoDC > float2fixed(50,20)){integ_errVoDC=float2fixed(50,20);} // max min 50<20
    		 if (integ_errVoDC <-float2fixed(50,20)){integ_errVoDC=-float2fixed(50,20);}

    		ILreff =integ_errVoDC + (errVoDC*Kpv); // (10+10) = 20
    		 if (ILreff > float2fixed(100,20)){ILreff=float2fixed(100,20);} // max min 100<<10
    		 if (ILreff < -float2fixed(100,20)){ILreff= -float2fixed(100,20);}

    		 // buat debug, bisa nyalain current controllernya aja, kalo dipadukan dengan MPPT dari DC-AC, harusnya sistem bisa tetap stabil selama DC-AC terhubung dengan grid
    		 if (flag_test == 1){
    			 ILreff = current_test;
    		 }

     	    Iref = float2fixed(1,10);
    		/*========= END Here =================*/

    		/*========== PI Current inner Loop, for 3 leg =============*/

    		errIL1 =( ILreff>>10)- IL1; //10
    		integ_errIL1boost = integ_errIL1boost + ((Kcc*(errIL1))>>4); // (10+14)-4= 20
    		 if (integ_errIL1boost > float2fixed(2,20)){integ_errIL1boost= float2fixed(2,20);} // max min 1<<20
    		 if (integ_errIL1boost < -float2fixed(2,20)){integ_errIL1boost=  -float2fixed(2,20);}

    		PWM1 = integ_errIL1boost + (Kpc*(errIL1)); // 10+10 = 20

    		 if (PWM1 >  786432){PWM1 =  786432;} //0.75<<20
    		 if (PWM1 <  0){PWM1= 0;}

    		errIL2 =( ILreff>>10) - IL2; // 10
    		integ_errIL2boost = integ_errIL2boost + ((Kcc*(errIL2))>>4); //(10+14)-4 = 20
    		 if (integ_errIL2boost > float2fixed(2,20) ){integ_errIL2boost= float2fixed(2,20);} // max min 1<<30
    		 if (integ_errIL2boost < -float2fixed(2,20)){integ_errIL2boost= -float2fixed(2,20);}

    		PWM2 = integ_errIL2boost + (Kpc*(errIL2)); // 10+10 = 20
    		 if (PWM2 >  786432){PWM2 =  786432;} //0.75 << 20
    	 	if (PWM2 < 0){PWM2= 0;}



    	}


        else if (MODE == 2) //MPPT Mode
    	{

    i++;
    if(i==320)
    {
    	// bagian mppt cari arus ang sesuai
    	dV = (PV_voltage) - temp_pv_volt; // 10
    	temp_pv_volt = (PV_voltage); //10
    	if ( (dV  <= float2fixed(0.25,10)) && (dV >= float2fixed(-0.25,10)) ) { dV = 0;}


    	dI = (PV_current) - temp_pv_curr; // 10
    	temp_pv_curr = (PV_current); //10

    	if ( (dI <= float2fixed(0.025,10)) && (dI >= float2fixed(-0.025,10)) ) { dI = 0;}


    	dP = PV_power - temp_pv_power; // 10
    	temp_pv_power = PV_power; // 10

    	if ( (dP <= float2fixed(5,10)) && (dP >= float2fixed(-5,10)) ) { dP = 0;}



    	i = 0;
    	// ini dibuat untuk mentoleransi ketidak-akuratan sensor
    	if (( PV_voltage >= float2fixed(250,10)) && PV_voltage < float2fixed(310,10)){
			if ( (dV  == 0) )
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

			else
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
				Iref = Iref - dIref;

				}
			}
    	}
    	else if ( PV_voltage >= float2fixed(310,10) && PV_power <Plimit){

    		Iref= Iref + dIref;
    	}
    	else if ( PV_voltage >= float2fixed(310,10) && PV_power >Plimit){

    		Iref= Iref - dIref;
    	}
    	else if ( PV_voltage < float2fixed(250,10)){

    		Iref= Iref - dIref;
    	}
    }
    	if (Iref  > float2fixed(10,10)){ Iref = float2fixed(10,10);}
    	else if ( Iref < float2fixed(0.1,10)) { Iref = float2fixed(0.1,10);}

 	    ILreff = Iref << 10;


 	    // bagian loop arus sama dengan yang control tegangan
    	errIL1 = Iref - IL1; // 10

    	integ_errIL1 = integ_errIL1 + ((Kic*errIL1)>>8); // (18+10)-8 = 20
    	 if (integ_errIL1 > float2fixed(2,20)){integ_errIL1= float2fixed(2,20);} // max min 1<<20
    	 if (integ_errIL1 < -float2fixed(2,20)){integ_errIL1=  -float2fixed(2,20);}

    	PWM1 = integ_errIL1 + (Kpc*(errIL1)); // 10+10 = 20

    	if (PWM1 > 786432){PWM1 = 786432;} //786432
    	else if (PWM1 < 0){PWM1=0;}

    	errIL2 = Iref - IL2; // 10
    	integ_errIL2 = integ_errIL2 + ((Kic*errIL2)>>8);  // (18+10)-8= 20

    	 if (integ_errIL2 > float2fixed(2,20)){integ_errIL2= float2fixed(2,20);} // max min 1<<20
    	 if (integ_errIL2 < -float2fixed(2,20)){integ_errIL2=  -float2fixed(2,20);}

    	PWM2 = integ_errIL2 + (Kpc*(errIL2)); // 10+10 = 20

    	if (PWM2 > 786432){ PWM2 = 786432;} // 0.75<<20
    	else if (PWM2 < 0){PWM2=0;}

    		}




        data_out[0] = 3749-fixed2float(PWM1,20)*3479;
        data_out[1] = 3749-fixed2float(PWM2,20)*3479;


    data_out[2] = PV_power; // 10 bit
    }
