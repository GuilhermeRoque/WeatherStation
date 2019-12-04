/* 
 * File:   BMP280.h
 * Author: marcone
 *
 * Created on 3 de Dezembro de 2019, 22:28
 */

#ifndef BMP280_H
#define	BMP280_H

#include <avr/io.h>

class BMP280 {
public:
    BMP280();
    ~BMP280();
    uint8_t readPress();
    uint8_t readTemp();
private:

};

#endif	/* BMP280_H */

