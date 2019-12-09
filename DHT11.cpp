/* 
 * File:   dht.cpp
 * Author: marcone
 * 
 * Created on 4 de Dezembro de 2019, 13:02
 */

#define F_CPU 16000000UL

#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "DHT11.h"
#include "UART.h"

DHT11::DHT11(uint8_t id): gpio(GPIO(id,GPIO::OUTPUT)){}

DHT11::~DHT11() {}

int8_t DHT11::read(uint8_t *temperature, uint8_t *humidity) {
	uint8_t bits[5];
	uint8_t i,j = 0;
	memset(bits, 0, sizeof(bits));
	//reset port
    gpio.clear();
    _delay_us(40);  
    gpio.set(1);
	_delay_ms(100);
	//send request
	gpio.clear();
    _delay_ms(18);
    gpio.set(1);
    gpio.changeDir(GPIO::INPUT);
	_delay_us(40);
	//check start condition 1
	if(gpio.get()) {
		return -1;
	}
	_delay_us(80);
	//check start condition 2
	if(!gpio.get()) {
		return -1;
	}
	_delay_us(80);
	//read the data
	uint16_t timeoutcounter = 0;
	for (j=0; j<5; j++) { //read 5 byte
		uint8_t result=0;
		for(i=0; i<8; i++) {//read every bit
			timeoutcounter = 0;
			while(!gpio.get()) { //wait for an high input (non blocking)
                timeoutcounter++;
				if(timeoutcounter > DHT_TIMEOUT) {
					return -1; //timeout
				}
			}
			_delay_us(30);
            if(gpio.get()) //if input is high after 30 us, get result
                result |= (1<<(7-i));
			timeoutcounter = 0;
			while(gpio.get()) { //wait until input get low (non blocking)
                timeoutcounter++;
				if(timeoutcounter > DHT_TIMEOUT) {
					return -1; //timeout
				}
			}
		}
		bits[j] = result;
	}
    gpio.changeDir(GPIO::OUTPUT);
	_delay_ms(100);
	//check checksum
	if ((uint8_t)(bits[0] + bits[1] + bits[2] + bits[3]) == bits[4]) {
		//return temperature and humidity
		*temperature = bits[2];
		*humidity = bits[0];
		return 0;
	}
	return -1;
}