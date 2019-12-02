    /* 
 * File:   UART.cpp
 * Author: aluno
 * 
 * Created on 16 de Setembro de 2019, 08:12
 */

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include "UART.h"

bool UART::_has_data = false;
BUFFER_UART_t UART::_rx_buffer;
BUFFER_UART_t UART::_tx_buffer;

UART::UART(uint32_t baud, DATABITS_t db, PARITY_t parity, STOPBITS_t sb){
    UBRR0 = F_CPU/16/baud-1;
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0); 
    UCSR0C = (db | parity | sb);
}

uint8_t UART::get(){
    return _rx_buffer.pop();
}
void UART::put(uint8_t data){
    _tx_buffer.push(data);
    UCSR0B |= (1 << UDRIE0);
}
void UART::puts(const char* msg){
    for(int i=0;msg[i] != 0; ++i)
        put((uint8_t) msg[i]);
    put(10);
}
bool UART::has_data(){
    return !_rx_buffer.empty();
}
void UART::rx_isr_handler(){
    if(!_rx_buffer.cheia()){
        _rx_buffer.push((uint8_t) UDR0);
    }  
}

void UART::tx_isr_handler(){
    UDR0 = _tx_buffer.pop();
    if(_tx_buffer.empty()){
        UCSR0B &= ~(1 << UDRIE0);
    }
}

ISR(USART0_RX_vect)
{
    UART::rx_isr_handler();
}
ISR(USART0_UDRE_vect)
{
    UART::tx_isr_handler();
}


