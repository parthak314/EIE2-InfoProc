import socket

HOST = "0.0.0.0"  # Listen on all available interfaces
PORT = 12000      # Must match the client's port

# Create TCP server socket
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_socket.bind((HOST, PORT))
    server_socket.listen()

    print(f"Server listening on port {PORT}...")

    conn, addr = server_socket.accept()
    print(f"Connected by {addr}")

    while True:
        data = conn.recv(1024)  # Read data from client
        if not data:
            print("Client disconnected.")
            break  # Exit loop if client closes connection
        
        print(f"Received: {data.decode()}")  # Debugging message

        conn.sendall(data)  # Echo back the data
