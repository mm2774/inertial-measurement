#ifndef __SHARED_STRUCTS_H__
#define __SHARED_STRUCTS_H__

/** Implement your structs here */

/**
 * This structure holds the process structure information
 */
struct process_state {
	/** stack pointer at last checkpoint */
	unsigned int *sp;
	/** needed for freeing properly */
	unsigned int *orig_sp;
	int n;
	/** pointer to next process in queue */
	struct process_state* next;
};

#endif
