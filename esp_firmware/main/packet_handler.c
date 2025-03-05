#include "packet_handler.h"
#include "device_state.h"
#include "comms.h"
#include "esp_log.h"
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "packet_handler";

// Helper function to calculate a simple checksum (unused in favor of inline calculation)
static uint8_t calculate_checksum(const uint8_t *data, size_t len) {
    uint8_t sum = 0;
    for (size_t i = 0; i < len; i++) {
        sum += data[i];
    }
    return sum;
}

/*
Packet format:
-------------------------------------------------
| HEADER (1) | TYPE (1) | SEQ (1) | LENGTH (1) |
| CHECKSUM (1) | PAYLOAD (variable) | FOOTER (1) |
-------------------------------------------------
*/

// Process an incoming packet
void process_packet(const uint8_t *data, size_t len) {
    // Minimal packet length check (6 bytes)
    if (len < 6) {
        ESP_LOGE(TAG, "Packet too short");
        return;
    }
    
    // Verify header and footer
    if (data[0] != PACKET_HEADER || data[len - 1] != PACKET_FOOTER) {
        ESP_LOGE(TAG, "Invalid packet header or footer");
        // If possible, use provided sequence number to send NACK
        if (len >= 3) {
            send_packet(PACKET_TYPE_NACK, data[2], NULL, 0);
        }
        return;
    }

    uint8_t type = data[1];
    uint8_t seq = data[2];           // Sequence number
    uint8_t payload_length = data[3];
    uint8_t received_checksum = data[4];

    // Check overall length: expected = payload_length + 6
    if ((size_t)payload_length + 6 != len) {
        ESP_LOGE(TAG, "Payload length mismatch: expected %d, got %d", payload_length, len - 6);
        send_packet(PACKET_TYPE_NACK, seq, NULL, 0);
        return;
    }

    // Calculate checksum: sum of TYPE, SEQ, LENGTH and payload bytes
    uint8_t calc_checksum = type + seq + payload_length;
    for (size_t i = 0; i < payload_length; i++) {
        calc_checksum += data[5 + i];
    }
    if (calc_checksum != received_checksum) {
        ESP_LOGE(TAG, "Checksum error: expected 0x%02x, got 0x%02x", calc_checksum, received_checksum);
        send_packet(PACKET_TYPE_NACK, seq, NULL, 0);
        return;
    }

    // Valid packet: send ACK
    send_packet(PACKET_TYPE_ACK, seq, NULL, 0);
    ESP_LOGI(TAG, "ACK sent for seq %d", seq);

    // Process packet based on its type
    switch (type) {
        case PACKET_TYPE_XP_UPDATE:
            if (payload_length < 2) {
                ESP_LOGE(TAG, "XP update payload too short");
            } else {
                uint16_t xp_update = (data[5] << 8) | data[6];
                update_device_xp(xp_update);
                ESP_LOGI(TAG, "XP updated by %d", xp_update);
            }
            break;
        case PACKET_TYPE_STATE_CHANGE:
            if (payload_length < 1) {
                ESP_LOGE(TAG, "State change payload too short");
            } else {
                uint8_t new_state = data[5];
                set_device_state(new_state);
                ESP_LOGI(TAG, "State updated to %d", new_state);
            }
            break;
        case PACKET_TYPE_NAME_CHANGE:
            if (payload_length == 0) {
                ESP_LOGE(TAG, "Name change payload is empty");
            } else {
                char new_name[32];
                size_t copy_len = (payload_length < 31) ? payload_length : 31;
                memcpy(new_name, &data[5], copy_len);
                new_name[copy_len] = '\0';
                update_device_name(new_name);
                ESP_LOGI(TAG, "Device name updated to: %s", new_name);
            }
            break;
        default:
            ESP_LOGW(TAG, "Unknown packet type: 0x%02x", type);
            break;
    }
}

// Task to continuously receive and process packets
void packet_handler_task(void *pvParameters) {
    uint8_t packet_buffer[256];
    static uint8_t last_seq = 0;
    
    while (1) {
        int len = comms_receive(packet_buffer, sizeof(packet_buffer));
        if (len > 0) {
            ESP_LOGI(TAG, "Received packet (length %d)", len);
            uint8_t received_seq = packet_buffer[2];

            // Ignore duplicate packets (same sequence number as last processed)
            if (received_seq == last_seq) {
                ESP_LOGW(TAG, "Duplicate packet detected, ignoring.");
                continue;
            }

            last_seq = received_seq;
            process_packet(packet_buffer, (size_t)len);
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

// Send a packet with the given type, sequence, and payload
void send_packet(uint8_t type, uint8_t seq, const uint8_t *payload, size_t payload_length) {
    uint8_t packet[256];
    size_t packet_size = payload_length + 6;  // HEADER, TYPE, SEQ, LENGTH, CHECKSUM, FOOTER

    if (packet_size > sizeof(packet)) {
        ESP_LOGE(TAG, "Payload too large for packet");
        return;
    }

    packet[0] = PACKET_HEADER;
    packet[1] = type;
    packet[2] = seq;
    packet[3] = (uint8_t)payload_length;

    uint8_t checksum = type + seq + (uint8_t)payload_length;
    for (size_t i = 0; i < payload_length; i++) {
        checksum += payload[i];
        packet[5 + i] = payload[i];
    }
    packet[4] = checksum;
    packet[packet_size - 1] = PACKET_FOOTER;

    comms_send(packet, packet_size);
    ESP_LOGI(TAG, "Packet sent: type 0x%02x, seq %d, payload length %d", type, seq, payload_length);
}
