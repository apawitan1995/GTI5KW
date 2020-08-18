/*
 * LCD_app.c
 *
 *  Created on: Sep 24, 2014
 *      Author: adjikharisma
 */

#include "external/LCD_app.h"

void lcd_month(unsigned char mont)
{
    unsigned char msg[5];
    switch(mont)
    {
        case 1  : sprintf(msg, "Jan");
                  lcd_puts(msg); break;
        case 2  : sprintf(msg, "Feb");
                  lcd_puts(msg); break;
        case 3  : sprintf(msg, "Mar");
                  lcd_puts(msg); break;
        case 4  : sprintf(msg, "Apr");
                  lcd_puts(msg); break;
        case 5  : sprintf(msg, "May");
                  lcd_puts(msg); break;
        case 6  : sprintf(msg, "Jun");
                  lcd_puts(msg); break;
        case 7  : sprintf(msg, "Jul");
                  lcd_puts(msg); break;
        case 8  : sprintf(msg, "Aug");
                  lcd_puts(msg); break;
        case 9  : sprintf(msg, "Sep");
                  lcd_puts(msg); break;
        case 10  : sprintf(msg, "Oct");
                  lcd_puts(msg); break;
        case 11  : sprintf(msg, "Nov");
                  lcd_puts(msg); break;
        case 12  : sprintf(msg, "Des");
                  lcd_puts(msg); break;
        default : break;
    }
}

//void write_data(unsigned char n, unsigned char pos)
//{
//	(n ? SET_BIT(LCDDAT_PORT->OUT,pos) : CLR_BIT(LCDDAT_PORT->OUT,pos));
//}

void lcd_write(unsigned char data)
{
	((data&0x01) ? SET_BIT(PORT0->OUT,D4) : CLR_BIT(PORT0->OUT,D4));
	((data>>1)&0x01) ? SET_BIT(PORT6->OUT,D5) : CLR_BIT(PORT6->OUT,D5);
	((data>>2)&0x01) ? SET_BIT(PORT6->OUT,D6) : CLR_BIT(PORT6->OUT,D6);
	((data>>3)&0x01) ? SET_BIT(PORT6->OUT,D7) : CLR_BIT(PORT6->OUT,D7);

//	write_data((data)&0x01, D4);
//	write_data((data>>1)&0x01, D5);
//	write_data((data>>2)&0x01, D6);
//	write_data((data>>3)&0x01, D7);

}

void lcd_cmd(unsigned char Data)
{

	RS_LOW(); //because sending command
    EN_LOW();

//  sprintf(kal,"data high \r\n");
//  UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

    lcd_write((Data >> 4) & 0x0F);

    EN_HIGH();
    My_Delay_ms(2);
    EN_LOW();

//  sprintf(kal,"data low \r\n");
//  UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

    lcd_write(Data & 0x0F);

    EN_HIGH();
    My_Delay_ms(2);
    EN_LOW();

//  sprintf(kal,"cmd success \r\n");
//  UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
}

void lcd_data(unsigned char l)
{
    RS_HIGH();  //because sending data
    EN_LOW();
    lcd_write((l >> 4) & 0x0F);

    EN_HIGH();
    My_Delay_us(40);
    EN_LOW();

    lcd_write(l & 0x0F);
    EN_HIGH();
    My_Delay_us(40);
    EN_LOW();
}

void lcd_init(void)
{


	//Set as output pin BUZZER P1.14 as lcd RESET PIN
	WR_REG(PORT1->IOCR12,PORT1_IOCR12_PC14_Msk,PORT1_IOCR12_PC14_Pos,0x10);
	SET_BIT(PORT1->OUT,RESET_LCD);

	//Set output RS	P6.0
	WR_REG(PORT6->IOCR0,PORT6_IOCR0_PC0_Msk,PORT6_IOCR0_PC0_Pos,0x10);
	//Set output RW	P6.1
	WR_REG(PORT6->IOCR0,PORT6_IOCR0_PC1_Msk,PORT6_IOCR0_PC1_Pos,0x10);
	//Set output EN	P6.2
	WR_REG(PORT6->IOCR0,PORT6_IOCR0_PC2_Msk,PORT6_IOCR0_PC2_Pos,0x10);

	//Set output D4 P0.15
	WR_REG(PORT0->IOCR12,PORT0_IOCR12_PC15_Msk,PORT0_IOCR12_PC15_Pos,0x10);
	//Set output D5 P6.4
	WR_REG(PORT6->IOCR4,PORT6_IOCR4_PC4_Msk,PORT6_IOCR4_PC4_Pos,0x10);
	//Set output D6 P6.5
	WR_REG(PORT6->IOCR4,PORT6_IOCR4_PC5_Msk,PORT6_IOCR4_PC5_Pos,0x10);
	//Set output D7 P.6.6
	WR_REG(PORT6->IOCR4,PORT6_IOCR4_PC6_Msk,PORT6_IOCR4_PC6_Pos,0x10);

	My_Delay_ms(40);
	CLR_BIT(PORT1->OUT,RESET_LCD); //BUZZER P1.14

	EN_LOW();
    RS_LOW();
    EN_LOW();
	RW_LOW();

	lcd_write(0x3);
	EN_HIGH();
    My_Delay_ms(2);
    EN_LOW();

    My_Delay_ms(2);

    //lcd_write(0x3);
    EN_HIGH();
    My_Delay_ms(2);
    EN_LOW();

    My_Delay_ms(2);

    //lcd_write(0x3);
    EN_HIGH();
    My_Delay_ms(2);
    EN_LOW();

    My_Delay_ms(2);

    lcd_write(2);
    EN_HIGH();
    My_Delay_ms(2);
    EN_LOW();

    My_Delay_ms(2);

    lcd_cmd(0x29);   // FUNCTION SET, set data length 4 bit 2 line, IS table = 1
    My_Delay_ms(2);

	lcd_cmd(0x14);   // INTERNAL OSC FREQ
	My_Delay_ms(2);

	lcd_cmd(0x7F);   // CONTRAST SET LOW BIT
	My_Delay_ms(2);

	lcd_cmd(0x5E);   // POWER/ICON/CONTRAST CONTROL HIGH BIT
	My_Delay_ms(2);

	lcd_cmd(0x6C);   // FOLLOWER CONTROL
	My_Delay_ms(100);
	My_Delay_ms(100);
	My_Delay_ms(100);

    lcd_cmd(0x0C);   // set display on cursor off blink off
	My_Delay_ms(2);

    lcd_cmd(0x01); // clear lcd
	My_Delay_ms(2);

    lcd_cmd(0x06);  // cursor shift direction
	My_Delay_ms(2);

    lcd_cmd(0x80);  //set ram address
    My_Delay_ms(2);
}

void lcd_NT7605_init(void)
{


	//Set as output pin BUZZER P1.14 as lcd RESET PIN
	WR_REG(PORT1->IOCR12,PORT1_IOCR12_PC14_Msk,PORT1_IOCR12_PC14_Pos,0x10);
	SET_BIT(PORT1->OUT,RESET_LCD);

	//Set output RS P6.0
	WR_REG(PORT6->IOCR0,PORT6_IOCR0_PC0_Msk,PORT6_IOCR0_PC0_Pos,0x10);
	//Set output RW P6.1
	WR_REG(PORT6->IOCR0,PORT6_IOCR0_PC1_Msk,PORT6_IOCR0_PC1_Pos,0x10);
	//Set output EN p6.2
	WR_REG(PORT6->IOCR0,PORT6_IOCR0_PC2_Msk,PORT6_IOCR0_PC2_Pos,0x10);

	//Set output D4 P0.15
	WR_REG(PORT0->IOCR12,PORT0_IOCR12_PC15_Msk,PORT0_IOCR12_PC15_Pos,0x10);
	//Set output D5 P6.4
	WR_REG(PORT6->IOCR4,PORT6_IOCR4_PC4_Msk,PORT6_IOCR4_PC4_Pos,0x10);
	//Set output D6 P6.5
	WR_REG(PORT6->IOCR4,PORT6_IOCR4_PC5_Msk,PORT6_IOCR4_PC5_Pos,0x10);
	//Set output D7 P6.6
	WR_REG(PORT6->IOCR4,PORT6_IOCR4_PC6_Msk,PORT6_IOCR4_PC6_Pos,0x10);

	My_Delay_ms(40);
	//SET_BIT(PORT6->OUT,RESET_LCD);

//	sprintf(kal,"lcd write 3 \r\n");
//	UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

	EN_LOW();
    RS_LOW();
    EN_LOW();
	RW_LOW();
    lcd_write(0x3);

    My_Delay_ms(40);

//  sprintf(kal,"lcd write 2 \r\n");
// 	UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

   	EN_HIGH();
    EN_LOW();
    My_Delay_ms(5);
    EN_HIGH();
    EN_LOW();
    My_Delay_ms(5);
    EN_HIGH();
    EN_LOW();
    My_Delay_ms(2);
    lcd_write(2);
    EN_HIGH();
    EN_LOW();

//  sprintf(kal,"set data length \r\n");
//  UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

    lcd_cmd(0x28);   //set data length 4 bit 2 line

//  sprintf(kal,"delay? \r\n");
//  UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

    My_Delay_ms(100);
    My_Delay_ms(100);
    My_Delay_ms(50);

//  sprintf(kal,"display on \r\n");
//  UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

    lcd_cmd(0x0C);   // set display on cursor off blink off
    My_Delay_ms(100);
	My_Delay_ms(100);
	My_Delay_ms(50);

//  sprintf(kal,"clear lcd \r\n");
//  UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

    lcd_cmd(0x01); // clear lcd
    My_Delay_ms(100);
	My_Delay_ms(100);
	My_Delay_ms(50);

//  sprintf(kal,"cursor shift direction \r\n");
//  UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

    lcd_cmd(0x06);  // cursor shift direction
    My_Delay_ms(100);
	My_Delay_ms(100);
	My_Delay_ms(50);

//  sprintf(kal,"set ram \r\n");
//  UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

    lcd_cmd(0x80);  //set ram address
    My_Delay_ms(100);
    My_Delay_ms(100);
    My_Delay_ms(50);

}

void lcd_wait(int x)
{
    int i;

    {
        for (i=0;i<=200;i++);
    }
}

void lcd_gotoxy(unsigned char  x,unsigned char y)
{
     if(x<40)
     {
      if(y) x|=0b01000000;
      x|=0b10000000;
      lcd_cmd(x);
     }
}

void lcd_puts(char * message)
{    // Write message to LCD (C string type)
    //lcd_cmd(0x8); // disable display;
    while (*message)
    {            // Look for end of string
    	lcd_data(*message++);
	}
	//lcd_cmd(0xE); // enable display;
}

/*
void lcd_print(char count, char msg[15])
{
    char i;
    while(i<count)
    {
        lcd_data(msg[i]);
        i++;
    }
}
*/
/*void lcd_puts(char *msg)
{    // Write message to LCD (C string type)
    //lcd_cmd(0x8); // disable display;
    int count = 0;

    while (msg[count]!=0)
    {            // Look for end of string
    	lcd_data(msg[count]);
        count++;
    }
	//lcd_cmd(0xE); // enable display;
}
 * */
void lcd_clear(void)
{
	//	Send command to LCD (0x01)
	lcd_cmd(0x01);
    My_Delay_ms(2);
}

void lcd_write_byte(unsigned char addr, unsigned char data)
{
	My_Delay_ms(40);
	lcd_cmd(addr);
	My_Delay_ms(40);
	RS_HIGH();
	lcd_data(data);
	My_Delay_ms(40);
}


