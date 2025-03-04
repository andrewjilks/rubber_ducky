#ifndef COMMS_H
#define COMMS_H

#include <stdint.h>
#include <stddef.h>

void comms_init(void);
int comms_receive(uint8_t *data, size_t length);
int comms_send(const uint8_t *data, size_t length);

#endif // COMMS_H
