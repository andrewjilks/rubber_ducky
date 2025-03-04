#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "packet_handler.h"
#include "esp_log.h"
#include "comms.h"
#include "device_state.h"


static const char *TAG = "packet_handler";

void process_packet(const uint8_t *data, size_t len) {
    // For demonstration, log the received packet length.
    ESP_LOGI(TAG, "Received packet of length %d", len);

    // Here you might parse the packet, validate it, and update device state.
    // For example, if this is an XP gain packet, call device_state_update(xp_gain);
}

void packet_handler_task(void *pvParameters) {
    uint8_t packet_buffer[256];
    while (1) {
        // Attempt to receive data from the comms interface.
        int len = comms_receive(packet_buffer, sizeof(packet_buffer));
        if (len > 0) {
            process_packet(packet_buffer, len);
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
