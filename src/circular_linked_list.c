/*
 * src/circular_linked_list.c: Circular linked list implementation in C
 *
 * St: 2016-09-28 Wed 05:26 PM
 * Up: 2016-09-28 Wed 11:15 PM
 *
 * Author: SPS
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

#include"mylib.h"

#define SKIP

/*
 * Create a Circular linked list
 *
 * @cpy: Copy function
 * @cmp: Compare function
 * @dval: Funciton to destroy `val' of a node
 */
struct cll *cll_create(void *(*cpy)(void *),
                       int (*cmp)(void *, void *),
		       void (*dval)(void *),
		       void (*printl)(struct cll_node *))
{
	struct cll *l;

	l = malloc(sizeof(struct cll));
	assert(l);

	l->head = NULL;
	l->tail = NULL;
	l->cpy = cpy;
	l->cmp = cmp;
	l->dval = dval;
	l->printl = printl;
	l->nmemb = 0;

	return l;
}

/*
 * Create a cll_node
 *
 * @val: value of the node
 */
static struct cll_node *cll_node_create(struct cll *l, void *val)
{
	struct cll_node *clln;

	clln = malloc(sizeof(struct cll_node));
	assert(clln);

	clln->val = l->cpy(val);

	clln->next = NULL;

	return clln;
}

/*
 * Insert a new value to linked list. This is different from
 * insertion in an ordinary singly linked list. Here, the last
 * node will point to the first node. So no node will have a
 * null pointer.
 *
 * @l: linked list
 * @val: new value to be inserted to the linked list
 */
void cll_insert(struct cll *l, void *val)
{
	struct cll_node *clln;

	assert(l);

	clln = cll_node_create(l, val);

	/*
	 * If the list is empty do the following:
	 *
	 * 1. Make the new node point to itself.
	 * 2. Make head point to the new node.
	 * 3. Make tail point to the new node.
	 *
	 * The result should be like seen below.
	 *
	 *
	 *   +------+            +------+
	 *   | tail |---+        | head |------+
	 *   +------+   |        +------+      |
	 *              |                      |
	 *              +------------------+   |
	 *                                 |   |
	 *                                 v   v
         *                               +----------+
	 *                         +---> | new_node |-----+
	 *                         |     +----------+     |
	 *                         |                      | 
	 *                         +----------------------+ 
	 *
	 *
	 * Else (if list is not empty) do the following:
	 *
	 * 1. Make the new_node point to where the old_head node was pointing.
	 * 2. Make tail point to old_head node.
	 * 3. Make the old_head node point to the new_node.
	 * 4. Make head point to the new_node.
	 *
	 * The result should be like seen below.
	 *
	 *
	 *	+------+                  +------+
	 *      | tail |----+             | head |-------------------+
	 *      +------+    |             +------+                   |
	 *                  |                                        |
         *                  +----------------------+                 |
         *                                         |                 |
	 *                                         v                 v
	 *      +--------+   +----------+   +---------------+    +----------+             
	 *  +-->| node-n |-->| node-n+1 |-->| old_head-node |--->| new_node |--+
	 *  |   +--------+   +----------+   +---------------+    +----------+  |
	 *  |                                                                  |
	 *  +------------------------------------------------------------------+
	 *
	 */

	if (l->head == NULL) {
		clln->next = clln;
		l->head = clln;
		l->tail = clln;
	} else {
		clln->next = l->head->next;
		l->tail = l->head;
		l->head->next = clln;
		l->head = clln;
	}

	l->nmemb++;
}

/*
 * Search for a val in linked list
 *
 * @l: Pointer to linked list
 * @val: Pointer to value to search
 */
int cll_search(struct cll *l, void *val)
{
	int retval;
	struct cll_node *cur;

	assert(l);
	assert(val);

	retval = 0;

	/* Return 0 if empty list */	
	if (l->head == NULL)
		return 0;

	/* 
	 * If list is not empty, search for
	 * the search item in the list.
	 */
	cur = l->head;
	do {
		if (l->cmp(cur->val, val) == 0) {
			retval = 1;
			break;
		}
		cur = cur->next;
	} while(cur != l->head);

	return retval;
}

/*
 * Remove a node from linked list
 *
 * If the node to be removed is head or tail, extra steps must be
 * performed.
 *
 * In case of head node perform following additional steps:
 *
 * 1. Check if it is also the tail node. If yes, then this is
 *    the only node in list. If yes, make head and tail point
 *    to NULL (to indicate that list is empty). Nothing else
 *    has to be done, so skip the remaining steps.
 *
 * 2. Make tails next point to heads next.
 *    i.e. l->tail->next = l->head->next
 *    OR - which is the same as - 
 *         l->tail->next = cur->next
 *
 * 2. Update l->head
 *    i.e. l->head = cur->next
 *    OR - which is the same as - 
 *         l->head = l->head->next
 *
 * Else if it is tail, then first of all it is not head, because
 * if it was head it would have already been dealt with. Now in
 * case of tail, do the following additional steps:
 *
 * 1. Update tail to point to prev node of current tail.
 * i.e. l->tail = prev
 *
 * In any case, (head node, or tail node, or any node), free the
 * memory occupied by the node by calling dval and free.
 * i.e. l->dval(cur->val);
 *      free(cur);
 *
 * Also note that this function does not update the l->nmemb count.
 * Caller function has to update l->nmemb.
 *
 * @l:    Pointer to list
 * @cur:  Node to remove 
 * @prev: Previous node of node to remove
 */
static void cll_remove_node(struct cll *l,
                            struct cll_node *cur,
			    struct cll_node *prev)
{
	if (cur == l->head) {
		if (cur == l->tail) {
			l->head = NULL;
			l->tail = NULL;
		} else {
			l->tail->next = cur->next; 
			l->head = cur->next;
		}
	} else {
		if (cur == l->tail)
			l->tail = prev;
		prev->next = cur->next;
	}

	l->dval(cur->val);
	free(cur);
}

/*
 * Delete a node from linked list. Has no effect if
 * there is no node whose value is `val'.
 *
 * @l: Pointer to linked list
 * @val: Value of the node to be deleted
 */
void cll_delete(struct cll *l, void *val)
{
	struct cll_node *cur;
	struct cll_node *prev;

	assert(l);

	/* Return if nothing to delete */
	if (val == NULL)
		return;
	
	/* Return if empty list */
	if (l->head == NULL)
		return;

	prev = l->head;
	cur  = l->head;

	/* Search for the node and delte it */
	do {
		if (l->cmp(cur->val, val) == 0) {
			cll_remove_node(l, cur, prev);
			l->nmemb--;
			break;
		}
		prev = cur;
		cur = cur->next;
	} while (cur != l->head);
}

/*
 * Destroy linked list
 *
 * @l: Pointer to linked list
 */
void cll_destroy(struct cll *l)
{
	struct cll_node *clln;
	struct cll_node *next;

	assert(l);

	if (l->head != NULL) {
		/*
		 * At first free all nodes except
		 * the head node.
		 * Reason being, we we test for the cur
		 * node to be the head node to break out
		 * of loop. Although we could still
		 * compare the _address_ of head node
		 * even if we free it, it is not done so.
		 */
		clln = l->head->next;
		while (clln != l->head) {
			next = clln->next;
			l->dval(clln->val);
			free(clln);
			clln = next;
		}

	        /* Free head node at last */
		l->dval(clln->val);
		free(clln);
	}

	/* TODO: Is this needed? */
	l->head = NULL;
	l->tail = NULL;

	free(l);
}

/*
 * Print linked list
 *
 * @l: Pointer to linked list
 */
void cll_print(struct cll *l)
{
	struct cll_node *cur;

	assert(l);

	/* Return if no print function */
	if (l->printl == NULL)
		return;
	
	/* Return if empty list  */
	if (l->head == NULL)
		return;

	/* Print each node */
	cur = l->head;
	do {
		l->printl(cur);
		cur = cur->next;
	} while (cur != l->head) ;
	putchar('\n');
}

