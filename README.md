# Virtual Memory Manager

## Overview
This project is a simulation of a **Virtual Memory Manager** written in C. It translates 16-bit logical addresses to physical addresses for a virtual address space of 65,536 bytes. The program implements demand paging, utilizing a **Page Table** and a **Translation Lookaside Buffer (TLB)** to optimize address translation.

## Features
* **Address Translation:** Extracts an 8-bit page number and an 8-bit page offset from logical addresses using bitwise operations.
* **Demand Paging:** Handles page faults by reading 256-byte chunks from a simulated hard drive (`BACKING_STORE.bin`) into physical memory.
* **Page Table:** Maintains a 256-entry page table to map pages to physical memory frames.
* **TLB (Translation Lookaside Buffer):** Utilizes a 16-entry hardware cache simulation with a **FIFO (First-In, First-Out)** replacement policy to speed up translations.
* **Statistics Tracking:** Calculates and outputs the overall Page Fault Rate and TLB Hit Rate.

---

## Project Structure
* `group40_manager.c`: The main C source code containing the memory management logic.
* `Makefile`: Automates the compilation process.
* `BACKING_STORE.bin`: A binary file representing the simulated hard drive (65,536 bytes).
* `addresses.txt`: A text file containing randomly generated 32-bit integer logical addresses to test the manager.
* `correct.txt`: The expected output provided for verification.
* `group40_report.pdf`: The final report detailing the design and implementation of the system.

---

## How to Build
A `Makefile` is included to simplify compilation. In your terminal, navigate to the project directory and run:
```
make
```

---

## How to Run
The program reads logical addresses from standard input. You can feed the `addresses.txt` file directly into the executable:

```
./group40_manager < addresses.txt
```

To save the output to a text file for easy comparison against `correct.txt`, use output redirection:
```
./group40_manager < addresses.txt > output.txt
```
