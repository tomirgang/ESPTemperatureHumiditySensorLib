#!/usr/bin/env python3

import socket
import time



if __name__ == '__main__':
    server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
    server.settimeout(0.2)

    while(1):
        server.sendto("Test message!".encode('utf-8'), ('255.255.255.255', 44221))
        print('UDP message was sent!')
        time.sleep(1)
