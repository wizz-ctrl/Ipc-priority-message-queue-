#!/bin/bash
# Run test suite for Safe Kernel Module
# Assignment 2 - OS Fall 2025

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo ""
echo "========================================="
echo "  Safe Kernel Module Test Runner"
echo "  OS Assignment 2 - Fall 2025"
echo "========================================="
echo ""

# Check if module is loaded
if [ ! -e /proc/safe_lkm ]; then
    echo -e "${RED}ERROR: Kernel module not loaded!${NC}"
    echo "Please load the module first:"
    echo "  sudo insmod safe_lkm.ko"
    echo ""
    exit 1
fi

# Check if we need sudo for writing to /proc
if [ ! -w /proc/safe_lkm ]; then
    echo -e "${YELLOW}Note: Tests need write access to /proc/safe_lkm${NC}"
    echo "Run this script with: sudo ./run_tests.sh"
    echo ""
fi

# Check if test binaries exist
if [ ! -f ./test_basic ] || [ ! -f ./test_edge ] || [ ! -f ./test_stress ]; then
    echo -e "${RED}ERROR: Test binaries not found!${NC}"
    echo "Compile tests first with: ./compile_tests.sh"
    echo ""
    exit 1
fi

# Run tests
echo -e "${BLUE}Starting test suite...${NC}"
echo ""

# Basic tests
echo -e "${GREEN}[1/3] Running Basic Tests${NC}"
./test_basic
BASIC_RESULT=$?
echo ""

# Edge case tests
echo -e "${GREEN}[2/3] Running Edge Case Tests${NC}"
./test_edge
EDGE_RESULT=$?
echo ""

# Stress tests
echo -e "${GREEN}[3/3] Running Stress Tests${NC}"
echo -e "${YELLOW}This may take a while...${NC}"
./test_stress
STRESS_RESULT=$?
echo ""

# Summary
echo "========================================="
echo "  Test Suite Summary"
echo "========================================="
echo ""

if [ $BASIC_RESULT -eq 0 ]; then
    echo -e "${GREEN}✓${NC} Basic Tests: PASSED"
else
    echo -e "${RED}✗${NC} Basic Tests: FAILED"
fi

if [ $EDGE_RESULT -eq 0 ]; then
    echo -e "${GREEN}✓${NC} Edge Case Tests: PASSED"
else
    echo -e "${RED}✗${NC} Edge Case Tests: FAILED"
fi

if [ $STRESS_RESULT -eq 0 ]; then
    echo -e "${GREEN}✓${NC} Stress Tests: PASSED"
else
    echo -e "${RED}✗${NC} Stress Tests: FAILED"
fi

echo ""
echo "========================================="
echo ""
echo -e "${BLUE}To view kernel logs:${NC}"
echo "  dmesg | tail -50"
echo ""
echo -e "${BLUE}To view current module status:${NC}"
echo "  cat /proc/safe_lkm"
echo ""

# Exit with error if any test failed
if [ $BASIC_RESULT -ne 0 ] || [ $EDGE_RESULT -ne 0 ] || [ $STRESS_RESULT -ne 0 ]; then
    exit 1
fi

exit 0
