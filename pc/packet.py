#!/usr/bin/env python3
# pc/packet.py

PACKET_HEADER = 0xAA
PACKET_FOOTER = 0x55

# Define a packet type for name change (e.g. 0x03)
PACKET_TYPE_NAME_CHANGE = 0x03

def calculate_checksum(packet_type, length, payload):
    """Simple checksum: sum of packet_type, length, and payload bytes, mod 256."""
    checksum = packet_type + length
    for b in payload:
        checksum += b
    return checksum & 0xFF

def create_packet(packet_type, payload):
    """
    Create a packet with the following format:
    [HEADER][TYPE][LENGTH][CHECKSUM][PAYLOAD...][FOOTER]
    """
    length = len(payload)
    checksum = calculate_checksum(packet_type, length, payload)
    packet = bytearray([PACKET_HEADER, packet_type, length, checksum])
    packet.extend(payload)
    packet.append(PACKET_FOOTER)
    return packet
