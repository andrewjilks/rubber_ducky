#include <stdio.h>
#include <string.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "driver/uart.h"

#define STORAGE_NAMESPACE "duck_data"
#define UART_NUM UART_NUM_0  // Use UART0 for serial communication
#define BUF_SIZE 512

void save_duck_data(const char *json_data) {
    nvs_handle_t my_handle;
    if (nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle) == ESP_OK) {
        nvs_set_str(my_handle, "data", json_data);
        nvs_commit(my_handle);
        nvs_close(my_handle);
    }
}

void get_duck_data(char *buffer, size_t buffer_size) {
    nvs_handle_t my_handle;
    size_t length = buffer_size;  // Ensure length is set before calling nvs_get_str

    if (nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle) == ESP_OK) {
        if (nvs_get_str(my_handle, "data", buffer, &length) != ESP_OK) {
            snprintf(buffer, buffer_size, "{\"name\":\"Rubber Ducky\",\"mood\":\"Neutral\",\"xp\":0}");
        }
        nvs_close(my_handle);
    }
}

void init_duck_data() {
    nvs_flash_init();
    
    nvs_handle_t my_handle;
    if (nvs_open(STORAGE_NAMESPACE, NVS_READONLY, &my_handle) != ESP_OK) {
        const char *default_duck = "{\"name\":\"Rubber Ducky\",\"mood\":\"Neutral\",\"xp\":0}";
        save_duck_data(default_duck);
    } else {
        nvs_close(my_handle);
    }
}

void app_main() {
    init_duck_data();

    char duck_data[256] = {0};
    get_duck_data(duck_data, sizeof(duck_data));

    printf("Duck Initialized. Waiting for commands...\n");

    // UART Configuration
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_driver_install(UART_NUM, BUF_SIZE, 0, 0, NULL, 0);
    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    char command[32] = {0};

    while (1) {
        int len = uart_read_bytes(UART_NUM, command, sizeof(command) - 1, 100 / portTICK_PERIOD_MS);
        
        if (len > 0) {
            command[len] = '\0';  // Null-terminate the received command

            if (strncmp(command, "get", 3) == 0) {
                char response[258]; // 256 + \r\n + \0
                snprintf(response, sizeof(response), "%.*s\r\n", (int)(sizeof(response) - 3), duck_data);
                uart_write_bytes(UART_NUM, response, strlen(response));
            } else {
                uart_write_bytes(UART_NUM, "Unknown command\r\n", 17);
            }

            memset(command, 0, sizeof(command));  // Clear buffer after handling
        }

        vTaskDelay(pdMS_TO_TICKS(100));  // Delay to prevent CPU overload
    }
}
