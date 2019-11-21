/* 
 * File:   Singleton.h
 * Author: marcone
 *
 * Created on 30 de Setembro de 2019, 08:46
 */

#ifndef SINGLETON_H
#define	SINGLETON_H

template<typename T>
class Singleton {
public:
    Singleton() { _singleton = static_cast<T*>(this); }
    ~Singleton() { _singleton = 0; }

    static T * self() { return _singleton; }

private:
    static T * _singleton;
};

template<typename T>
T * Singleton<T>::_singleton;


#endif	/* SINGLETON_H */

