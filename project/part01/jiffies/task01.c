#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/jiffies.h>

#define BUFFER_SIZE 128
#define PROC_NAME "jiffies"

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
    rv = sprintf(buffer, "%lu", jiffies);

    size_t length = 0;
    char *ptr = buffer; // Pointer to traverse the buffer

    while (*ptr != '\0') // Traverse until null terminator
    {
        length++;
        ptr++; // Increment the pointer
    }

    // Append newline character and null terminator
    buffer[length] = '\n';
    buffer[length + 1] = '\0';

    /* copies kernel space buffer to user space usr buf */
    if (copy_to_user(usr_buf, buffer, rv + 1) != 0) // Include space for newline and null terminator
    {
        printk(KERN_ALERT "Error copying data to user\n");
        return -EFAULT; // Return an error code if copy fails
    }

    return rv + 1; // Include space for newline and null terminator
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello Module");
MODULE_AUTHOR("SGG");
