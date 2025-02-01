import socket
import time

SERVER_IP = "13.60.246.82"  # Replace with EC2 instance IP
SERVER_PORT = 12000                 # Must match the server's port

NUM_ITERATIONS = 500
running_sum = 0  # Sum of all RTTs

# Create TCP client socket
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
    client_socket.connect((SERVER_IP, SERVER_PORT))  # Connect to server
    
    for i in range(1, NUM_ITERATIONS + 1):
        message = "42"  # Sending an integer as a string
        
        # Start time before sending data
        start_time = time.time()
        
        client_socket.sendall(message.encode())  # Send data

        data = client_socket.recv(1024)  # Ensure data is received
        
        if not data:
            print("Connection lost. Exiting.")
            break  # If no data is received, exit loop

        # End time after receiving response
        end_time = time.time()
        
        # Compute RTT
        rtt = (end_time - start_time) * 1000  # Convert to milliseconds
        running_sum += rtt
        
        # Compute running average
        avg_rtt = running_sum / i

        # Print results
        print(f"Iteration {i}: RTT = {rtt:.3f} ms, Running Average RTT = {avg_rtt:.3f} ms")

    client_socket.close()  # Close connection
