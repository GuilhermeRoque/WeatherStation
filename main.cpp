#define F_CPU 16000000UL

#include "Machine.h"
#include <time.h>


int main(int argc, char** argv) {
    sei();
    //Instância o sensor DHT11
    DHT11 dht = DHT11(58);
    //Instância o sensor BMP280
    BMP280 bmp = BMP280();
    //Instância o LCD
    LCD display = LCD(59,60,61,9);
    //Instância a UART
    UART uart = UART(8600, UART::DATABITS_8, UART::NONE, UART::STOPBIT_1);
    //Instância o conversor ADC
    ADConverter adc = ADConverter(ADConverter::AVCC);
    //Cria a máquina com os seus periféricos
    Machine machine = Machine(dht, bmp, display, uart, adc);
 
    while (true){
        machine.timer.timeoutManager();
    }   
    
    return 0;
}