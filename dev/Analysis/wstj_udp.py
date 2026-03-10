import socket
import struct

UDP_PORT = 2237  # Default WSJT-X port

class WSJTXListener:
    def __init__(self, port=UDP_PORT):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.bind(('', port))
        self.sock.setblocking(False)  # Non-blocking

    def read_utf8_string(self, data, offset):
        length = struct.unpack('>i', data[offset:offset+4])[0]
        offset += 4
        string = data[offset:offset+length].decode('utf-8') if length > 0 else ''
        return string, offset + length

    def get_status(self):
        try:
            data, _ = self.sock.recvfrom(1024)
        except BlockingIOError:
            return None  # No data available yet

        if data[0:4] != b'WSJT':
            return None

        message_type = struct.unpack('>H', data[8:10])[0]
        if message_type != 0x0002:  # Not a Status message
            return None

        offset = 10
        dial_freq = struct.unpack('>Q', data[offset:offset+8])[0]
        offset += 8

        callsign, offset = self.read_utf8_string(data, offset)
        grid, offset = self.read_utf8_string(data, offset)
        mode, offset = self.read_utf8_string(data, offset)
        # Skip next 4 strings (tx_enabled, transmitting, decoding, rx_df)
        for _ in range(4):
            _, offset = self.read_utf8_string(data, offset)

        return {
            'frequency': dial_freq,
            'callsign': callsign,
            'grid': grid,
            'mode': mode
        }
