#include <linuxmt/config.h>
/*#include <linuxmt/rd.h>*/
#include <linuxmt/major.h>
#include <linuxmt/kernel.h>
#include <linuxmt/debug.h>
#include <linuxmt/errno.h>

#ifdef CONFIG_BLK_DEV_SSD

#define MAJOR_NR 3 /* FLOPPY_MAJOR	/* As the're fairly similar in practice */

#define SSDDISK
#include "blk.h"

#define NUM_SECTS 256		/* 128K disk */
#define MEM_SIZE NUM_SECTS*32
#define SEG_SIZE 16

static int ssd_initialised = 0;

static int ssd_open(inode, filp);
static int ssd_release(inode, filp);
static int ssd_ioctl(inode, file, cmd, arg);

void rd_load() {}

static struct file_operations ssd_fops =
{
	NULL,			/* lseek */
	block_read,		/* read */
	block_write,		/* write */
	NULL,			/* readdir */
	NULL,			/* select */
	ssd_ioctl,		/* ioctl */
	ssd_open,		/* open */
	ssd_release,		/* release */
#ifdef BLOAT_FS
	NULL,			/* fsync */
	NULL,			/* check_media_change */
	NULL,			/* revalidate */
#endif
};

void ssd_init()
{
	int i;

	printk("SSD driver (Major = %d)\n", MAJOR_NR);
	if ((i = register_blkdev(MAJOR_NR, DEVICE_NAME, &ssd_fops)) == 0) {
		blk_dev[MAJOR_NR].request_fn = DEVICE_REQUEST;
		/* blksize_size[MAJOR_NR] = 1024; */
		/* read_ahead[MAJOR_NR] = 2; */
		ssd_initialised = 1;
	} else {
		printk("SSD failed to register.\n");
	}
}
	
static int ssd_open(inode, filp)
struct inode *inode;
struct file *filp;
{
	int target;

	target = DEVICE_NR(inode->i_rdev);
	/*printk("SSD_OPEN %d\n",target);*/
	if (ssd_initialised == 0)
		return (-ENXIO);
/*	if (rd_busy[target])
		return (-EBUSY); */
	return 0;
}

static int ssd_release(inode, filp)
struct inode *inode;
struct file *filp;
{
	printk("SSD_RELEASE \n");
	return 0;
}

static int ssd_ioctl(inode, file, cmd, arg)
register struct inode *inode;
struct file *file;
unsigned int cmd;
unsigned int arg;
{
	int target = DEVICE_NR(inode->i_rdev);
/*
	if (!suser())
		return -EPERM;
	printd_ssd2("SSD_IOCTL %d %s\n", target, (cmd ? "kill" : "make"));
	switch(cmd) {
		case SSDCREATE:
			if (rd_segment[target]) {
				return -EBUSY;
			} else if ((rd_segment[target] = mm_alloc(MEM_SIZE,0)) == -1)
				return -ENOMEM;
			fmemset(0, rd_segment[target], 0, MEM_SIZE * SEG_SIZE);
			return 0;
			break;
		case SSDDESTROY:
			if (rd_segment[target]) {
				mm_free(rd_segment[target]);
				rd_segment[target] = NULL;
				invalidate_inodes(inode->i_rdev);
				invalidate_buffers(inode->i_rdev);
				return 0;
			} else
				return -EINVAL;
			break;
	}*/
	return -EINVAL;
}

static void do_ssd_request()
{
	unsigned long count;
	unsigned long start;
	register char *buff;
	int target;

	while(1) {
		if (!CURRENT || CURRENT->rq_dev <0)
			return;

		INIT_REQUEST;

		if (CURRENT == NULL || CURRENT->rq_sector == -1)
			return;

		if (ssd_initialised != 1) {
			printk("SSD not initilised");
			end_request(0);
			continue;
		}

		/* Remember 1 sector = 512 bytes */
		count = 2 /*CURRENT->rq_nr_sectors*/;
		start = CURRENT->rq_sector;
		buff = CURRENT->rq_buffer;
	
		/* Devis minor */
		target = DEVICE_NR(CURRENT->rq_dev);

		if ((start >= NUM_SECTS) || (start + count >= NUM_SECTS)) {
			/* too big for disk or disk not active */
			printk("Illegal Request\n");
			end_request(0);
			continue;
		}
		if (CURRENT->rq_cmd == WRITE) {
			printk("SSD_REQUEST writing to %ld size %ld\n", start,count);
			ssd_write_blk(target, start, buff, count );
		}
		if (CURRENT->rq_cmd == READ) {
			ssd_read_blk(target, start, buff, count );
		}
		end_request(1);
	}
}

ssd_write_blk(target, start, buff, count )
int target;
unsigned long start;
register char *buff;
unsigned long count;
{
	/* write a number of sectors onto ssd */
}

ssd_read_blk(target, start, buff, count )
int target;
unsigned long start;
register char *buff;
unsigned long count;
{
	/* read a number of sectors from ssd */
	unsigned int address_high, address_low;
	unsigned int loop;	
	char * destination;

	destination = buff;

	address_high = (unsigned int)(start >> 7); /* Start * 512/65536 */
	address_low = (unsigned int)((start & 0x7F) << 9);

	/*printk("SSD high = %x, low %x\n", address_high, address_low);*/

	for (loop = 0 ; loop < (count * 512); loop ++)
	{
		*destination = ssd_read4(address_high, (address_low + loop));
		destination++;
	}
}

#endif /* CONFIG_BLK_DEV_SSD */