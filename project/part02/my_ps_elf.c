#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sched/signal.h> // for_each_process, for_each_thread

/* Function prototypes */
int simple_init(void);
void simple_exit(void);
void dfs(struct task_struct *task);

/* Recursive function for DFS traversal */
void dfs(struct task_struct *task)
{
    struct task_struct *child;
    struct list_head *list;

    // Output task information
    printk(KERN_INFO "Command: %s,\t\tPID: %d\n", task->comm, task->pid);

    // Iterate over each child of the current task
    list_for_each(list, &task->children)
    {
        child = list_entry(list, struct task_struct, sibling);
        dfs(child); // Recursive call
    }
}

/* This function is called when the module is loaded. */
int simple_init(void)
{
    printk(KERN_INFO "Loading Kernel Module\n");

    // Start DFS from init_task (process 1)
    dfs(&init_task);

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
MODULE_DESCRIPTION("My PS -ELF");
MODULE_AUTHOR("KATTA");
