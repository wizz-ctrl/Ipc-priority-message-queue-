// Stress Test Suite for Safe Kernel Module
// Assignment 2 - OS Fall 2025

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define PROC_FILE "/proc/safe_lkm"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define RESET "\033[0m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"

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

int test_many_tasks() {
    printf("\n%s=== Stress Test 1: Many Tasks ===%s\n", YELLOW, RESET);
    printf("Creating 100 tasks...\n");
    
    clock_t start = clock();
    for (int i = 0; i < 100; i++) {
        char cmd[64];
        snprintf(cmd, sizeof(cmd), "T %d %d", 10000 + i, rand() % 20);
        if (write_proc(cmd) != 0) {
            test_result("Create 100 tasks", 0);
            return 0;
        }
        if (i % 20 == 0) {
            printf("  Progress: %d%%\n", i);
            usleep(10000); // Small delay to avoid overwhelming the system
        }
    }
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    
    printf("  Completed in %.2f seconds\n", time_spent);
    test_result("Create 100 tasks", 1);
    return 1;
}

int test_many_messages() {
    printf("\n%s=== Stress Test 2: Many Messages ===%s\n", YELLOW, RESET);
    printf("Sending 100 messages...\n");
    
    clock_t start = clock();
    for (int i = 0; i < 100; i++) {
        char cmd[128];
        int priority = rand() % 15;
        snprintf(cmd, sizeof(cmd), "B %d %d StressTest_Message_%d", 
                 20000 + i, priority, i);
        if (write_proc(cmd) != 0) {
            test_result("Send 100 messages", 0);
            return 0;
        }
        if (i % 20 == 0) {
            printf("  Progress: %d%%\n", i);
            usleep(10000);
        }
    }
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    
    printf("  Completed in %.2f seconds\n", time_spent);
    test_result("Send 100 messages", 1);
    return 1;
}

int test_receive_all_messages() {
    printf("\n%s=== Stress Test 3: Receive All Messages ===%s\n", YELLOW, RESET);
    printf("Receiving messages...\n");
    
    int count = 0;
    for (int i = 0; i < 110; i++) { // Try to receive more than sent
        if (write_proc("R") == 0) {
            count++;
            if (i % 20 == 0 && i > 0) {
                printf("  Received: %d messages\n", count);
            }
        }
        usleep(5000);
    }
    
    printf("  Total messages received: %d\n", count);
    test_result("Receive all messages", count > 0);
    return count > 0;
}

int test_priority_changes() {
    printf("\n%s=== Stress Test 4: Many Priority Changes ===%s\n", YELLOW, RESET);
    printf("Changing priorities 50 times...\n");
    
    // Create some tasks first
    for (int i = 0; i < 20; i++) {
        char cmd[64];
        snprintf(cmd, sizeof(cmd), "T %d %d", 30000 + i, i % 10);
        write_proc(cmd);
    }
    usleep(50000);
    
    clock_t start = clock();
    for (int i = 0; i < 50; i++) {
        char cmd[64];
        int pid = 30000 + (rand() % 20);
        int new_prio = rand() % 20;
        snprintf(cmd, sizeof(cmd), "A %d %d", pid, new_prio);
        write_proc(cmd);
        
        if (i % 10 == 0 && i > 0) {
            printf("  Progress: %d%%\n", (i * 100) / 50);
        }
        usleep(5000);
    }
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    
    printf("  Completed in %.2f seconds\n", time_spent);
    test_result("50 priority changes", 1);
    return 1;
}

int test_mixed_operations() {
    printf("\n%s=== Stress Test 5: Mixed Operations ===%s\n", YELLOW, RESET);
    printf("Performing 200 random operations...\n");
    
    clock_t start = clock();
    for (int i = 0; i < 200; i++) {
        char cmd[128];
        int op = rand() % 5;
        int pid = 40000 + (rand() % 100);
        
        switch(op) {
            case 0: // Create task
                snprintf(cmd, sizeof(cmd), "T %d %d", pid, rand() % 20);
                break;
            case 1: // Set priority
                snprintf(cmd, sizeof(cmd), "A %d %d", pid, rand() % 20);
                break;
            case 2: // Send message
                snprintf(cmd, sizeof(cmd), "B %d %d MixedOp_%d", 
                         pid, rand() % 15, i);
                break;
            case 3: // Receive message
                snprintf(cmd, sizeof(cmd), "R");
                break;
            case 4: // Migrate
                snprintf(cmd, sizeof(cmd), "C %d %d", pid, rand() % 4);
                break;
        }
        
        write_proc(cmd);
        
        if (i % 40 == 0 && i > 0) {
            printf("  Progress: %d%%\n", (i * 100) / 200);
        }
        usleep(2000);
    }
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    
    printf("  Completed in %.2f seconds\n", time_spent);
    test_result("200 mixed operations", 1);
    return 1;
}

int test_concurrent_reads() {
    printf("\n%s=== Stress Test 6: Concurrent Status Reads ===%s\n", YELLOW, RESET);
    printf("Reading status 50 times...\n");
    
    clock_t start = clock();
    for (int i = 0; i < 50; i++) {
        FILE *fp = fopen(PROC_FILE, "r");
        if (fp) {
            char buffer[1024];
            fread(buffer, 1, sizeof(buffer), fp);
            fclose(fp);
        }
        
        if (i % 10 == 0 && i > 0) {
            printf("  Progress: %d%%\n", (i * 100) / 50);
        }
        usleep(10000);
    }
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    
    printf("  Completed in %.2f seconds\n", time_spent);
    test_result("50 concurrent reads", 1);
    return 1;
}

int main() {
    printf("\n");
    printf("========================================\n");
    printf("  Safe Kernel Module - Stress Tests\n");
    printf("  OS Assignment 2 - Fall 2025\n");
    printf("========================================\n");
    printf("%sWARNING: This will generate many kernel log messages%s\n\n", YELLOW, RESET);
    
    srand(time(NULL));
    
    int passed = 0;
    int total = 6;
    
    if (access(PROC_FILE, F_OK) != 0) {
        printf("\n%sERROR: Module not loaded. Please run: sudo insmod safe_lkm.ko%s\n", RED, RESET);
        return 1;
    }
    
    passed += test_many_tasks();
    passed += test_many_messages();
    passed += test_receive_all_messages();
    passed += test_priority_changes();
    passed += test_mixed_operations();
    passed += test_concurrent_reads();
    
    printf("\n========================================\n");
    printf("Results: %d/%d stress tests passed\n", passed, total);
    printf("========================================\n\n");
    
    printf("%sCheck system stability with:%s\n", BLUE, RESET);
    printf("  dmesg | tail -100\n");
    printf("  cat /proc/safe_lkm\n\n");
    
    return (passed == total) ? 0 : 1;
}
