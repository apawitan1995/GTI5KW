/*
 * fixed_operation.c
 *
 *  Created on: Oct 24, 2016
 *  Updated on: Jan 13, 2017
 *      Author: Arif
 */


#include "algorithm/fixed_operation.h"

/*===============================================================================================
fungsi float to fixed point

input 	: nilai double yang akan di konversi dan stop bitnya dalam int
output	: fixed point dengan tipe data float_64

Fungsi ini akan merima nilai double yang akan dikonversi lalu dirubah menjadi fixed point
jumlah bit pecahan fixed point dapat diatur dan akan disimpan di dalam tipe bentukan float_64

=================================================================================================*/

int32_t  float2fixed(float x, int bit){
	int32_t temp;
	temp = (x * (1 << bit))+(1>>(bit-1));
	return temp;
}

/*===============================================================================================
fungsi fixed point to float

input 	: nilai fixed point dalam tipe bentukan fixed_64
output	: nilai dalam bentuk double

Fungsi ini akan merima fixed point dalam bentuk fixed_64 dan melakukan konversi ke double sesuai
dengan bit pecahan yang digunakannya
=================================================================================================*/

float fixed2float(int32_t x, int bit){
	float temp;
	temp = x;
	temp = temp / (1 << bit );

	return temp;
}


/*===============================================================================================
fungsi perkalian fixed point

input 	: dua buah nilai fixed point dengan tipe fixed_64 dan nilai bit pecahan yang diinginkan pada hasil akhir
output	: fixed point dengan tipe data float_64 dan nilai bit pecahan sesuai dengan yang di set user

fungsi ini akan mengalikan dua buah nilai fixed point dan menghasilkan nilai fixed point juga dengan
nilah bit pecahan sesuai dengan input user

=================================================================================================*/

fixed_32 fixed_perkalian(fixed_32 x,fixed_32 y, int bit){
	fixed_32 temp;
	int64_t temp1,temp2;
	int geser_x;
	int geser_y;
	temp1 = x.nilai;
	temp2 = y.nilai;

	if(x.fract_bits != bit){
		geser_x = x.fract_bits - bit;
	}

	if(y.fract_bits != bit){
		geser_y = y.fract_bits - bit;
	}

	temp.nilai = ((temp1*temp2)+(1<<bit+geser_x+geser_y-1))>> (bit+geser_x+geser_y); // ada rounding sederhana
   	temp.fract_bits= bit;
	return temp;
}

/*===============================================================================================
fungsi pembagian fixed point

input 	: dua buah nilai fixed point dengan tipe fixed_64 dan nilai bit pecahan yang diinginkan pada hasil akhir
output	: fixed point dengan tipe data float_64 dan nilai bit pecahan sesuai dengan yang di set user

fungsi ini akan membagikan dua buah nilai fixed point dan menghasilkan nilai fixed point juga dengan
nilah bit pecahan sesuai dengan input user

=================================================================================================*/
fixed_32 fixed_pembagian(fixed_32 x,fixed_32 y, int bit){
	fixed_32 temp;
	int64_t temp1,temp2,temp3;
	int geser_temp;
	int geser;
	int geser_x;
	int geser_y;
	temp1 = x.nilai;
	temp2 = y.nilai;

	if ( x.fract_bits > y.fract_bits){
		geser_temp = x.fract_bits;
		}
	else {
		geser_temp = y.fract_bits;
		}

	if(x.fract_bits != geser_temp){
		geser_x = x.fract_bits - geser_temp;
		if ( geser_x < 0){
            temp1 = x.nilai << -geser_x;
		}
		else{
            temp1 = (x.nilai+(1<<(geser_x-1))) >> geser_x;
		}
	}

	if(y.fract_bits != geser_temp){
		geser_y = y.fract_bits - geser_temp;
		if ( geser_y < 0){
            temp2 = y.nilai << -geser_y;
		}
		else{
            temp2 = (y.nilai+(1<<(geser_y-1))) >> geser_y;
		}
	}

	temp3 = (temp1<<geser_temp)/temp2;

	geser = geser_temp - bit;

	if ( geser < 0){
        temp3 = temp3<< -geser;
	}
	else {
        temp3 = (temp3+(1<<(geser-1)))>> geser;
	}
	temp.nilai = temp3;
    temp.fract_bits= bit;
	return temp;
}
/*===============================================================================================
fungsi penjumlahan fixed point

input 	: dua buah bilangan fixed point dengan tipe fixed_64 dan nilai bit pecahan yang diinginkan pada hasil akhir
output	: fixed point dengan tipe data float_64

Fungsi ini akan merima 2 buah bilangan fixed point dan menjumlahkannya
pertama-tama akan dilakukan penyamaan nilai bit pecahan menjadi sesuai input user dan setelah itu dilakukan penjumlahan seperti biasa
jumlah bit pecahan fixed point dapat diatur dan akan disimpan di dalam tipe bentukan float_64
=================================================================================================*/

fixed_32 fixed_penjumlahan(fixed_32 x,fixed_32 y, int bit){
	fixed_32 temp;
	int geser_x;
	int geser_y;

	int64_t temp1 = x.nilai;
	int64_t temp2 = y.nilai;

	if(x.fract_bits != bit){
		geser_x = x.fract_bits - bit;
		if ( geser_x < 0){
            temp1 = x.nilai << -geser_x;
		}
		else{
            temp1 = (x.nilai+(1<<(geser_x-1))) >> geser_x;
		}
	}

	if(y.fract_bits != bit){
		geser_y = y.fract_bits - bit;
		if ( geser_y < 0){
            temp2 = y.nilai << -geser_y;
		}
		else{
            temp2 = (y.nilai+(1<<(geser_y-1))) >> geser_y;
		}
	}

	temp.nilai = temp1+temp2;
	temp.fract_bits = bit;

	return temp;
}
/*===============================================================================================
fungsi pengurangan fixed point

input 	: dua buah bilangan fixed point dengan tipe fixed_64 dan nilai bit pecahan yang diinginkan pada hasil akhir
output	: fixed point dengan tipe data float_64

Fungsi ini akan merima 2 buah bilangan fixed point dan mengurangkannya
pertama-tama akan dilakukan penyamaan nilai bit pecahan menjadi sesuai input user dan setelah itu dilakukan pengurangan seperti biasa
jumlah bit pecahan fixed point dapat diatur dan akan disimpan di dalam tipe bentukan float_64

=================================================================================================*/

fixed_32 fixed_pengurangan(fixed_32 x,fixed_32 y, int bit){
	fixed_32 temp;
	int geser_x;
	int geser_y;

	int64_t temp1 = x.nilai;
	int64_t temp2 = y.nilai;

	if(x.fract_bits != bit){
		geser_x = x.fract_bits - bit;
		if ( geser_x < 0){
            temp1 = x.nilai << -geser_x;
		}
		else{
            temp1 = (x.nilai+(1<<(geser_x-1))) >> geser_x;
		}
	}

	if(y.fract_bits != bit){
		geser_y = y.fract_bits - bit;
		if ( geser_y < 0){
            temp2 = y.nilai << -geser_y;
		}
		else{
            temp2 = (y.nilai+(1<<(geser_y-1))) >> geser_y;
		}
	}

	temp.nilai = temp1-temp2;
	temp.fract_bits = bit;

	return temp;
}

/*===============================================================================================
fungsi trigonometri

input 	: mendapatkan input fixed point theta dalam radian ( HARUS KETELITIAN 20 BIT) dan mode operasi ( 0 untuk sinus, 1 untuk cosinus)
output	: fixed point dengan tipe data fixed_32

=================================================================================================*/


//Trigonometri in 16 bit signed fixed point math 11.20, range for non negative number
#define half_pi 0x00006487


fixed_32 fixed_trigonometri(fixed_32 theta, int mode){
	fixed_32 temp;
	int geser_theta;
	int bit = 20;
	int64_t Tetaref_fix;
	int64_t TetaSine_fix;
	int64_t TetaCos_fix;
	int64_t integSine_fix;
	int64_t integCos_fix;

	Tetaref_fix  = theta.nilai;

	if(theta.fract_bits != bit){
		geser_theta = theta.fract_bits - bit;
		if ( geser_theta < 0){
            Tetaref_fix  = Tetaref_fix  << -geser_theta;
		}
		else{
            Tetaref_fix  = (Tetaref_fix +(1<<(geser_theta-1))) >> geser_theta;
		}
	}

	while (Tetaref_fix > 3294206 ) {Tetaref_fix -= 6588413;} // 20 bit

	TetaSine_fix = Tetaref_fix;
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


	if ( mode == 0){
		temp.nilai = integSine_fix;
	}
	else if ( mode == 1){
		temp.nilai = integCos_fix;
	}

	temp.fract_bits = bit;

	return temp;
}

