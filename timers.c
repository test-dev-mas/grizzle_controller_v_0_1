#include <avr/io.h>

#include "timers.h"

void init_timer0() {
    TCCR0B |= (1 << CS02) | (1 << CS00);
    TIMSK0 |= (1 << TOIE0);
}