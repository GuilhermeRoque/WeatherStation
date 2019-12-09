#define F_CPU 16000000UL

#include "Machine.h"

int main(int argc, char** argv) {
    sei();
    DHT11 dht = DHT11(2);
    Machine machine = Machine(dht);

    while (true){
        machine.timer.timeoutManager();
    }
    return 0;
}