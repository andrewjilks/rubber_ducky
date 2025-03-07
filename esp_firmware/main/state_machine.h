#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdint.h>

/* LIST OF MACHINE STATES
 * 0 - default/state switcher
 * 1 - serial communication mode
 * 2 - wifi mode
 * 3 - gpio mode
 * 4 - bluetooth mode
 * 5 - debug mode
 * 6 - config mode
 */

typedef enum {
    STATE_DEFAULT = 0,
    STATE_SERIAL = 1,
    STATE_WIFI = 2,
    STATE_GPIO = 3,
    STATE_BLUETOOTH = 4,
    STATE_DEBUG = 5,
    STATE_CONFIG = 6
} state_t;

void state_machine_init(void);
void state_machine_run(void);
void state_machine_switch_state(state_t new_state);

#endif // STATE_MACHINE_H
