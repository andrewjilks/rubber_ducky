#include "device_state.h"
#include "esp_log.h"

static const char *TAG = "device_state";
static device_state_t current_state;

void device_state_init(void) {
    current_state.xp = 0;
    ESP_LOGI(TAG, "Device state initialized: XP = %d", current_state.xp);
}

void device_state_update(uint32_t xp_gain) {
    current_state.xp += xp_gain;
    ESP_LOGI(TAG, "XP updated: %d", current_state.xp);
}

device_state_t device_state_get(void) {
    return current_state;
}
