#ifndef COMMS_H
#define COMMS_H

#include <stdint.h>
#include <stddef.h>  // ✅ Fix: Include this for size_t

void comms_init(void);
int comms_receive(uint8_t *data, size_t length);
int comms_send(const uint8_t *data, size_t length);
void send_device_info(void);

#endif // COMMS_H
