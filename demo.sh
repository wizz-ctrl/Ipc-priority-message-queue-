#!/bin/bash
# Demo script - Shows all features of Safe Kernel Module
# OS Assignment 2 - Fall 2025

# Colors
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[0;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo ""
echo "========================================="
echo "  Safe Kernel Module Demo"
echo "  OS Assignment 2 - Fall 2025"
echo "========================================="
echo ""

# Check if module is loaded
if [ ! -e /proc/safe_lkm ]; then
    echo -e "${RED}ERROR: Module not loaded!${NC}"
    echo "Please run: sudo insmod safe_lkm.ko"
    exit 1
fi

echo -e "${BLUE}=== Initial Status ===${NC}"
cat /proc/safe_lkm
echo ""
read -p "Press Enter to continue..."

echo ""
echo -e "${BLUE}=== Demo 1: Create Tasks (Option A) ===${NC}"
echo "Creating task PID 5001 with priority 3..."
echo "T 5001 3" | sudo tee /proc/safe_lkm > /dev/null
sleep 1

echo "Creating task PID 5002 with priority 7..."
echo "T 5002 7" | sudo tee /proc/safe_lkm > /dev/null
sleep 1

echo "Creating task PID 5003 with priority 5..."
echo "T 5003 5" | sudo tee /proc/safe_lkm > /dev/null
sleep 1

echo -e "${GREEN}✓ Tasks created${NC}"
cat /proc/safe_lkm
echo ""
read -p "Press Enter to continue..."

echo ""
echo -e "${BLUE}=== Demo 2: Change Priority (Option A) ===${NC}"
echo "Changing PID 5001 priority from 3 to 9..."
echo "A 5001 9" | sudo tee /proc/safe_lkm > /dev/null
sleep 1

echo -e "${GREEN}✓ Priority changed${NC}"
cat /proc/safe_lkm
echo ""
read -p "Press Enter to continue..."

echo ""
echo -e "${BLUE}=== Demo 3: Send Messages (Option B) ===${NC}"
echo "Sending normal priority message (priority 2)..."
echo "B 5001 2 Normal_Priority_Message" | sudo tee /proc/safe_lkm > /dev/null
sleep 1

echo "Sending another normal priority message (priority 3)..."
echo "B 5002 3 Another_Normal_Message" | sudo tee /proc/safe_lkm > /dev/null
sleep 1

echo "Sending HIGH priority message (priority 10)..."
echo "B 5003 10 HIGH_PRIORITY_URGENT" | sudo tee /proc/safe_lkm > /dev/null
sleep 1

echo "Sending another HIGH priority message (priority 8)..."
echo "B 5001 8 Another_High_Priority" | sudo tee /proc/safe_lkm > /dev/null
sleep 1

echo -e "${GREEN}✓ Messages sent${NC}"
cat /proc/safe_lkm
echo ""
read -p "Press Enter to continue..."

echo ""
echo -e "${BLUE}=== Demo 4: Receive Messages (Option B) ===${NC}"
echo -e "${YELLOW}Note: High priority messages should be received first!${NC}"
echo ""

echo "Receiving message 1..."
echo "R" | sudo tee /proc/safe_lkm > /dev/null
sleep 1

echo "Receiving message 2..."
echo "R" | sudo tee /proc/safe_lkm > /dev/null
sleep 1

echo "Receiving message 3..."
echo "R" | sudo tee /proc/safe_lkm > /dev/null
sleep 1

echo "Receiving message 4..."
echo "R" | sudo tee /proc/safe_lkm > /dev/null
sleep 1

echo -e "${GREEN}✓ Messages received (check dmesg for details)${NC}"
echo ""
read -p "Press Enter to continue..."

echo ""
echo -e "${BLUE}=== Demo 5: Process Migration (Option C) ===${NC}"
echo "Migrating PID 5001 to CPU 0..."
echo "C 5001 0" | sudo tee /proc/safe_lkm > /dev/null
sleep 1

echo "Migrating PID 5002 to CPU 1..."
echo "C 5002 1" | sudo tee /proc/safe_lkm > /dev/null
sleep 1

echo "Migrating PID 5003 to CPU 2..."
echo "C 5003 2" | sudo tee /proc/safe_lkm > /dev/null
sleep 1

echo -e "${GREEN}✓ Processes migrated${NC}"
echo ""
read -p "Press Enter to continue..."

echo ""
echo -e "${BLUE}=== Final Status ===${NC}"
cat /proc/safe_lkm
echo ""

echo ""
echo -e "${BLUE}=== Recent Kernel Logs ===${NC}"
echo -e "${YELLOW}Last 30 lines from dmesg:${NC}"
dmesg | grep safe_lkm | tail -30
echo ""

echo "========================================="
echo -e "${GREEN}Demo Complete!${NC}"
echo "========================================="
echo ""
echo "All three options demonstrated:"
echo "  ✓ Option A: Priority Scheduling"
echo "  ✓ Option B: IPC Message Queue"
echo "  ✓ Option C: Process Migration"
echo ""
