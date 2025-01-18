# Lab 2 documentation
This lab focuses on the process of designing and deploying a **NIOS II system**, culminating in the creation of a CLI-based program that runs on the Max10 FPGA chip, which is integrated into the DE10-Lite board. 
This involves the design, programming, and testing of an embedded system, leveraging the capabilities of the NIOS II system.
Also see the [Hello_World_Lab_Manual](./Hello_World_Lab_Manual_DE10_Lite.pdf)

## Table of Contents
1. Subdirectory structure
2. Background
3. Task 1: Designing an NIOS II System
4. Task 2: Programming an NIOS II System
5. Task 3: Extension & Challenges
6. Troubleshooting and issues encountered
---

## ==Subdirectory Structure==


---
## Background
In this lab, we are using the 32-bit NIOS II soft-core processor which is implemented in the FPGA fabric.

Soft-core processors have customisable architecture, as opposed to fixed architecture in hard-core processors. The processor architecture contains Register files, ALU, iCache and dCache, Instruction decoding and Instruction bus and data bus.
This is similar to the Harvard architecture of a processor with Instruction and Data kept separate. 
![NiosII-Processor.png](./images/NiosII-Processor.png)

The blue boxes show the essential components:
- Instruction-fetch-decode logic
- Registers (32 32-bit registers)
- Exception Logic
- ALU
This is a typical RISC architecture
The grey boxes show the optional elements to enhance capability:
- Shadow Registers: Reduce overhead when switching between functions by moving 32 registers from main to here as opposed to using a stack (for interrupts).
- dCache and iCache: Especially useful if Nios II is located off-chip. Also cache-bypass methods for specific instructions.
- Memory Management Unit: Can be added to implement virtual memory and memory protection making it secure for OS hosting. 
- Tightly Coupled memory: memory bandwidth often limits performance, but with TCM, there is no data/memory bottleneck since the memory is directly handled by the programmer.
- Custom Instructions: Saves time and enhances performance.

This means that it has a portable design, capable of running on any FPGA that can synthesize the NIOS II Verilog. However, this is configured as bitstream, as opposed to running on silicon resulting in less performance and lower energy efficiency 
[Introduction to NIOS II by Cornell](https://people.ece.cornell.edu/land/courses/ece5760/DE2/tut_nios2_introduction.pdf)

---
## Task 1: Designing an NIOS II System
**Tldr**: instantiate a soft processor (NIOS II) on the FPGA, establish a communication with the host-PC and display a message on the terminal.

Since the Nios II processor is supported by a lot of peripherals and 'IP' blocks that can be configured and connected, we can use the QSys tool to make this easier.
The complete design of the system is to use:
- QSys: Select an IP to make connections and generate a system
- Quartus: FPGA Design with HDL files (like Lab 1)
	- Editing hardware spec, assigning Pin-out, timing constraints, compilation and downloading
- NIOS II: Software Development when building, debugging and running applications.
- Target Hardware (FPGA device): Download and run the application built on NIOS II with the hardware spec defined by the Quartus HDL files.

==Continue from Page 11==



---
## Task 2: Programming an NIOS II System

---
## Task 3: Extension and Challenges

---
## Troubleshooting and Issues encountered