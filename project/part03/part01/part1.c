#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/list.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A Kernel Module to manage a linked list of colors");
MODULE_VERSION("0.1");

struct color
{
    int red;
    int blue;
    int green;
    struct list_head list;
};

static LIST_HEAD(color_list);

static int __init color_list_init(void)
{
    struct color *violet, *indigo, *blue, *green;
    struct color *ptr;

    printk(KERN_INFO "Loading Color List Module...\n");

    // Create and initialize four colors
    violet = kmalloc(sizeof(*violet), GFP_KERNEL);
    violet->red = 138;
    violet->blue = 43;
    violet->green = 226;
    INIT_LIST_HEAD(&violet->list);
    list_add_tail(&violet->list, &color_list);

    indigo = kmalloc(sizeof(*indigo), GFP_KERNEL);
    indigo->red = 75;
    indigo->blue = 0;
    indigo->green = 130;
    INIT_LIST_HEAD(&indigo->list);
    list_add_tail(&indigo->list, &color_list);

    blue = kmalloc(sizeof(*blue), GFP_KERNEL);
    blue->red = 0;
    blue->blue = 0;
    blue->green = 255;
    INIT_LIST_HEAD(&blue->list);
    list_add_tail(&blue->list, &color_list);

    green = kmalloc(sizeof(*green), GFP_KERNEL);
    green->red = 0;
    green->blue = 255;
    green->green = 0;
    INIT_LIST_HEAD(&green->list);
    list_add_tail(&green->list, &color_list);

    // Traverse and print the list
    list_for_each_entry(ptr, &color_list, list)
    {
        printk(KERN_INFO "Color - R: %d, G: %d, B: %d\n", ptr->red, ptr->green, ptr->blue);
    }

    return 0;
}

static void __exit color_list_exit(void)
{
    struct color *ptr, *next;

    printk(KERN_INFO "Unloading Color List Module...\n");

    // Remove and free each element
    list_for_each_entry_safe(ptr, next, &color_list, list)
    {
        list_del(&ptr->list);
        printk(KERN_INFO "Removing Color - R: %d, G: %d, B: %d\n", ptr->red, ptr->green, ptr->blue);
        kfree(ptr);
    }
}

module_init(color_list_init);
module_exit(color_list_exit);
