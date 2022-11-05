#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include "test.h"

unsigned long (*func)(unsigned long) = NULL;
unsigned long arg = 0;
unsigned long res = 0;

static long test_ioctl(struct file* file, unsigned int cmd, unsigned long argp) {
    printk(KERN_INFO "test.c: test_ioctl(file: %lx, cmd: %x, argp: %lx)\n", 
        file, cmd, argp);

    switch (cmd)
    {
        case IOCTL_DO:
            printk(KERN_INFO "test.c: function: %lx, arg: %lx\n",
                func, arg);

            res = func(arg);
            break;
        
        case IOCTL_SET_FUNCTION:
            printk(KERN_INFO "test.c: function = %lx\n",
                argp);

            func = argp;
            break;

        case IOCTL_SET_ARGUMENT:
            printk(KERN_INFO "test.c: arg = %lx\n",
                argp);

            arg = argp;
            break;
        case IOCTL_GET_RESPONSE:
            printk(KERN_INFO "test.c: res: %lx",
                res);

            copy_to_user(__user (unsigned long*)argp, &res, sizeof(unsigned long));
            break;
        default:
            break;
    }

    return 0;
}

static int test_open(struct inode* inode, struct file* file) {
    printk(KERN_INFO "test.c: test_open(inode: %lx, file: %lx)\n",
        inode, file);

    return 0;
}

static int test_close(struct inode* inode, struct file* file) {
    printk(KERN_INFO "test.c: test_close(inode: %lx, file: %lx)\n",
        inode, file);

    return 0;
}

static int test_uevent(struct device *dev, struct kobj_uevent_env *env)
{
    add_uevent_var(env, "DEVMODE=%#o", 0666);
    return 0;
}

static struct file_operations fops = {
    .open           = test_open,
    .release        = test_close,
    .unlocked_ioctl = test_ioctl
};

static dev_t dev_id;
static struct class* class;

static int __init module_initialize(void) { 
    printk(KERN_INFO "test.c: module_initialize(void)\n");

    dev_id = register_chrdev(0, DEVICE_NAME, &fops);
    class = class_create(THIS_MODULE, DEVICE_NAME);
    class->dev_uevent = test_uevent;
    device_create(class, NULL, MKDEV(dev_id, 0), NULL, DEVICE_NAME);

    return 0;
}

static void __exit module_cleanup(void) {
    printk(KERN_INFO "test.c: module_cleanup(void)\n");

    device_destroy(class, MKDEV(dev_id, 0));
    class_destroy(class);
    unregister_chrdev(dev_id, DEVICE_NAME);

    return;
}

module_init(module_initialize);
module_exit(module_cleanup);

MODULE_LICENSE("GPL");