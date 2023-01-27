import socket
import hashlib
import hmac
import math
import time

def server_program():
    with open('secret_key', 'r') as file:
        data = file.read().rstrip()
    print(generate_totp(data))

def generate_totp(shared_key: str, length: int = 6) -> str:
    now_in_seconds = math.floor(time.time())
    step_in_seconds = 20
    t = math.floor(now_in_seconds / step_in_seconds)
    hash = hmac.new(
        bytes(shared_key, encoding="utf-8"),
        t.to_bytes(length=8, byteorder="big"),
        hashlib.sha256,
    )

    return dynamic_truncation(hash, length)
    
def dynamic_truncation(raw_key: hmac.HMAC, length: int) -> str:
    bitstring = bin(int(raw_key.hexdigest(), base=16))
    last_four_bits = bitstring[-4:]
    offset = int(last_four_bits, base=2)
    chosen_32_bits = bitstring[offset * 8 : offset * 8 + 32]
    full_totp = str(int(chosen_32_bits, base=2))
    return full_totp[-length:]
    
if __name__ == '__main__':
    server_program()
