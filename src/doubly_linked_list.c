/*
 * dll.c: Double linked list implementation in C
 *
 * St: 2016-09-28 Wed 12:29 AM
 * Up: 2016-09-28 Wed 12:43 AM
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
 * Create a doubly linked list
 *
 * @cpy: Copy function
 * @cmp: Compare function
 * @dval: Funciton to destroy `val' of a node
 */
struct dll *dll_create(void *(*cpy)(void *), int (*cmp)(void *, void *),
		       void (*dval)(void *), void (*printl)(struct dll_node *))
{
	struct dll *l;

#ifdef IS_THIS_NEEDED
	assert(cpy);
	assert(cmp);
#endif /* IS_THIS_NEEDED */

	l = malloc(sizeof(struct dll));
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
 * Create a dll_node
 *
 * @val: value of the node
 */
static struct dll_node *dll_node_create(struct dll *l, void *val)
{
	struct dll_node *dlln;

	/* TODO: Is this assert needed? */
	assert(l);

	assert(val);

	dlln = malloc(sizeof(struct dll_node));
	assert(dlln);

	dlln->val = l->cpy(val);
	/*
	 * No assertion should be needed,
	 * l->cpy should do assertions
	 */

	dlln->next = NULL;
	dlln->prev = NULL;

	return dlln;
}

/*
 * Insert a new value to linked list
 *
 * @l: linked list
 * @val: new value to be inserted to the linked list
 */
void dll_insert(struct dll *l, void *val)
{
	struct dll_node *dlln;

	assert(l);

	dlln = dll_node_create(l, val);
	dlln->next = l->head;
	if (l->head != NULL)
		l->head->prev = dlln;
	l->head = dlln;
	l->nmemb++;
}

/*
 * Search for a val in linked list
 *
 * @l: Pointer to linked list
 * @val: Pointer to value to search
 */
int dll_search(struct dll *l, void *val)
{
	int retval;
	struct dll_node *cur;

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
void dll_delete(struct dll *l, void *val)
{
	struct dll_node *cur;
	struct dll_node **prev;

	assert(l);

	/* Do nothing if nothing to delete */
	if (val == NULL)
		return;
	
	prev = &l->head;
	while (*prev != NULL) {
		cur = *prev;
		if (l->cmp(cur->val, val) == 0) {
			*prev = cur->next;
			if (cur->next != NULL)
				cur->next->prev = cur->prev;
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
void dll_destroy(struct dll *l)
{
	struct dll_node *dlln;
	struct dll_node *next;

	assert(l);

	dlln = l->head;
	while (dlln != NULL) {
		next = dlln->next;
		l->dval(dlln->val);
		free(dlln);
		dlln = next;
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
void dll_print(struct dll *l)
{
	struct dll_node *cur;

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

