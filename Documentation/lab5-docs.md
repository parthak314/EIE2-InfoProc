This lab aims to setup and instantiate an EC2 instance on AWS, communicate with this via SSH clients and run a TCP client and server.


## Table of Contents
1. [Instantiating an EC2 instance](#instantiating-an-ec2-instance)
2. [Communicating with EC2](#communicating-with-ec2)
3. [Running a TCP Server on EC2](#running=a=tcp-server-on-ec2)
4. [Exercises](#exercises)

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

==PAGE 10==

---
## Running a TCP Server on EC2


---
## Exercises
