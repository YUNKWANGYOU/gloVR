import socket

target_host = "127.0.0.1"
target_port = 8000

# create a socket object
client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
client.bind((target_host,target_port))
# receive some data
data, addr = client.recvfrom(200)

print(data)