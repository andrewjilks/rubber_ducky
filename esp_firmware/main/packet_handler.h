#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include <stddef.h>
#include <stdint.h>

// Packet framing
#define PACKET_HEADER         0xAA
#define PACKET_FOOTER         0x55

// Packet types
#define PACKET_TYPE_XP_UPDATE     0x01
#define PACKET_TYPE_STATE_CHANGE  0x02
#define PACKET_TYPE_NAME_CHANGE   0x03
#define PACKET_TYPE_ACK           0xF0
#define PACKET_TYPE_NACK          0xF1
#define PACKET_TYPE_HANDSHAKE     0xFE
#define PACKET_TYPE_HANDSHAKE_ACK 0xFF

// Function prototypes
void process_packet(const uint8_t *data, size_t len);
void packet_handler_task(void *pvParameters);
void send_packet(uint8_t type, uint8_t seq, const uint8_t *payload, size_t payload_length);

#endif // PACKET_HANDLER_H
