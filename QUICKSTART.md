# Quick Start Guide - Safe Kernel Module

## 🚀 Fast Setup (5 minutes)

### Step 1: Compile Everything
```bash
cd /home/taimoor-safdar/os-assignment2
make                    # Compile kernel module
./compile_tests.sh      # Compile test programs
```

### Step 2: Load the Module
```bash
sudo insmod safe_lkm.ko
```

### Step 3: Verify It's Loaded
```bash
lsmod | grep safe_lkm
cat /proc/safe_lkm
```

### Step 4: Run Tests (Optional)
```bash
sudo ./run_tests.sh
```

---

## 📝 Quick Command Reference

### Module Management
```bash
# Load module
sudo insmod safe_lkm.ko

# Check if loaded
lsmod | grep safe_lkm

# Unload module
sudo rmmod safe_lkm

# View kernel logs
dmesg | tail -20
```

### Using the Module

#### Create a task:
```bash
echo "T 2001 5" | sudo tee /proc/safe_lkm
```

#### Set priority:
```bash
echo "A 2001 8" | sudo tee /proc/safe_lkm
```

#### Send message (normal priority):
```bash
echo "B 2001 3 Hello" | sudo tee /proc/safe_lkm
```

#### Send message (high priority):
```bash
echo "B 2002 10 Urgent" | sudo tee /proc/safe_lkm
```

#### Receive message:
```bash
echo "R" | sudo tee /proc/safe_lkm
```

#### Migrate process:
```bash
echo "C 2001 2" | sudo tee /proc/safe_lkm
```

#### View status:
```bash
cat /proc/safe_lkm
```

---

## 🧪 Quick Test

Try this complete workflow:
```bash
# Load module
sudo insmod safe_lkm.ko

# View initial status
cat /proc/safe_lkm

# Create some tasks
echo "T 2001 3" | sudo tee /proc/safe_lkm
echo "T 2002 7" | sudo tee /proc/safe_lkm

# Send messages (high priority first when received)
echo "B 2001 2 Normal_Message" | sudo tee /proc/safe_lkm
echo "B 2002 10 High_Priority" | sudo tee /proc/safe_lkm

# Receive messages (high priority comes first!)
echo "R" | sudo tee /proc/safe_lkm
echo "R" | sudo tee /proc/safe_lkm

# View status
cat /proc/safe_lkm

# Check kernel logs
dmesg | tail -20

# Unload module
sudo rmmod safe_lkm
```

---

## ⚠️ Common Issues

### "Module not found"
```bash
# Make sure you compiled it
make
ls -l safe_lkm.ko
```

### "Operation not permitted"
```bash
# Need sudo to load kernel modules
sudo insmod safe_lkm.ko
```

### "Module already loaded"
```bash
# Unload first, then reload
sudo rmmod safe_lkm
sudo insmod safe_lkm.ko
```

### "Permission denied" when writing to /proc
```bash
# Use sudo or tee
echo "T 2001 5" | sudo tee /proc/safe_lkm
```

---

## 🧹 Cleanup

```bash
# Unload module
sudo rmmod safe_lkm

# Clean build files
make clean
rm -f test_basic test_edge test_stress
```

---

## 📊 What Each Test Does

### test_basic
- Creates tasks
- Sets priorities
- Sends/receives messages
- Tests migration
- **Takes: ~10 seconds**

### test_edge
- Tests invalid operations
- Boundary conditions
- Error handling
- Message ordering
- **Takes: ~20 seconds**

### test_stress
- 100 tasks
- 100 messages
- Mixed operations
- Performance testing
- **Takes: ~60 seconds**

---

## 📚 More Information

For detailed documentation, see **README.md**

For implementation details, see comments in **safe_lkm.c**

---

**Need help?** Check `dmesg` for kernel logs!
