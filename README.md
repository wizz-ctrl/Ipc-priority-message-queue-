# Advanced IPC Priority Message Queue - OS Assignment 2

**Assignment 2 — Operating Systems Fall 2025**  
**Group Assignment - Option B**

## 📋 Overview

This project implements a **safe, educational kernel module** that demonstrates an **Advanced Inter-Process Communication (IPC) mechanism** with priority-based message queuing. The module does not modify the actual Linux kernel and provides a complete, working implementation of:

### ✨ Key Features

- **Dual Priority Queues**: Separate high and normal priority message queues
- **Priority-Based Delivery**: High priority messages (type ≥ 5) delivered first
- **Thread-Safe Operations**: Spinlock protection for concurrent access
- **Dynamic Memory Management**: Efficient kernel memory allocation
- **Proc Filesystem Interface**: Easy user-space interaction via `/proc/safe_lkm`
- **FIFO Ordering**: First-In-First-Out within each priority level

---

## 🎯 What This Module Does

### IPC Priority Message Queue (Option B)
- ✅ Send messages with custom priority levels (0-255)
- ✅ Automatic priority classification (HIGH: type ≥5, NORMAL: type <5)
- ✅ Guaranteed priority-based message retrieval
- ✅ Thread-safe concurrent operations
- ✅ Comprehensive kernel logging
- ✅ Clean resource management

---

## 🛠️ Project Structure

```
os-assignment2/
├── safe_lkm.c           # Main kernel module source
├── Makefile             # Build configuration
├── test_basic.c         # Basic functionality tests
├── test_edge.c          # Edge case tests
├── test_stress.c        # Stress/performance tests
├── compile_tests.sh     # Script to compile tests
├── run_tests.sh         # Script to run all tests
└── README.md            # This file
```

---

## 📦 Prerequisites

### Required Packages
```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install build-essential linux-headers-$(uname -r) gcc make

# Fedora/RHEL
sudo dnf install kernel-devel kernel-headers gcc make
```

### Verify Installation
```bash
# Check kernel headers
ls /lib/modules/$(uname -r)/build

# Check gcc
gcc --version
```

---

## 🚀 Quick Start

### 1. Compile the Kernel Module
```bash
cd /home/taimoor-safdar/os-assignment2
make
```

**Expected output:**
```
make -C /lib/modules/5.x.x-generic/build M=/home/taimoor-safdar/os-assignment2 modules
make[1]: Entering directory '/usr/src/linux-headers-5.x.x-generic'
  CC [M]  /home/taimoor-safdar/os-assignment2/safe_lkm.o
  MODPOST /home/taimoor-safdar/os-assignment2/Module.symvers
  CC [M]  /home/taimoor-safdar/os-assignment2/safe_lkm.mod.o
  LD [M]  /home/taimoor-safdar/os-assignment2/safe_lkm.ko
make[1]: Leaving directory '/usr/src/linux-headers-5.x.x-generic'
```

### 2. Load the Module
```bash
sudo insmod safe_lkm.ko
```

### 3. Verify Module is Loaded
```bash
# Check loaded modules
lsmod | grep safe_lkm

# View kernel logs
dmesg | tail -10

# Check proc entry
ls -l /proc/safe_lkm
cat /proc/safe_lkm
```

---

## 🧪 Testing

### Compile Tests
```bash
chmod +x compile_tests.sh
./compile_tests.sh
```

### Run All Tests
```bash
chmod +x run_tests.sh
sudo ./run_tests.sh
```

### Run Individual Tests
```bash
# Basic functionality
sudo ./test_basic

# Edge cases
sudo ./test_edge

# Stress testing
sudo ./test_stress
```

---

## 📖 Usage Guide

### Command Interface

All commands are sent via the `/proc/safe_lkm` interface:

#### Send Message
```bash
echo "S <pid> <type> <message>" | sudo tee /proc/safe_lkm
```

**Parameters:**
- `pid`: Process ID of sender (any number)
- `type`: Message priority (0-255)
  - **type ≥ 5**: HIGH priority
  - **type < 5**: NORMAL priority
- `message`: Text content (up to 255 characters)

**Examples:**
```bash
# Send normal priority message
echo "S 1001 3 Hello" | sudo tee /proc/safe_lkm

# Send high priority message
echo "S 1002 10 UrgentAlert" | sudo tee /proc/safe_lkm
```

#### Receive Message
```bash
echo "R" | sudo tee /proc/safe_lkm
```

**Behavior:**
- Retrieves **high priority messages first**
- Then retrieves normal priority messages
- FIFO order within each priority level
- Message logged to kernel log (`dmesg`)

#### View Status
```bash
cat /proc/safe_lkm
```

**Shows:**
- Total message count
- High priority message count
- Normal priority message count
- Available commands
- Usage examples

### Complete Example Session
```bash
# Load module
sudo insmod safe_lkm.ko

# Create some tasks
echo "T 2001 3" | sudo tee /proc/safe_lkm
echo "T 2002 7" | sudo tee /proc/safe_lkm

# Send messages
echo "B 2001 4 Hello_Normal" | sudo tee /proc/safe_lkm
echo "B 2002 10 Hello_High" | sudo tee /proc/safe_lkm

# Receive messages (high priority first)
echo "R" | sudo tee /proc/safe_lkm
echo "R" | sudo tee /proc/safe_lkm

# Change priority
echo "A 2001 9" | sudo tee /proc/safe_lkm

# Migrate process
echo "C 2001 3" | sudo tee /proc/safe_lkm

# View status
cat /proc/safe_lkm

# Check kernel logs
dmesg | tail -20

# Unload module
sudo rmmod safe_lkm
```

---

## 🔍 Implementation Details

### Data Structures

#### Task Structure
```c
struct demo_task {
    int pid;                  // Process ID
    int prio;                 // Priority level
    struct list_head list;    // Kernel linked list
};
```

#### Message Structure
```c
struct demo_msg {
    int pid;                  // Sender PID
    int type;                 // Message priority
    char text[256];           // Message content
    struct list_head list;    // Kernel linked list
};
```

### Thread Safety
- Uses spinlocks for concurrent access protection
- `spin_lock_irqsave()` / `spin_unlock_irqrestore()` for interrupt safety
- Separate locks for task list and message queue

### Memory Management
- Dynamic allocation with `kmalloc()` / `kfree()`
- Automatic cleanup on module unload
- Prevents memory leaks through proper resource tracking

---

## 📊 Test Coverage

### Basic Tests (test_basic.c)
1. ✅ Create task
2. ✅ Set priority
3. ✅ Send message
4. ✅ Receive message
5. ✅ Migrate process
6. ✅ Read status

### Edge Case Tests (test_edge.c)
1. ✅ Non-existent PID handling
2. ✅ Invalid command handling
3. ✅ Empty queue operations
4. ✅ Priority boundaries (0, 99, negative)
5. ✅ Message priority threshold (4, 5, 6)
6. ✅ Long message truncation
7. ✅ Rapid operations
8. ✅ Message priority ordering

### Stress Tests (test_stress.c)
1. ✅ 100 task creations
2. ✅ 100 message sends
3. ✅ Bulk message receives
4. ✅ 50 priority changes
5. ✅ 200 mixed operations
6. ✅ 50 concurrent reads

---

## 🐛 Debugging

### View Kernel Logs
```bash
# Real-time logs
sudo dmesg -w

# Last 50 lines
dmesg | tail -50

# Filter module logs
dmesg | grep safe_lkm
```

### Check Module Info
```bash
# Module information
modinfo safe_lkm.ko

# Loaded module details
cat /proc/modules | grep safe_lkm

# System calls
cat /proc/kallsyms | grep safe_lkm
```

### Common Issues

**Problem**: Module won't load
```bash
# Check kernel version compatibility
uname -r
ls /lib/modules/$(uname -r)/build

# Clean and rebuild
make clean
make
```

**Problem**: Permission denied
```bash
# Ensure you're using sudo
sudo insmod safe_lkm.ko
sudo ./run_tests.sh
```

**Problem**: Module already loaded
```bash
# Unload first
sudo rmmod safe_lkm
# Then reload
sudo insmod safe_lkm.ko
```

---

## 🧹 Cleanup

### Unload Module
```bash
sudo rmmod safe_lkm
```

### Clean Build Files
```bash
make clean
rm -f test_basic test_edge test_stress
```

### Verify Cleanup
```bash
lsmod | grep safe_lkm  # Should return nothing
ls /proc/safe_lkm      # Should not exist
```

---

## 📚 Educational Value

### What This Module Teaches

1. **Kernel Module Development**
   - Module initialization and cleanup
   - Proper use of module macros
   - Kernel logging with `printk()`

2. **Process Filesystem Interface**
   - Creating `/proc` entries
   - Implementing read/write operations
   - User-kernel space communication

3. **Synchronization**
   - Spinlocks for critical sections
   - Interrupt-safe locking
   - Race condition prevention

4. **Memory Management**
   - Kernel space allocation
   - Resource cleanup
   - Memory leak prevention

5. **Data Structures**
   - Kernel linked lists
   - List operations (add, delete, iterate)
   - List safety macros

6. **System Programming**
   - Priority scheduling concepts
   - IPC mechanisms
   - Load balancing principles

---

## ⚠️ Safety Notes

This module is **SAFE** because it:
- ✅ Does NOT modify kernel source code
- ✅ Does NOT hook into scheduler functions
- ✅ Does NOT affect real system processes
- ✅ Only simulates OS concepts in isolated data structures
- ✅ Can be loaded/unloaded without system restart
- ✅ Cleans up all resources on unload

**This is a demonstration module for educational purposes only.**

---

## 🎓 Assignment Requirements

### What's Implemented
- ✅ All three options (A, B, C) in one module
- ✅ Kernel module compilation and loading
- ✅ Proper synchronization (spinlocks)
- ✅ Memory management (no leaks)
- ✅ User interface via /proc
- ✅ Comprehensive testing suite
- ✅ Detailed documentation

### Testing Evidence
- ✅ Basic functionality tests
- ✅ Edge case handling
- ✅ Stress/performance tests
- ✅ Kernel log outputs
- ✅ Status reporting

---

## 👥 Group Information

**Assignment**: OS Fall 2025 - Assignment 2  
**Type**: Group Assignment  
**Module**: Safe Kernel Module Demo  
**Date**: November 2025

---

## 📄 License

GPL (GNU General Public License)  
Educational use only.

---

## 🔗 References

- Linux Kernel Module Programming Guide
- Linux Device Drivers, 3rd Edition
- Linux kernel source: `/proc` filesystem
- Linux kernel documentation on synchronization

---

## 📞 Support

For issues or questions:
1. Check `dmesg` for kernel logs
2. Review this README thoroughly
3. Ensure all prerequisites are installed
4. Verify kernel headers match your running kernel

---

**End of Documentation**
