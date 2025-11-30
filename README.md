# Advanced IPC Priority Message Queue - Kernel Module# Advanced IPC Priority Message Queue - OS Assignment 2



## Assignment Information**Assignment 2 — Operating Systems Fall 2025**  

**Group Assignment - Option B**

**Course:** Operating Systems - Fall 2025  

**Assignment:** Assignment 2  ## Overview

**Option Implemented:** Option B - Advanced IPC Mechanism (Priority Message Queue)

This project implements a **safe, educational kernel module** that demonstrates an **Advanced Inter-Process Communication (IPC) mechanism** with priority-based message queuing. The module does not modify the actual Linux kernel and provides a complete, working implementation of:

## Project Overview

### Key Features

This project implements a Linux kernel module that provides an Advanced Inter-Process Communication (IPC) mechanism with priority-based message queuing. The module operates as a loadable kernel module (LKM) and does not require modification of the kernel source code.

- **Dual Priority Queues**: Separate high and normal priority message queues

### Implementation Features- **Priority-Based Delivery**: High priority messages (type ≥ 5) delivered first

- **Thread-Safe Operations**: Spinlock protection for concurrent access

- Dual-priority message queuing system (HIGH and NORMAL priority levels)- **Dynamic Memory Management**: Efficient kernel memory allocation

- Non-blocking send and receive operations- **Proc Filesystem Interface**: Easy user-space interaction via `/proc/safe_lkm`

- Thread-safe operations using spinlocks with interrupt safety- **FIFO Ordering**: First-In-First-Out within each priority level

- Dynamic memory management using kernel memory allocator

- User-space interface via /proc filesystem---

- FIFO ordering within each priority level

- Automatic priority classification based on message type## What This Module Does



### Priority Classification### IPC Priority Message Queue (Option B)

- Send messages with custom priority levels (0-255)

- Messages with type >= 5: HIGH priority queue- Automatic priority classification (HIGH: type ≥5, NORMAL: type <5)

- Messages with type < 5: NORMAL priority queue- Guaranteed priority-based message retrieval

- High priority messages are always delivered before normal priority messages- Thread-safe concurrent operations

- Messages within the same priority level follow FIFO order- Comprehensive kernel logging

- Clean resource management

## Prerequisites

---

### Required Software

## Project Structure

- Linux operating system (tested on Ubuntu 24.04 LTS)

- Kernel headers matching your running kernel version```

- GCC compileros-assignment2/

- GNU Make├── safe_lkm.c           # Main kernel module source

├── Makefile             # Build configuration

### Installation of Prerequisites├── test_basic.c         # Basic functionality tests

├── test_edge.c          # Edge case tests

For Ubuntu/Debian-based systems:├── test_stress.c        # Stress/performance tests

```bash├── compile_tests.sh     # Script to compile tests

sudo apt-get update├── run_tests.sh         # Script to run all tests

sudo apt-get install build-essential linux-headers-$(uname -r)└── README.md            # This file

``````



For Fedora/RHEL-based systems:---

```bash

sudo dnf install kernel-devel kernel-headers gcc make## Prerequisites

```

### Required Packages

### Verification```bash

# Ubuntu/Debian

Verify that kernel headers are installed:sudo apt-get update

```bashsudo apt-get install build-essential linux-headers-$(uname -r) gcc make

ls /lib/modules/$(uname -r)/build

```# Fedora/RHEL

sudo dnf install kernel-devel kernel-headers gcc make

Verify GCC installation:```

```bash

gcc --version### Verify Installation

``````bash

# Check kernel headers

## Project Structurels /lib/modules/$(uname -r)/build



```# Check gcc

os-assignment2/gcc --version

├── safe_lkm.c           # Kernel module source code```

├── Makefile             # Build configuration

├── test_basic.c         # Basic functionality test program---

├── test_stress.c        # Stress test program (100 messages)

├── test_edge.c          # Edge case test program## Quick Start

├── README.md            # This file

└── OPTION_B_ONLY_CONFIRMED.md  # Implementation verification### 1. Compile the Kernel Module

``````bash

cd /home/taimoor-safdar/os-assignment2

## Building the Projectmake

```

### 1. Compile the Kernel Module

**Expected output:**

```bash```

makemake -C /lib/modules/5.x.x-generic/build M=/home/taimoor-safdar/os-assignment2 modules

```make[1]: Entering directory '/usr/src/linux-headers-5.x.x-generic'

  CC [M]  /home/taimoor-safdar/os-assignment2/safe_lkm.o

Expected output:  MODPOST /home/taimoor-safdar/os-assignment2/Module.symvers

```  CC [M]  /home/taimoor-safdar/os-assignment2/safe_lkm.mod.o

make -C /lib/modules/6.14.0-35-generic/build M=/path/to/os-assignment2 modules  LD [M]  /home/taimoor-safdar/os-assignment2/safe_lkm.ko

  CC [M]  /path/to/os-assignment2/safe_lkm.omake[1]: Leaving directory '/usr/src/linux-headers-5.x.x-generic'

  MODPOST /path/to/os-assignment2/Module.symvers```

  CC [M]  /path/to/os-assignment2/safe_lkm.mod.o

  LD [M]  /path/to/os-assignment2/safe_lkm.ko### 2. Load the Module

``````bash

sudo insmod safe_lkm.ko

This creates `safe_lkm.ko`, the compiled kernel module.```



### 2. Compile Test Programs### 3. Verify Module is Loaded

```bash

```bash# Check loaded modules

gcc -o test_basic test_basic.clsmod | grep safe_lkm

gcc -o test_stress test_stress.c

gcc -o test_edge test_edge.c# View kernel logs

```dmesg | tail -10



Alternatively, use the provided script:# Check proc entry

```bashls -l /proc/safe_lkm

./compile_tests.shcat /proc/safe_lkm

``````



## Running Instructions---



### Step 1: Load the Kernel Module##  Testing



```bash### Compile Tests

sudo insmod safe_lkm.ko```bash

```chmod +x compile_tests.sh

./compile_tests.sh

### Step 2: Verify Module is Loaded```



Check that the module is loaded:### Run All Tests

```bash```bash

lsmod | grep safe_lkmchmod +x run_tests.sh

```sudo ./run_tests.sh

```

Verify the proc interface was created:

```bash### Run Individual Tests

ls -l /proc/safe_lkm```bash

```# Basic functionality

sudo ./test_basic

Check kernel logs:

```bash# Edge cases

sudo dmesg | grep safe_lkm | tail -10sudo ./test_edge

```

# Stress testing

Expected output in dmesg:sudo ./test_stress

``````

[12345.678] [safe_lkm] IPC Priority Message Queue loaded

[12345.679] [safe_lkm] Use: echo 'S <pid> <type> <msg>' > /proc/safe_lkm---

[12345.680] [safe_lkm] Use: echo 'R' > /proc/safe_lkm

```## 📖 Usage Guide



### Step 3: Run Test Programs### Command Interface



Execute the three test programs to verify functionality:All commands are sent via the `/proc/safe_lkm` interface:



#### Test 1: Basic Functionality Test#### Send Message

```bash```bash

sudo ./test_basicecho "S <pid> <type> <message>" | sudo tee /proc/safe_lkm

``````



This test performs 6 basic IPC operations:**Parameters:**

- Send normal priority message- `pid`: Process ID of sender (any number)

- Send high priority message- `type`: Message priority (0-255)

- Verify priority ordering  - **type ≥ 5**: HIGH priority

- Handle empty queue  - **type < 5**: NORMAL priority

- Multiple messages with mixed priorities- `message`: Text content (up to 255 characters)

- Read queue status

**Examples:**

#### Test 2: Stress Test```bash

```bash# Send normal priority message

sudo ./test_stressecho "S 1001 3 Hello" | sudo tee /proc/safe_lkm

```

# Send high priority message

This test performs high-load operations:echo "S 1002 10 UrgentAlert" | sudo tee /proc/safe_lkm

- Rapidly send 100 messages```

- Rapidly receive messages

- Verify priority ordering under stress#### Receive Message

- Test concurrent send/receive operations```bash

- Test queue capacityecho "R" | sudo tee /proc/safe_lkm

```

#### Test 3: Edge Case Test

```bash**Behavior:**

sudo ./test_edge- Retrieves **high priority messages first**

```- Then retrieves normal priority messages

- FIFO order within each priority level

This test handles error conditions:- Message logged to kernel log (`dmesg`)

- Empty queue handling

- Invalid commands#### View Status

- Priority boundary conditions```bash

- Extreme priority valuescat /proc/safe_lkm

- Long message truncation```

- Special characters

- System stability under rapid operations**Shows:**

- Total message count

### Step 4: View Results- High priority message count

- Normal priority message count

Check kernel logs to see IPC operations:- Available commands

```bash- Usage examples

sudo dmesg | grep safe_lkm | tail -30

```### Complete Example Session

```bash

View current queue status:# Load module

```bashsudo insmod safe_lkm.ko

sudo cat /proc/safe_lkm

```# Create some tasks

echo "T 2001 3" | sudo tee /proc/safe_lkm

### Step 5: Unload the Moduleecho "T 2002 7" | sudo tee /proc/safe_lkm



After testing, unload the module:# Send messages

```bashecho "B 2001 4 Hello_Normal" | sudo tee /proc/safe_lkm

sudo rmmod safe_lkmecho "B 2002 10 Hello_High" | sudo tee /proc/safe_lkm

```

# Receive messages (high priority first)

Verify clean unload:echo "R" | sudo tee /proc/safe_lkm

```bashecho "R" | sudo tee /proc/safe_lkm

sudo dmesg | grep safe_lkm | tail -5

```# Change priority

echo "A 2001 9" | sudo tee /proc/safe_lkm

Expected output:

```# Migrate process

[12400.500] [safe_lkm] Cleaned up X messagesecho "C 2001 3" | sudo tee /proc/safe_lkm

[12400.501] [safe_lkm] IPC Priority Message Queue unloaded

```# View status

cat /proc/safe_lkm

## Usage Examples

# Check kernel logs

### Manual IPC Operationsdmesg | tail -20



#### Send Messages# Unload module

sudo rmmod safe_lkm

Send a normal priority message (type < 5):```

```bash

echo "S 1001 3 Hello_Normal" | sudo tee /proc/safe_lkm---

```

##  Implementation Details

Send a high priority message (type >= 5):

```bash### Data Structures

echo "S 1002 10 Urgent_Message" | sudo tee /proc/safe_lkm

```#### Task Structure

```c

#### Receive Messagesstruct demo_task {

    int pid;                  // Process ID

Receive the next message (high priority delivered first):    int prio;                 // Priority level

```bash    struct list_head list;    // Kernel linked list

echo "R" | sudo tee /proc/safe_lkm};

``````



#### Check Queue Status#### Message Structure

```c

```bashstruct demo_msg {

sudo cat /proc/safe_lkm    int pid;                  // Sender PID

```    int type;                 // Message priority

    char text[256];           // Message content

Output format:    struct list_head list;    // Kernel linked list

```};

=== IPC Priority Message Queue - Assignment 2 (Option B) ===```



Current Status:### Thread Safety

  Total messages: 5- Uses spinlocks for concurrent access protection

  High priority (type >= 5): 2 messages- `spin_lock_irqsave()` / `spin_unlock_irqrestore()` for interrupt safety

  Normal priority (type < 5): 3 messages- Separate locks for task list and message queue



Available Commands (write to this file):### Memory Management

  S <pid> <type> <message> - Send message- Dynamic allocation with `kmalloc()` / `kfree()`

  R                        - Receive message- Automatic cleanup on module unload

```- Prevents memory leaks through proper resource tracking



## Command Reference---



### Module Commands##  Test Coverage



| Command | Format | Description | Example |### Basic Tests (test_basic.c)

|---------|--------|-------------|---------|1. Create task

| Send | S <pid> <type> <message> | Send an IPC message | S 1001 10 HelloWorld |2. Set priority

| Receive | R | Receive next message | R |3. Send message

4. Receive message

### System Commands5. Migrate process

6. Read status

| Command | Description |

|---------|-------------|### Edge Case Tests (test_edge.c)

| `sudo insmod safe_lkm.ko` | Load the kernel module |1. Non-existent PID handling

| `sudo rmmod safe_lkm` | Unload the kernel module |2. Invalid command handling

| `lsmod | grep safe_lkm` | Check if module is loaded |3. Empty queue operations

| `sudo dmesg | grep safe_lkm` | View kernel logs |4. Priority boundaries (0, 99, negative)

| `cat /proc/safe_lkm` | Read queue status |5. Message priority threshold (4, 5, 6)

| `echo "command" > /proc/safe_lkm` | Send command to module |6. Long message truncation

7. Rapid operations

## Technical Implementation Details8. Message priority ordering



### Data Structures### Stress Tests (test_stress.c)

1. 100 task creations

The module uses the following kernel data structures:2. 100 message sends

3. Bulk message receives

- **struct demo_msg**: Message structure containing PID, type, text, and list node4. 50 priority changes

- **struct demo_msg_queue**: Queue structure with separate high and normal priority lists5. 200 mixed operations

- **spinlock_t**: For thread-safe operations6. 50 concurrent reads



### Kernel APIs Used---



- `kmalloc()`/`kfree()`: Dynamic memory allocation##  Debugging

- `spin_lock_irqsave()`/`spin_unlock_irqrestore()`: Thread synchronization

- `list_add_tail()`/`list_del()`: Linked list operations### View Kernel Logs

- `proc_create()`/`proc_remove()`: /proc filesystem interface```bash

- `copy_from_user()`/`copy_to_user()`: User-kernel data transfer# Real-time logs

sudo dmesg -w

### Memory Management

# Last 50 lines

- All messages are dynamically allocated using `kmalloc(GFP_KERNEL)`dmesg | tail -50

- Proper cleanup on module unload frees all allocated memory

- No memory leaks under normal or abnormal termination# Filter module logs

dmesg | grep safe_lkm

### Thread Safety```



- All queue operations are protected by spinlocks### Check Module Info

- Interrupt-safe spinlock operations (`spin_lock_irqsave`)```bash

- Safe for concurrent access from multiple processes# Module information

modinfo safe_lkm.ko

## Testing Summary

# Loaded module details

The implementation includes three comprehensive test programs:cat /proc/modules | grep safe_lkm



### Test Program 1: Basic Functionality (test_basic.c)# System calls

- **Purpose**: Verify normal operationcat /proc/kallsyms | grep safe_lkm

- **Tests**: 6 tests covering send, receive, priority ordering```

- **Expected Result**: All 6 tests pass

### Common Issues

### Test Program 2: Stress Test (test_stress.c)

- **Purpose**: Test under heavy load**Problem**: Module won't load

- **Tests**: 5 tests with 100+ messages```bash

- **Expected Result**: All 5 tests pass, no crashes or memory leaks# Check kernel version compatibility

uname -r

### Test Program 3: Edge Cases (test_edge.c)ls /lib/modules/$(uname -r)/build

- **Purpose**: Test error conditions and boundaries

- **Tests**: 9 tests covering invalid input, empty queues, extreme values# Clean and rebuild

- **Expected Result**: All 9 tests pass, graceful error handlingmake clean

make

## Troubleshooting```



### Module fails to load**Problem**: Permission denied

```bash

**Error**: `insmod: ERROR: could not insert module`# Ensure you're using sudo

sudo insmod safe_lkm.ko

**Solution**: Check if kernel headers match your running kernelsudo ./run_tests.sh

```bash```

uname -r

ls /lib/modules/$(uname -r)/build**Problem**: Module already loaded

``````bash

# Unload first

### Permission deniedsudo rmmod safe_lkm

# Then reload

**Error**: `Permission denied` when loading modulesudo insmod safe_lkm.ko

```

**Solution**: Use `sudo` for all module operations

```bash---

sudo insmod safe_lkm.ko

```##  Cleanup



### Module already loaded### Unload Module

```bash

**Error**: `File exists` when loading modulesudo rmmod safe_lkm

```

**Solution**: Unload existing module first

```bash### Clean Build Files

sudo rmmod safe_lkm```bash

sudo insmod safe_lkm.komake clean

```rm -f test_basic test_edge test_stress

```

### Compilation errors

### Verify Cleanup

**Error**: `fatal error: linux/module.h: No such file or directory````bash

lsmod | grep safe_lkm  # Should return nothing

**Solution**: Install kernel headersls /proc/safe_lkm      # Should not exist

```bash```

sudo apt-get install linux-headers-$(uname -r)

```---



## Cleaning Up##  Educational Value



To remove all compiled files:### What This Module Teaches

```bash

make clean1. **Kernel Module Development**

```   - Module initialization and cleanup

   - Proper use of module macros

To remove compiled test programs:   - Kernel logging with `printk()`

```bash

rm -f test_basic test_stress test_edge2. **Process Filesystem Interface**

```   - Creating `/proc` entries

   - Implementing read/write operations

## Safety and Limitations   - User-kernel space communication



### Safety Features3. **Synchronization**

   - Spinlocks for critical sections

- Does not modify kernel source code   - Interrupt-safe locking

- Does not affect real system processes   - Race condition prevention

- Can be safely unloaded at any time

- Automatic resource cleanup on unload4. **Memory Management**

- No system instability   - Kernel space allocation

   - Resource cleanup

### Limitations   - Memory leak prevention



- Maximum message size: 255 characters5. **Data Structures**

- Queue size limited only by available kernel memory   - Kernel linked lists

- Module must be loaded with root privileges   - List operations (add, delete, iterate)

- /proc interface requires read/write permissions   - List safety macros



## Assignment Compliance6. **System Programming**

   - Priority scheduling concepts

This implementation satisfies all requirements for Option B:   - IPC mechanisms

   - Load balancing principles

- [x] Implements advanced IPC mechanism

- [x] Priority-based message queue---

- [x] Non-blocking send/receive operations

- [x] Three test programs (basic, stress, edge cases)##  Safety Notes

- [x] Clear test output showing pass/fail

- [x] Complete documentationThis module is **SAFE** because it:

- [x] Proper error handling- Does NOT modify kernel source code

- [x] Thread-safe implementation- Does NOT hook into scheduler functions

- Does NOT affect real system processes

## Author Information- Only simulates OS concepts in isolated data structures

- Can be loaded/unloaded without system restart

**Course**: Operating Systems - Fall 2025  - Cleans up all resources on unload

**Institution**: SEECS, NUST  

**Date**: November 30, 2025**This is a demonstration module for educational purposes only.**



## Additional Documentation---



For more detailed technical information, see:##  Assignment Requirements

- `OPTION_B_ONLY_CONFIRMED.md` - Implementation verification

- `VERIFICATION_SUMMARY.md` - Detailed analysis### What's Implemented

- Kernel logs via `dmesg` command- All three options (A, B, C) in one module

- Kernel module compilation and loading

## License- Proper synchronization (spinlocks)

- Memory management (no leaks)

This code is provided for educational purposes as part of an academic assignment.- User interface via /proc

- Comprehensive testing suite
- Detailed documentation

### Testing Evidence
- Basic functionality tests
- Edge case handling
- Stress/performance tests
- Kernel log outputs
- Status reporting

---



---

## License

GPL (GNU General Public License)  
Educational use only.

---

## References

- Linux Kernel Module Programming Guide
- Linux Device Drivers, 3rd Edition
- Linux kernel source: `/proc` filesystem
- Linux kernel documentation on synchronization

---

##  Support

For issues or questions:
1. Check `dmesg` for kernel logs
2. Review this README thoroughly
3. Ensure all prerequisites are installed
4. Verify kernel headers match your running kernel

---

**End of Documentation**
