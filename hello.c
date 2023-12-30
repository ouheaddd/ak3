#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/printk.h>
#include <linux/errno.h>
#include <linux/list.h>

MODULE_AUTHOR("Kramar Mykyta");
MODULE_DESCRIPTION("hello module for lab3");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int hello_counter = 1;
module_param(hello_counter, uint, 0644);
MODULE_PARM_DESC(hello_counter, "counter of 'Hello world' printing");

struct lst_obj {
	struct list_head list;
	ktime_t timestamp;
};
static LIST_HEAD(lst_head);

static int __init hello_init(void)
{
	if (hello_counter > 10) {
		pr_err("Error! hello_counter > 10\n");
		return -EINVAL;
	}
	else if (hello_counter == 0 || (hello_counter >= 5 && hello_counter <= 10))
		pr_warn("Warning! hello_counter == 0 or 5 <= hello_counter <= 10\n");

	int i;

	for (i = 0; i < hello_counter; i++) {
		struct lst_obj *lst_elem = kmalloc(sizeof(*lst_elem), GFP_KERNEL);

		lst_elem->timestamp = ktime_get();
		list_add(&lst_elem->list, &lst_head);
		printk(KERN_EMERG "Hello, world!\n");
	}

	return 0;
}

static void __exit hello_exit(void)
{
	struct lst_obj *lst_elem, *temp;

	list_for_each_entry_safe(lst_elem, temp, &lst_head, list) {
		pr_info("Time stamp: %lld\n", lst_elem->timestamp);
		list_del(&lst_elem->list);
		kfree(lst_elem);
	}
}

module_init(hello_init);
module_exit(hello_exit);
