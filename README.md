# 🦆 Rubber Ducky - ESP32 Virtual Desk Pet

**Rubber Ducky** is a programmable, wireless-enabled desk pet based on the **ESP32**, designed to interact with your development workflow. Inspired by Cube World, it gains XP for coding tasks like making Git commits and integrates with a desktop app for enhanced functionality.

---

## 📁 Project Structure

    rubber_ducky/
    ├── esp_firmware/               # ESP32 firmware
    │   ├── build/                  # Compiled binaries and build-related files
    │   ├── main/                   # Core project source files
    │   │   ├── main.c              # Main program entry point
    │   │   ├── packet_handler.c    # Handles packet operations
    │   │   ├── packet_handler.h    # Packet handler header file
    │   │   ├── device_state.c      # Stores and modifies device state
    │   │   ├── device_state.h      # Header for device state management
    │   │   ├── comms.c             # Handles serial communication
    │   │   ├── comms.h             # Header for serial communication
    │   │   ├── state_machine.c     # Main state machine implementation
    │   │   ├── state_machine.h     # Header for state machine source
    │   ├── CMakeLists.txt          # ESP-IDF build configuration
    ├── pc/                         # PC-side interface
    │   ├── duck_interface.py       # Handles serial communication with ESP32
    │   ├── packet.py               # Defines packet structure and encoding/decoding
    │   ├── gui.py                  # Optional GUI for enhanced functionality
    │   ├── config.json             # Configuration file (device settings)
    │   ├── README.md               # Documentation for PC-side interface
    ├── .gitignore                  # Files to ignore in version control
    ├── README.md                   # Project overview and documentation
    ├── requirements.txt            # Python dependencies

---

## 🛠 Features

- **ESP32 Virtual Pet:** Tracks XP, state, and interacts with the user.
- **Serial Communication:** Uses structured packets between ESP32 and PC.
- **GitHub Integration:** Gains XP for making commits and coding tasks.
- **Packet Handling:** Build, send, receive, decode, and process packets.
- **Desktop App (Planned):** GUI for monitoring the pet’s state and actions.

---

## 🚀 Getting Started

### **1. Clone the Repository**

    git clone git@github.com:andrewjilks/rubber_ducky.git
    cd rubber_ducky

### **2. Setting Up the ESP32 Firmware**

Make sure you have **ESP-IDF** installed. Then, navigate to the firmware directory:

    cd esp_firmware
    idf.py set-target esp32
    idf.py menuconfig   # Optional: Configure settings
    idf.py build flash monitor

### **3. Setting Up the PC Interface**

Ensure you have Python installed, then install dependencies:

    cd pc
    pip install -r requirements.txt

To communicate serially via text interface:

    python3 duck_interface.py /dev/ttyUSB0

---

## 📡 Communication Protocol

Packets follow a structured format for reliable communication:

| Byte    | Field         | Description                               |
|---------|--------------|-------------------------------------------|
| 0       | **Header**    | Start of packet identifier (0xAA)         |
| 1       | **Type/ID**   | Command type (e.g., XP update, state change) |
| 2       | **Payload Length** | Length of the data payload            |
| 3       | **Checksum**  | Error-checking byte                        |
| 4 - N   | **Data**      | Actual payload (XP value, state, etc.)    |
| N+1     | **Footer**    | End of packet identifier (0x55)          |

### **Example Packet (XP Gain)**

    AA 01 02 BC 000A 55

- **AA** → Header (Start of packet)  
- **01** → Type/ID (XP gain command)  
- **02** → Payload Length (2 bytes of data)  
- **BC** → Checksum (basic validation)  
- **000A** → Data (XP increase of 10)  
- **55** → Footer (End of packet)  

---

## State Machine Implementation

To enhance the modularity and maintainability of our firmware, we've integrated a state machine architecture. This design allows the system to transition between various operational modes, each encapsulated within distinct states. Such an approach not only streamlines the control flow but also simplifies debugging and future feature expansions.

### State Definitions

The state machine comprises the following states:

1. **STATE_INIT**: Initialization and cleanup operations.
2. **STATE_SERIAL**: Handling serial communication tasks.
3. **STATE_WIFI**: Managing Wi-Fi functionalities.
4. **STATE_GPIO**: Controlling General-Purpose Input/Output (GPIO) operations.
5. **STATE_BLUETOOTH**: Placeholder for future Bluetooth functionalities.
6. **STATE_DEBUG**: Performing debugging and logging activities.
7. **STATE_CONFIG**: Modifying settings and enabling content sharing.

Each state is associated with a specific function that executes the tasks pertinent to that state.

### State Transition Mechanism

Transitions between states are managed by the `set_state(device_state_t new_state)` function. This function ensures that the system moves seamlessly from one state to another based on predefined conditions or events. For instance, after completing initialization in `STATE_INIT`, the system might transition to `STATE_SERIAL` to handle communication tasks.

### State Handlers

Each state has a dedicated handler function responsible for executing its specific operations. These handlers are organized in an array for efficient access:

static const state_function_t state_functions[] = {
    default_state,
    serial_mode,
    wifi_mode,
    gpio_mode,
    bluetooth_mode,
    debug_mode,
    config_mode
};

---

## 📌 To-Do List

- [x] Set up project structure
- [x] Initialize Git repository
- [ ] Implement ESP32 firmware logic
- [ ] Develop Python serial interface
- [ ] Build XP tracking and state machine
- [ ] Create a simple GUI (optional)

---

## 📜 License

This project is licensed under the **MIT License**.

---

## 🏗 Contributors

- **Andrew Jilks** - Creator & Developer
Last successful build: 2025-03-06 19:45:49
