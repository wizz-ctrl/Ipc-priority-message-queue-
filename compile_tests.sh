#!/bin/bash
# Compile test programs for Safe Kernel Module
# Assignment 2 - OS Fall 2025

echo "========================================="
echo "  Compiling Test Suite"
echo "========================================="
echo ""

# Compile basic tests
echo "[1/3] Compiling test_basic.c..."
gcc -o test_basic test_basic.c -Wall
if [ $? -eq 0 ]; then
    echo "  ✓ test_basic compiled successfully"
else
    echo "  ✗ Failed to compile test_basic"
    exit 1
fi

# Compile edge case tests
echo "[2/3] Compiling test_edge.c..."
gcc -o test_edge test_edge.c -Wall
if [ $? -eq 0 ]; then
    echo "  ✓ test_edge compiled successfully"
else
    echo "  ✗ Failed to compile test_edge"
    exit 1
fi

# Compile stress tests
echo "[3/3] Compiling test_stress.c..."
gcc -o test_stress test_stress.c -Wall
if [ $? -eq 0 ]; then
    echo "  ✓ test_stress compiled successfully"
else
    echo "  ✗ Failed to compile test_stress"
    exit 1
fi

echo ""
echo "========================================="
echo "  All tests compiled successfully!"
echo "========================================="
echo ""
echo "Run tests with: ./run_tests.sh"
echo ""
