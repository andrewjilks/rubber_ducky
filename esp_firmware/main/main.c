#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "device_state.h"
#include "comms.h"
#include "packet_handler.h"

static const char *TAG = "rubber_ducky";

void app_main(void) {
    ESP_LOGI(TAG, "Starting Rubber Ducky Firmware");

    // Initialize device state and communication interface
    device_state_init();
    comms_init();

    // Create a task to handle incoming packets
    xTaskCreate(packet_handler_task, "packet_handler_task", 4096, NULL, 5, NULL);

    // Additional application logic can be added here
    // For example: create periodic tasks or process other events
}
