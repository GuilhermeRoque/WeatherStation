/* 
 * File:   fila.h
 * Author: felipe
 *
 * Created on 22 de Setembro de 2019, 14:04
 */

#ifndef FILA_H
#define	FILA_H

template <typename T,int cap> class Fila {
public:
    Fila(){
        _cap = cap;
        _N = 0;
        _inicio = 0;
        _fim = 0;  
        
    }
    void push(const T & data){
        _buffer[_fim] = data;
        _N++;
        _fim++;
        if(_fim == _cap) _fim = 0;  
    }
    T pop(){
        T temp=_buffer[_inicio];
        _N--;
        _inicio++;
        if(_inicio == _cap) _inicio=0;
        return temp;
    }
    bool empty(){
        return (_N == 0);
    
    }
    bool cheia() const{
        return (_N == _cap);
    }
    unsigned int size(){
        return _N;
    }
private:
    unsigned int _N, _cap;
    T _buffer[cap];
    unsigned int _inicio, _fim;
};

#endif	/* FILA_H */

