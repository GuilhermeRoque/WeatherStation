/* 
 * File:   Machine.cpp
 * Author: guilherme
 * 
 * Created on 5 de Dezembro de 2019, 21:06
 */

#include "Machine.h"
#include "EEPROM.h"


Machine::Machine(DHT11 dht, BMP280 bmp, LCD display, UART uart, ADConverter adc): dht(dht), bmp(bmp), display(display), uart(uart), adc(adc){
//Machine::Machine(DHT11 dht, BMP280 bmp, LCD display): dht(dht), bmp(bmp), display(display){
    display.LCD_Init();
    _delay_ms(10);
    display.LCD_String("  Reading.....");
    display.LCD_Command(0xC0);
    display.LCD_String("     ......");
	
    timer.addTimeout(1000,Machine::event_Command,this); //check if there is command
	timer.addTimeout(5000,Machine::event_Read,this); //read sensors
    
    _state = Idle;
    data = 0;
    e.init();
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
    e.write((const void *)&v,2);
    display.LCD_String(value);
    char s[2];
    memcpy(s,&v,2);
    uart.put(s[0]);
    uart.put(s[1]);
}

void Machine::upload(){
    uint16_t n = e.count;
    char p[n];
    e.read((void *)p);
    uart.put(0xFF);
    for (uint16_t i = 0;i<n;i++){
        uart.put(p[i]);
        _delay_ms(50);
    }    
    uart.put(0xFE);
}

void Machine::record8(uint8_t v) {
    char value[sizeof (uint8_t)];
    ltoa(v, value, 10);
    e.write((const void *)&v,1);
    uart.put(v);
    display.LCD_String(value);
}

void Machine::handle_fsm(int event){    
    if(event == Command){
        if(!uart.has_data()){
            return;
        }
        else{
            info = uart.get();
        }
    }
    switch(_state){        
        case Idle:
            display.LCD_Clear();
            if(event == Read){
                uint8_t result = 0;

                uart.put(0xAA);//debug

                //write timestamp
                uint32_t cdata = (data + (timer.millis()/1000));
                e.write((const void *)&cdata,4);
                
                //read LDR
                single = adc.single_read(ADConverter::A0);
                _delay_ms(50);
                display.LCD_String(" L: ");
                single = (single*100/1023);
                //single = 100 - single;                
                record16(single);
                display.LCD_String("%");

                //read DHT
                result = dht.read(&temperature,&humidity);
                _delay_ms(100);
                display.LCD_String(" T: ");

                if (result < 0)
                    record8(0);
                else
                    record8(temperature);

                display.LCD_Char((char)0xDF); // imprime "°"
                display.LCD_String("C");
                display.LCD_Command(0xC0);

                display.LCD_String(" H: ");

                if (result < 0)
                    record8(0);
                else
                    record8(humidity);

                display.LCD_String("%");                
                display.LCD_String(" P: ");	
                record8(bmp.readPress()); //RAND
                display.LCD_String("atm");	
                   
                count++;
                if(count == 10){
                    upload();
                    count = 0;
                }

                _state = Idle;

            }
            else if(event == Command){
                if (info == 1){
                    display.LCD_String("Comando: ");
                    display.LCD_Char(info +'0');
                    _state = Conf;
                }
                else if (info == 2){
                    display.LCD_String("Comando: ");
                    display.LCD_Char(info +'0');
                    upload();
                    count = 0;
                    _state = Idle;
                
                }                
                else if (info == 3){
                    display.LCD_String("Comando: ");
                    display.LCD_Char(info +'0');
                    _state = Synck;
                }
                else{
                    display.LCD_String("     Comando  ");
                    display.LCD_Command(0xC0);		/* Go to 2nd line*/
                    display.LCD_String("    Inválido!  ");
                    _delay_ms(2000);  // só para teste
                    display.LCD_Clear();
                }
            }
            break;            


        case Conf:
            display.LCD_Clear();
                uart.put(0xBB);
                display.LCD_String(" Digite o valor");
                display.LCD_Command(0xC0);		/* Go to 2nd line*/
                display.LCD_String("  do intervalo");
                _delay_ms(1000);
                display.LCD_Clear();
                display.LCD_String(" Digite o valor");
                display.LCD_Command(0xC0);		/* Go to 2nd line*/
                display.LCD_String("   em segundos");
                _delay_ms(1000);
                //begin = timer.millis();
                while(true){
                    display.LCD_Clear();
                    display.LCD_String("Aguardando...");
                    _delay_ms(1000);
                    if(!uart.has_data()){
//                        if ((timer.millis() - begin) > 5000){
//                            _state = Idle;
//                            return;
//                        }
                        continue;
                    }
                    intervalo = uart.get();
                    display.LCD_Clear();
                    display.LCD_String("   Intervalo");
                    display.LCD_Command(0xC0);		/* Go to 2nd line*/
                    display.LCD_String("  Digitado: ");

                    char value[sizeof (uint8_t)];
                    ltoa(intervalo, value, 10);
                    display.LCD_String(value);

                    _delay_ms(2000);
                    timer.setTimeout((intervalo)*1000,1);
                    if(uart.has_data()){
                        uart.get();
                    }
                    break;
                }
                _state = Idle;
                break;


        case Synck:
            uart.put(0xCC);
            display.LCD_Clear();
            _delay_ms(500);
            getTime();
            break;

    }
    
    
}

void Machine::getTime(){
    char t[4];
    int i = 0;
    begin = timer.millis();
    while(1){
        if(uart.has_data()){
            t[i] = uart.get();
            i++;
            uart.put(i);
        }
        if (i == 3) {
            //memcpy((&data)[i],t,4);
            break;
        }
        if ((timer.millis() - begin) > 5000)  break;
    }
    _state = Idle;
}
