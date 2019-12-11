/* 
 * File:   LDR.h
 * Author: guilherme
 *
 * Created on 10 de Dezembro de 2019, 20:08
 */

#ifndef LDR_H
#define	LDR_H
#include <stdint.h>
#include "ADConverter.h"

class LDR {
public:
    LDR(uint8_t id,ADConverter *adc);
    ~LDR();
    int8_t read(uint8_t * lightness);
private:
    ADConverter *adc;
    uint8_t id;
};

#endif	/* LDR_H */

