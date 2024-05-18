#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/jiffies.h>
#include <asm/param.h>

#define BUFFER_SIZE 128
#define PROC_NAME "seconds"

unsigned long start_jiffies, end_jiffies;
unsigned long elapsed_time_in_seconds; // Using unsigned long for elapsed time

ssize_t proc_read(struct file *file, char __user *usr_buf,
                  size_t count, loff_t *pos);

static struct proc_ops proc_ops = {
    .proc_read = proc_read,
};

int proc_init(void);
void proc_exit(void);

int proc_init(void)
{
    /* creates the /proc/hello entry */
    proc_create(PROC_NAME, 0666, NULL, &proc_ops);
    start_jiffies = jiffies;
    return 0;
}

void proc_exit(void)
{
    /* removes the /proc/hello entry */
    remove_proc_entry(PROC_NAME, NULL);
}

ssize_t proc_read(struct file *file, char __user *usr_buf,
                  size_t count, loff_t *pos)
{
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;

    if (completed)
    {
        completed = 0;
        return 0;
    }

    completed = 1;
    // Capture ending jiffy count
    end_jiffies = jiffies;

    // Calculate elapsed time in seconds (integer arithmetic)
    elapsed_time_in_seconds = (end_jiffies - start_jiffies) / HZ;

    rv = snprintf(buffer, BUFFER_SIZE, "%lu\n", elapsed_time_in_seconds); // Using snprintf for safer string formatting

    /* copies kernel space buffer to user space usr buf */
    if (copy_to_user(usr_buf, buffer, rv) != 0)
    {
        printk(KERN_ALERT "Error copying data to user\n");
        return -EFAULT;
    }

    return rv;
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("SECOND");
MODULE_AUTHOR("KATTA");
