#ifndef DEVICE_STATE_H
#define DEVICE_STATE_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    char name[32];
    char model[32];
    uint32_t xp;
    uint8_t state;
} DeviceState;

extern DeviceState device;  // Makes 'device' available to other files

void init_device_state(void);
void update_device_xp(uint32_t amount);
void set_device_state(uint8_t new_state);
uint8_t get_device_state(void);
void update_device_name(const char *new_name);
void get_device_info(char *buffer, size_t buffer_size);

#endif // DEVICE_STATE_H
