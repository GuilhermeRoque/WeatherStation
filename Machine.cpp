/* 
 * File:   Machine->cpp
 * Author: guilherme
 * 
 * Created on 5 de Dezembro de 2019, 21:06
 */

#include "Machine.h"

Machine::Machine(DHT11 *dht,LCD *display,BMP280 *bmp,EEPROM *e,LDR *ldr, Timer *timer, UART *uart)
: dht(dht),display(display),bmp(bmp),e(e),ldr(ldr),timer(timer),uart(uart)
{
    display->LCD_Init();
    _delay_ms(100);
    display->LCD_String("  Reading....");
    display->LCD_Command(0xC0);
    display->LCD_String("     ....");

    timer->addTimeout(1000,Machine::event_Command,this); //check if there is command
	timer->addTimeout(5000,Machine::event_Read,this); //read sensors
    
    _state = Idle;
    data = 0;
    e->init();
}
Machine::~Machine() {}

void Machine::event_Command(void *p){   
    ((Machine *)p) -> handle_fsm(Command);    
}

void Machine::event_Read(void *p){
    ((Machine *)p) -> handle_fsm(Read);
}

void Machine::upload(){
    count = 0;
    uint16_t n = e->count;
    char p[n];
    e->read(p);
    for (uint16_t i = 0;i<n;i++){
        uart->put(p[i]);
        _delay_ms(10);
    }    
}

void Machine::record8(uint8_t v) {
    char value[sizeof (uint8_t)];
    e->write(&v,1);
    ltoa(v, value, 10);
    display->LCD_String(value);
}

void Machine::readLightness(){
    uint8_t lightnes = 0;

    ldr->read(&lightnes);
    display->LCD_String(" L: ");

    record8(lightnes);
//    record8(0x11);    

    display->LCD_String("%");
}                    //read LDR

void Machine::readTempHumidity() {
    uint8_t temperature = 0;
    uint8_t humidity = 0;

    uint8_t result = dht->read(&temperature,&humidity);
    _delay_ms(100);
    display->LCD_String(" T: ");

    if (result < 0)
        record8(0);
    else
        record8(temperature);

//    record8(0x66);    

    display->LCD_Char((char)0xDF); // imprime "°"
    display->LCD_String("C");
    display->LCD_Command(0xC0);

    display->LCD_String(" H: ");

    if (result < 0)
        record8(0);
    else
        record8(humidity);

//    record8(0x33);
    
    display->LCD_String("%");                
}

void Machine::readPreassure(){
    uint8_t preassure = 0;
        
    display->LCD_String(" P: ");	
    record8(bmp->readPress());
//    record8(0x15);    
    display->LCD_String("atm");	
}

void Machine::writeHeader(){
    uint8_t flag = 0xFE;
    e->write(&flag,1);
    uint32_t cdata = (data + (timer->millis()/1000));
    e->write(&cdata,4);
}    

void Machine::handle_fsm(int event){    
    char info;
    Milliseconds begin;

    if(event == Command){
        if(!uart->has_data()){
            return;
        }
        else{
            info = uart->get();
        }
    }
    switch(_state){        
        case Idle:
            display->LCD_Clear();
            if(event == Read){
                
                writeHeader();
                readLightness();
                readTempHumidity();
                readPreassure();
                
                count++;
                if(count == 10){
                    upload();
                }

                _state = Idle;

            }
            else if(event == Command){
                if (info == 1){
                    display->LCD_String("Comando: ");
                    display->LCD_Char(info +'0');
                    _state = Conf;
                }
                else if (info == 2){
                    display->LCD_String("Comando: ");
                    display->LCD_Char(info +'0');
                    upload();
                    count = 0;
                    _state = Idle;
                
                }                
                else if (info == 3){
                    display->LCD_String("Comando: ");
                    display->LCD_Char(info +'0');
                    _state = Synck;
                }
                else{
                    display->LCD_String("     Comando  ");
                    display->LCD_Command(0xC0);		/* Go to 2nd line*/
                    display->LCD_String("   Inválido!  ");
                    _delay_ms(2000);  // só para teste
                    display->LCD_Clear();
                }
            }
            break;            


        case Conf:
            display->LCD_Clear();
                uart->put(0x22);
                display->LCD_String(" Digite o valor");
                display->LCD_Command(0xC0);		/* Go to 2nd line*/
                display->LCD_String("  do intervalo");
                _delay_ms(1000);
                display->LCD_Clear();
                display->LCD_String(" Digite o valor");
                display->LCD_Command(0xC0);		/* Go to 2nd line*/
                display->LCD_String("   em segundos");
                _delay_ms(1000);
                begin = timer->millis();
                while(true){
                    display->LCD_Clear();
                    display->LCD_String("Aguardando...");
                    _delay_ms(1000);
                    if(!uart->has_data()){
                        if ((timer->millis() - begin) > 5000){
                            _state = Idle;
                            uart->put(0xFF);
                            return;
                        }
                        continue;
                    }
                    info = uart->get();
                    timer->setTimeout((info)*1000,1);
                    uart->put(0x22);
                    display->LCD_Clear();
                    display->LCD_String("   Intervalo");
                    display->LCD_Command(0xC0);		/* Go to 2nd line*/
                    display->LCD_String("  Digitado: ");

                    char value[sizeof (uint8_t)];
                    ltoa(info, value, 10);
                    display->LCD_String(value);

                    _delay_ms(2000);
                    if(uart->has_data()){
                        uart->get();
                    }
                    break;
                }
                _state = Idle;
                break;


        case Synck:
            uart->put(0x33);
            display->LCD_Clear();
            _delay_ms(500);
            getTime();
            break;

    }
    
    
}

void Machine::getTime(){
    char t[4];
    int i = 0;
    Milliseconds begin;
    begin = timer->millis();
    while(1){
        if(uart->has_data()){
            t[i] = uart->get();
            i++;
        }
        if (i == 4) {
            memcpy(&data,t,4);
            uart->put(0x33);
            break;
        }
        if ((timer->millis() - begin) > 5000){
            uart->put(0xFF);
            break;
        }  
    }
    _state = Idle;
}
