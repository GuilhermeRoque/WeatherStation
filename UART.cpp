/* 
 * File:   UART.cpp
 * Author: marcone
 * 
 * Created on 30 de Setembro de 2019, 08:46
 */
#define F_CPU 16000000UL

#include "UART.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Singleton.h"
#include "Queue.h"

UART::UART(uint16_t baud, DataBits_t db, Parity_t par, StopBit_t sb, uint8_t double_speed)
{
    // Set baud rate
    UBRR0 = F_CPU/16/baud;

    // Set TX and RX
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

    // Set frame format (asynchronous mode)
    UCSR0C = par | sb | db;

    // Set double speed
    UCSR0A = (double_speed << U2X0);

    _tx_buffer.clear();
    _rx_buffer.clear();
}

UART::~UART() {
    // TODO Auto-generated destructor stub
}

void UART::put(uint8_t data) {
	while (_tx_buffer.is_full());

    _tx_buffer.put(data);
    UCSR0B |= (1 << UDRIE0);
}

void UART::puts(const char* data) {
    for (int i=0;data[i]!=0;i++) put(data[i]);
}

uint8_t UART::get() {
	uint8_t data = _rx_buffer.get();
    return data;
}

bool UART::has_data( ) {
    return _rx_buffer.items();
}

// Interrupt Handlers
ISR(USART0_RX_vect)
{ UART::rxc_isr_handler(); }

void UART::rxc_isr_handler() {
	if (self()->_rx_buffer.is_full()) return;
	self()->_rx_buffer.put((uint8_t) UDR0);
}

ISR(USART0_UDRE_vect)
{ UART::udre_isr_handler(); }

void UART::udre_isr_handler() {
	UDR0 = self()->_tx_buffer.get();
	if (self()->_tx_buffer.is_empty())
		UCSR0B &= ~(1 << UDRIE0);
}

