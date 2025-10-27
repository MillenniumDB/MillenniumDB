import socket

HOST = "127.0.0.1"  # Server IP or hostname
PORT = 1234  # Server port


def encode_chunks(s: bytes):
    encoded = b""

    while len(s) > 0:
        chunk = s[:1400]
        size = len(chunk)
        encoded_size = size.to_bytes(2, byteorder="big")
        encoded += encoded_size + chunk
    encoded += b"\x00\x00"
    print(encoded)
    return encoded


data = b"\x01"
message = b"MDB_DRVR\x00\x01\x00\x00\x00"

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    try:
        # Connect to the server
        s.connect((HOST, PORT))

        s.sendall(message)

    except ConnectionRefusedError:
        print(f"Cannot connect to {HOST}:{PORT}")
    except Exception as e:
        print("Error:", e)
