uni#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

unsigned long *__sys_call_table = (unsigned long*)kallsyms_lookup_name("sys_call_table");

static int __init startup(void){

    printk(KERN_INFO "The Kernel is initiating!\n");

    return 0;
}

static void __exit mod_exit(void){

    printk(KERN_NOTICE "Kernel will now exit.\n");
}

MODULE_LICENSE("GPL");

module_init(startup);
module_exit(mod_exit);