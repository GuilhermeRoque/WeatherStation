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

//setup port
#define DHT_DDR DDRE
#define DHT_PORT PORTE
#define DHT_PIN PINE
#define DHT_INPUTPIN PE4

//timeout retries
#define DHT_TIMEOUT 200

//functions
extern uint8_t dht_gettemperature(uint8_t *temperature);
extern uint8_t dht_gethumidity(uint8_t *humidity);
extern uint8_t dht_gettemperaturehumidity(uint8_t *temperature, uint8_t *humidity);

#endif	/* DHT11_H */

