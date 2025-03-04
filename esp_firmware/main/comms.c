#include "comms.h"
#include "driver/uart.h"
#include "esp_log.h"

static const char *TAG = "comms";

#define UART_NUM       UART_NUM_1
#define BUF_SIZE       (1024)

void comms_init(void) {
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    uart_param_config(UART_NUM, &uart_config);
    uart_driver_install(UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0);
    ESP_LOGI(TAG, "UART initialized on UART%d", UART_NUM);
}

int comms_receive(uint8_t *data, size_t length) {
    // Read bytes from UART; timeout set to 20ms.
    int len = uart_read_bytes(UART_NUM, data, length, 20 / portTICK_PERIOD_MS);
    return len;
}

int comms_send(const uint8_t *data, size_t length) {
    int len = uart_write_bytes(UART_NUM, (const char*)data, length);
    return len;
}
