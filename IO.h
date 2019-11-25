/* 
 * File:   IO.h
 * Author: guilherme
 *
 * Created on 21 de Novembro de 2019, 15:36
 */

#ifndef IO_H
#define	IO_H
#include <stdint.h>


class IO {
public:
    IO();
    ~IO();
    virtual void read(char *ptr, uint8_t len) = 0;
    virtual void write(char *ptr, uint8_t len) = 0;
};

#endif	/* IO_H */

