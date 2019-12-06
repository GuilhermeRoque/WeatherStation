#define F_CPU 16000000UL

#include "Machine.h"
#include "Timer.h"

Timer timer = Timer(1000);
Machine machine = Machine();

int main(int argc, char** argv) {
    sei();

	timer.addTimeout(1000,Machine::event_Command,&machine);
	timer.addTimeout(5000,Machine::event_Read,&machine);

    while (true){
        timer.timeoutManager();                
    }
    
    return 0;
}