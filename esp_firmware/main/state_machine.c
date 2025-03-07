/* State machine implementation guide
 *  1. INIT STATE
 *  2. READING NVS
 *  3. SWITCHING TO STATE MACHINE
 *  4. WRITE NVS FOR STATE UPDATE
 *  5. SWITCH STATES
 */

/* 1. STATE INIT + 2. READING NVS
 * FROM device_state.c
 * 
 * static const char *TAG = "device_state";
 * DeviceState device;  // Global instance
 *
 * #define NVS_NAMESPACE "storage"
 * #define NVS_KEY_NAME  "name"
 * #define NVS_KEY_XP    "xp"
 * #define NVS_KEY_STATE "state"
 * 
 * void set_device_state(uint8_t new_state) {
    device.state = new_state;
    nvs_handle_t handle;
    if (nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle) == ESP_OK) {
        nvs_set_u8(handle, NVS_KEY_STATE, device.state);
        nvs_commit(handle);
        nvs_close(handle);
    }
}
* 3. SWITCHING TO STATE MACHINE
* This is the function for tracking and updating the device state numerical code on the device
* this will be the point of entry for the state machine
* since the device state will already be stored on the device as NVS data it can be read directly
* 
* 4. WRITING AND UPDATING NVS DATA
* Each function will need to be capable of using the 0 state as an exit point to switch states
* in state 0 the previous paramaters wil determine the new state and do the apropriate switch
* lastky the NVS data wil be updated to reflect the new state
* 
* 5. SWITCHING STATE
* The newly written NVS data is read and the main state 0 function is called
* Depening on device.state data the proper function is called from the functions list
* */

/* CODE I WILL NEED TO ALTER
 * 
 * device_state.c as listed above to include new file
 * main.c - Start the main state listener / switcher
*/
#include "state_machine.h"
#include "device_state.h"
#include <stdio.h>

// Define function pointer type for state functions.
typedef void (*state_function_t)(void);

// Default state: acts as the switcher that decides the next state.
// (For demonstration, it automatically switches to serial mode.)
static void default_state(void) {
    printf("State: DEFAULT (Switcher mode)\n");
    // In a real implementation, add logic here (e.g., based on input) to choose the next state.
    //state_machine_switch_state(STATE_SERIAL);
}

// Serial communication mode:
static void serial_mode(void) {
    printf("State: SERIAL COMMUNICATION MODE\n");
    // Add serial communication handling code here.
    // After processing, switch back to default or another mode.
    //state_machine_switch_state(STATE_DEFAULT);
}

// WiFi mode:
static void wifi_mode(void) {
    printf("State: WIFI MODE\n");
    // Add WiFi initialization and handling code here.
    //state_machine_switch_state(STATE_DEFAULT);
}

// GPIO mode:
static void gpio_mode(void) {
    printf("State: GPIO MODE\n");
    // Add GPIO control code here.
    //state_machine_switch_state(STATE_DEFAULT);
}

// Bluetooth mode:
static void bluetooth_mode(void) {
    printf("State: BLUETOOTH MODE\n");
    // Add Bluetooth-related code here.
    //state_machine_switch_state(STATE_DEFAULT);
}

// Debug mode:
static void debug_mode(void) {
    printf("State: DEBUG MODE\n");
    // Add debugging routines here.
    //state_machine_switch_state(STATE_DEFAULT);
}

// Config mode:
static void config_mode(void) {
    printf("State: CONFIG MODE\n");
    // Add configuration-related code here.
    //state_machine_switch_state(STATE_DEFAULT);
}

// Array mapping state codes to their corresponding functions.
static const state_function_t state_functions[] = {
    default_state,
    serial_mode,
    wifi_mode,
    gpio_mode,
    bluetooth_mode,
    debug_mode,
    config_mode
};

static state_t current_state = STATE_DEFAULT;

void state_machine_init(void)
{
    // Initialize device state from NVS (device_state.c handles this)
    init_device_state();
    current_state = STATE_DEFAULT;
    set_device_state(current_state);
}

void state_machine_run(void)
{
    // Update current_state from the global device state.
    current_state = (state_t) device.state;
    if (current_state < (sizeof(state_functions) / sizeof(state_function_t))) {
         state_functions[current_state]();
    } else {
         printf("Error: Undefined state %d\n", current_state);
    }
}

void state_machine_switch_state(state_t new_state)
{
    current_state = new_state;
    set_device_state(new_state);
}
