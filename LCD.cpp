/* 
 * File:   LCD.cpp
 * Author: marcone
 * 
 * Created on 27 de Novembro de 2019, 09:47
 */
#define F_CPU 16000000UL

#include "LCD.h";
#include <avr/io.h>
#include <util/delay.h>

#define LCD_Data_Dir DDRK		//Registrador do dos dados do lcd
#define LCD_Command_Dir DDRL		// registrador dosc comandos do lcd
#define LCD_Data_Port PORTK		//Port dos dados
#define LCD_Command_Port PORTL		//Port dos comandos
#define RS PL0				// Pino de controle RS
#define RW PL1				// Pino de controle RW
#define EN PL2				// Pino de controle EN

LCD::LCD(){

}

void LCD::LCD_Command(unsigned char cmnd)
{
	LCD_Data_Port= cmnd; //comando em hexa nos pinos de controle
	LCD_Command_Port &= ~(1<<RS);	//coloca Rs em 0
	LCD_Command_Port &= ~(1<<RW);	// Rw em 0 para modo escrita
	LCD_Command_Port |= (1<<EN);	// Enable 1 , para escrever comando.
	_delay_us(1);
	LCD_Command_Port &= ~(1<<EN);
	_delay_ms(3);
}

void LCD::LCD_Char (unsigned char char_data)
{
	LCD_Data_Port= char_data;
	LCD_Command_Port |= (1<<RS);
	LCD_Command_Port &= ~(1<<RW);
	LCD_Command_Port |= (1<<EN);
	_delay_us(1);
	LCD_Command_Port &= ~(1<<EN);
	_delay_ms(1);
}

void LCD::LCD_Init (void)
{
	LCD_Command_Dir = 0xFF;		//define como saida 11111111 pinos de controle
	LCD_Data_Dir = 0xFF;		// pinos de dados como saida
	_delay_ms(20);			//aguarda inicio do lcd (15ms minimos)

	LCD_Command (0x38);		// comando para lcd funcionar em 16x2 e 8bits de dados
	LCD_Command (0x0C);		// display ON cursor off
	LCD_Command (0x06);		// cursor incrementa sozinho
	LCD_Command (0x01);		// clear no display
	LCD_Command (0x80);		// cursor no inicio
}

void LCD::LCD_String (char *str)
{
	int i;
	for(i=0;str[i]!=0;i++)
	{
		LCD_Char (str[i]);
	}
}


void LCD::LCD_Clear()
{
	LCD_Command (0x01);		//limpa display
	LCD_Command (0x80);		// cursor no inicio
    _delay_ms(100);
}

