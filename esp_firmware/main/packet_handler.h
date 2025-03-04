#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include <stdint.h>
#include <stddef.h>

#define PACKET_HEADER 0xAA
#define PACKET_FOOTER 0x55

// Packet types
typedef enum {
    PACKET_TYPE_XP_UPDATE    = 0x01,
    PACKET_TYPE_STATE_CHANGE = 0x02,
    PACKET_TYPE_NAME_CHANGE  = 0x03,  // NEW: Change duck's name
} PacketType;

void process_packet(const uint8_t *data, size_t len);
void packet_handler_task(void *pvParameters);
void send_packet(uint8_t type, const uint8_t *payload, size_t payload_length);

#endif // PACKET_HANDLER_H
