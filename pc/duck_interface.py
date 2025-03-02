import serial
import time

# Serial port configuration (adjust as needed)
SERIAL_PORT = "/dev/ttyUSB0"  # Change this based on your OS
BAUD_RATE = 115200

def send_command(ser, command):
    """Send a command to the ESP32 and read the response."""
    ser.write((command + "\n").encode())
    time.sleep(0.1)  # Give ESP32 time to process
    response = ser.read_all().decode().strip()
    return response

def main():
    try:
        # Open serial connection
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        time.sleep(2)  # Wait for ESP32 to initialize

        # Get current duck data
        print("Fetching current duck data...")
        duck_data = send_command(ser, "get")
        print(f"Current Duck Data: {duck_data}")

        if '"name":"' in duck_data:
            print("A name is already set.")
        else:
            # Ask user for a name
            new_name = input("Enter a name for your duck: ").strip()

            if len(new_name) > 20:  # Prevent overly long names
                print("Name is too long. Using the first 20 characters.")
                new_name = new_name[:20]

            # Send new name to ESP32
            response = send_command(ser, f"setname {new_name}")
            print(response)

            # Confirm update
            updated_data = send_command(ser, "get")
            print(f"Updated Duck Data: {updated_data}")

        ser.close()

    except serial.SerialException as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    main()
