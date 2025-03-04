#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include <stdint.h>
#include <stddef.h>

void process_packet(const uint8_t *data, size_t len);
void packet_handler_task(void *pvParameters);

#endif // PACKET_HANDLER_H
