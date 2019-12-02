#define F_CPU 16000000UL

#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "UART.h"
#include "LCD.h"
#include "ADConverter.h"

void tx_serial(uint16_t v);
void convert(uint16_t v);
int total = 0;

UART uart(9600, UART::DATABITS_8, UART::NONE, UART::STOPBIT_1);

ADConverter adc = ADConverter(ADConverter::AVCC);

LCD display = LCD();

int main(int argc, char** argv) {
    char info;
    sei(); //Ativa interrup��o global
    display.LCD_Init();
    while(1){
        display.LCD_Clear();
        if(uart.has_data()){
            info = uart.get();
            uart.put(info);
            display.LCD_Clear();
            display.LCD_String("Valor digitado: ");
            display.LCD_Command(0xC0);		/* Go to 2nd line*/
            display.LCD_Char(info);   
            _delay_ms(5000);
        }
        uart.puts("Bloqueante");
        _delay_ms(100);
        uint16_t single = adc.single_read(ADConverter::A0);
        display.LCD_String("L: ");	/* write string on 1st line of LCD*/
        convert(single);
        _delay_ms(1000);
    }
    return 0;
}

void convert(uint16_t v) {
    char value[sizeof (uint16_t)];
    ltoa(v, value, 10);
    uart.puts(value);
    display.LCD_String(value);
}


void tx_serial(uint16_t v) {
    char value[sizeof (uint16_t)];
    ltoa(v, value, 10);
    uart.puts(value);
}