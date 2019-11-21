/* 
 * File:   TWIMaster.h
 * Author: marcone
 *
 * Created on 30 de Outubro de 2019, 08:36
 */

#ifndef TWIMASTER_H
#define	TWIMASTER_H
#define F_CPU 16000000UL                            /* Define CPU clock Frequency e.g. here its 16MHz */

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

#define SLOW_TWI 100000UL
#define FAST_TWI 400000UL

class TWIMaster{
public:
    TWIMaster(unsigned long freq);
    ~TWIMaster();
    
    void write(const char* data,uint8_t bytes, uint8_t addr,uint8_t reg);
  
    void receive(char* data,uint8_t bytes, uint8_t addr,uint8_t reg);
    
private:
    uint8_t Start(char write_address);
    uint8_t Repeated_Start(char read_address);
    void Stop();
    void Start_Wait(char write_address);
    uint8_t Write(char data);
    char Read_Ack();
    char Read_Nack();
};

#endif	/* TWIMASTER_H */

