import socket
import time

ITERATIONS = 500
rtt = 0
rttSum = 0

print("We're in tcp client...")

server_name = '13.60.246.82'
server_port = 12000

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

client_socket.connect((server_name, server_port))

for i in range(1, ITERATIONS + 1):
    message = "42"
    start_time = time.time()
    client_socket.send(message.encode())
    response = client_socket.recv(1024)
    end_time = time.time()
    rtt = (end_time - start_time) * 1000
    rttSum += rtt

print(f'Total RTT: {rttSum} ms')
print(f'Average RTT: {rttSum / ITERATIONS} ms')
client_socket.close()
