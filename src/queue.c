/*
 * q.c:
 *
 * St: 2016-09-26 Mon 06:12 PM
 * Up: 2016-10-04 Tue 05:17 AM
 *
 * Author: SPS
 *
 * This file is copyright 2016 SPS.
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL SPS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

#include"mylib.h"

#define SKIP

/*
 * Create a queue
 *
 * @cpy: Pointer to copy function
 * @cmp: Pointer to compare function
 * @printn: Pointer to node print function
 */
struct queue *q_create(void *(*cpy)(void *), 
                       int (*cmp)(void *, void *),
		       void (*dval) (void *),
		       void (*printn)(struct q_node *))
{
	struct queue *q;

	q = malloc(sizeof(struct queue));
	assert(q);

	q->head = NULL;
	q->tail = NULL;
	q->cpy = cpy;
	q->cmp = cmp;
	q->dval = dval;
	q->printn = printn;
	q->nmemb = 0;

	return q;
}

/*
 * Create a q_node, and return pointer to it
 *
 * @q: Pointer to the queue
 * @val: Pointer to value to copied to new q_node's val
 */
static struct q_node *q_node_create(struct queue *q, void *val)
{
	struct q_node *qn;

	assert(q);
	assert(val);

	qn = malloc(sizeof(struct q_node));
	assert(qn);

	qn->val = q->cpy(val);
	qn->next = NULL;

	return qn;
}

/*
 * Push a new value to queue. New nodes are pushed
 * at the tail side. q_pop will pop off node from the
 * head side.
 *
 * @q: Pointer to queue 
 * @val: Pointer to value to be inserted
 */
void q_push(struct queue *q, void *val)
{
	struct q_node *qn;

	assert(q);

	qn = q_node_create(q, val);

	qn->next = NULL;
	/*
	 * If queue is empty, then `head' will point
	 * to the new node. Otherwise the tail node,
	 * ie. the node pointed by `tail' currently,
	 * will point to the new node.
	 * `tail' is always updated to point to the
	 * new node
	 */
	if (q_is_empty(q))
		q->head = qn;
	else
		q->tail->next = qn;
	q->tail = qn;

	q->nmemb++;
}

/*
 * Pop a value from front of the queue. Node from the head
 * side is popped off.
 *
 * If the queue is empty, return `NULL' to indicate that
 * queue is empty.
 *
 * Else perform the following actions:
 *
 * 1. Update q->head to point to next node (point to NULL if
 *    the popped off node is the only remaining node in queue).
 * 2. Update q->tail _if_ the node getting popped off is the
 *    last one.
 * 3. Make a copy of value of first node.
 * 4. Free the first node (and its value).
 * 5. Update queue member count.
 * 6. Return the copy. 
 *
 * @q: Pointer to the queue
 */
void *q_pop(struct queue *q)
{
	void *retval;
	struct q_node *popn;

	assert(q);

	if (q_is_empty(q)) {
		retval = NULL;
	} else {
		popn = q->head;
		q->head = q->head->next;
		if (q->head == NULL) 
			q->tail = NULL;
		retval = q->cpy(popn->val);
		/* free(popn->val);  */
		q->dval(popn->val);
		free(popn);
		q->nmemb--;
	}
	
	return retval;
}

/*
 * Destroy a queue
 *
 * @s: Pointer to the queue
 */
void q_destroy(struct queue *q)
{
	struct q_node *cur;
	struct q_node *next;

	assert(q);

	/*
	 * Free each node, and the val 
	 * in it
	 */
	cur = q->head;
	while (cur != NULL) {
		next = cur->next;
		free(cur->val);
		free(cur);
		cur = next;
	}

	/* Free queue structure */
	free(q);
}

/*
 * Check if queue is empty.
 *
 * @q: Pointer to the queue
 */
int q_is_empty(struct queue *q)
{
	int retval;

	assert(q);

	if (q->head == NULL)
		retval = 1;
	else 
		retval = 0;

	return retval;
}

/*
 * Print queue to stdout.
 *
 * @q: Pointer to the queue
 */
void q_print(struct queue *q)
{
	struct q_node *cur;

	assert(q);

	if (!q_is_empty(q)) {
		cur = q->head;
		while (cur != NULL) {
			q->printn(cur);
			cur = cur->next;
		}
		putchar('\n');
	}
}

/*
 * Return value at front of the queue without
 * removing it
 *
 * @q: Pointer to the queue structure
 */
void *q_peek(struct queue *q)
{
	void *retval;

	retval = NULL;

	if (!q_is_empty(q))
		retval = q->cpy(q->head->val);

	return retval;
}

