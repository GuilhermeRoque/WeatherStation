/* 
 * File:   LDR.cpp
 * Author: guilherme
 * 
 * Created on 10 de Dezembro de 2019, 20:08
 */

#include "LDR.h"

LDR::LDR(uint8_t id):id(id) {
}

LDR::~LDR() {
}

int8_t LDR::read(uint8_t * lightnes){
    uint16_t single = adc.single_read((ADConverter::CHANNEL_t)id);
    *lightnes = (single*100/1021);
    *lightnes = 100 - *lightnes;                
    return 0;
}


