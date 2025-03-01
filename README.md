# Rubber Ducky - ESP32 Communication Companion

## Overview
**Rubber Ducky** is a programmable, wireless-enabled desk pet based on the ESP32, designed to interact with your development workflow. It acts as a real-time coding assistant, providing helpful automation, notifications, and feedback during programming sessions. The project prioritizes **ESP32-to-PC communication** and **workflow integration** while keeping most of the processing on the PC to optimize ESP32’s limited flash memory.

## Current Functionality
### ✅ **Project Setup & Infrastructure**
- ESP-IDF environment fully set up and integrated.
- Git version control with push/pull automation.
- Makefile for **building, flashing, and syncing** ESP32 firmware.
- Standard ESP-IDF project structure initialized.

### 🚀 **Built Features**
- Basic ESP32 firmware structure using ESP-IDF.
- **ESP32 ↔ PC serial communication enabled**.
- **Command-based interaction**: PC can send commands to retrieve stored data from the ESP32.
- **Persistent storage using NVS (Non-Volatile Storage)** for saving duck data.
- **Python interface script for interacting with the ESP32**.

## Installation & Setup
### **1. Clone the Repository**
```sh
cd ~
git clone git@github.com:your-username/rubber_ducky.git
cd rubber_ducky
```

### **2. Setup ESP-IDF Environment**
Ensure that ESP-IDF is correctly installed and sourced:
```sh
source ~/esp-idf/export.sh
```

### **3. Activate Virtual Environment**
```sh
source venv/bin/activate
```

### **4. Build and Flash the ESP32**
```sh
make build
make flash
make monitor  # View ESP32 serial output
```

## Using the ESP32 ↔ PC Communication

### **1. Start the Python Interface**
Ensure the ESP32 is connected via USB and start the interface script:
```sh
python duck_interface.py
```

### **2. Send Commands**
Once the interface is running, enter commands in the terminal:
- `get` → Retrieves stored duck data from the ESP32.
- `exit` → Closes the interface.

### **3. Expected Output**
Example:
```
Rubber Ducky Interface - Type 'get' to retrieve data, 'exit' to quit.
Enter command: get
ESP32 Response: {"name":"Rubber Ducky","mood":"Neutral","xp":0}
```

## Terminal Commands
| Command           | Description |
|------------------|-------------|
| `make build`   | Compiles the ESP32 firmware. |
| `make flash`   | Uploads firmware to ESP32. |
| `make monitor` | Opens serial monitor to view ESP32 logs. |
| `make clean`   | Cleans the build directory. |
| `make sync`    | Commits & pushes changes to GitHub. |

## Next Steps
- **Enhance ESP32-PC communication**: Add more commands and responses.
- **Develop GUI for Rubber Ducky interaction**.
- **Expand firmware functionality** for more interactive responses.
- **Introduce Wi-Fi/Bluetooth connectivity** for wireless communication.

---
This README will be updated as more features are developed. 🚀

