/* 
 * File:   DHT11.h
 * Author: marcone
 *
 * Created on 3 de Dezembro de 2019, 21:54
 */

#ifndef DHT11_H
#define	DHT11_H

#include <avr/io.h>

class DHT11 {
public:
    DHT11(); // colocar o pino de uso
    ~DHT11();
    uint8_t read();
private:
    static uint8_t I_RH;
    static uint8_t D_RH;
    static uint8_t I_Temp;
    static uint8_t D_Temp;
    static uint8_t CheckSum;
    void Request();
    void Response();
    uint8_t Receive_data();
    

};

#endif	/* DHT11_H */

