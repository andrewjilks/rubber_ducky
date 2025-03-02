import serial
import time

SERIAL_PORT = "/dev/ttyUSB0"  # Adjust for your system
BAUD_RATE = 115200

def send_command(command):
    """Send a command to ESP32 and get the full response."""
    try:
        with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=2) as ser:
            ser.write((command + "\n").encode())  # Send command with newline
            time.sleep(0.1)  # Give ESP32 time to respond
            
            response = ser.read_until(b"\r\n").decode(errors="ignore").strip()  # Read until newline
            return response if response else "No response received."

    except serial.SerialException as e:
        return f"Serial Error: {e}"

def main():
    print("Rubber Ducky Interface - Type 'get' to retrieve data, 'exit' to quit.")

    while True:
        cmd = input("Enter command: ").strip()
        if cmd.lower() == "exit":
            break
        elif cmd == "get":
            response = send_command(cmd)
            print(f"ESP32 Response: {response}")
        else:
            print("Invalid command. Use 'get' or 'exit'.")

if __name__ == "__main__":
    main()
