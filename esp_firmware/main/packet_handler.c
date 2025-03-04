#include "packet_handler.h"
#include "device_state.h"
#include "comms.h"
#include "esp_log.h"
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "packet_handler";

// Helper function to calculate a simple checksum
static uint8_t calculate_checksum(const uint8_t *data, size_t len) {
    uint8_t sum = 0;
    for (size_t i = 0; i < len; i++) {
        sum += data[i];
    }
    return sum;
}

void process_packet(const uint8_t *data, size_t len) {
    // Check for minimal packet length (HEADER, TYPE, LENGTH, CHECKSUM, FOOTER)
    if (len < 5) {
        ESP_LOGE(TAG, "Packet too short");
        return;
    }
    // Verify header and footer
    if (data[0] != PACKET_HEADER || data[len - 1] != PACKET_FOOTER) {
        ESP_LOGE(TAG, "Invalid packet header or footer");
        return;
    }
    
    uint8_t type = data[1];
    uint8_t payload_length = data[2];
    uint8_t received_checksum = data[3];

    // Ensure the length matches the payload length plus overhead (5 bytes)
    if ((size_t)payload_length + 5 != len) {
        ESP_LOGE(TAG, "Payload length mismatch: expected %d, got %d", payload_length, len - 5);
        return;
    }
    
    // Calculate checksum: sum of type, length, and payload bytes
    uint8_t calc_checksum = type + payload_length;
    for (size_t i = 0; i < payload_length; i++) {
        calc_checksum += data[4 + i];
    }
    if (calc_checksum != received_checksum) {
        ESP_LOGE(TAG, "Checksum error: expected 0x%02x, got 0x%02x", calc_checksum, received_checksum);
        return;
    }
    
    // Process the packet based on its type
    switch (type) {
        case PACKET_TYPE_XP_UPDATE:
            if (payload_length < 2) {
                ESP_LOGE(TAG, "XP update payload too short");
            } else {
                uint16_t xp_update = (data[4] << 8) | data[5];
                update_device_xp(xp_update);
                ESP_LOGI(TAG, "XP updated by %d", xp_update);
            }
            break;
        case PACKET_TYPE_STATE_CHANGE:
            if (payload_length < 1) {
                ESP_LOGE(TAG, "State change payload too short");
            } else {
                uint8_t new_state = data[4];
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
                memcpy(new_name, &data[4], copy_len);
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

void packet_handler_task(void *pvParameters) {
    uint8_t packet_buffer[256];
    while (1) {
        int len = comms_receive(packet_buffer, sizeof(packet_buffer));
        if (len > 0) {
            ESP_LOGI(TAG, "Received packet (length %d)", len);
            process_packet(packet_buffer, (size_t)len);
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void send_packet(uint8_t type, const uint8_t *payload, size_t payload_length) {
    uint8_t packet[256];
    size_t packet_size = payload_length + 5;  // HEADER, TYPE, LENGTH, CHECKSUM, FOOTER
    if (packet_size > sizeof(packet)) {
        ESP_LOGE(TAG, "Payload too large for packet");
        return;
    }
    packet[0] = PACKET_HEADER;
    packet[1] = type;
    packet[2] = (uint8_t)payload_length;
    
    uint8_t checksum = type + (uint8_t)payload_length;
    for (size_t i = 0; i < payload_length; i++) {
        checksum += payload[i];
        packet[4 + i] = payload[i];
    }
    packet[3] = checksum;
    packet[packet_size - 1] = PACKET_FOOTER;
    
    comms_send(packet, packet_size);
    ESP_LOGI(TAG, "Packet sent: type 0x%02x, payload length %d", type, payload_length);
}
