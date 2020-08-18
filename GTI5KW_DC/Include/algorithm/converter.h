
#ifndef CONVERTER_H_
#define CONVERTER_H_

#include <DAVE3.h>
#include "common/global.h"
#include "algorithm/fixed_operation.h"
#include "peripheral/ADC_app.h"
#include "peripheral/PWM_app.h"

#include "peripheral/ISR_app.h"

#include "peripheral/DAC_app.h"


/* definisi nilai yang digunakan */

void inisiasi_control(void);
void calculate_pwm(void);
void restart_control(void );

// untuk kalibrasi infineon 2

extern volatile int DCPT1_infineon_2;
extern volatile int DCPT2_infineon_2;
extern volatile int CT1_infineon_2;
extern volatile int CT5_infineon_2;
extern volatile int CT7_infineon_2;
extern volatile int ACPT2_infineon_2;
extern volatile int GRPT2_infineon_2;
extern volatile int flag_test;


extern volatile int Prefered_Mode;
extern volatile int data_in[12];
extern volatile int data_out[12];
extern volatile int control_mode;
extern volatile float Vreff;
extern volatile int Vreff_inv; // untuk test algoritma inverter
extern volatile int enable_reff_signal;
extern volatile int32_t PWM1;
extern volatile int32_t PWM2;
extern volatile int32_t PWM_M; // untuk test algoritma inverter
extern volatile int32_t Tetaref; // untuk test algoritma inverter

extern volatile int flag_count_pwm;

// untuk debug konstant pengendali inv
extern volatile float pengali_Kcv_inv;
extern volatile float pengali_Kpv_inv;
extern volatile float pengali_Kcc_inv;
extern volatile float pengali_Kpc_inv;
extern volatile float pengali_Kic_inv;

extern volatile int32_t current_test;

volatile int32_t Plimit;

/*variabel yang dapat dirubah berdasarkan kondisi sebelumnya */

    // temp memory untuk menyimpan kondisi sebelumnya
    extern volatile int32_t temp_pv_volt;
    extern volatile int32_t temp_pv_curr;
    extern volatile int32_t temp_pv_power;


    // variabel yang mengatur mode dari conveter
    extern volatile int MODE;

    // beberapa konstanta dan tempat menjumlah nilai error pada kendali tegangan loop pertama mode 1
     extern volatile int32_t errVoDC;
     extern volatile int32_t integ_errVoDC; // total error integral


    // beberapa konstanta dan tempat menjumlah nilai error pada kendali arus 1 dan 2 mode 1
    extern volatile int32_t errIL1;
    extern volatile int32_t errIL2;
    extern volatile int32_t integ_errIL1boost; // total error integral
    extern volatile int32_t integ_errIL2boost; // total error integral

    // variabel untuk menyimpan error arus induktor pada mode 2 ( MPPT )
    extern volatile int32_t integ_errIL1;
    extern volatile int32_t integ_errIL2;

     extern volatile int32_t tempKaw_IL1; // <-- gak tau ini buat apa
     extern volatile int32_t tempKaw_IL2; // <-- gak tau ini buat apa

	// variabel variabel untuk fungsi MPPT dan konstant P
	 extern volatile int32_t dV, dP, dI;
	 extern volatile int32_t Vref, Iref;

	// variabel untuk menyimpan nilai pwm
	 extern volatile int32_t tempPWM1;
	 extern volatile int32_t tempPWM2;

	// variabel untuk menyimpan nilai output
	 extern volatile int32_t PWM1;
	 extern volatile int32_t PWM2;


	// variabel untuk menyimpan input
	 extern volatile int32_t  VinDC;
	 extern volatile int32_t  VoDC;
	 extern volatile int32_t  Ipv;
	 extern volatile int32_t  IL1;
	 extern volatile int32_t  IL2;
	 extern volatile int32_t  Qref;

	//variabel untuk memproses lookup table
	 extern volatile int32_t  indexQ;
	 extern volatile int32_t  tempP;
	 extern volatile int32_t  temp_lookuptable;
	 extern volatile int32_t  tempP1;
	 extern volatile int32_t  Ppvref;
	 extern volatile int     temp_indexQ;

	//variabel untuk menyimpan nilai tegangan dan arus pv yang akan digunakan untuk perhitungan daya
	 extern volatile int32_t PV_voltage;
	 extern volatile int32_t PV_current;
	 extern volatile int32_t PV_power;

	//variabel untuk iterasi
	 extern volatile int i;
	 extern volatile int start_inisiasi;

	// lookup table yang digunakan
	// tipe data yang digunakan adalah int32_t, lihat deskripsi tipe bentukan pada converter_fixed.h
	 extern volatile int32_t Psqrt[30];


	// variabel nilai iterasi pada operasi kontrol mode 2 dan 3
    extern volatile  int32_t dVref ;
    extern volatile int32_t dIref ;  // nilai iterasi tidak akan diubah ubah

    // variabel untuk konstanta pengendali
     extern volatile int32_t Kcv ; // nilai Konstanta integral voltage
     extern volatile int32_t Kpv ; // nilai konstanta proporsional voltage

     extern volatile int32_t Kcc ; // nilai konstantta integral current
     extern volatile int32_t Kpc ; // nilai konstanta proporsional current

    extern volatile  int32_t Kic ;     //konstanta untuk loop control mppt (mode 2)

    extern volatile float Vreff;

// end of dari arif

#endif /* CONVERTER_H_ */
