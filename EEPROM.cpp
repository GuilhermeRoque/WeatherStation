/* 
 * File:   EEPROM.cpp
 * Author: guilherme
 * 
 * Created on 8 de Dezembro de 2019, 12:35
 */

#include "EEPROM.h"

EEPROM::EEPROM() {
    count = 0;
}

EEPROM::~EEPROM() {
}

void EEPROM::read(void * data){
    eeprom_read_block(data,(const void *)2,count);
    clear();
}

void EEPROM::write(const void * data, uint8_t len){
    eeprom_write_block(data,(void *)(count + 2),len);
    count += len;
    eeprom_write_word((uint16_t *)0,count);
}

void EEPROM::init(){
    count = eeprom_read_word((uint16_t *)0);

}

void EEPROM::clear(){
    count = 0;
    eeprom_write_word((uint16_t *)0,count);
}

