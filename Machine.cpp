/* 
 * File:   Machine.cpp
 * Author: guilherme
 * 
 * Created on 5 de Dezembro de 2019, 21:06
 */

#include "Machine.h"

Machine::Machine(DHT11 dht): dht(dht){
    display.LCD_Init();
    display.LCD_String("  Reading.....");
    display.LCD_Command(0xC0);		/* Go to 2nd line*/
    display.LCD_String("     ......");
	timer.addTimeout(1000,Machine::event_Command,this);
	timer.addTimeout(5000,Machine::event_Read,this);
    _state = Idle;
}
Machine::~Machine() {}

void Machine::event_Command(void *p){   
    ((Machine *)p) -> handle_fsm(Command);    
}

void Machine::event_Read(void *p){
    ((Machine *)p) -> handle_fsm(Read);
}

void Machine::record16(uint16_t v) {
    char value[sizeof (uint16_t)];
    ltoa(v, value, 10);
    display.LCD_String(value);
    uart.puts(value);
}

void Machine::record8(uint8_t v) {
    char value[sizeof (uint8_t)];
    ltoa(v, value, 10);
    uart.puts(value);
    display.LCD_String(value);
}

void Machine::handle_fsm(int event){    
    if(event == Command){
        if(!uart.has_data()){
            return;
        }
        else{
            info = uart.get();
            uart.put(info);
        }
    }
    
    switch(_state){        
        case Idle:
            display.LCD_Clear();
            if(event == Read){
                uint8_t result = 0;
                uart.puts("\nCase Idle");
                single = adc.single_read(ADConverter::A0);
                display.LCD_String(" L: ");
                single = (single*100/1021);
                single = 100 - single;                
                record16(single);

                result = dht.read(&temperature,&humidity);
                display.LCD_String("%");
                display.LCD_String("  H: ");

                if (result < 0)
                    record8(0);
                else
                    record8(humidity);
    
                display.LCD_String("%");
                display.LCD_Command(0xC0);		/* Go to 2nd line*/
                display.LCD_String(" T: ");	

                if (result < 0)
                    record8(0);
                else
                    record8(temperature);
             
                display.LCD_String("  P: ");	
                record8(bmp.readPress()); //RAND
                _state = Idle;
            }
            else if(event == Command){
                if (info == '1'){
                    display.LCD_String("Valor digitado:");
                    display.LCD_Char(info);
                    _state = Conf;
                }
                else if (info == '2'){
                    display.LCD_String("Valor digitado:");
                    display.LCD_Char(info);   
                    _state = Get;
                }
                else if (info == '3'){
                    display.LCD_String("Valor digitado:");
                    display.LCD_Char(info);   
                    _state = Synck;
                }
                else{
                    uart.put(info);
                    display.LCD_String("     Valor  ");
                    display.LCD_Command(0xC0);		/* Go to 2nd line*/
                    display.LCD_String("   Inválido!  ");
                    _delay_ms(2000);  // só para teste
                    display.LCD_Clear();
                }
            }
            break;            


        case Conf:
            display.LCD_Clear();
            uart.puts("\nCase Conf");
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
                    //timer.setTimeout(intervalo*1000, &event_Read,0);
                    _delay_ms(2000);
                    if(uart.has_data()){
                        uart.get();
                    }
                    _delay_ms(2000);
                    break;
                }
                _state = Idle;
                break;


        case Get:
            display.LCD_Clear();
            uart.puts("Case Get");
            _delay_ms(2000);
            _state = Idle;
            break;


        case Synck:
            display.LCD_Clear();
            uart.puts("Case Synck");
            _delay_ms(2000);
            _state = Idle;
            break;
    }
}
