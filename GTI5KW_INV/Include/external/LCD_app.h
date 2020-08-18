/*
 * LCD_app.h
 *
 *  Created on: Sep 24, 2014
 *      Author: angga
 */

/*-------------------- lcd.c ---------------------*/
/* thanks to: Ferdaus Ario Nurman 13206049 EP-ITB */

#ifndef LCD_APP_H_
#define LCD_APP_H_

#include <DAVE3.h>
#include "common/delay.h"
//debug
#include <stdio.h>
#include "peripheral/UART_app.h"

//#define RS  4 //P6.4
//#define RW  3 //P6.3
//#define EN  2 //P6.2

//#define D4  14 //P1.14
//#define D5  13 //P1.13
//#define D6  12 //P1.12
//#define D7  11 //P1.11
//#define RESET_LCD 0 //P1.0

//#define LCDCON_PORT PORT6
//#define LCDDAT_PORT PORT1

//------------------------------------------
//#define RS  0 //P1.0
//#define RW  1 //P1.1
//#define EN  2 //P1.2

//#define D4  5 //P6.5
//#define D5  15 //P1.15
//#define D6  6 //P6.6
//#define D7  10 //P1.10

//#define RESET_LCD 4 //P6.4

//#define LCDCON_PORT PORT1
//#define LCDDAT_PORT PORT6
//------------------------------------------
#define RS  0 //P6.0
#define RW  1 //P6.1
#define EN  2 //P6.2
#define D5  4 //P6.4
#define D6  5 //P6.5
#define D7  6 //P6.6

#define RESET_LCD 14 //P1.14

#define D4  15 //P0.15

#define RS_HIGH() SET_BIT(PORT6->OUT,RS)
#define RS_LOW() CLR_BIT(PORT6->OUT,RS)

#define RW_HIGH() SET_BIT(PORT6->OUT,RW)
#define RW_LOW() CLR_BIT(PORT6->OUT,RW)
//#define TOGGLE_SCLK() LCD_DATA->OUT^=((uint32_t)1<<SCLK)

#define EN_HIGH() SET_BIT(PORT6->OUT,EN)
#define EN_LOW() CLR_BIT(PORT6->OUT,EN)

void lcd_write(unsigned char data);
void write_data(unsigned char n, unsigned char pos);
void lcd_month(unsigned char mont);
void lcd_gotoxy(unsigned char  ,unsigned char );
void lcd_init(void);
void lcd_cmd(unsigned char);
void lcd_data(unsigned char);
void lcd_wait(int);
void lcd_puts(char * message);
void lcd_write_byte(unsigned char addr, unsigned char data);
void define_char(unsigned char *pc,unsigned char char_code);
void lcd_clear(void);
void lcd_NT7605_init(void);

//void lcd_print(char count, char msg[15]);

#endif /* LCD_APP_H_ */
