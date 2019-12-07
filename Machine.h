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

#include "UART.h"
#include "LCD.h"
#include "ADConverter.h"
#include "DHT11.h"
#include "BMP280.h"
#include "Timer.h"


enum States {
    Idle,
    Conf, 
    Get, 
    Synck
};

enum Event {
    Read,
    Command,
};


class Machine {
    public:
        Machine(DHT11 dht);
        ~Machine();
        void handle_fsm(int event);    

        static void event_Command(void *p);
        static void event_Read(void *p);
        UART uart = UART(9600, UART::DATABITS_8, UART::NONE, UART::STOPBIT_1);
        int _state;
        Timer timer = Timer(1000);

    private:

        void record16(uint16_t v);
        void record8(uint8_t v);

        ADConverter adc = ADConverter(ADConverter::AVCC);
        LCD display = LCD();
        BMP280 bmp = BMP280();
        DHT11 dht;

        char info, intervalo;
        uint16_t single;
        uint8_t temperature = 0;
        uint8_t humidity = 0;
};

#endif	/* MACHINE_H */

