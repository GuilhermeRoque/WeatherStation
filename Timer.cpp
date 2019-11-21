/*
 * Timer.cpp
 *
 *  Created on: 28 de out de 2018
 *      Author: vini
 */

#include "Timer.h"
#include "Timeout.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#define F_CPU 16000000UL                            /* Define CPU clock Frequency e.g. here its 16MHz */

using namespace Timeout;

Timer::Timer(Hertz freq, uint8_t timer)
: _ticks(0), _timer_base(0), _us_per_tick(0), _timeout_counter(0)
{
	static unsigned int Timer_Top;
	volatile uint8_t *TCCRnA, *TCCRnB, *TIMSKn;

	// Registers select
	switch (timer)
	{
	case TIMER8_0:
		Timer_Top = 0xFF;
		TCCRnA = &TCCR0A;
		TCCRnB = &TCCR0B;
		TIMSKn = &TIMSK0;
		_pin_counter = (volatile uint16_t *) &TCNT0;
		break;

	case TIMER16_1:
		Timer_Top = 0xFFFF;
		TCCRnA = &TCCR1A;
		TCCRnB = &TCCR1B;
		TIMSKn = &TIMSK1;
		_pin_counter = &TCNT1;
		break;

	case TIMER16_3:
		Timer_Top = 0xFFFF;
		TCCRnA = &TCCR3A;
		TCCRnB = &TCCR3B;
		TIMSKn = &TIMSK3;
		_pin_counter = &TCNT3;
		break;

	case TIMER16_4:
		Timer_Top = 0xFFFF;
		TCCRnA = &TCCR4A;
		TCCRnB = &TCCR4B;
		TIMSKn = &TIMSK4;
		_pin_counter = &TCNT4;
		break;

	case TIMER16_5:
		Timer_Top = 0xFFFF;
		TCCRnA = &TCCR5A;
		TCCRnB = &TCCR5B;
		TIMSKn = &TIMSK5;
		_pin_counter = &TCNT5;
		break;

	default:
		return;

	}

	static const Hertz Fmax_1024 = F_CPU / 1024;
	static const Hertz Fmax_256  = F_CPU / 256;
	static const Hertz Fmax_64   = F_CPU / 64;
	static const Hertz Fmax_8    = F_CPU / 8;
	static const Hertz Fmax_1    = F_CPU / 1;
	static const Microseconds Period_div[5] = {
			   (1*1000000) / F_CPU, //1
			   (8*1000000) / F_CPU, //8
			  (64*1000000) / F_CPU, //64
			 (256*1000000) / F_CPU, //256
			(1024*1000000) / F_CPU  //1024
	};

	*TCCRnA = 0x00;	// normal operation
	int div = 0;

	// lógica para selecionar divisor
	if(freq <= Fmax_1024)
	{
		*TCCRnB = 0x05; // div 1024
		div = 1024;
	}
	else if(freq <= Fmax_256)
	{
		*TCCRnB = 0x04; // div 256
		div = 256;
	}
	else if(freq <= Fmax_64)
	{
		*TCCRnB = 0x03; // div 64
		div = 64;
	}
	else if(freq <= Fmax_8)
	{
		*TCCRnB = 0x02; // div 8
		div = 8;
	}
	else if(freq <= Fmax_1)
	{
		*TCCRnB = 0x01; // div 1
		div = 1;
	}
	else // #error "Frequency out of range (should be between 62Hz and 250kHz)."
		return;

	// calcular ciclos de timer
	Hertz ciclos = (F_CPU/div)/freq;
	if(ciclos > Timer_Top) return; // Frequência solicitada é muito alta!
	_timer_base = (Timer_Top + 1) - ciclos;

	//TODO: compute _us_per_tick
	_us_per_tick = ciclos * Period_div[(*TCCRnB & 0x07) - 1];

	*_pin_counter = _timer_base;
	*TIMSKn = 0x01;	// liga int de ov

	for(int i=0;i<4;i++) _timeouts[i] = Timeout::Timeout();
}

Milliseconds Timer::millis()
{
	return micros() / 1000;
}

Microseconds Timer::micros()
{
	return _ticks * _us_per_tick;
}

void Timer::delay(Milliseconds ms)
{
	udelay(ms*1000);
}

void Timer::udelay(Microseconds us)
{
	Microseconds start = micros();
	while((micros() - start) <= us);
}

ISR(TIMER0_OVF_vect) { Timer::ovf_isr_handler(); }
ISR(TIMER1_OVF_vect) { Timer::ovf_isr_handler(); }
ISR(TIMER3_OVF_vect) { Timer::ovf_isr_handler(); }
ISR(TIMER4_OVF_vect) { Timer::ovf_isr_handler(); }
ISR(TIMER5_OVF_vect) { Timer::ovf_isr_handler(); }
void Timer::ovf_isr_handler() {
	*self()->_pin_counter = self()->_timer_base;
	self()->_ticks++;
	for(uint32_t i=0;i<self()->_timeout_counter;i++)
		self()->_timeouts[i].checkTimeout();
}

bool Timer::addTimeout(uint32_t interval, CALLBACK_t callback)
{
	if (_timeout_counter > 3) return 0;

	_timeouts[_timeout_counter].config(interval, callback);
	_timeout_counter++;
	return 1;
}

void Timer::timeoutManager() {
	for(uint32_t i=0;i<_timeout_counter;i++) {
		if (_timeouts[i].event())
		_timeouts[i].callback();
	}
}