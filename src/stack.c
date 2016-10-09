/*
 * st.c: Stack implementation
 *
 * St: 2016-09-26 Mon 03:21 PM
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
 * Create a stack
 *
 * @cpy: Pointer to copy function
 * @cmp: Pointer to comapre function
 * @printn: Pointer to node print function
 */
struct st *st_create(void *(*cpy)(void *),
                     int (*cmp)(void *, void *),
		     void (*dval)(void *),
		     void (*printn)(void *))
{
	struct st *s;

	s = malloc(sizeof(struct st));
	assert(s);

	s->head = NULL;
	s->cpy = cpy;
	s->cmp = cmp;
	s->dval = dval;
	s->printn = printn;
	s->nmemb = 0;

	return s;
}

/*
 * Create a new st_node, and return a pointer to it
 *
 * @s:
 * @val:
 */
static struct st_node *st_node_create(struct st *s, void *val)
{
	struct st_node *stn;

	stn = malloc(sizeof(struct st_node));
	assert(stn);

	stn->val = s->cpy(val);
	stn->next = NULL;

	return stn;
}

/*
 * Push a new value on top of stack
 *
 * @s: Pointer to stack
 * @val: New value to be pushed
 */
void st_push(struct st *s, void *val)
{
	struct st_node *stn;

	assert(s);

	stn = st_node_create(s, val);
	stn->next = s->head;
	s->head = stn;
	s->nmemb++;
}

/*
 * Pop value from top of the stack
 *
 * @s: Pointer to stack
 */
void *st_pop(struct st *s)
{
	void *retval;
	struct st_node *stn;


	if (st_is_empty(s)) {
		/* Return NULL if stack is empty */
		retval = NULL;
	} else {
		/* Copy the value of node, this copy
		 * will be returned
		 */
		retval = s->cpy(s->head->val);

		/* Remove the node from top of the
		 * stack, and free memory occupied
		 * by that node
		 */
		stn = s->head;
		s->head = s->head->next;
		s->dval(stn->val);
		free(stn);
		s->nmemb--;
	}

	return retval;
}

/*
 * Destroy stack
 *
 * @s: Pointer to the stack
 */
void st_destroy(struct st *s)
{
	struct st_node *cur;
	struct st_node *next;

	assert(s);

	cur = s->head;
	while (cur != NULL) {
		next = cur->next;
		s->dval(cur->val);
		free(cur);
		cur = next;
	}

	free(s);
}

/*
 * Check if stack is empty. Return 1 if stack is empty,
 * 0 otherwise.
 *
 * @s: Pointer to stack
 */
int st_is_empty(struct st *s)
{
	int retval;

	assert(s);

	if (s->head == NULL)
		retval = 1;
	else
		retval = 0;

	return retval;
}

/*
 * Print stack to stdout
 *
 * @s: Pointer to stack
 */
void st_print(struct st *s)
{
	struct st_node *cur;

	assert(s);

	/* Print only if this stack
	 * has the printn function
	 */
	if (s->printn != NULL) {
		cur = s->head;
		while(cur != NULL) {
			s->printn(cur->val);
			cur = cur->next;
		}
		putchar('\n');
	}
}


/*
 * Return the value at top of stack without
 * popping it.
 *
 * @s: Pointer to the stack structure
 */
void *st_peek(struct st *s)
{
	void *retval;

	retval = NULL;

	if (!st_is_empty(s))
		retval = s->cpy(s->head->val);

	return retval;
}

