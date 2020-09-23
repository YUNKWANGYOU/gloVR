import socket

sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)

sock.sendto("HI".encode(),("127.0.0.1",8000))