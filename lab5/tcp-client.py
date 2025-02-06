import socket
import time

SERVER_IP = "13.60.246.82"  
SERVER_PORT = 12000                 

NUM_ITERATIONS = 500
running_sum = 0  

# Create TCP client socket
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
    client_socket.connect((SERVER_IP, SERVER_PORT)) 
    
    for i in range(1, NUM_ITERATIONS + 1):
        message = "42"  
        
        
        start_time = time.time()
        
        client_socket.sendall(message.encode()) 

        data = client_socket.recv(1024) 
        
        if not data:
            print("Connection lost. Exiting.")
            break  

        end_time = time.time()
        
        rtt = (end_time - start_time) * 1000
        running_sum += rtt
        avg_rtt = running_sum / i

        print(f"Iteration {i}: RTT = {rtt:.3f} ms, Running Average RTT = {avg_rtt:.3f} ms")

    client_socket.close()
