#ifndef DEVICE_STATE_H
#define DEVICE_STATE_H

#include <stdint.h>

typedef struct {
    uint32_t xp;
    // Add additional state variables as needed.
} device_state_t;

void device_state_init(void);
void device_state_update(uint32_t xp_gain);
device_state_t device_state_get(void);

#endif // DEVICE_STATE_H
