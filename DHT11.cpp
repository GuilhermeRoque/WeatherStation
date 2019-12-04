/* 
 * File:   DHT11.cpp
 * Author: marcone
 * 
 * Created on 3 de Dezembro de 2019, 21:54
 */
#define F_CPU 16000000UL

#include "DHT11.h"
#include "UART.h"
#include <stdlib.h>
#include <util/delay.h>

#define DHT11_PIN PG1

uint8_t DHT11::I_RH;
uint8_t DHT11::D_RH;
uint8_t DHT11::I_Temp;
uint8_t DHT11::D_Temp;
uint8_t DHT11::CheckSum;
uint8_t c = 0;
    UART uart1(9600, UART::DATABITS_8, UART::NONE, UART::STOPBIT_1);

DHT11::DHT11() {

    //definir o pino de funcionamento
}

DHT11::~DHT11() {
}

uint8_t DHT11::read(){
    return rand();  //retornando um valor aleatorio para teste
    Request();
    Response();
    uart1.puts("SET variaveis");
    I_RH = Receive_data();      /* store first eight bit in I_RH */
    D_RH = Receive_data();      /* store next eight bit in D_RH */
    I_Temp = Receive_data();	/* store next eight bit in I_Temp */
    D_Temp = Receive_data();	/* store next eight bit in D_Temp */
    CheckSum = Receive_data();  /* store next eight bit in CheckSum */
    //return I_RH;
}

uint8_t DHT11::Receive_data(){					/* receive data */
    for (int q=0; q<8; q++){
		while((PING & (1<<DHT11_PIN)) == 0);	/* check received bit 0 or 1 */
		_delay_us(30);
		if(PING & (1<<DHT11_PIN))				/* if high pulse is greater than 30ms */
		c = (c<<1)|(0x01);						/* then its logic HIGH */
		else									/* otherwise its logic LOW */
		c = (c<<1);
		while(PING & (1<<DHT11_PIN));
	}
	return c;
}

void DHT11::Request(){/* Microcontroller send start pulse or request */
    uart1.puts("Request");
	DDRG |= (1<<DHT11_PIN);
	PORTG &= ~(1<<DHT11_PIN);		/* set to low pin */
	_delay_ms(20);					/* wait for 20ms */
	PORTG |= (1<<DHT11_PIN);		/* set to high pin */
}

void DHT11::Response(){						/* receive response from DHT11 */
	uart1.puts("Response");
    DDRG &= ~(1<<DHT11_PIN);
	while(PING & (1<<DHT11_PIN));
	while((PING & (1<<DHT11_PIN))==0);
	while(PING & (1<<DHT11_PIN));
}

