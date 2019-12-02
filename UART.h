/* 
 * File:   UART.h
 * Author: aluno
 *
 * Created on 16 de Setembro de 2019, 08:12
 */

#ifndef UART_H
#define	UART_H

#include "fila.h"

typedef Fila<uint8_t,40> BUFFER_UART_t;

class UART {
    
public:
    enum DATABITS_t {
        DATABITS_5 = (0 << UCSZ00),
        DATABITS_6 = (1 << UCSZ00),
        DATABITS_7 = (2 << UCSZ00),
        DATABITS_8 = (3 << UCSZ00)  
    };
    enum PARITY_t{
        NONE = (0 << UPM00),
        EVEN = (2 << UPM00),
        ODD = (3 << UPM00)
    };
    enum STOPBITS_t{
        STOPBIT_1 = (0 << USBS0),
        STOPBIT_2 = (1 << USBS0)
    };
    
    UART(uint32_t baud = 19200,
         DATABITS_t db = DATABITS_8, 
         PARITY_t parity = NONE,
         STOPBITS_t sb = STOPBIT_1);
    
    void put(uint8_t data);
    void puts(const char * msg);
    uint8_t get();
    bool has_data();
    static void rx_isr_handler();
    static void tx_isr_handler();
private:
    static bool _has_data;
    static BUFFER_UART_t _rx_buffer;
    static BUFFER_UART_t _tx_buffer;

};

#endif	/* UART_H */

