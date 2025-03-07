#include "device_state.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

static const char *TAG = "device_state";
DeviceState device;  // Global instance

#define NVS_NAMESPACE "storage"
#define NVS_KEY_NAME  "name"
#define NVS_KEY_XP    "xp"
#define NVS_KEY_STATE "state"

void init_device_state(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
       ESP_ERROR_CHECK(nvs_flash_erase());
       err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    nvs_handle_t handle;
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
         ESP_LOGE(TAG, "Error (%s) opening NVS handle!", esp_err_to_name(err));
         // Use defaults if NVS cannot be opened
         strcpy(device.name, "RubberDucky");
         strcpy(device.model, "ESP32-PET");
         device.xp = 0;
         device.state = 0;
         return;
    }

    // Read device name
    size_t required_size = sizeof(device.name);
    err = nvs_get_str(handle, NVS_KEY_NAME, device.name, &required_size);
    if (err != ESP_OK) {
         ESP_LOGI(TAG, "Name not found, setting default");
         strcpy(device.name, "RubberDucky");
         nvs_set_str(handle, NVS_KEY_NAME, device.name);
    }

    // Device model is hard-coded.
    strcpy(device.model, "ESP32-PET");

    // Read XP
    err = nvs_get_u32(handle, NVS_KEY_XP, &device.xp);
    if (err != ESP_OK) {
         device.xp = 0;
         nvs_set_u32(handle, NVS_KEY_XP, device.xp);
    }

    // Read state
    err = nvs_get_u8(handle, NVS_KEY_STATE, &device.state);
    if (err != ESP_OK) {
         device.state = 0;
         nvs_set_u8(handle, NVS_KEY_STATE, device.state);
    }

    nvs_commit(handle);
    nvs_close(handle);
}

void update_device_xp(uint32_t amount) {
    device.xp += amount;
    nvs_handle_t handle;
    if (nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle) == ESP_OK) {
        nvs_set_u32(handle, NVS_KEY_XP, device.xp);
        nvs_commit(handle);
        nvs_close(handle);
    }
}

void set_device_state(uint8_t new_state) {
    device.state = new_state;
    nvs_handle_t handle;
    if (nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle) == ESP_OK) {
        nvs_set_u8(handle, NVS_KEY_STATE, device.state);
        nvs_commit(handle);
        nvs_close(handle);
    }
}

uint8_t get_device_state(void) {
    uint8_t state = 0;
    nvs_handle_t handle;
    if (nvs_open(NVS_NAMESPACE, NVS_READONLY, &handle) == ESP_OK) {
         nvs_get_u8(handle, NVS_KEY_STATE, &state);
         nvs_close(handle);
    }
    return state;
}

void update_device_name(const char *new_name) {
    strncpy(device.name, new_name, sizeof(device.name) - 1);
    device.name[sizeof(device.name) - 1] = '\0';
    nvs_handle_t handle;
    if (nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle) == ESP_OK) {
        nvs_set_str(handle, NVS_KEY_NAME, device.name);
        nvs_commit(handle);
        nvs_close(handle);
    }
}

void get_device_info(char *buffer, size_t buffer_size) {
    snprintf(buffer, buffer_size, "NAME:%s MODEL:%s XP:%" PRIu32 " STATE:%d",
             device.name, device.model, device.xp, device.state);
}
