#define F_CPU 16000000UL

#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>



#include "UART.h"
#include "LCD.h"
#include "ADConverter.h"
#include "DHT11.h"
#include "BMP280.h"

void tx_serial(uint16_t v);
void tx_serial8(uint8_t v);
void convert(uint16_t v);
void convert8(uint8_t v);

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
//Iniciando sensor de umidade
DHT11 dht = DHT11();
//Iniciando sensor de temperatura e pressão
BMP280 bmp = BMP280();

int main(int argc, char** argv) {
    char info, intervalo;
    uint16_t single;
    sei(); //Ativa interrupção global
    display.LCD_Init();
    _state = Idle; 
    while(1){
        display.LCD_Clear();
        if(uart.has_data()){
            info = uart.get();
            uart.put(info);
            if (info == '1'){
                display.LCD_Clear();
                display.LCD_String("Valor digitado:");
                display.LCD_Char(info);
                _state = Conf;
            }
            else if (info == '2'){
                display.LCD_Clear();
                display.LCD_String("Valor digitado:");
                display.LCD_Char(info);   
                _state = Get;
            }
            else if (info == '3'){
                display.LCD_Clear();
                display.LCD_String("Valor digitado:");
                display.LCD_Char(info);   
                _state = Synck;
            }
            else{
                display.LCD_Clear();
                display.LCD_String("     Valor  ");
                display.LCD_Command(0xC0);		/* Go to 2nd line*/
                display.LCD_String("   Inválido!  ");
                _delay_ms(2000);  // só para teste
                display.LCD_Clear();
            }
        }
        
        switch(_state){
            case Idle:
                uart.puts("\nCase Idle");
                single = adc.single_read(ADConverter::A0);
                display.LCD_String(" L: ");
                single = (single*100/1021);
                convert(single);
                display.LCD_String("%");
                display.LCD_String("  H: ");
                single = dht.read();
                convert8(single);
                display.LCD_Command(0xC0);		/* Go to 2nd line*/
                display.LCD_String(" T: ");	
                convert8(bmp.readTemp());
                display.LCD_String("  P: ");	
                convert8(bmp.readPress());
                _delay_ms(2000); // retirar, para deixar pelo timer
                
                _state = Idle;
                break;            
            
                
            case Conf:
                uart.puts("\nCase Conf");
                display.LCD_Clear();
                display.LCD_String(" Digite o valor");
                display.LCD_Command(0xC0);		/* Go to 2nd line*/
                display.LCD_String("  do intervalo");
                _delay_ms(1000);
                display.LCD_Clear();
                display.LCD_String(" Digite o valor");
                display.LCD_Command(0xC0);		/* Go to 2nd line*/
                display.LCD_String("   em minutos");
                _delay_ms(1000);
                while(true){
                    display.LCD_Clear();
                    display.LCD_String("Aguardando...");
                    _delay_ms(1000);
                    if(!uart.has_data()){
                        continue;
                    }
                    intervalo = uart.get();
                    uart.put(intervalo);
                    display.LCD_Clear();
                    display.LCD_String("   Intervalo");
                    display.LCD_Command(0xC0);		/* Go to 2nd line*/
                    display.LCD_String("  Digitado: ");
                    display.LCD_Char(intervalo);
                    _delay_ms(2000);
                    break;
                }
                _state = Idle;
                break;
            
                
            case Get:
                uart.puts("\nCase Get");
                _delay_ms(2000);
                _state = Idle;
                break;
            
                
            case Synck:                
                uart.puts("\nCase Synck");
                _delay_ms(2000);
                _state = Idle;
                break;
        }
    }
    return 0;
}

void convert(uint16_t v) {
    char value[sizeof (uint16_t)];
    ltoa(v, value, 10);
    display.LCD_String(value);
    uart.puts(value);
}

void convert8(uint8_t v) {
    char value[sizeof (uint8_t)];
    ltoa(v, value, 10);
    uart.puts(value);
    display.LCD_String(value);
}

void tx_serial(uint16_t v) {
    char value[sizeof (uint16_t)];
    ltoa(v, value, 10);
    uart.puts(value);
}
void tx_serial8(uint8_t v) {
    char value[sizeof (uint8_t)];
    ltoa(v, value, 10);
    uart.puts(value);
}