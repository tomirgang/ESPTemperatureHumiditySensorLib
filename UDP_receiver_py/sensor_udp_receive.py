#!/usr/bin/env python3

import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
# sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
sock.bind(('', 44221))

while True:
    data, address = sock.recvfrom(1024)
    print("%r: %s" % (address, data.decode('utf-8')))
