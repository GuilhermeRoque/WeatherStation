/* 
 * File:   LCD.cpp
 * Author: marcone
 * 
 * Created on 27 de Novembro de 2019, 09:47
 */
#include "LCD.h"
#include <avr/io.h>
#include <util/delay.h>


LCD::LCD(uint8_t rs,uint8_t rw, uint8_t en, uint8_t port):RS(GPIO(rs,GPIO::OUTPUT)),RW(GPIO(rw,GPIO::OUTPUT)),EN(GPIO(en,GPIO::OUTPUT)){
  _Px = GPIO_PORT::AllPorts[port];
  _Px->dir_byte(1);
    
}

void LCD::LCD_Command(unsigned char cmnd){
    _Px->write_byte(cmnd);
    RS.clear();
    RW.clear();
    EN.set(1);
	_delay_us(1);
    EN.clear();
	_delay_ms(3);
}

void LCD::LCD_Char (unsigned char char_data){
    _Px->write_byte(char_data);
    RS.set(1);
    RW.clear();
    EN.set(1);
	_delay_us(1);
    EN.clear();
	_delay_ms(1);
}

void LCD::LCD_Init (void){
	_delay_ms(20);			//aguarda inicio do lcd (15ms minimos)

	LCD_Command (0x38);		// comando para lcd funcionar em 16x2 e 8bits de dados
	LCD_Command (0x0C);		// display ON cursor off
	LCD_Command (0x06);		// cursor incrementa sozinho
	LCD_Command (0x01);		// clear no display
	LCD_Command (0x80);		// cursor no inicio
}

void LCD::LCD_String (char *str){
	int i;
	for(i=0;str[i]!=0;i++)
	{
		LCD_Char (str[i]);
	}
}


void LCD::LCD_Clear(){
	LCD_Command (0x01);		//limpa display
	LCD_Command (0x80);		// cursor no inicio
    _delay_ms(100);
}

