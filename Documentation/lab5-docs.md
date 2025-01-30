This lab aims to setup and instantiate an EC2 instance on AWS, communicate with this via SSH clients and run a TCP client and server.


## Table of Contents
1. [Instantiating an EC2 instance](#instantiating-an-ec2-instance)
2. [Communicating with EC2](#communicating-with-ec2)
3. [Running a TCP Server on EC2](#running=a=tcp-server-on-ec2)
4. [Exercises](#exercises)

>[!NOTE]
**Prerequisites**
Install `PuTTY` and create a basic amazon AWS account:
> AWS: [Signup](https://signin.aws.amazon.com/signup?request_type=register) [Login](https://eu-north-1.signin.aws.amazon.com/oauth?client_id=arn:aws:signin:::console/canvas&code_challenge=pqejMN_rdpkGTTYxmZYVXnBvCWbRkA5Z9wwbQcMEex0&code_challenge_method=SHA-256&response_type=code&redirect_uri=https://console.aws.amazon.com/console/home?hashArgs%3D%2523%26isauthcode%3Dtrue%26nc2%3Dh_ct%26oauthStart%3D1738190574779%26src%3Dheader-signin%26state%3DhashArgsFromTB_eu-north-1_aa88296770a4b44d)

---
## Instantiating an EC2 instance
EC2 (Elastic Compute Cloud) is a web service to rent virtual computers to run our own apps on the cloud.

After signing in as the root user, we can select `Services > EC2` and launch the instance from here.
### Machine Specifications
**Name and Machine Image**
As well as the machine name (e.g. `lab5`), we need to specify the Software Image (AMI) which is the Ubuntu Instance.

>[!WARNING]
>As of 29/01/2025, there are two options for free tier available. `24.04` and `20.04`. This is simply a difference in the linux kernel used and won't make a substantial difference to the set up later as long as we use the **64-bit (x86) architecture**  -> This needs to be selected from AMI from catalog. 

**Instance Type**
Sticking to the free tier, we select the `t3` family option of `t3.micro` with `2 vCPU`and `1 GiB Memory`.

There isn't a huge difference between T3 and T2 for the free tier except for a newer Intel Xeon Skylake/AWS Graviton2 vs Intel Xeon (old gen).
The difference between T3 and C5/other versions is that T3 is general-purpose and burstable. Making it best for balanced workloads.

**Create a Key Pair**
For encryption purposes we need a a public and private key pair which is needed when connecting to our server via SSH clients.

Click on Create a New Key Pair then create it with name `<EC2-name>-<DATE>` (e.g.`lab5-29-01-2025`), type RSA, of format `.ppk`.

**Network Settings**
For TCP client-server communication, it is vital to set this up.
Click on Edit on Network Settings. Select `inbound security groups > Type > All TCP` which means that whilst it may be less secure, it allows our simple TCP client to communicate with the server without additional security.

**Storage**
Default values

**Advanced Details**
Create an IAM role for lab6 here. 
We Create a new role with:
- Use Case: EC2
- Permission Policies: AmazonDynamoDBFullAccess
- Name: DynamoDBAccessEC2
This allows us to communicate with the DynamoDB via EC2 instance.
We can now go back to the EC2 configuration and add DynamoDBAccessEC2 to the IAM instance profile.

Now we can launch the instance and see the details by going to `Services > EC2 > Instances`. At this stage, go to the security tab and note the Security group.
Then navigate to `Network & Security > Security Groups`
Select the same security group and go to edit inbound rules.
We can modify the current rules, and set the Source to `Anywhere-IPv4` this means that any address has `0.0.0.0/0`.

>[!NOTE]
>For the project, don't do this and PLEASE set an SSH instance with the security groups with authentication done with python or authorise a specific IP during an instance - since the IP address will change in the future.

---
## Communicating with EC2

The PuTTY system contains several components. These are:
- PuTTY application: technical emulator to run linux commands on the EC2 instance.
- PuTTYgen: convert the `.pem` into a `.ppk` to store private keys.
- PSFTP: secure SSH based file transfer tool for files between local computer and server. This is similar FileZilla.

Since we already downloaded the `.ppk` from our EC2 instance, we can continue with the authentication.
On Putty we enter our EC2 instance IP on port `22`.
Navigating to `Connection > SSH > Auth > Credentials`, and entering our private key (`.ppk`).

We can save the session for future use and  open Putty.
Logging in as `ubuntu`, we can view all the current files with `ls -alF`.
Now we can install python by:
```Shell
sudo add-apt-repository ppa:deadsnakes/ppa 
sudo apt-get update
sudo apt install python3.7
python3
>>> 3+5 # test the python installation
>>> exit() # or Ctrl+D to exit
```
By using `ppa:deadsnakes` we can install previous versions of python and then update the ubuntu instance before installing python.

Install FileZilla now and create a new site via Site manager.
In this new site, we can configure it for the SFTP (SSH File Transfer Protocol). From our online aws instance, we can copy the IPv4 DNS on port 22 and enter the private keyfile with the `ubuntu` user.

A simple python script can be created locally and then transferred to our EC2 machine:
```python
print(3 + 5)
```

Now typing in `python3 simpleAdd.py` allows us to run this file. 

---
## Running a TCP Server on EC2

Using the [tcp-server.py](../lab4/tcp-server.py), we can move it to the EC2 instance and execute this.

This file has an IP address of `0.0.0.0` used in the `bind()` function. This will listen on every available network interface.

This results in the following output on the ubuntu instance:
```
ubuntu@<IP>:~$ python3 tcp-server.py
We're in tcp server...
Server running on port 12000
```

Similarly, we can create the [tcp-client.py](../lab4/tcp-client.py) file.
With our server IP address.

Running it locally, we can see the output:
```Shell
PS C:\Users\Parth\Downloads\EIE2-InfoProc\lab5> python tcp-client.py
We're in tcp client...
Enter a string to test if it is alphanumeric: nope$£"!
Not alphanumeric.
```

This will only run as long as the SSH connection. We can change this by using `systemd` which runs this after the SSH terminates (i.e. in the background) so we need to run our `tcp-server.py` as a service i.e. a daemon.

We need a configuration file first in `/etc/systemd/system` called `tcpservice.service`.
Due to errors, this can be seen below post debugging.

Now we can save this and launch the service by:
```Shell
sudo systemctl daemon-reload 
sudo systemctl enable tcp-server.service 
sudo systemctl start tcp-server.service

ps -ef
```
This reloads the services information, enables our service and starts our service.
This can be confirmed by the final line. (it's a long list so `ps -ef | grep "tcp-server"` will give us the operation in 1 line).

To communicate, we can just run the tcp client locally.
However, this initially resulted in a client socket connection issue.
We can observe any potential issues by running the command below which shows the following program:
```bash
ubuntu@ip-172-31-44-39:~$ sudo systemctl status tcp-server.service
× tcp-server.service - TCP server service
     Loaded: loaded (/etc/systemd/system/tcp-server.service; enabled; preset: enabled)
     Active: failed (Result: exit-code) since Thu 2025-01-30 13:54:14 UTC; 2h 26min ago
   Duration: 6ms
    Process: 5773 ExecStart=/usr/bin/python /home/ubuntu/tcp-server.py (code=exited, status=203/EXEC)
   Main PID: 5773 (code=exited, status=203/EXEC)
        CPU: 3ms

Jan 30 13:54:14 ip-172-31-44-39 systemd[1]: Started tcp-server.service - TCP server service.
Jan 30 13:54:14 ip-172-31-44-39 systemd[1]: tcp-server.service: Main process exited, code=exited, status=203/EXEC
Jan 30 13:54:14 ip-172-31-44-39 systemd[1]: tcp-server.service: Failed with result 'exit-code'.

```
Looking at the status, we can see that there is an `203/EXEC` issue. Referencing the previous program, we can see that the issue is likely to occur with `ExecStart`.
To check the error, we can run several commands below:
```bash
which python
which python3
```
This will tell us the path for the python file. In the case where `python` is invalid, we should see `python3` present.

Running results in an error, but this works with `python3`. Now we have a working system with the following service file.

```
[Unit]
Description=TCP server service 
After=multi-user.target 

[Service] 
Type=simple 
ExecStart=/usr/bin/python3 /home/ubuntu/tcp-server.py 

[Install] 
WantedBy=multi-user.target
```

This is verified by using `ps -ef | grep "tcp-server"` and we are able to see a valid entry here, showing that this is now a service.

To kill this, we can copy the process id via the command above.
THis can be obtained by:
```bash
ps -ef | grep "tcp" | grep -v grep | awk '{print $2}'
```
Which finds the tcp-server file from the current running script, removes any entries containing grep and then prints the second value.

We can then run the following lines which will kill this service:
```bash
sudo kill -9 <PROCESS-ID> # Kill a specific process
sudo pkill python # Kill all python processes
```

---
## Exercises
### Average RTT
To find the average RTT - Time for an integer to travel from the client to the server and back, over 500 communications.

This can be done by:
- Recording the timestamp
- sending an integer from the client to the server
- Receive the same integer back from the server
- Find the RTT (difference in timestamps) and append this value to an array.
- After 500 iterations, find the average.

The client side now runs a for loop for 500 iterations and prints the output:
```python
for i in range(1, ITERATIONS + 1):
    message = "42"
    start_time = time.time()
    client_socket.send(message.encode())
    response = client_socket.recv(1024)
    end_time = time.time()
    rtt = (end_time - start_time) * 1000
    rttSum += rtt
```

Similarly, for the server side, it is much simpler, as we just receive the data and transmit it:
```python
while True:
    connection_socket, caddr = welcome_socket.accept()
    cmsg = connection_socket.recv(1024)
    cmsg = cmsg.decode()
    if not cmsg:
        break
    connection_socket.send(cmsg.encode())
```

The produces the following output:


