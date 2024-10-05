# _**Assignment Overview**_

This repository contains solutions to Assignment 1 for the course CS250 : Data Structures and Algorithms , Fall 2024 

# **Focus of Assignment : Linked lists ( Singly and Circular ) and Primality Testing for Large Numbers.**
It comprises of two problems given below :  

### Problem 1 : Simple Process Scheduling Algorithm

This problem involves designing and implementing a CPU Process Scheduling Algorithm using a Circular Linked List. Each process is represented as a node in the linked list with attributes like Process_Id, Execution_Time, and Remaining_Time. The algorithm schedules processes in a round-robin manner until all processes complete their execution and the system comes in the empty state.
Task 1 has an optional activity carrying grades (2 marks , not optional by the way ) saying Simulate a scenario where a new process can arrive while the scheduler is running.

### Problem 2 : Very Large Prime Number Calculation

The second problem involves storing a 1024-bit random number in a Singly Linked List , where each node can contain a 64-bit integer. The task is to determine whether this large number is prime, using any efficient method (such as probabilistic primality tests) while considering memory usage.

## _**Approach :**_

### Problem 1: Process Scheduling Algorithm

Circular Linked List: The processes are represented as nodes in a circular linked list. The list is traversed cyclically to simulate CPU time-sharing, with the scheduler reducing the remaining_time of each process in every cycle.
Execution after each cycle , the system outputs the state of the remaining processes , and processes that complete execution are removed from the list. Dynamic Process Insertion , If a new process arrives while the scheduler is running, it is dynamically added to the circular linked list, and the scheduler continues without disruption.


### Problem 2: Large Prime Number Calculation
Singly Linked List : The large 1024-bit number is divided into 64-bit chunks, with each chunk stored in a node of a singly linked list.

Primality Check : The solution implements a probabilistic primality test (such as the Miller-Rabin test), ensuring that the entire number, represented across multiple nodes, is tested for primality without partial checks.


## _**Assumptions :**_ 

Process Scheduling:
The time quantum (CPU Time Per Cycle) is fixed that is 3. All processes initially have unique IDs. A new process can arrive at any cycle, and it will be handled immediately.

Prime Number Calculation:
The 1024-bit number is split into chunks, ensuring that each node of the linked list contains exactly 64 bits. The primality test operates across the full number, and no chunk is tested independently.

## _**Challenges Faced**_

### **Linked List Manipulation:** 
Implementing efficient node insertion and deletion operations in the circular linked list for the process scheduling algorithm posed challenges when handling dynamic insertions.
### _**Handling Large Numbers:**_ 
Since standard data types cannot store 1024-bit numbers, managing these large numbers across multiple nodes in the linked list and performing arithmetic operations on them was complex.

### **Primality Test:**
Implementing a primality test that operates across the entire linked list, while ensuring memory efficiency, required careful handling of large number operations.

## _**Code Structure**_
### _**Process_Manager.cpp :**_ 
Contains the implementation of the process scheduling algorithm using a circular linked list.

### **_Prime_Number_Checking_for_Large_Integers.cpp :_** 
Contains the implementation for storing a 1024-bit number in a singly linked list and performing primality tests on it.

## **_Output Snaps :_**

### **Primality Test:**
![Screenshot (66)](https://github.com/user-attachments/assets/06ea138e-1024-4af2-b58e-c31b9c0f086f)
........................................................................................................................................................................................................................................................................
### **Process Manager:**
![Screenshot (65)](https://github.com/user-attachments/assets/1d23c069-f801-41d1-b066-ff981437892a)

.......................................................................................................................................................................................................................................................................
![Screenshot (64)](https://github.com/user-attachments/assets/556fc8d8-6f12-45c9-8d06-f30699ad2a75)





