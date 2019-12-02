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

enum States {Idle,
             Conf, 
             Get, 
             Synck
};

int _state;

//Iniciando a serial
UART uart(9600, UART::DATABITS_8, UART::NONE, UART::STOPBIT_1);
//Iniciando o conversor ADC
ADConverter adc = ADConverter(ADConverter::AVCC);
//Iniciando o Display
LCD display = LCD();

int main(int argc, char** argv) {
    char info;
    uint16_t single;
    sei(); //Ativa interrupção global
    display.LCD_Init();
    _state = Idle;    
    while(1){
        display.LCD_Clear();
        if(uart.has_data()){
            info = uart.get();
            uart.put(info);
            display.LCD_Clear();
            display.LCD_String("Valor digitado: ");
            display.LCD_Command(0xC0);		/* Go to 2nd line*/
            display.LCD_Char(info);   
            _state = Conf;
            _delay_ms(1000);  // só para teste
        }
        
        switch(_state){
            case Idle:
                uart.puts("Case Idle");
                single = adc.single_read(ADConverter::A0);
                display.LCD_String("L: ");	
                convert(single);
                _delay_ms(1000);
                break;            
            
                
            case Conf:
                uart.puts("Case Conf");
                break;
            
                
            case Get:
                uart.puts("Case Get");
                break;
            
                
            case Synck:                
                uart.puts("Case Synck");
                break;
               
        }
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