/* 
 * File:   GPIO.h
 * Author: marcone
 *
 * Created on 9 de Setembro de 2019, 07:47
 */

#ifndef GPIO_H
#define	GPIO_H

#include <stdint.h>
#include "GPIO_Port.h"

class GPIO {
public:
    enum PortDirection_t{
        INPUT = 0,
        OUTPUT = 1
    };
    GPIO(uint8_t id, PortDirection_t dir);
    ~GPIO();
    
    bool get();
    void set(bool val = 1);
    void clear();
    void toggle();
 
    void read(char *ptr, uint8_t len){}
    void write(char *ptr, uint8_t len){}

    private:
	GPIO_PORT::GPIO_Port * _Px;    
    uint8_t _bit;
    uint8_t _id;
    PortDirection_t _dir;
};

#endif	/* GPIO_H */

