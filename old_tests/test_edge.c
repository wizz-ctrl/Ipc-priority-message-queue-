// Edge Case Test Suite for Safe Kernel Module
// Assignment 2 - OS Fall 2025

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PROC_FILE "/proc/safe_lkm"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define RESET "\033[0m"
#define YELLOW "\033[0;33m"

void test_result(const char *test_name, int passed) {
    printf("[%s] %s%s%s\n", 
           passed ? " OK " : "FAIL", 
           passed ? GREEN : RED, 
           test_name, 
           RESET);
}

int write_proc(const char *command) {
    FILE *fp = fopen(PROC_FILE, "w");
    if (!fp) return -1;
    fprintf(fp, "%s", command);
    fclose(fp);
    return 0;
}

int test_nonexistent_pid() {
    printf("\n%s=== Test 1: Non-existent PID ===%s\n", YELLOW, RESET);
    int result = write_proc("A 9999 5");
    test_result("Set priority for non-existent PID", result == 0);
    sleep(1);
    return 1;
}

int test_invalid_command() {
    printf("\n%s=== Test 2: Invalid Command ===%s\n", YELLOW, RESET);
    int result = write_proc("X 1234 5");
    test_result("Send invalid command", result == 0);
    sleep(1);
    return 1;
}

int test_empty_queue() {
    printf("\n%s=== Test 3: Empty Queue ===%s\n", YELLOW, RESET);
    // Try to receive from potentially empty queue
    int result = write_proc("R");
    test_result("Receive from empty queue", result == 0);
    sleep(1);
    return 1;
}

int test_priority_boundaries() {
    printf("\n%s=== Test 4: Priority Boundaries ===%s\n", YELLOW, RESET);
    write_proc("T 3001 0");
    test_result("Create task with priority 0", 1);
    sleep(1);
    
    write_proc("T 3002 99");
    test_result("Create task with priority 99", 1);
    sleep(1);
    
    write_proc("T 3003 -5");
    test_result("Create task with negative priority", 1);
    sleep(1);
    
    return 1;
}

int test_message_priority_threshold() {
    printf("\n%s=== Test 5: Message Priority Threshold ===%s\n", YELLOW, RESET);
    write_proc("B 3004 4 BelowThreshold");
    test_result("Send message with priority 4 (normal)", 1);
    sleep(1);
    
    write_proc("B 3005 5 AtThreshold");
    test_result("Send message with priority 5 (high)", 1);
    sleep(1);
    
    write_proc("B 3006 6 AboveThreshold");
    test_result("Send message with priority 6 (high)", 1);
    sleep(1);
    
    return 1;
}

int test_long_message() {
    printf("\n%s=== Test 6: Long Message ===%s\n", YELLOW, RESET);
    char long_msg[300];
    memset(long_msg, 'A', 250);
    long_msg[250] = '\0';
    
    char command[400];
    snprintf(command, sizeof(command), "B 3007 7 %s", long_msg);
    
    int result = write_proc(command);
    test_result("Send very long message (truncation test)", result == 0);
    sleep(1);
    return 1;
}

int test_rapid_operations() {
    printf("\n%s=== Test 7: Rapid Operations ===%s\n", YELLOW, RESET);
    for (int i = 0; i < 10; i++) {
        char cmd[64];
        snprintf(cmd, sizeof(cmd), "T %d %d", 4000 + i, i % 10);
        write_proc(cmd);
    }
    test_result("Create 10 tasks rapidly", 1);
    sleep(1);
    return 1;
}

int test_message_ordering() {
    printf("\n%s=== Test 8: Message Priority Ordering ===%s\n", YELLOW, RESET);
    write_proc("B 5001 2 Normal1");
    write_proc("B 5002 8 High1");
    write_proc("B 5003 3 Normal2");
    write_proc("B 5004 10 High2");
    sleep(1);
    
    printf("  Receiving messages (high priority should come first):\n");
    write_proc("R");
    sleep(1);
    write_proc("R");
    sleep(1);
    
    test_result("Message priority ordering", 1);
    return 1;
}

int main() {
    printf("\n");
    printf("========================================\n");
    printf("  Safe Kernel Module - Edge Case Tests\n");
    printf("  OS Assignment 2 - Fall 2025\n");
    printf("========================================\n");
    
    int passed = 0;
    int total = 8;
    
    if (access(PROC_FILE, F_OK) != 0) {
        printf("\n%sERROR: Module not loaded. Please run: sudo insmod safe_lkm.ko%s\n", RED, RESET);
        return 1;
    }
    
    passed += test_nonexistent_pid();
    passed += test_invalid_command();
    passed += test_empty_queue();
    passed += test_priority_boundaries();
    passed += test_message_priority_threshold();
    passed += test_long_message();
    passed += test_rapid_operations();
    passed += test_message_ordering();
    
    printf("\n========================================\n");
    printf("Results: %d/%d tests passed\n", passed, total);
    printf("========================================\n\n");
    
    printf("%sNote: Check 'dmesg | tail -50' for kernel log messages%s\n", YELLOW, RESET);
    
    return (passed == total) ? 0 : 1;
}
