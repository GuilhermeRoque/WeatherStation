/* 
 * File:   EEPROM.h
 * Author: guilherme
 *
 * Created on 8 de Dezembro de 2019, 12:35
 */

#ifndef EEPROM_H
#define	EEPROM_H
#include <avr/eeprom.h>

class EEPROM {
public:
    EEPROM();
    ~EEPROM();
    
    void read(void * data);
    void write(const void * data, uint8_t len);
    void init();
    void clear();
    
    uint16_t count; //number of bytes writen in EEPROM

private:

};

#endif	/* EEPROM_H */

