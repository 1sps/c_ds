/*
 * ll.c:
 *
 * St: 2016-09-26 Mon 01:58 AM
 * Up: 2016-09-26 Mon 05:26 AM
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
 * Create a singly linked list
 *
 * @cpy: Copy function
 * @cmp: Compare function
 * @dval: Funciton to destroy `val' of a node
 */
struct ll *ll_create(void *(*cpy)(void *),
                     int (*cmp)(void *, void *),
		     void (*dval)(void *),
		     void (*printl)(struct ll_node *))
{
	struct ll *l;

#ifdef IS_THIS_NEEDED
	assert(cpy);
	assert(cmp);
#endif /* IS_THIS_NEEDED */

	l = malloc(sizeof(struct ll));
	assert(l);

	l->head = NULL;
	l->cpy = cpy;
	l->cmp = cmp;
	l->dval = dval;
	l->printl = printl;
	l->nmemb = 0;

	return l;
}

/*
 * Create a ll_node
 *
 * @val: value of the node
 */
static struct ll_node *ll_node_create(struct ll *l, void *val)
{
	struct ll_node *lln;

	/* TODO: Is this assert needed? */
	assert(l);

	assert(val);

	lln = malloc(sizeof(struct ll_node));
	assert(lln);

	lln->val = l->cpy(val);
	/*
	 * No assertion should be needed,
	 * l->cpy should do assertions
	 */

	lln->next = NULL;

	return lln;
}

/*
 * Insert a new value to linked list
 *
 * @l: linked list
 * @val: new value to be inserted to the linked list
 */
void ll_insert(struct ll *l, void *val)
{
	struct ll_node *lln;

	assert(l);

	lln = ll_node_create(l, val);
	lln->next = l->head;
	l->head = lln;
	l->nmemb++;
}

/*
 * Search for a val in linked list
 *
 * @l: Pointer to linked list
 * @val: Pointer to value to search
 */
int ll_search(struct ll *l, void *val)
{
	int retval;
	struct ll_node *cur;

	assert(l);
	assert(val);

	retval = 0;

	cur = l->head;
	while(cur != NULL) {
		if (l->cmp(cur->val, val) == 0) {
			retval = 1;
			break;
		}
		cur = cur->next;
	}

	return retval;
}


/*
 * Delete a node from linked list. Has no effect if
 * there is no node whose value is `val'.
 *
 * @l: Pointer to linked list
 * @val: Value of the node to be deleted
 */
void ll_delete(struct ll *l, void *val)
{
	struct ll_node *cur;
	struct ll_node **prev;

	assert(l);

	/* Do nothing if nothing to delete */
	if (val == NULL)
		return;
	
	prev = &l->head;
	while (*prev != NULL) {
		cur = *prev;
		if (l->cmp(cur->val, val) == 0) {
			*prev = cur->next;
			l->dval(cur->val);
			free(cur);
			l->nmemb--;
			break;
		}
		prev = &cur->next;
	}
}

/*
 * Destroy linked list
 *
 * @l: Pointer to linked list
 */
void ll_destroy(struct ll *l)
{
	struct ll_node *lln;
	struct ll_node *next;

	assert(l);

	lln = l->head;
	while (lln != NULL) {
		next = lln->next;
		l->dval(lln->val);
		free(lln);
		lln = next;
	}

	/* TODO: Is this needed? */
	l->head = NULL;

	free(l);
}

/*
 * Print linked list
 *
 * @l: Pointer to linked list
 */
void ll_print(struct ll *l)
{
	struct ll_node *cur;

	assert(l);

	/* If no print function, just return  */
	if (l->printl == NULL)
		return;
	
	/* Print each node */
	cur = l->head;
	while (cur != NULL) {
		l->printl(cur);
		cur = cur->next;
	}
	putchar('\n');
}

/*
 * Test if linked list is empty
 *
 * @l: Pointer to the linked list structure
 */
int ll_is_empty(struct ll *l)
{
	int retval;

	if (l->head == NULL)
		retval = 1;
	else
		retval = 0;

	return retval;
}

/*
 * Initialize iterator for the linked list.
 * Returns the pointer to first item.
 *
 * @l: Pointer to the linked list structure
 */
void* ll_first(struct ll *l)
{
	void *first;

	first = l->head;

	return first;
}

/*
 * Get the next value from linked list iteration.
 * ll_first should be called before this function is called.
 *
 * @itrp: Pointer to iterator
 */
void* ll_next(struct ll *l, void **itrp)
{
	void *retval;
	struct ll_node *lln;

	lln = (struct ll_node *) *itrp;

	/* Make a copy of current node val to return */
	retval = l->cpy(lln->val);

	/* Update iterator */
	*itrp = lln->next;

	return retval;
}

/*
 * Check if iteration is completed
 *
 * @itr: Iterator
 */
int ll_done(void *itr)
{
	int retval;

	if (itr == NULL)
		retval = 1;
	else
		retval = 0;

	return retval;
}


