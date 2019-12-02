/* 
 * File:   ADC.cpp
 * Author: felipe
 * 
 * Created on 3 de Novembro de 2019, 16:52
 */


#include <avr/interrupt.h>
#include "ADConverter.h"


uint8_t ADConverter::_count;
uint8_t ADConverter::_count_event;
bool ADConverter::_event;
bool ADConverter::_read_free;
Fila<uint16_t,50> ADConverter::_buffer;

ADConverter::ADConverter(REF_t ref, PRESCALER_t clock) {
    ADMUX |= (ref<<REFS0);
    ADCSRA |=(clock<<ADPS0) | (1<<ADEN);
    _read_free = true;
}
void ADConverter::select_channel(CHANNEL_t channel){
    //Zerando inicialmente caso tenha algo da inicialização antiga
    ADMUX &=~(31<<MUX0);
    ADCSRB = 0x00;
    
    if(channel<=ADConverter::A7)
        ADMUX |= (channel<<MUX0);
    else {
        ADCSRB |= (1<<MUX5);
        ADMUX |= channel & ~(1<<5);
    }
}

void ADConverter::read(CHANNEL_t channel, uint8_t count, TRIGGER_SRC_t mode){
    if(_read_free){
        _read_free=false;
        _event=false;
        select_channel(channel);
        _count = count;
        _count_event=0;
        ADCSRA |=(1<<ADIE);
        if(mode == ADConverter::NONE){
            if(count > 1){
                ADCSRB |= (ADConverter::FREE_RUN<<ADTS0);
                ADCSRA |=(1<<ADATE);
            }
            ADCSRA |=(1<<ADSC);
        }
        else{
            ADCSRB |= (mode<<ADTS0);
            ADCSRA |=(1<<ADATE);
            if(mode == ADConverter::FREE_RUN){
                ADCSRA |=(1<<ADSC);
            }
        }
    }
    
}

bool ADConverter::free(){
    return _event;
}
uint16_t ADConverter::single_read(CHANNEL_t channel){
    select_channel(channel);
    
    ADCSRA |=(1<<ADSC);
    while(ADCSRA & (1<<ADSC))
        ;
    return ADC;
}

void ADConverter::stop_conversion(){
    ADCSRA &=~(1<<ADATE);
    ADCSRA &=~(1<<ADIE);
}

void ADConverter::left_adjust(){
    ADMUX = (1<<ADLAR);
}
void ADConverter::handler(){
    _count_event++;
    _buffer.push((uint16_t)ADC);
    if(_count_event==_count){
        stop_conversion();
        _event=true;
        _read_free = true;
    }
}
ISR(ADC_vect) {
    ADConverter::handler();
}