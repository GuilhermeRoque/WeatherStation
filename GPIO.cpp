#include <avr/eeprom.h>
#include "GPIO.h"
#include <avr/io.h>


GPIO::GPIO(uint8_t id, PortDirection_t dir){
    _dir = dir;
	_bit = GPIO_PORT::id_to_bit[id];
    _Px->dir(_bit, dir);
	_Px = GPIO_PORT::AllPorts[GPIO_PORT::id_to_port[id]];
}

GPIO::~GPIO() {}

bool GPIO::get() {
	return _Px->get(_bit);
}

void GPIO::set(bool val) {
	_Px->set(_bit, val);
}

void GPIO::clear() {
	this->set(0);
}

void GPIO::toggle() {
	_Px->toggle(_bit);
}

void GPIO::changeDir(PortDirection_t dir){
    _dir = dir;
    _Px->dir(_bit, dir);  
}