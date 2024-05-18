#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>

/* Function prototypes */
int simple_init(void);
void simple_exit(void);

/* This function is called when the module is loaded. */
int simple_init(void)
{
    struct task_struct *task;

    printk(KERN_INFO "Loading Kernel Module\n");

    // Iterate through all processes
    for_each_process(task)
    {
        // Output task information
        printk(KERN_INFO "Command: %s,\t\tPID: %d\n", task->comm, task->pid);
    }

    return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void)
{
    printk(KERN_INFO "Removing Kernel Module\n");
}

/* Macros for registering module entry and exit points. */
module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("My PS -EL");
MODULE_AUTHOR("Your Name");
