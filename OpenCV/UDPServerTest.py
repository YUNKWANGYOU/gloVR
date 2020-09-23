import socket

sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
sock.bind(('127.0.0.1',8000))

data,addr = sock.recvfrom(200)

print("receive data : ",data)
print("receive addr : ",addr[0])
print("receive port : ",addr[1])
