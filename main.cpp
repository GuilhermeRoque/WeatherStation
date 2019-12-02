#define F_CPU 16000000UL

#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "UART.h"
#include "LCD.h"
#include "ADConverter.h"
#include "Timer.h"

void tx_serial(uint16_t v);
void convert(uint16_t v);

enum States {Idle,
             Conf, 
             Get, 
             Synck
};

enum Event {
    Read,
    Command,
};

int _state;

//Iniciando a serial
UART uart(9600, UART::DATABITS_8, UART::NONE, UART::STOPBIT_1);
//Iniciando o conversor ADC
ADConverter adc = ADConverter(ADConverter::AVCC);
//Iniciando o Display
LCD display = LCD();

Timer timer = Timer(1000);

char info;
uint16_t single;


void handle_fsm(int event){

    display.LCD_Clear();
    switch(_state){
        
        if(event == Command){
            if(!uart.has_data()){
                return;
            }
            else{
                info = uart.get();
                uart.put(info);
            }
        }
        
        case Idle:
            if(event == Read){
                uart.puts("Case Idle");
                single = adc.single_read(ADConverter::A0);
                display.LCD_String("L: ");	
                convert(single);
                _delay_ms(1000);
            }
            else if(event == Command){
                if (info == '1'){
                    display.LCD_Clear();
                    display.LCD_String("Valor digitado: ");
                    display.LCD_Char(info);   
                    _state = Conf;
                }
                if (info == '2'){
                    display.LCD_Clear();
                    display.LCD_String("Valor digitado: ");
                    display.LCD_Char(info);   
                    _state = Get;
                }
                if (info == '3'){
                    display.LCD_Clear();
                    display.LCD_String("Valor digitado: ");
                    display.LCD_Char(info);   
                    _state = Synck;
                }
            }
            break;            


        case Conf:
            uart.puts("Case Conf");
            _delay_ms(2000);
            _state = Idle;
            break;


        case Get:
            uart.puts("Case Get");
            _delay_ms(2000);
            _state = Idle;
            break;


        case Synck:                
            uart.puts("Case Synck");
            _delay_ms(2000);
            _state = Idle;
            break;
    }
};

void event_Read(){
    handle_fsm(Read);
}
void event_Command(){
    handle_fsm(Command);
}

int main(int argc, char** argv) {
    sei(); //Ativa interrupção global
    display.LCD_Init();
    _state = Idle;
	timer.addTimeout(60000, &event_Read);
	timer.addTimeout(1000, &event_Command);

    while (true){
        timer.timeoutManager();
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