/* 
 * File:   main.cpp
 * Author: marcone
 *
 * Created on 24 de Outubro de 2019, 20:38
 */
#define F_CPU 16000000UL                            /* Define CPU clock Frequency e.g. here its 16MHz */
#include <avr/io.h>                                 
#include <util/delay.h>                             
#include <avr/interrupt.h>
#include "TWIMaster.h"
#include "UART.h"
#include "GPIO.h"

/*          AT24C08A - 8k EEPROM 
Address    1 | 0 | 1 | 0 | A2 | P1 | P0 | R/W
 */

/*7bit address (R/W will be manage by master functions)*/
#define EEPROM_1_Address      0xA0            /* 10100000 = 0xA0*/
#define EEPROM_2_Address      0xA8            /* 10101000 = 0xA8*/
#define SLAVE_Address         0xB0            /* 10110000 = 0xB0*/

UART uart(9600, UART::EIGHT_DB, UART::NONE_PAR, UART::ONE_SB, UART::DS_DISABLE);
GPIO EEPROM_1(54, GPIO::OUTPUT);                    /* Utilizado para controle do endereçamento da EEPROM_1 - PF0 */
    
int main(int argc, char** argv){
    EEPROM_1.set(0);                        /* Controla o bit A2 da EEPROM 1 */
    TWIMaster master(SLOW_TWI);
    char  input[] = "ARDUINO";
    char  input2[] = "EEPROM";
    char  output[10];
    char c;

    sei();
    while (1) {
        if (uart.has_data()) {
            c = uart.get();
            uart.puts("Escrevendo: ");
            uart.puts(input);
            _delay_ms(100);
            master.write(input,7,SLAVE_Address,0x00);
            master.receive(output,7,SLAVE_Address,0x00);
            _delay_ms(100);
            uart.put('\n');
            uart.puts("Leu: ");
            uart.puts(output);
            uart.put('\n');
            uart.puts("Escrevendo: ");
            uart.puts(input2);
            _delay_ms(100);
            master.write(input2,7,EEPROM_1_Address,0x00);
            master.receive(output,7,EEPROM_1_Address,0x00);
            uart.put('\n');
            uart.puts("Leu: ");
            uart.puts(output);
            _delay_ms(100);
        }
    }          
    return 0;
}

