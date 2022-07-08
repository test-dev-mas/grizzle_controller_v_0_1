#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdbool.h>

#include "spi.h"
#include "timers.h"
#include "uart.h"

#define BLINK_MS    1000

void init_system();
void idle();
void test();
void abort();
void end();
// void transition_look_up(struct state_machine_t *state_machine, enum event_t event);

/* define all possible states */
enum state_t {
    _IDLE,
    _TEST,
    _ABORT,
    _END
};

/* define all events */
enum event_t {
    test_start,
    test_abort,
    test_complete,
    test_none
};

/* define a row in state transition matrix */
struct state_transit_row_t {
    enum state_t current_state;
    enum event_t event;
    enum state_t next_state;
};

/* define a matrix of state transistion */
static struct state_transit_row_t state_transition_matrix[] = {
    {_IDLE, test_start, _TEST},
    {_TEST, test_abort, _ABORT},
    {_TEST, test_complete, _END},
    {_ABORT, test_none, _END}
};

/* define a row in state function matrix */
struct state_function_row_t {
    const char* name;
    void (*func)(void);
};

/* define a matrix of state functions */
static struct state_function_row_t state_function_matrix[] = {
    {"STATE IDLE", idle},
    {"STATE TEST", test},
    {"STATE ABORT", abort},
    {"STATE END", end}
};

struct state_machine_t {
    enum state_t current_state;
};

void transition_look_up(struct state_machine_t* state_machine, enum event_t event) {
    for (uint8_t i=0;i<sizeof(state_transition_matrix)/sizeof(state_transition_matrix[0]);i++) {
        if (state_transition_matrix[i].current_state == state_machine->current_state) {
            if (state_transition_matrix[i].event == event) {
                /* transition to next state */
                state_machine->current_state = state_transition_matrix[i].next_state;
                /* run function */
                (state_function_matrix[state_machine->current_state].func)();
            }
        }
    }
}

int main() {
    init_system();

    /* this init step gets state machine going */
    struct state_machine_t state_machine; 
    state_machine.current_state = _IDLE;
    /* event = function() 
        pass this event to transition_look_up()
    */
    for (;;) {
        transition_look_up(state_machine, event);

    }
}

void init_system() {
    DDRB |= (1 << PB7);

    DDRE |= (1 << PE3) | (1 << PE4) | (1 << PE5);
    PORTE |= (1 << PE3) | (1 << PE4) | (1 << PE5);

    DDRG |= (1 << PG5);
    PORTG |= (1 << PG5);

    uart0_init();
    init_timer0();

    set_sleep_mode(0);

    sei();
}

void idle() {
    uart0_puts("system ready");

    sleep_mode();
}

void test() {
    // TODO
}

void abort() {
    // TODO
}

void end() {
    // TODO
}

// void transition_look_up(struct state_machine_t *state_machine, enum event_t event) {
//     for (uint8_t i=0;i<sizeof(state_transition_matrix)/sizeof(state_transition_matrix[0]);i++) {
//         if (state_transition_matrix[i].current_state == state_machine->current_state) {
//             if (state_transition_matrix[i].event == event) {
//                 /* transition to next state */
//                 state_machine->current_state = state_transition_matrix[i].next_state;
//                 /* run function */
//                 (state_function_matrix[state_machine->current_state].func)();
//             }
//         }
//     }
// }

ISR(TIMER0_OVF_vect) {
    PORTB ^= (1 << PB7);
}