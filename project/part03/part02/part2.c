#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A Kernel Module to generate the Collatz sequence");
MODULE_VERSION("0.1");

// Parameter passing
static int start = 25;
module_param(start, int, 0);
MODULE_PARM_DESC(start, "An integer parameter to start the Collatz sequence");

struct collatz_node
{
    int value;
    struct list_head list;
};

static LIST_HEAD(collatz_list);

static void generate_collatz_sequence(int start_value)
{
    struct collatz_node *new_node;
    int current_value = start_value;

    while (current_value != 1)
    {
        // Allocate memory for a new node
        new_node = kmalloc(sizeof(*new_node), GFP_KERNEL);
        if (!new_node)
        {
            printk(KERN_ERR "Memory allocation failed for collatz node\n");
            return;
        }

        // Set the value and initialize the list head
        new_node->value = current_value;
        INIT_LIST_HEAD(&new_node->list);

        // Add the node to the list
        list_add_tail(&new_node->list, &collatz_list);

        // Calculate the next value in the sequence
        if (current_value % 2 == 0)
        {
            current_value /= 2;
        }
        else
        {
            current_value = 3 * current_value + 1;
        }
    }

    // Add the final value 1
    new_node = kmalloc(sizeof(*new_node), GFP_KERNEL);
    if (!new_node)
    {
        printk(KERN_ERR "Memory allocation failed for collatz node\n");
        return;
    }
    new_node->value = 1;
    INIT_LIST_HEAD(&new_node->list);
    list_add_tail(&new_node->list, &collatz_list);
}

static int __init collatz_init(void)
{
    struct collatz_node *ptr;

    printk(KERN_INFO "Loading Collatz Module with start value: %d\n", start);

    generate_collatz_sequence(start);

    // Traverse and print the Collatz sequence
    list_for_each_entry(ptr, &collatz_list, list)
    {
        printk(KERN_INFO "Collatz sequence value: %d\n", ptr->value);
    }

    return 0;
}

static void __exit collatz_exit(void)
{
    struct collatz_node *ptr, *next;

    printk(KERN_INFO "Unloading Collatz Module\n");

    // Remove and free each element
    list_for_each_entry_safe(ptr, next, &collatz_list, list)
    {
        list_del(&ptr->list);
        printk(KERN_INFO "Removing Collatz value: %d\n", ptr->value);
        kfree(ptr);
    }
}

module_init(collatz_init);
module_exit(collatz_exit);
