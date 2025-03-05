#!/usr/bin/env python3
# pc/packet.py

PACKET_HEADER = 0xAA
PACKET_FOOTER = 0x55

# Packet type definitions
PACKET_TYPE_XP_UPDATE    = 0x01
PACKET_TYPE_STATE_CHANGE = 0x02
PACKET_TYPE_NAME_CHANGE  = 0x03
PACKET_TYPE_HANDSHAKE    = 0xFE  # Handshake initiation
PACKET_TYPE_HANDSHAKE_ACK = 0xFF  # Handshake acknowledgment
PACKET_TYPE_ACK          = 0xF0
PACKET_TYPE_NACK         = 0xF1

def calculate_checksum(packet_type, seq, length, payload):
    """Calculate checksum as the sum of packet_type, seq, length, and payload bytes modulo 256."""
    checksum = packet_type + seq + length
    for b in payload:
        checksum += b
    return checksum & 0xFF

def create_packet(packet_type, seq, payload):
    """
    Create a packet with the following format:
    [HEADER][TYPE][SEQ][LENGTH][CHECKSUM][PAYLOAD...][FOOTER]

    Args:
        packet_type (int): Type of the packet.
        seq (int): Sequence number.
        payload (bytes or bytearray): The payload data.
        
    Returns:
        bytearray: The complete packet.
    """
    if not isinstance(payload, (bytes, bytearray)):
        raise ValueError("Payload must be bytes or bytearray")
        
    length = len(payload)
    checksum = calculate_checksum(packet_type, seq, length, payload)
    packet = bytearray([PACKET_HEADER, packet_type, seq, length, checksum])
    packet.extend(payload)
    packet.append(PACKET_FOOTER)
    return packet
