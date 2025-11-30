# Operating Systems - Assignment 2 Report

**Course:** Operating Systems - Fall 2025  
**Institution:** SEECS, NUST  
**Assignment:** Assignment 2  
**Option Implemented:** Option B - Advanced IPC Mechanism (Priority Message Queue)  
**Date:** November 30, 2025

---

## Table of Contents

1. [Setup Guide](#1-setup-guide)
2. [Implementation Summary](#2-implementation-summary)
3. [Challenges and Solutions](#3-challenges-and-solutions)

---

## 1. Setup Guide

### 1.1 Development Environment

#### Operating System and Kernel

The implementation was developed and tested on the following platform:

- **Operating System:** Ubuntu 24.04 LTS (Long Term Support)
- **Kernel Version:** Linux 6.14.0-35-generic
- **Architecture:** x86_64 (64-bit)

Kernel version verification:
```bash
uname -r
# Output: 6.14.0-35-generic
```

#### Tools and Utilities Used

The following development tools were utilized during implementation:

| Tool | Version | Purpose |
|------|---------|---------|
| GCC | 13.3.0 | C compiler for kernel module and test programs |
| GNU Make | 4.3 | Build system for kernel module compilation |
| Kernel Headers | 6.14.0-35-generic | Required for kernel module compilation |
| Git | 2.43.0 | Version control and source code management |
| Visual Studio Code | Latest | Source code editor and development environment |

#### Required Dependencies

Before building the project, the following packages must be installed on Ubuntu/Debian-based systems:

```bash
# Update package database
sudo apt-get update

# Install build essentials
sudo apt-get install build-essential

# Install kernel headers matching your running kernel
sudo apt-get install linux-headers-$(uname -r)
```

Verification of installed dependencies:

```bash
# Verify GCC installation
gcc --version
# Expected: gcc (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0

# Verify kernel headers
ls /lib/modules/$(uname -r)/build
# Should show kernel build directory
```

### 1.2 How to Compile the System

#### Compiling the Kernel Module

The kernel module uses the standard Linux kernel build system (kbuild). Navigate to the project directory and run:

```bash
cd /path/to/os-assignment2
make
```

**Expected Output:**
```
make -C /lib/modules/6.14.0-35-generic/build M=/home/user/os-assignment2 modules
  CC [M]  /home/user/os-assignment2/safe_lkm.o
  MODPOST /home/user/os-assignment2/Module.symvers
  CC [M]  /home/user/os-assignment2/safe_lkm.mod.o
  LD [M]  /home/user/os-assignment2/safe_lkm.ko
```

This generates **safe_lkm.ko**, the compiled loadable kernel module.

#### Compiling Test Programs

Three test programs need to be compiled using GCC:

```bash
# Compile each test program individually
gcc -o test_basic test_basic.c
gcc -o test_stress test_stress.c
gcc -o test_edge test_edge.c
```

Alternatively, use the provided compilation script:

```bash
chmod +x compile_tests.sh
./compile_tests.sh
```

**Verification:**
```bash
ls -lh test_basic test_stress test_edge
# Should show three executable files
```

### 1.3 How to Run the System

#### Step 1: Load the Kernel Module

```bash
# Load the module into the kernel
sudo insmod safe_lkm.ko

# Verify the module is loaded
lsmod | grep safe_lkm
```

**Expected Output:**
```
safe_lkm               16384  0
```

Check kernel logs to confirm successful loading:

```bash
sudo dmesg | grep safe_lkm | tail -10
```

**Expected Log Output:**
```
[12345.678] [safe_lkm] IPC Priority Message Queue loaded
[12345.679] [safe_lkm] Use: echo 'S <pid> <type> <msg>' > /proc/safe_lkm
[12345.680] [safe_lkm] Use: echo 'R' > /proc/safe_lkm
```

Verify the /proc interface was created:

```bash
ls -l /proc/safe_lkm
```

#### Step 2: Run Test Programs

Execute all three test programs in sequence:

**Test 1: Basic Functionality**
```bash
sudo ./test_basic
```

This test validates:
- Send normal priority messages
- Send high priority messages
- Verify priority-based delivery order
- Handle empty queue conditions
- Multiple message handling
- Queue status reading

**Test 2: Stress Testing**
```bash
sudo ./test_stress
```

This test validates system behavior under load:
- Rapid send operations (100+ messages)
- Rapid receive operations
- Priority ordering under stress
- Concurrent send/receive operations
- Queue capacity handling

**Test 3: Edge Cases**
```bash
sudo ./test_edge
```

This test validates error handling:
- Empty queue operations
- Invalid command handling
- Priority boundary conditions
- Extreme priority values
- Message truncation handling
- Special character handling
- System stability

#### Step 3: Manual Testing (Optional)

You can manually interact with the IPC mechanism:

**Send Messages:**
```bash
# Send normal priority message (type < 5)
echo "S 1001 3 HelloNormal" | sudo tee /proc/safe_lkm

# Send high priority message (type >= 5)
echo "S 1002 10 UrgentMessage" | sudo tee /proc/safe_lkm
```

**Receive Messages:**
```bash
echo "R" | sudo tee /proc/safe_lkm
```

**Check Queue Status:**
```bash
sudo cat /proc/safe_lkm
```

#### Step 4: Unload the Module

After testing, unload the kernel module:

```bash
# Unload the module
sudo rmmod safe_lkm

# Verify clean removal
sudo dmesg | grep safe_lkm | tail -5
```

**Expected Output:**
```
[12400.500] [safe_lkm] Cleaned up X messages
[12400.501] [safe_lkm] IPC Priority Message Queue unloaded
```

---

## 2. Implementation Summary

### 2.1 Feature Selection

For this assignment, I implemented **Option B: Advanced Inter-Process Communication (IPC) Mechanism** with a priority-based message queue system.

**Rationale for Selection:**

This option was chosen because it provides comprehensive learning opportunities in:

1. **Kernel Data Structure Design:** Designing and implementing efficient queue structures in kernel space
2. **Thread Synchronization:** Using spinlocks and interrupt-safe locking mechanisms
3. **User-Kernel Communication:** Implementing /proc filesystem interface for user space interaction
4. **Memory Management:** Dynamic memory allocation and deallocation in kernel context
5. **Real-World Applicability:** Priority-based IPC is fundamental to many operating system services

### 2.2 High-Level Architecture

#### System Design Overview

The implementation consists of a loadable kernel module (LKM) that provides an IPC mechanism through the /proc filesystem interface. The core design uses a dual-priority queue system:

**Priority Classification:**
- **High Priority Queue:** Messages with type value >= 5
- **Normal Priority Queue:** Messages with type value < 5

**Message Delivery Policy:**
- High priority messages are always delivered before normal priority messages
- Within each priority level, messages follow FIFO (First-In-First-Out) ordering
- Receive operations are non-blocking (return immediately if queue is empty)

#### Key Components

**1. Data Structures**

Two primary structures form the foundation of the implementation:

```c
struct demo_msg {
    pid_t pid;              // Process ID of sender
    int type;               // Message priority type
    char text[256];         // Message content
    struct list_head list;  // Kernel linked list node
};

struct demo_msg_queue {
    struct list_head high_priority;   // High priority list head
    struct list_head normal_priority; // Normal priority list head
    spinlock_t lock;                  // Synchronization lock
};
```

**2. Core Operations**

The module implements four main operations:

| Function | Purpose | Key Behavior |
|----------|---------|--------------|
| demo_send_msg() | Add message to queue | Classifies by priority, adds to appropriate queue |
| demo_receive_msg() | Retrieve message | Checks high priority first, then normal priority |
| proc_write() | Handle user commands | Parses S (send) and R (receive) commands |
| proc_read() | Display queue status | Shows message counts and available commands |

**3. User Interface**

The module creates /proc/safe_lkm as the communication interface:

```bash
# Command Format
S <pid> <type> <message>  # Send message
R                          # Receive message
```

### 2.3 Changes Made (Detailed)

#### File: safe_lkm.c (359 lines)

**Module Initialization (module_init):**
- Initialize dual priority queues using INIT_LIST_HEAD()
- Initialize spinlock for thread synchronization
- Create /proc/safe_lkm interface using proc_create()
- Register proc_ops structure with read/write callbacks

**Message Send Operation:**
```c
static int demo_send_msg(pid_t pid, int type, const char *text) {
    // Allocate message structure
    struct demo_msg *msg = kmalloc(sizeof(*msg), GFP_KERNEL);
    
    // Initialize message fields
    msg->pid = pid;
    msg->type = type;
    strncpy(msg->text, text, 255);
    
    // Thread-safe queue insertion
    spin_lock_irqsave(&msg_queue.lock, flags);
    
    // Priority-based classification
    if (msg->type >= 5) {
        list_add_tail(&msg->list, &msg_queue.high_priority);
    } else {
        list_add_tail(&msg->list, &msg_queue.normal_priority);
    }
    
    spin_unlock_irqrestore(&msg_queue.lock, flags);
}
```

**Message Receive Operation:**
```c
static int demo_receive_msg(struct demo_msg *out_msg) {
    spin_lock_irqsave(&msg_queue.lock, flags);
    
    // Check high priority queue first
    if (!list_empty(&msg_queue.high_priority)) {
        node = msg_queue.high_priority.next;
    } else if (!list_empty(&msg_queue.normal_priority)) {
        node = msg_queue.normal_priority.next;
    }
    
    if (node) {
        list_del(node);
        // Copy message data
        memcpy(out_msg, msg, sizeof(*out_msg));
        kfree(msg);
        result = 0;
    }
    
    spin_unlock_irqrestore(&msg_queue.lock, flags);
}
```

**Proc Interface Write Handler:**
- Parse user commands using sscanf()
- Validate input format
- Route to appropriate operation (send/receive)
- Return error codes for invalid input

**Proc Interface Read Handler:**
- Count messages in both queues
- Format status output
- Display usage instructions

**Module Cleanup (module_exit):**
- Remove /proc interface
- Iterate through both queues
- Free all pending messages
- Log cleanup statistics

#### Test Programs (3 files)

**test_basic.c (6 tests):**
1. Send normal priority message and verify
2. Send high priority message and verify
3. Verify high priority delivered before normal
4. Test empty queue behavior
5. Test multiple messages with mixed priorities
6. Read and verify queue status

**test_stress.c (5 tests):**
1. Rapid send of 100 messages
2. Rapid receive of all messages
3. Verify priority ordering under load
4. Concurrent send/receive operations
5. Queue capacity testing

**test_edge.c (9 tests):**
1. Receive from empty queue
2. Invalid command handling
3. Priority boundary testing (type 4 vs 5)
4. Extreme priority values (negative, very large)
5. Message truncation (>255 characters)
6. Special characters in message
7. Malformed commands
8. Rapid operations
9. System stability verification

### 2.4 Key Implementation Decisions

**Decision 1: Dual Queue vs Single Sorted List**

Chose dual separate queues over a single sorted list.

**Reason:** Provides O(1) insertion and O(1) retrieval of highest priority message, compared to O(n) insertion in a sorted list. This design scales better under load.

**Decision 2: Non-Blocking Receive**

Implemented non-blocking receive operations that return immediately if no messages are available.

**Reason:** Simpler implementation, avoids kernel waiting/sleeping mechanisms, suitable for testing and educational purposes.

**Decision 3: /proc Interface**

Used /proc filesystem instead of system calls or netlink sockets.

**Reason:** Simpler implementation, easier testing with standard tools (echo, cat), appropriate for educational kernel module.

**Decision 4: Spinlocks with IRQ Safety**

Used spin_lock_irqsave() instead of mutexes or semaphores.

**Reason:** Spinlocks are appropriate for short critical sections, IRQ-safe variants prevent deadlocks from interrupt context.

---

## 3. Challenges and Solutions

### Challenge 1: Race Conditions and Thread Safety

**Problem Description:**

During initial testing, the module experienced crashes when multiple test programs ran simultaneously. The issue was caused by concurrent access to the message queues without proper synchronization.

**Error Symptoms:**
- Kernel oops messages in dmesg
- Corrupted linked lists
- System freezes during stress tests

**Root Cause:**

Multiple processes writing to /proc/safe_lkm simultaneously caused race conditions. The linked list operations (list_add_tail, list_del) are not atomic and were being interrupted mid-operation.

**Solution Implemented:**

Implemented spinlock protection with interrupt-safe operations:

```c
unsigned long flags;
spin_lock_irqsave(&msg_queue.lock, flags);
// Critical section: queue operations
spin_unlock_irqrestore(&msg_queue.lock, flags);
```

**Why This Works:**
- spin_lock_irqsave() disables interrupts on local CPU and acquires lock
- Prevents both process-level and interrupt-level race conditions
- Ensures atomic queue operations

**Verification:**

Ran stress test with 5 concurrent instances successfully without crashes.

### Challenge 2: Input Parsing and Validation

**Problem Description:**

User input from /proc interface could cause kernel crashes due to:
- Buffer overflows from long messages
- Malformed command strings
- Missing parameters
- Special characters

**Error Symptoms:**
- Kernel panics on invalid input
- Memory corruption
- Segmentation faults in kernel space

**Root Cause:**

Initial implementation used simple string operations without bounds checking or validation.

**Solution Implemented:**

1. **Input validation using sscanf with format specifiers:**
```c
if (sscanf(buf, "S %d %d %255[^\n]", &pid, &type, text) == 3) {
    // Valid send command
}
```

2. **Buffer size limits:**
```c
char text[256];  // Maximum 255 characters + null terminator
strncpy(msg->text, text, 255);
msg->text[255] = '\0';  // Ensure null termination
```

3. **Command validation:**
```c
if (buf[0] != 'S' && buf[0] != 'R') {
    pr_err("[safe_lkm] Invalid command\n");
    return -EINVAL;
}
```

**Verification:**

Edge case tests confirm proper handling of:
- 500-character messages (truncated to 255)
- Special characters (!, @, #, etc.)
- Malformed commands (return error without crashing)

### Challenge 3: Memory Management and Leaks

**Problem Description:**

Memory leaks occurred when:
- Module was unloaded with pending messages
- Receive operations failed mid-execution
- Module initialization failed

**Error Symptoms:**
- `dmesg` showed memory allocation without corresponding frees
- Available kernel memory decreased over time
- System slowdown after multiple load/unload cycles

**Root Cause:**

Incomplete cleanup in error paths and module exit handler.

**Solution Implemented:**

1. **Comprehensive cleanup in module_exit:**
```c
static void cleanup_queue(struct list_head *queue) {
    struct demo_msg *msg, *tmp;
    list_for_each_entry_safe(msg, tmp, queue, list) {
        list_del(&msg->list);
        kfree(msg);
        msg_count++;
    }
}

static void __exit demo_exit(void) {
    cleanup_queue(&msg_queue.high_priority);
    cleanup_queue(&msg_queue.normal_priority);
    proc_remove(proc_entry);
    pr_info("[safe_lkm] Cleaned up %d messages\n", msg_count);
}
```

2. **Error path cleanup:**
```c
if (!msg) {
    pr_err("[safe_lkm] Memory allocation failed\n");
    return -ENOMEM;
}
```

**Verification:**

- Loaded and unloaded module 100 times with pending messages
- Monitored kernel memory with `/proc/meminfo`
- No memory leaks detected

### Challenge 4: Priority Queue Design

**Problem Description:**

Needed to decide between multiple priority queue implementations:
- Single sorted linked list
- Dual separate queues (chosen)
- Priority heap
- Array-based priority queue

**Considerations:**

| Approach | Insertion | Retrieval | Complexity | Memory |
|----------|-----------|-----------|------------|--------|
| Sorted List | O(n) | O(1) | High | Low |
| Dual Queues | O(1) | O(1) | Low | Low |
| Heap | O(log n) | O(log n) | High | Medium |
| Array | O(1) | O(n) | Medium | High |

**Solution Chosen:**

Dual separate queues with two-level priority (high/normal).

**Rationale:**
- **Performance:** O(1) for both insertion and retrieval
- **Simplicity:** Easy to implement and understand
- **Memory:** Minimal overhead (two list heads)
- **Suitable:** Two priority levels sufficient for assignment requirements

**Trade-off:**

Limited to two priority levels. If more granular priorities were needed, a heap or sorted list would be more appropriate.

### Challenge 5: Testing Methodology

**Problem Description:**

Needed comprehensive testing strategy to ensure:
- Correct functionality
- Performance under load
- Stability with edge cases
- No memory leaks or crashes

**Solution Implemented:**

Three-tier testing approach:

**Tier 1: Basic Functionality (test_basic.c)**
- Unit-test style validation
- One operation at a time
- Clear pass/fail criteria
- Tests fundamental operations

**Tier 2: Stress Testing (test_stress.c)**
- High-volume operations (100+ messages)
- Concurrent access patterns
- Performance validation
- Memory pressure testing

**Tier 3: Edge Cases (test_edge.c)**
- Boundary conditions
- Invalid input
- Error handling
- System stability

**Benefits of This Approach:**

1. **Progressive validation:** Basic tests must pass before stress tests
2. **Clear failure isolation:** Know which category failed
3. **Comprehensive coverage:** Functional, performance, and stability testing
4. **Reproducible:** Same tests can be run repeatedly

**Results:**

All 20 tests (6 + 5 + 9) pass successfully, confirming robust implementation.

### Challenge 6: Kernel Module Safety

**Problem Description:**

Ensuring the kernel module doesn't destabilize the system or cause:
- Kernel panics
- System freezes
- Data corruption
- Security vulnerabilities

**Solution Implemented:**

1. **No modification of existing kernel structures**
2. **Proper error handling at every step**
3. **Bounds checking on all user input**
4. **Safe memory operations (kmalloc/kfree)**
5. **Proper cleanup on module unload**
6. **Logging all operations for debugging**

**Safety Verification:**

- Module loads and unloads cleanly
- No kernel panics during testing
- System remains stable under stress
- Clean removal even with pending messages

---

## 4. Conclusion

### Implementation Summary

Successfully implemented Option B (Advanced IPC Mechanism) with a priority-based message queue system. The implementation is:

- **Functional:** All test programs pass (20/20 tests)
- **Stable:** No crashes or memory leaks under stress testing
- **Safe:** Proper thread synchronization and error handling
- **Educational:** Demonstrates kernel programming concepts effectively

### Learning Outcomes

This assignment provided practical experience with:

1. **Linux Kernel Module Development:** Loading, testing, and debugging LKMs
2. **Kernel Synchronization Primitives:** Spinlocks with interrupt safety
3. **Memory Management:** Dynamic allocation in kernel space with proper cleanup
4. **User-Kernel Communication:** /proc filesystem interface design
5. **Data Structures:** Linked lists using Linux kernel list API
6. **Testing:** Comprehensive testing strategies for kernel code
7. **Debugging:** Using dmesg, printk, and systematic debugging approaches

### Technical Achievements

- **359 lines** of kernel code implementing full IPC system
- **Three test programs** with 20 total test cases
- **Zero memory leaks** confirmed through repeated load/unload cycles
- **Thread-safe** operations validated with concurrent access
- **Professional documentation** including README and technical report

### Repository Information

Complete source code, documentation, and test programs are available at:

**GitHub Repository:** https://github.com/wizz-ctrl/Ipc-priority-message-queue-

**Repository Contents:**
- safe_lkm.c - Kernel module source code
- test_basic.c, test_stress.c, test_edge.c - Test programs
- Makefile - Build configuration
- README.md - Comprehensive documentation
- REPORT.md - This report

---

## Appendix: Quick Reference

### Module Commands

| Command | Format | Example |
|---------|--------|---------|
| Load module | `sudo insmod safe_lkm.ko` | - |
| Check module | `lsmod | grep safe_lkm` | - |
| Send message | `echo "S <pid> <type> <msg>" > /proc/safe_lkm` | `echo "S 1001 10 Hello" > /proc/safe_lkm` |
| Receive message | `echo "R" > /proc/safe_lkm` | - |
| Check status | `cat /proc/safe_lkm` | - |
| View logs | `sudo dmesg | grep safe_lkm` | - |
| Unload module | `sudo rmmod safe_lkm` | - |

### Test Program Summary

| Program | Tests | Purpose |
|---------|-------|---------|
| test_basic | 6 | Basic functionality validation |
| test_stress | 5 | Performance under load |
| test_edge | 9 | Error handling and edge cases |

**Total:** 20 comprehensive test cases

---

**End of Report**

**Submitted by:** Student Name  
**Course:** Operating Systems - Fall 2025  
**Institution:** SEECS, NUST  
**Date:** November 30, 2025
