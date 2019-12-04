/* 
 * File:   BMP280.cpp
 * Author: marcone
 * 
 * Created on 3 de Dezembro de 2019, 22:28
 */

#include "BMP280.h"
#include <stdlib.h>

BMP280::BMP280() {
}

BMP280::~BMP280() {
}

uint8_t BMP280::readPress(){
    return rand();  //retornando um valor aleatorio para teste  
}

uint8_t BMP280::readTemp(){
    return rand();  //retornando um valor aleatorio para teste  
}
