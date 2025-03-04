#!/usr/bin/env python3
# pc/duck_interface.py

import serial
import sys
from packet import create_packet, PACKET_TYPE_NAME_CHANGE

def main():
    if len(sys.argv) != 3:
        print("Usage: {} <serial_port> <new_name>".format(sys.argv[0]))
        sys.exit(1)

    serial_port = sys.argv[1]
    new_name = sys.argv[2]

    # Encode the new name to bytes (ASCII encoding)
    payload = new_name.encode('ascii')

    # Build the packet for a name change
    packet = create_packet(PACKET_TYPE_NAME_CHANGE, payload)
    print("Sending packet:", packet.hex())

    try:
        ser = serial.Serial(serial_port, 115200, timeout=1)
        ser.write(packet)
        ser.close()
        print("Packet sent successfully")
    except Exception as e:
        print("Failed to send packet:", e)

if __name__ == '__main__':
    main()
