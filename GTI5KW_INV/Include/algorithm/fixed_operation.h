/*
 * fixed_operation.h
 *
 *  Created on: Oct 24, 2016
 *  Updated on: Jan 13, 2017
 *      Author: Arif
 */

#ifndef FIXED_OPERATION_H_
#define FIXED_OPERATION_H_

#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

/* fungsi untuk operasi fixed point */

	typedef struct{
	int32_t nilai;		// variabel nilai akan menyimpan angka yang sudah dalam bentuk fixed point
	int8_t fract_bits;		// variabel fract_bits akan menyimpan jumlah bit yang digunakan untuk pecahan (fraction)
}	fixed_32;

int32_t float2fixed(float x, int bit);
float fixed2float(int32_t x, int bit);
fixed_32 fixed_perkalian(fixed_32 x,fixed_32 y, int bit);
fixed_32 fixed_pembagian(fixed_32 x,fixed_32 y, int bit);
fixed_32 fixed_penjumlahan(fixed_32 x,fixed_32 y, int bit);
fixed_32 fixed_pengurangan(fixed_32 x,fixed_32 y, int bit);
fixed_32 fixed_trigonometri(fixed_32 theta, int mode);

#endif /* FIXED_OPERATION_H_ */
