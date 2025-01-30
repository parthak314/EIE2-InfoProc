import socket
print("We're in tcp server...")

server_port = 12000

welcome_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
welcome_socket.bind(('0.0.0.0', server_port))
welcome_socket.listen(1)

print('Server running on port', server_port)

while True:
    connection_socket, caddr = welcome_socket.accept()
    
    cmsg = connection_socket.recv(1024)
    cmsg = cmsg.decode()

    if not cmsg:
        break
    connection_socket.send(cmsg.encode())


