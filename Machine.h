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
#include "LDR.h"


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
        Machine(DHT11 *dht,LCD *display,BMP280 *bmp,EEPROM *e,LDR *ldr, Timer *timer, UART *uart);
        ~Machine();

        static void event_Command(void *p);
        static void event_Read(void *p);
        static void event_Upload(void *p);
        

    private:

        void record8(uint8_t v);
        void getTime();
        void readTempHumidity();
        void readLightness();
        void readPreassure();
        void writeHeader();

        void handle_fsm(int event);    
        void upload();

        LCD *display;
        BMP280 *bmp;
        DHT11 *dht;
        EEPROM *e;
        LDR *ldr;
        Timer *timer;
        UART *uart;
        
        uint32_t data;
        uint8_t count =0;
        int _state;

};

#endif	/* MACHINE_H */

