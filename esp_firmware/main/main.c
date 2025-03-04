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

    // Initialize device state and communication interface.
    init_device_state();
    comms_init();

    // Optionally, send initial device info.
    send_device_info();

    // Create a task to handle incoming packets.
    xTaskCreate(packet_handler_task, "packet_handler_task", 4096, NULL, 5, NULL);

    // Application logic continues...
    ESP_LOGI(TAG, "Returned from app_main()");
}
