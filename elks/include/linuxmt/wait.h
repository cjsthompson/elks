#ifndef _LINUX_WAIT_H
#define _LINUX_WAIT_H

#define WNOHANG		0x00000001
#define WUNTRACED	0x00000002

#define wake_up(_a) _wake_up(_a,1)
#define wake_up_interruptible(_a) _wake_up(_a,0)

struct wait_queue {
	struct task_struct * task;
	struct wait_queue * next;
};

#ifdef __KERNEL__

struct select_table_entry {
	struct wait_queue wait;
	struct wait_queue ** wait_address;
};

typedef struct select_table_struct {
	int nr;
	struct select_table_entry * entry;
} select_table;

#define __MAX_SELECT_TABLE_ENTRIES 32

#endif /* __KERNEL__ */

#endif
