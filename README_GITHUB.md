# 🚀 Advanced IPC Priority Message Queue - Kernel Module

> **OS Assignment 2 - Fall 2025**  
> **Option B: Inter-Process Communication with Priority Queuing**

[![Linux](https://img.shields.io/badge/Linux-Kernel%20Module-blue?logo=linux)](https://www.kernel.org/)
[![License](https://img.shields.io/badge/License-GPL-green.svg)](LICENSE)
[![Language](https://img.shields.io/badge/Language-C-orange.svg)](https://en.wikipedia.org/wiki/C_(programming_language))

## 📋 Overview

This project implements a **safe, educational Linux kernel module** that demonstrates an **Advanced Inter-Process Communication (IPC) mechanism** with **priority-based message queuing**. The module provides a complete working implementation without modifying the actual kernel.

### ✨ Key Features

- 🎯 **Dual Priority Queues**: Separate high and normal priority message queues
- ⚡ **Priority-Based Delivery**: High priority messages (type ≥ 5) delivered first
- 🔒 **Thread-Safe Operations**: Spinlock protection for concurrent access
- 💾 **Dynamic Memory Management**: Efficient kernel memory allocation
- 📁 **Proc Filesystem Interface**: Easy user-space interaction via `/proc/safe_lkm`
- 📊 **FIFO Ordering**: First-In-First-Out within each priority level

## 🏗️ Architecture

```
┌─────────────────────────────────────────────┐
│        IPC Priority Message Queue           │
├─────────────────────────────────────────────┤
│                                             │
│  📮 HIGH Priority Queue (type ≥ 5)         │
│  ┌─────┐  ┌─────┐  ┌─────┐                │
│  │ 10  │→ │  8  │→ │  7  │   FIFO         │
│  └─────┘  └─────┘  └─────┘                │
│                                             │
│  📬 NORMAL Priority Queue (type < 5)        │
│  ┌─────┐  ┌─────┐  ┌─────┐                │
│  │  3  │→ │  2  │→ │  1  │   FIFO         │
│  └─────┘  └─────┘  └─────┘                │
│                                             │
│  Receive: HIGH messages first → NORMAL     │
└─────────────────────────────────────────────┘
```

## 🚀 Quick Start

### Prerequisites

```bash
# Install kernel headers and build tools
sudo apt-get update
sudo apt-get install build-essential linux-headers-$(uname -r)
```

### Build & Load

```bash
# 1. Clone the repository
git clone https://github.com/YOUR_USERNAME/ipc-priority-queue.git
cd ipc-priority-queue

# 2. Compile the module
make

# 3. Load the module
sudo insmod safe_lkm.ko

# 4. Verify it's loaded
lsmod | grep safe_lkm
cat /proc/safe_lkm
```

## 📖 Usage

### Send Messages

```bash
# Send normal priority message (type < 5)
echo "S 1001 3 HelloWorld" | sudo tee /proc/safe_lkm

# Send high priority message (type ≥ 5)
echo "S 1002 10 UrgentAlert" | sudo tee /proc/safe_lkm
```

### Receive Messages

```bash
# Receive message (high priority first)
echo "R" | sudo tee /proc/safe_lkm
```

### Check Status

```bash
# View queue status
cat /proc/safe_lkm

# View kernel logs
dmesg | grep safe_lkm | tail -20
```

### Unload Module

```bash
sudo rmmod safe_lkm
```

## 🧪 Testing

```bash
# Compile tests
./compile_tests.sh

# Run basic tests
sudo ./test_basic
```

## 📊 Complete Example

```bash
# Load module
sudo insmod safe_lkm.ko

# Send mixed priority messages
echo "S 1001 2 NormalMsg1" | sudo tee /proc/safe_lkm
echo "S 1002 10 HighMsg1" | sudo tee /proc/safe_lkm
echo "S 1003 3 NormalMsg2" | sudo tee /proc/safe_lkm
echo "S 1004 8 HighMsg2" | sudo tee /proc/safe_lkm

# Check status (should show 2 high, 2 normal)
cat /proc/safe_lkm

# Receive in priority order
echo "R" | sudo tee /proc/safe_lkm  # Gets HighMsg1 (type=10)
echo "R" | sudo tee /proc/safe_lkm  # Gets HighMsg2 (type=8)
echo "R" | sudo tee /proc/safe_lkm  # Gets NormalMsg1 (type=2)
echo "R" | sudo tee /proc/safe_lkm  # Gets NormalMsg2 (type=3)

# View logs
dmesg | grep safe_lkm | tail -15

# Unload
sudo rmmod safe_lkm
```

## 🔍 Implementation Details

### Data Structures

```c
struct demo_msg {
    int pid;              // Sender process ID
    int type;             // Priority (0-255)
    char text[256];       // Message content
    struct list_head list; // Kernel linked list
};

struct demo_msg_queue {
    struct list_head high;    // High priority queue
    struct list_head normal;  // Normal priority queue
    int count;                // Total message count
};
```

### Priority Logic

- **HIGH Priority**: `type >= 5` → Added to high priority queue
- **NORMAL Priority**: `type < 5` → Added to normal priority queue
- **Retrieval**: HIGH queue checked first, then NORMAL queue
- **Ordering**: FIFO within each priority level

### Thread Safety

- Uses `spinlock_t` for queue protection
- `spin_lock_irqsave/restore` for interrupt safety
- No race conditions or data corruption

### Memory Management

- Dynamic allocation with `kmalloc/kfree`
- Automatic cleanup on module unload
- No memory leaks

## 📚 Documentation

- **[EXPLANATION.md](EXPLANATION.md)** - Complete technical walkthrough with diagrams
- **[README.md](README.md)** - Detailed usage guide
- **[QUICKSTART.md](QUICKSTART.md)** - Quick command reference
- **[FINAL_SUMMARY.md](FINAL_SUMMARY.md)** - Project overview

## 🛡️ Safety Features

✅ **No Kernel Modification** - Doesn't touch kernel source  
✅ **Memory Safe** - Proper allocation/deallocation  
✅ **Thread Safe** - Spinlock protection  
✅ **Interrupt Safe** - Can be used in any context  
✅ **Error Handling** - Validates all inputs  

## 🎓 Learning Outcomes

This project demonstrates:

- Inter-Process Communication (IPC) mechanisms
- Priority queue implementation
- Linux kernel module development
- Synchronization with spinlocks
- Kernel memory management
- Proc filesystem interface
- Kernel linked lists

## 📁 Project Structure

```
.
├── safe_lkm.c           # Main kernel module source
├── Makefile             # Build configuration
├── test_basic.c         # Basic functionality tests
├── test_edge.c          # Edge case tests
├── test_stress.c        # Stress tests
├── compile_tests.sh     # Test compilation script
├── run_tests.sh         # Test execution script
├── demo.sh              # Interactive demo
├── README.md            # This file
├── EXPLANATION.md       # Technical documentation
├── QUICKSTART.md        # Quick reference
├── FINAL_SUMMARY.md     # Project summary
└── SUBMISSION.md        # Grading checklist
```

## 🔧 System Requirements

- **OS**: Linux (Ubuntu 20.04+ recommended)
- **Kernel**: 5.0+ (tested on 6.14.0)
- **Compiler**: GCC 9.0+
- **Tools**: make, kernel headers

## 🐛 Troubleshooting

### Module won't load
```bash
# Check kernel headers
ls /lib/modules/$(uname -r)/build

# Clean and rebuild
make clean && make
```

### Permission denied
```bash
# Need sudo for module operations
sudo insmod safe_lkm.ko
sudo ./test_basic
```

### Module already loaded
```bash
# Unload first
sudo rmmod safe_lkm
sudo insmod safe_lkm.ko
```

## 📈 Performance

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| Send message | O(1) | Add to tail |
| Receive message | O(1) | Remove from head |
| Check status | O(n) | Count all messages |

## 🤝 Contributing

This is an educational project for OS Fall 2025 Assignment 2. For improvements or bug reports, please open an issue.

## 📄 License

GPL - Educational use only

## 👥 Authors

**OS Fall 2025 - Group Assignment**  
SEECS, NUST

## 🙏 Acknowledgments

- Linux Kernel Documentation
- Linux Device Drivers, 3rd Edition
- Operating Systems course materials

## 📞 Support

For questions or issues:
1. Check the [EXPLANATION.md](EXPLANATION.md) for detailed technical info
2. Review kernel logs: `dmesg | grep safe_lkm`
3. Ensure kernel headers are installed

---

**⭐ If this project helped you understand IPC mechanisms, please star it!**

---

<div align="center">
Made with ❤️ for OS Fall 2025 Assignment
</div>
