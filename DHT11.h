/* 
 * File:   DHT11.h
 * Author: marcone
 *
 * Created on 3 de Dezembro de 2019, 21:54
 */

#ifndef DHT11_H
#define	DHT11_H

#include <stdio.h>
#include <avr/io.h>
#include "GPIO.h"

#define DHT_TIMEOUT 200

class DHT11{
public:
    DHT11(uint8_t id);
    ~DHT11();
    int8_t read(uint8_t *temperature, uint8_t *humidity);
private:
    GPIO gpio;
    
};
#endif	/* DHT11_H */

