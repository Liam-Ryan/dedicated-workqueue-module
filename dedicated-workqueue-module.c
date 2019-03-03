#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/slab.h>

struct workqueue_struct *wq;

struct work_data {
	struct work_struct my_work;
	int data;
};

static void do_work(struct work_struct *work)
{
	struct work_data *data = container_of(work, struct work_data, my_work);
	printk("Work queue module handler %s, data is %d\n", __FUNCTION__, data->data);
	kfree(data);
}

static int __init onload(void)
{
	struct work_data *data;
	printk("Work queue module init %s %d\n", __FUNCTION__, __LINE__);
	wq = create_singlethread_workqueue("my single thread workqueue");
	data = kmalloc(sizeof(struct work_data), GFP_KERNEL);
	data->data = 42;
	INIT_WORK(&data->my_work, do_work);
	queue_work(wq, &data->my_work);
	return 0;
}

static void __exit onunload(void)
{
	flush_workqueue(wq);
	destroy_workqueue(wq);
	printk("Work queue module exit %s %d\n", __FUNCTION__, __LINE__);
}

module_init(onload);
module_exit(onunload);
MODULE_AUTHOR("Liam Ryan <liamryandev@gmail.com>");
MODULE_LICENSE("GPL");
