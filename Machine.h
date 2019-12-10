/* 
 * File:   Machine.h
 * Author: guilherme
 *
 * Created on 5 de Dezembro de 2019, 21:06
 */

#ifndef MACHINE_H
#define	MACHINE_H
#define F_CPU 16000000UL

#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>

#include "UART.h"
#include "GPIO.h"
#include "LCD.h"
#include "ADConverter.h"
#include "DHT11.h"
#include "BMP280.h"
#include "Timer.h"
#include "EEPROM.h"
#include <time.h>


enum States {
    Idle,
    Conf, 
    Get, 
    Synck
};

enum Event {
    Read,
    Command,
    Upload
};


class Machine {
    public:
        Machine(DHT11 dht, BMP280 bmp, LCD display, UART uart, ADConverter adc);
        ~Machine();
        void handle_fsm(int event);    

        static void event_Command(void *p);
        static void event_Read(void *p);
        static void event_Upload(void *p);
        void upload();
        
        //UART uart = UART(9600, UART::DATABITS_8, UART::NONE, UART::STOPBIT_1);
        //UART uart;
        int _state;
        Timer timer = Timer(1000);

    private:

        void record16(uint16_t v);
        void record8(uint8_t v);
        void getTime();

        ADConverter adc;// = ADConverter(ADConverter::AVCC);
        LCD display;// = LCD(49,48,47,9);
        BMP280 bmp;// = BMP280();
        DHT11 dht;
        UART uart;
        EEPROM e = EEPROM();

        char info, intervalo;
        uint16_t single;
        uint8_t temperature;
        uint8_t humidity;
        uint32_t data;
        uint8_t count;
        Milliseconds begin;
};

#endif	/* MACHINE_H */

