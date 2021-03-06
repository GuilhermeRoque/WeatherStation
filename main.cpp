#define F_CPU 16000000UL

#include "Machine.h"

int main(int argc, char** argv) {
    sei();
    DHT11 dht = DHT11(58);
    LCD display = LCD(59,60,61,9);
    BMP280 bmp = BMP280();
    EEPROM e = EEPROM();
    ADConverter adc = ADConverter(ADConverter::AVCC);
    LDR ldr = LDR(54,&adc);
    Timer timer = Timer(1000);
    UART uart = UART(9600, UART::DATABITS_8, UART::NONE, UART::STOPBIT_1);

    Machine machine = Machine(&dht,&display,&bmp,&e,&ldr,&timer,&uart);

    while (true){
        timer.timeoutManager();
    }   
    
    return 0;
}