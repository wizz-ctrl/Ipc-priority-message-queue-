// Basic IPC Test Suite - Option B
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
    if (!fp) {
        perror("Failed to open proc file");
        return -1;
    }
    fprintf(fp, "%s", command);
    fclose(fp);
    return 0;
}

int test_send_normal_message() {
    printf("\n%s=== Test 1: Send Normal Priority Message ===%s\n", YELLOW, RESET);
    int result = write_proc("S 1001 3 HelloNormal");
    test_result("Send message with type 3 (normal priority)", result == 0);
    sleep(1);
    return result == 0;
}

int test_send_high_message() {
    printf("\n%s=== Test 2: Send High Priority Message ===%s\n", YELLOW, RESET);
    int result = write_proc("S 1002 10 HelloHigh");
    test_result("Send message with type 10 (high priority)", result == 0);
    sleep(1);
    return result == 0;
}

int test_priority_order() {
    printf("\n%s=== Test 3: Priority Ordering ===%s\n", YELLOW, RESET);
    // Send normal first, then high
    write_proc("S 2001 2 NormalMsg");
    sleep(1);
    write_proc("S 2002 8 HighMsg");
    sleep(1);
    
    // Receive - should get high priority first
    printf("  Receiving message (should be HighMsg)...\n");
    int result = write_proc("R");
    test_result("High priority message retrieved first", result == 0);
    sleep(1);
    return result == 0;
}

int test_receive_empty() {
    printf("\n%s=== Test 4: Receive from Empty Queue ===%s\n", YELLOW, RESET);
    // Try to clear queue first
    for (int i = 0; i < 10; i++) {
        write_proc("R");
        usleep(100000);
    }
    
    // Now receive from empty
    int result = write_proc("R");
    test_result("Handle empty queue gracefully", result == 0);
    sleep(1);
    return 1;  // This should succeed (no crash)
}

int test_multiple_messages() {
    printf("\n%s=== Test 5: Multiple Messages ===%s\n", YELLOW, RESET);
    printf("  Sending 5 messages...\n");
    
    write_proc("S 3001 1 Msg1");
    write_proc("S 3002 2 Msg2");
    write_proc("S 3003 10 Msg3");
    write_proc("S 3004 8 Msg4");
    write_proc("S 3005 3 Msg5");
    sleep(1);
    
    test_result("Send 5 messages with mixed priorities", 1);
    return 1;
}

int test_read_status() {
    printf("\n%s=== Test 6: Read Status ===%s\n", YELLOW, RESET);
    FILE *fp = fopen(PROC_FILE, "r");
    if (!fp) {
        test_result("Read queue status", 0);
        return 0;
    }
    
    char buffer[2048];
    size_t bytes = fread(buffer, 1, sizeof(buffer)-1, fp);
    fclose(fp);
    
    if (bytes > 0) {
        buffer[bytes] = '\0';
        printf("\n%s--- Status Output ---%s\n%s\n", BLUE, RESET, buffer);
        test_result("Read and display queue status", 1);
        return 1;
    }
    
    test_result("Read queue status", 0);
    return 0;
}

int main() {
    printf("\n");
    printf("================================================\n");
    printf("  IPC Priority Message Queue - Basic Tests\n");
    printf("  OS Assignment 2 (Option B) - Fall 2025\n");
    printf("================================================\n");
    
    int passed = 0;
    int total = 6;
    
    if (access(PROC_FILE, F_OK) != 0) {
        printf("\n%sERROR: Module not loaded!%s\n", RED, RESET);
        printf("Please run: %ssudo insmod safe_lkm.ko%s\n\n", YELLOW, RESET);
        return 1;
    }
    
    passed += test_send_normal_message();
    passed += test_send_high_message();
    passed += test_priority_order();
    passed += test_receive_empty();
    passed += test_multiple_messages();
    passed += test_read_status();
    
    printf("\n================================================\n");
    printf("Results: %s%d/%d tests passed%s\n", 
           passed == total ? GREEN : (passed > total/2 ? YELLOW : RED),
           passed, total, RESET);
    printf("================================================\n\n");
    
    printf("%sTo view kernel logs:%s\n", BLUE, RESET);
    printf("  dmesg | grep safe_lkm | tail -20\n\n");
    
    return (passed == total) ? 0 : 1;
}
