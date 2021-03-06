/*
 * Memalloc, encoder memory allocation driver (kernel module)
 *
 * Copyright (C) 2011  Hantro Products Oy.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
--------------------------------------------------------------------------------
--
--  Abstract : Allocate memory blocks
--
------------------------------------------------------------------------------*/

#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

/* Our header */
#include "memalloc.h"

/* module description */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hantro Products Oy");
MODULE_DESCRIPTION("RAM allocation");

#ifndef HLINA_START_ADDRESS
/* SPEAr1340: start at 256 MiB (default settings) */
#define HLINA_START_ADDRESS 0x10000000
#endif

#define MAX_OPEN 32
#define ID_UNUSED 0xFF
#define MEMALLOC_BASIC 0
#define MEMALLOC_MAX_OUTPUT 1
#define MEMALLOC_BASIC_X2 2
#define MEMALLOC_BASIC_AND_16K_STILL_OUTPUT 3
#define MEMALLOC_BASIC_AND_MVC_DBP 4
#define MEMALLOC_BASIC_AND_4K_OUTPUT 5
#define MEMALLOC_SPEAR1340_30MiB 6
#define MEMALLOC_SPEAR1340_48MiB 7
#define MEMALLOC_SPEAR1340_64MiB 8
#define MEMALLOC_SPEAR1340_128MiB 9
#define MEMALLOC_SPEAR1340_256MiB 10

/* selects the memory allocation method,i.e. which allocation
scheme table is used by default */
unsigned int alloc_method = MEMALLOC_SPEAR1340_64MiB;

static const char memalloc_dev_name[] = "memalloc";

static int memalloc_major;

int id[MAX_OPEN] = { ID_UNUSED };

static struct memalloc_dev device;

/* module_param(name, type, perm) */
module_param(alloc_method, uint, 0);

/* here's all the must remember stuff */
struct allocation {
	struct list_head list;
	void *buffer;
	unsigned int order;
	int fid;
};

struct list_head heap_list;

static DEFINE_SPINLOCK(mem_lock);

struct hlina_chunk {
	unsigned int bus_address;
	unsigned int used;
	unsigned int size;
	int file_id;
};

static unsigned int *size_table;
static size_t chunks;

unsigned int size_table_0[] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1,
	4, 4, 4, 4, 4, 4, 4, 4,
	10, 10, 10, 10,
	22, 22, 22, 22,
	38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38,
	50, 50, 50, 50, 50, 50, 50,
	75, 75, 75, 75, 75,
	86, 86, 86, 86, 86,
	113, 113,
	152, 152,
	162, 162, 162,
	270, 270, 270,
	403, 403, 403, 403,
	403, 403,
	450, 450,
	893, 893,
	893, 893,
	1999,
	3997,
	4096,
	8192
};

unsigned int size_table_1[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0,
	0, 0,
	0, 0, 0,
	0, 0, 0,
	0, 0,
	0, 64,
	64, 128,
	512,
	3072,
	8448
};

unsigned int size_table_2[] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
	10, 10, 10, 10, 10, 10, 10, 10,
	22, 22, 22, 22, 22, 22, 22, 22,
	38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38,
	38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38,
	50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
	75, 75, 75, 75, 75, 75, 75, 75, 75, 75,
	86, 86, 86, 86, 86, 86, 86, 86, 86, 86,
	113, 113, 113, 113,
	152, 152, 152, 152,
	162, 162, 162, 162, 162, 162,
	270, 270, 270, 270, 270, 270,
	403, 403, 403, 403, 403, 403, 403, 403,
	403, 403, 403, 403,
	450, 450, 450, 450,
	893, 893, 893, 893,
	893, 893, 893, 893,
	1999, 1999,
	3997, 3997,
	4096, 4096,
	8192, 8192
};

unsigned int size_table_3[] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1,
	4, 4, 4, 4, 4, 4, 4, 4,
	10, 10, 10, 10,
	22, 22, 22, 22,
	38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38,
	50, 50, 50, 50, 50, 50, 50,
	75, 75, 75, 75, 75,
	86, 86, 86, 86, 86,
	113, 113,
	152, 152,
	162, 162, 162,
	270, 270, 270,
	403, 403, 403, 403,
	403, 403,
	450, 450,
	893, 893,
	893, 893,
	1999,
	3997,
	4096,
	8192,
	19000
};

unsigned int size_table_4[] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1,
	4, 4, 4, 4, 4, 4, 4, 4,
	10, 10, 10, 10,
	22, 22, 22, 22,
	38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38,
	50, 50, 50, 50, 50, 50, 50,
	75, 75, 75, 75, 75,
	86, 86, 86, 86, 86,
	113, 113,
	152, 152,
	162, 162, 162,
	270, 270, 270,
	403, 403, 403, 403,
	403, 403,
	450, 450,
	893, 893,
	893, 893,
	1999,
	3997,
	3997, 3997, 3997, 3997, 3997,
	4096,
	8192,
};

unsigned int size_table_5[] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1,
	4, 4, 4, 4, 4, 4, 4, 4,
	10, 10, 10, 10,
	22, 22, 22, 22,
	38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38,
	50, 50, 50, 50, 50, 50, 50,
	75, 75, 75, 75, 75,
	86, 86, 86, 86, 86,
	113, 113,
	152, 152,
	162, 162, 162,
	270, 270, 270,
	403, 403, 403, 403, 403, 403,
	450, 450,
	893, 893, 893, 893,
	1999,
	3997,
	4096,
	7200, 7200, 7200, 7200,
	14000,
	17400, 17400
};

unsigned int size_table_SPEAR1340_30MiB[] = {
	1, 1, 1, 1, 1, 1, 1, 1,
	4, 4, 4, 4, 4, 4, 4, 4,
	10, 10, 10, 10, 10, 10,
	22, 22, 22,
	38, 38, 38,
	50, 50, 50, 50, 50,
	60, 60, 60, 60, 60,
	75, 75, 75, 75,
	113,
	403, 403, 403, 403, 403, 403, 403, 403, 403, 403,
	516, 516, 516, 516
}; /* ~ 29MiB */ /* 7337 * 4 = 29348 KiB*/

unsigned int size_table_SPEAR1340_48MiB[] = {
	1, 1, 1, 1, 1, 1, 1, 1,
	4, 4, 4, 4, 4, 4, 4, 4,
	10, 10, 10, 10, 10, 10,
	22, 22, 22,
	38, 38, 38,
	50, 50, 50, 50, 50,
	60, 60, 60, 60, 60,
	75, 75, 75, 75,
	113,
	403, 403, 403, 403, 403, 403, 403, 403, 403, 403,
	516, 516, 516, 516,
	893, 893,
	1500, 1500
};/* (4786+7337) * 4 = 48492 KiB*/

unsigned int size_table_SPEAR1340_64MiB[] = {
	1, 1, 1, 1, 1, 1, 1, 1,
	4, 4, 4, 4, 4, 4, 4, 4,
	10, 10, 10, 10, 10, 10,
	22, 22, 22,
	38, 38, 38,
	50, 50, 50, 50, 50,
	60, 60, 60, 60, 60,
	75, 75, 75, 75,
	113,
	403, 403, 403, 403, 403, 403, 403, 403, 403, 403,
	516, 516, 516, 516, 516,
	893, 893, 893, 893,
	1500, 1500, 1500
};/* (3802+4786+7337) * 4 = 15925*4 = 63700 KiB*/

unsigned int size_table_SPEAR1340_128MiB[] = {
	1, 1, 1, 1, 1, 1, 1, 1,
	4, 4, 4, 4, 4, 4, 4, 4,
	10, 10, 10, 10, 10, 10,
	22, 22, 22,
	38, 38, 38,
	50, 50, 50, 50, 50,
	60, 60, 60, 60, 60,
	75, 75, 75, 75,
	113,
	403, 403, 403, 403, 403, 403, 403, 403, 403, 403,
	516, 516, 516, 516, 516,
	893, 893, 893, 893,
	1500, 1500, 1500,
	3997,
	4096,
	8192
};/* (15925 + 16285) * 4 = 32210*4 = 128840 KiB ~= 125.8 MiB*/

unsigned int size_table_SPEAR1340_256MiB[] = {
	1, 1, 1, 1, 1, 1, 1, 1,
	4, 4, 4, 4, 4, 4, 4, 4,
	10, 10, 10, 10, 10, 10,
	22, 22, 22,
	38, 38, 38,
	50, 50, 50, 50, 50,
	60, 60, 60, 60, 60,
	75, 75, 75, 75,
	113,
	403, 403, 403, 403, 403, 403, 403, 403, 403, 403,
	516, 516, 516, 516, 516,
	893, 893, 893, 893,
	1500, 1500, 1500,
	1999, 1999,
	3997, 3997,
	4096, 4096, 4096,
	8192, 8192, 8192
};/* (15925 + 48856) * 4 = 64781*4 = 259124 KiB ~= 253 MiB*/

/*static hlina_chunk hlina_chunks[256];*/
static struct hlina_chunk hlina_chunks[256];

static int AllocMemory(unsigned *busaddr, unsigned int size, struct file *filp);
static int FreeMemory(unsigned long busaddr);
static void ResetMems(void);


static long memalloc_ioctl(struct file *filp, unsigned int cmd,
			unsigned long arg)
{
	int err = 0;
	int ret;

	PDEBUG("ioctl cmd 0x%08x\n", cmd);

	if (filp == NULL || arg == 0)
		return -EFAULT;

	/*
	 * extract the type and number bitfields, and don't decode
	 * wrong cmds: return ENOTTY (inappropriate ioctl) before access_ok()
	 */
	if (_IOC_TYPE(cmd) != MEMALLOC_IOC_MAGIC)
		return -ENOTTY;
	if (_IOC_NR(cmd) > MEMALLOC_IOC_MAXNR)
		return -ENOTTY;

	if (_IOC_DIR(cmd) & _IOC_READ)
		err = !access_ok(VERIFY_WRITE, (void *) arg, _IOC_SIZE(cmd));
	else if (_IOC_DIR(cmd) & _IOC_WRITE)
		err = !access_ok(VERIFY_READ, (void *) arg, _IOC_SIZE(cmd));
	if (err)
		return -EFAULT;

	switch (cmd) {
	case MEMALLOC_IOCHARDRESET:

		PDEBUG("HARDRESET\n");
		ResetMems();

		break;

	case MEMALLOC_IOCXGETBUFFER:
		{
			int result;
			struct MemallocParams memparams;

			PDEBUG("GETBUFFER\n");
			spin_lock(&mem_lock);

			if (__copy_from_user(&memparams, (const void *) arg,
						sizeof(memparams)))
				return -EFAULT;

			result = AllocMemory(&memparams.busAddress,
					memparams.size, filp);

			if (__copy_to_user((void *) arg, &memparams,
						sizeof(memparams)))
				return -EFAULT;

			spin_unlock(&mem_lock);

			return result;
		}
	case MEMALLOC_IOCSFREEBUFFER:
		{

			unsigned long busaddr;

			PDEBUG("FREEBUFFER\n");
			spin_lock(&mem_lock);
			__get_user(busaddr, (unsigned long *) arg);
			ret = FreeMemory(busaddr);

			spin_unlock(&mem_lock);
			return ret;
		}
	}
	return 0;
}

static int memalloc_open(struct inode *inode, struct file *filp)
{
	int i = 0;

	for (i = 0; i < MAX_OPEN + 1; i++) {
		if (i == MAX_OPEN)
			return -1;
		if (id[i] == ID_UNUSED) {
			id[i] = i;
			filp->private_data = id + i;
			break;
		}
	}
	PDEBUG("dev opened\n");
	return 0;

}

static int memalloc_release(struct inode *inode, struct file *filp)
{

	int i = 0;

	for (i = 0; i < chunks; i++) {
		if (hlina_chunks[i].file_id == *((int *) filp->private_data)) {
			hlina_chunks[i].used = 0;
			hlina_chunks[i].file_id = ID_UNUSED;
		}
	}
	*((int *) filp->private_data) = ID_UNUSED;
	PDEBUG("dev closed\n");
	return 0;
}

/* VFS methods */
static const struct file_operations memalloc_fops = {
	.open		= memalloc_open,
	.release	= memalloc_release,
	.unlocked_ioctl	= memalloc_ioctl,
};


int memalloc_sysfs_register(struct memalloc_dev *device, dev_t dev,
			const char *memalloc_dev_name)
{
	int err = 0;
	struct device *mdev;

	device->memalloc_class = class_create(THIS_MODULE, memalloc_dev_name);
	if (IS_ERR(device->memalloc_class)) {
		err = PTR_ERR(device->memalloc_class);
		goto init_class_err;
	}
	mdev = device_create(device->memalloc_class, NULL, dev, NULL,
			memalloc_dev_name);
	if (IS_ERR(mdev))	{
		err = PTR_ERR(mdev);
		goto init_mdev_err;
	}

	/* Success! */
	return 0;

	/* Error handling */
init_mdev_err:
	class_destroy(device->memalloc_class);
init_class_err:

	return err;
}



int __init memalloc_init(void)
{
	int result;
	int i = 0;
	dev_t dev = 0;

	memalloc_major = 0; /* dynamic if set to 0 */
	size_table = NULL;
	chunks = 0;

	PDEBUG("module init\n");
	pr_info("memalloc: 8190 Linear Memory Allocator, %s\n", "Rev. 1.2");
	pr_info("memalloc: linear memory base = 0x%08x\n", HLINA_START_ADDRESS);

	switch (alloc_method) {

	case MEMALLOC_MAX_OUTPUT:
		size_table = size_table_1;
		chunks = (sizeof(size_table_1) / sizeof(*size_table_1));
		pr_info("memalloc: allocation method: MEMALLOC_MAX_OUTPUT\n");
		break;
	case MEMALLOC_BASIC_X2:
		size_table = size_table_2;
		chunks = (sizeof(size_table_2) / sizeof(*size_table_2));
		pr_info("memalloc: allocation method: MEMALLOC_BASIC x 2\n");
		break;
	case MEMALLOC_BASIC_AND_16K_STILL_OUTPUT:
		size_table = size_table_3;
		chunks = (sizeof(size_table_3) / sizeof(*size_table_3));
		pr_info("memalloc: allocation method: MEMALLOC_BASIC_AND_16K_STILL_OUTPUT\n");
		break;
	case MEMALLOC_BASIC_AND_MVC_DBP:
		size_table = size_table_4;
		chunks = (sizeof(size_table_4) / sizeof(*size_table_4));
		pr_info("memalloc: allocation method: MEMALLOC_BASIC_AND_MVC_DBP\n");
		break;
	case MEMALLOC_BASIC_AND_4K_OUTPUT:
		size_table = size_table_5;
		chunks = (sizeof(size_table_5) / sizeof(*size_table_5));
		pr_info("memalloc: allocation method: MEMALLOC_BASIC_AND_4K_OUTPUT\n");
		break;
	case MEMALLOC_SPEAR1340_30MiB:
		size_table = size_table_SPEAR1340_30MiB;
		chunks = (sizeof(size_table_SPEAR1340_30MiB) /
			sizeof(*size_table_SPEAR1340_30MiB));
		pr_info("memalloc: allocation method: MEMALLOC_SPEAR1340_30MiB\n");
		break;
	case MEMALLOC_SPEAR1340_48MiB:
		size_table = size_table_SPEAR1340_48MiB;
		chunks = (sizeof(size_table_SPEAR1340_48MiB) /
			sizeof(*size_table_SPEAR1340_48MiB));
		pr_info("memalloc: allocation method: MEMALLOC_SPEAR1340_48MiB\n");
		break;
	case MEMALLOC_SPEAR1340_64MiB:
		size_table = size_table_SPEAR1340_64MiB;
		chunks = (sizeof(size_table_SPEAR1340_64MiB) /
			sizeof(*size_table_SPEAR1340_64MiB));
		pr_info("memalloc: allocation method: MEMALLOC_SPEAR1340_64MiB\n");
		break;
	case MEMALLOC_SPEAR1340_128MiB:
		size_table = size_table_SPEAR1340_128MiB;
		chunks = (sizeof(size_table_SPEAR1340_128MiB) /
			sizeof(*size_table_SPEAR1340_128MiB));
		pr_info("memalloc: allocation method: MEMALLOC_SPEAR1340_128MiB\n");
		break;
	case MEMALLOC_SPEAR1340_256MiB:
		size_table = size_table_SPEAR1340_256MiB;
		chunks = (sizeof(size_table_SPEAR1340_256MiB) /
			sizeof(*size_table_SPEAR1340_256MiB));
		pr_info("memalloc: allocation method: MEMALLOC_SPEAR1340_256MiB\n");
		break;
	default:
		size_table = size_table_0;
		chunks = (sizeof(size_table_0) / sizeof(*size_table_0));
		pr_info("memalloc: allocation method: MEMALLOC_BASIC\n");
		break;
	}


	if (0 == memalloc_major) {
		/* auto select a major */
		result = alloc_chrdev_region(&dev, 0, 1, memalloc_dev_name);
		memalloc_major = MAJOR(dev);
	} else {
		/* use load time defined major number */
		dev = MKDEV(memalloc_major, 0);
		result = register_chrdev_region(dev, 1, memalloc_dev_name);
	}

	if (result)
		goto init_chrdev_err;

	memset(&device, 0, sizeof(device));

	/* initialize our char dev data */
	cdev_init(&device.cdev, &memalloc_fops);
	device.cdev.owner = THIS_MODULE;
	device.cdev.ops = &memalloc_fops;

	/* register char dev with the kernel */
	result = cdev_add(&device.cdev, dev, 1/*count*/);
	if (result)
		goto init_cdev_err;

	result = memalloc_sysfs_register(&device, dev, memalloc_dev_name);
	if (result)
		goto init_sysfs_err;

	ResetMems();

	/* We keep a register of out customers, reset it */
	for (i = 0; i < MAX_OPEN; i++)
		id[i] = ID_UNUSED;

	return 0;

init_sysfs_err:
	cdev_del(&device.cdev);
init_cdev_err:
	unregister_chrdev_region(dev, 1/*count*/);
init_chrdev_err:
	PDEBUG("memalloc: module not inserted\n");
	return -EFAULT;
}

void __exit memalloc_cleanup(void)
{

	PDEBUG("clenup called\n");

	unregister_chrdev(memalloc_major, "memalloc");

	PDEBUG("memalloc: module removed\n");
	return;
}

module_init(memalloc_init);
module_exit(memalloc_cleanup);

/* Cycle through the buffers we have, give the first free one */
static int AllocMemory(unsigned *busaddr, unsigned int size, struct file *filp)
{

	int i = 0;

	*busaddr = 0;

	for (i = 0; i < chunks; i++) {
		if (!hlina_chunks[i].used && (hlina_chunks[i].size >= size)) {
			*busaddr = hlina_chunks[i].bus_address;
			hlina_chunks[i].used = 1;
			hlina_chunks[i].file_id = *((int *) filp->private_data);
			break;
		}
	}

	if (*busaddr == 0)
		pr_info("memalloc: Allocation FAILED: size = %d\n",
			size);
	else
		PDEBUG("MEMALLOC OK: size: %d, size reserved: %d\n", size,
			   hlina_chunks[i].size);

	return 0;
}

/* Free a buffer based on bus address */
static int FreeMemory(unsigned long busaddr)
{
	int i = 0;

	for (i = 0; i < chunks; i++) {
		if (hlina_chunks[i].bus_address == busaddr) {
			hlina_chunks[i].used = 0;
			hlina_chunks[i].file_id = ID_UNUSED;
		}
	}

	return 0;
}

/* Reset "used" status */
void ResetMems(void)
{
	int i = 0;
	unsigned int ba = HLINA_START_ADDRESS;

	for (i = 0; i < chunks; i++) {

		hlina_chunks[i].bus_address = ba;
		hlina_chunks[i].used = 0;
		hlina_chunks[i].file_id = ID_UNUSED;
		hlina_chunks[i].size = 4096 * size_table[i];

		ba += hlina_chunks[i].size;
	}

	pr_info("memalloc: %d bytes (%dMB) configured.\n",
		   ba - (unsigned int)(HLINA_START_ADDRESS),
		  (ba - (unsigned int)(HLINA_START_ADDRESS)) / (1024 * 1024));

	if (ba - (unsigned int)(HLINA_START_ADDRESS) > 96 * 1024 * 1024)
		PDEBUG("MEMALLOC ERROR: MEMORY ALLOC BUG\n");

}
