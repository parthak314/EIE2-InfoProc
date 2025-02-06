import socket

HOST = "0.0.0.0" 
PORT = 12000    

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_socket.bind((HOST, PORT))
    server_socket.listen()

    print(f"Server listening on port {PORT}...")

    conn, addr = server_socket.accept()
    print(f"Connected by {addr}")

    while True:
        data = conn.recv(1024)  
        if not data:
            print("Client disconnected.")
            break  
        
        print(f"Received: {data.decode()}")  

        conn.sendall(data)
