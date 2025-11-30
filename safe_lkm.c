// ============================================================================
// Safe Kernel Module - Advanced IPC Priority Message Queue
// Assignment 2 — OS Fall 2025 (Option B: IPC Mechanism)
// ============================================================================
//
// This module implements a safe, educational IPC message queue with priority
// handling without modifying the actual kernel. It demonstrates:
//   - Priority-based message queuing (High vs Normal priority)
//   - Thread-safe operations using spinlocks
//   - Kernel memory management
//   - /proc filesystem interface for user interaction
//
// ============================================================================

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/string.h>

#define PROC_NAME "safe_lkm"
#define MSG_SIZE 256
#define HIGH_PRIO_THRESHOLD 5

// ---------------------------------------------------------------------------
// IPC Message Queue Data Structures
// ---------------------------------------------------------------------------

// Message structure - represents a single IPC message
struct demo_msg {
    int pid;                  // Sender process ID
    int type;                 // Message priority/type
    char text[MSG_SIZE];      // Message content
    struct list_head list;    // Kernel linked list node
};

// Message queue with dual priority levels
struct demo_msg_queue {
    struct list_head high;    // High priority queue (type >= 5)
    struct list_head normal;  // Normal priority queue (type < 5)
    int count;                // Total message count
};

static struct demo_msg_queue msg_queue;
static spinlock_t demo_msg_lock;

// ---------------------------------------------------------------------------
// IPC Functions - Send Message
// ---------------------------------------------------------------------------

// Send a message to the IPC queue
// Parameters:
//   pid  - Process ID of sender
//   type - Message priority/type (>= 5 is high priority)
//   text - Message content (max 255 chars)
// Returns: 0 on success, -ENOMEM on allocation failure
int demo_send_msg(int pid, int type, const char *text)
{
    struct demo_msg *m;
    unsigned long flags;

    m = kmalloc(sizeof(*m), GFP_KERNEL);
    if (!m) {
        printk(KERN_WARNING "[safe_lkm] Failed to allocate message\n");
        return -ENOMEM;
    }

    m->pid = pid;
    m->type = type;
    strncpy(m->text, text, MSG_SIZE-1);
    m->text[MSG_SIZE-1] = '\0';
    INIT_LIST_HEAD(&m->list);

    spin_lock_irqsave(&demo_msg_lock, flags);
    if (type >= HIGH_PRIO_THRESHOLD) {
        list_add_tail(&m->list, &msg_queue.high);
        printk(KERN_INFO "[safe_lkm] High priority message from PID %d: %s\n", pid, text);
    } else {
        list_add_tail(&m->list, &msg_queue.normal);
        printk(KERN_INFO "[safe_lkm] Normal priority message from PID %d: %s\n", pid, text);
    }
    msg_queue.count++;
    spin_unlock_irqrestore(&demo_msg_lock, flags);

    return 0;
}

// ---------------------------------------------------------------------------
// IPC Functions - Receive Message
// ---------------------------------------------------------------------------

// Receive a message from the IPC queue
// Priority: HIGH priority messages are retrieved first, then NORMAL
// Parameters:
//   out - Pointer to message structure to store received message
// Returns: 0 on success, -ENOMSG if queue is empty
int demo_receive_msg(struct demo_msg *out)
{
    struct demo_msg *m = NULL;
    unsigned long flags;
    int ret = -ENOMSG;

    spin_lock_irqsave(&demo_msg_lock, flags);
    if (!list_empty(&msg_queue.high)) {
        m = list_first_entry(&msg_queue.high, struct demo_msg, list);
        list_del(&m->list);
        printk(KERN_INFO "[safe_lkm] Received high priority message: %s\n", m->text);
    } else if (!list_empty(&msg_queue.normal)) {
        m = list_first_entry(&msg_queue.normal, struct demo_msg, list);
        list_del(&m->list);
        printk(KERN_INFO "[safe_lkm] Received normal priority message: %s\n", m->text);
    }
    
    if (m) {
        memcpy(out, m, sizeof(*out));
        kfree(m);
        msg_queue.count--;
        ret = 0;
    } else {
        printk(KERN_INFO "[safe_lkm] No messages available\n");
    }
    spin_unlock_irqrestore(&demo_msg_lock, flags);
    
    return ret;
}

// ---------------------------------------------------------------------------
// Proc Filesystem Interface
// ---------------------------------------------------------------------------

// Read function - displays current queue status
static ssize_t proc_read(struct file *file, char __user *buf,
                         size_t count, loff_t *ppos)
{
    char kbuff[512];
    int len;
    unsigned long flags;
    int high_count = 0, normal_count = 0;
    struct demo_msg *m;

    if (*ppos > 0) return 0;

    // Count messages in each queue
    spin_lock_irqsave(&demo_msg_lock, flags);
    list_for_each_entry(m, &msg_queue.high, list) {
        high_count++;
    }
    list_for_each_entry(m, &msg_queue.normal, list) {
        normal_count++;
    }
    spin_unlock_irqrestore(&demo_msg_lock, flags);

    // Build status information
    len = snprintf(kbuff, sizeof(kbuff), 
                   "=== IPC Priority Message Queue - Assignment 2 (Option B) ===\n\n"
                   "Current Status:\n"
                   "  Total messages: %d\n"
                   "  High priority (type >= %d): %d messages\n"
                   "  Normal priority (type < %d): %d messages\n\n"
                   "Available Commands (write to this file):\n"
                   "  S <pid> <type> <message> - Send message\n"
                   "  R                        - Receive message\n\n"
                   "Priority Rules:\n"
                   "  - Messages with type >= %d are HIGH priority\n"
                   "  - Messages with type < %d are NORMAL priority\n"
                   "  - HIGH priority messages are received first\n"
                   "  - Within same priority: FIFO order\n\n"
                   "Examples:\n"
                   "  echo \"S 1001 3 Hello\" > /proc/safe_lkm    (Normal priority)\n"
                   "  echo \"S 1002 10 Urgent\" > /proc/safe_lkm  (High priority)\n"
                   "  echo \"R\" > /proc/safe_lkm                 (Receive message)\n\n",
                   msg_queue.count, HIGH_PRIO_THRESHOLD, high_count,
                   HIGH_PRIO_THRESHOLD, normal_count,
                   HIGH_PRIO_THRESHOLD, HIGH_PRIO_THRESHOLD);

    if (copy_to_user(buf, kbuff, len)) return -EFAULT;
    *ppos = len;
    return len;
}

// Write function - handles commands from user space
static ssize_t proc_write(struct file *file, const char __user *buf,
                          size_t count, loff_t *ppos)
{
    char kbuf[128];
    int pid, type;
    char text[64];
    struct demo_msg received_msg;

    if (count > sizeof(kbuf)-1) return -EINVAL;
    if (copy_from_user(kbuf, buf, count)) return -EFAULT;
    kbuf[count] = '\0';

    // Parse and execute commands
    if (sscanf(kbuf, "S %d %d %63[^\n]", &pid, &type, text) == 3) {
        // Send message command
        demo_send_msg(pid, type, text);
    } else if (strncmp(kbuf, "R", 1) == 0) {
        // Receive message command
        if (demo_receive_msg(&received_msg) == 0) {
            printk(KERN_INFO "[safe_lkm] User received: PID=%d, Type=%d, Text=%s\n",
                   received_msg.pid, received_msg.type, received_msg.text);
        }
    } else {
        printk(KERN_WARNING "[safe_lkm] Unknown command: %s\n", kbuf);
        printk(KERN_INFO "[safe_lkm] Valid commands: S <pid> <type> <msg> or R\n");
    }

    return count;
}

static const struct proc_ops proc_fops = {
    .proc_read = proc_read,
    .proc_write = proc_write,
    .proc_lseek = default_llseek,
};

// ---------------------------------------------------------------------------
// Module Cleanup Functions
// ---------------------------------------------------------------------------

// Clean up all messages in the queue
static void cleanup_messages(void)
{
    struct demo_msg *m, *tmp;
    unsigned long flags;
    int count = 0;

    spin_lock_irqsave(&demo_msg_lock, flags);
    list_for_each_entry_safe(m, tmp, &msg_queue.high, list) {
        list_del(&m->list);
        kfree(m);
        count++;
    }
    list_for_each_entry_safe(m, tmp, &msg_queue.normal, list) {
        list_del(&m->list);
        kfree(m);
        count++;
    }
    msg_queue.count = 0;
    spin_unlock_irqrestore(&demo_msg_lock, flags);

    if (count > 0) {
        printk(KERN_INFO "[safe_lkm] Cleaned up %d messages\n", count);
    }
}

// ---------------------------------------------------------------------------
// Module Initialization
// ---------------------------------------------------------------------------

static int __init safe_lkm_init(void)
{
    // Initialize spinlock for thread-safe operations
    spin_lock_init(&demo_msg_lock);

    // Initialize message queue data structures
    INIT_LIST_HEAD(&msg_queue.high);
    INIT_LIST_HEAD(&msg_queue.normal);
    msg_queue.count = 0;

    // Create /proc entry for user interface
    if (!proc_create(PROC_NAME, 0666, NULL, &proc_fops)) {
        printk(KERN_ERR "[safe_lkm] Failed to create /proc/%s\n", PROC_NAME);
        return -ENOMEM;
    }

    printk(KERN_INFO "[safe_lkm] IPC Priority Message Queue loaded\n");
    printk(KERN_INFO "[safe_lkm] Use: echo 'S <pid> <type> <msg>' > /proc/%s\n", PROC_NAME);
    printk(KERN_INFO "[safe_lkm] Use: echo 'R' > /proc/%s\n", PROC_NAME);
    printk(KERN_INFO "[safe_lkm] View status: cat /proc/%s\n", PROC_NAME);
    printk(KERN_INFO "[safe_lkm] Priority threshold: %d (>= HIGH, < NORMAL)\n", HIGH_PRIO_THRESHOLD);
    
    return 0;
}

// ---------------------------------------------------------------------------
// Module Exit/Cleanup
// ---------------------------------------------------------------------------

static void __exit safe_lkm_exit(void)
{
    // Clean up all allocated messages
    cleanup_messages();
    
    // Remove /proc entry
    remove_proc_entry(PROC_NAME, NULL);
    
    printk(KERN_INFO "[safe_lkm] IPC Priority Message Queue unloaded\n");
}

// ---------------------------------------------------------------------------
// Module Metadata
// ---------------------------------------------------------------------------

module_init(safe_lkm_init);
module_exit(safe_lkm_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("OS Fall 2025 - Group Assignment");
MODULE_DESCRIPTION("Advanced IPC Priority Message Queue - Option B");
MODULE_VERSION("1.0");

// ============================================================================
// USAGE INSTRUCTIONS
// ============================================================================
//
// COMPILATION:
//   $ make
//
// LOAD MODULE:
//   $ sudo insmod safe_lkm.ko
//
// SEND MESSAGES:
//   $ echo "S 1001 3 HelloWorld" > /proc/safe_lkm    # Normal priority
//   $ echo "S 1002 10 Urgent" > /proc/safe_lkm       # High priority
//
// RECEIVE MESSAGES:
//   $ echo "R" > /proc/safe_lkm                      # Receives high priority first
//
// VIEW STATUS:
//   $ cat /proc/safe_lkm
//
// CHECK LOGS:
//   $ dmesg | tail -20
//
// UNLOAD MODULE:
//   $ sudo rmmod safe_lkm
//
// ============================================================================
// IMPLEMENTATION DETAILS
// ============================================================================
//
// PRIORITY MECHANISM:
//   - Messages with type >= 5 are HIGH priority
//   - Messages with type < 5 are NORMAL priority
//   - Two separate queues (high and normal)
//   - High priority messages retrieved first
//   - FIFO order within each priority level
//
// THREAD SAFETY:
//   - Spinlocks protect concurrent access
//   - spin_lock_irqsave/restore for interrupt safety
//
// MEMORY MANAGEMENT:
//   - Dynamic allocation with kmalloc/kfree
//   - Automatic cleanup on module unload
//   - No memory leaks
//
// DATA STRUCTURES:
//   - Kernel linked lists (list_head)
//   - Separate high/normal priority queues
//
// ============================================================================
// End of IPC Priority Message Queue Module
// ============================================================================
