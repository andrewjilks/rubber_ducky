#!/usr/bin/env python3
# pc/duck_interface.py

import serial
import sys
from packet import create_packet, \
    PACKET_TYPE_NAME_CHANGE, PACKET_TYPE_XP_UPDATE, PACKET_TYPE_STATE_CHANGE

# Global sequence counter for packets
global_seq = 0

def print_menu():
    print("\n=== ESP Duck Control Interface ===")
    print("1. Change Name")
    print("2. Update XP")
    print("3. Change State")
    print("4. Exit")

def send_packet(serial_port, packet):
    try:
        with serial.Serial(serial_port, 115200, timeout=1) as ser:
            ser.write(packet)
            print("Packet sent successfully!")
    except Exception as e:
        print("Failed to send packet:", e)

def change_name(serial_port):
    global global_seq
    new_name = input("Enter new name: ")
    payload = new_name.encode('ascii')
    packet = create_packet(PACKET_TYPE_NAME_CHANGE, global_seq, payload)
    global_seq = (global_seq + 1) % 256  # Wrap sequence number if needed
    print("Sending packet:", packet.hex())
    send_packet(serial_port, packet)

def update_xp(serial_port):
    global global_seq
    xp_input = input("Enter XP update (number): ")
    try:
        xp_value = int(xp_input)
        # Convert XP to 2 bytes big-endian. Adjust if your protocol differs.
        payload = xp_value.to_bytes(2, byteorder='big')
        packet = create_packet(PACKET_TYPE_XP_UPDATE, global_seq, payload)
        global_seq = (global_seq + 1) % 256
        print("Sending packet:", packet.hex())
        send_packet(serial_port, packet)
    except ValueError:
        print("Invalid XP value. Please enter a valid integer.")

def change_state(serial_port, state_index):
    global global_seq
    print("\nAvailable States:")
    for state_id, state_desc in state_index.items():
        print(f"  {state_id}: {state_desc}")
    state_input = input("Enter state id: ")
    try:
        state_value = int(state_input)
        payload = state_value.to_bytes(1, byteorder='big')
        packet = create_packet(PACKET_TYPE_STATE_CHANGE, global_seq, payload)
        global_seq = (global_seq + 1) % 256
        print("Sending packet:", packet.hex())
        send_packet(serial_port, packet)
    except ValueError:
        print("Invalid state id. Please enter a valid integer.")

def main():
    if len(sys.argv) != 2:
        print("Usage: {} <serial_port>".format(sys.argv[0]))
        sys.exit(1)

    serial_port = sys.argv[1]

    # State index mapping state IDs to human-readable descriptions
    state_index = {
        0: "Idle",
        1: "Working",
        2: "Sleeping",
        3: "Error"
    }

    while True:
        print_menu()
        choice = input("Select an option: ")
        if choice == '1':
            change_name(serial_port)
        elif choice == '2':
            update_xp(serial_port)
        elif choice == '3':
            change_state(serial_port, state_index)
        elif choice == '4':
            print("Exiting...")
            break
        else:
            print("Invalid option. Please try again.")

if __name__ == '__main__':
    main()
