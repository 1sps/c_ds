/*
 * llTest.c: Test code for singly linked list (ll.c)
 *
 * St: 2016-09-26 Mon 02:04 AM
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
#include<string.h>
#include<assert.h>

#include"../src/mylib.h"

/* Total nodes to insert while doing insert test */
#define INITIAL_INS_COUNT 10


/*
 *******************************************************************************
 * Int Test Section
 *******************************************************************************
 */

/*
 * Make a copy of an int 
 *
 * @src: source ; whose copy is to be made
 */
void *cpy_i(void *src)
{
	int *dest; 

	/* TODO: Make this function agnostic of empty src, 
	 * which means, that the below assert should go away
	 */
	assert(src);

	dest = NULL;
	dest = malloc(sizeof(int));
	assert(dest);

	*dest = *((int *) src);

	return dest;
}

/*
 * Compare two int. Return 1, 0 or -1  if val1 is greater
 * than, equal, or smaller than val2. 
 *
 * @val1: Pointer to first int
 * @val2: Pointer to second int
 */
int cmp_i(void *val1, void *val2)
{
	int u;
	int v;
	int retval;

	assert(val1);
	assert(val2);

	u = *((int *) val1);
	v = *((int *) val2);

	if (u > v)
		retval = 1;
	else if (u == v)
		retval = 0;
	else
		retval = -1;

	return retval;
}

/*
 * Print int linked list
 *
 * @l: Pointer to linked list
 */
void print_int_lln(struct ll_node *lln)
{
	printf("%d, ", *(int *)lln->val);
}

/*
 * Test create function for int linked list 
 *
 * @l: pointer to linked list
 */
int test_int_create(struct ll *l)
{
	int val1;
	int val2;
	int *val1_ptr;
	int *val2_ptr;
	int *img;

	assert(l);

	val1_ptr = &val1;
	val2_ptr = &val2;

	/* Test head */
	assert(s->head == NULL);

	/* Test cpy */
	val1 = 10;
	img = l->cpy(val1_ptr);
	assert(*img = val1);
	free(img);

	/* Test cmp */
	val1 = 10;
	val2 = 5;
	assert(l->cmp(val1_ptr, val2_ptr) == 1);

	val1 = -10;
	val2 = -10;
	assert(l->cmp(val1_ptr, val2_ptr) == 0);

	val1 = -10;
	val2 = 5;
	assert(l->cmp(val1_ptr, val2_ptr) == -1);

	/* Test nmemb */
	assert(l->nmemb == 0);

	return 1;
}

/*
 * Test insert in int linked list
 *
 * @l: Pointer to linked list
 */
int test_int_insert(struct ll *l)
{
	int i;
	int *iptr;
	size_t nmemb_prev;
	void *head_prev;

	head_prev = l->head;
	nmemb_prev = l->nmemb;
	iptr = &i;
	for (i = 0; i < INITIAL_INS_COUNT; i++) {
		ll_insert(l, iptr);
		assert(l->cmp(l->head->val, iptr) == 0);
		assert(l->nmemb == nmemb_prev + 1);
		assert(l->head->next == head_prev);
		nmemb_prev = l->nmemb;
		head_prev = l->head;
	}

#ifdef VISUAL_PRINT
	ll_print(l);
#endif /* VISUAL_PRINT */

	return 1;
}

/*
 * Test search in a int linked list
 *
 * @l: Pointer to linked list
 */
int test_int_search(struct ll *l)
{
	int i;
	int *iptr;

	assert(l);

	iptr = &i;

	/* TODO: This is very much dependent on the knowledge
	 * of what is inserted during `test_int_insert()'. It will
	 * be better if this knowledge dependency is removed.
	 */

	i = 0;
	assert(ll_search(l, iptr) == 1);
	i = INITIAL_INS_COUNT-1;
	assert(ll_search(l, iptr) == 1);
	i = INITIAL_INS_COUNT/2;
	assert(ll_search(l, iptr) == 1);
	i = INITIAL_INS_COUNT;
	assert(ll_search(l, iptr) == 0);
	i = -1;
	assert(ll_search(l, iptr) == 0);

	return 1;
}

/*
 * Test delete in int linked list
 *
 * @l: Pointer to linked list
 */
int test_int_delete(struct ll *l)
{
	int i;
	int *iptr;
	int nmemb_prev;

	assert(l);

	iptr = &i;

	/* TODO: This is very much dependent on the knowledge
	 * of what is inserted during `test_int_insert()'. It will
	 * be better if this knowledge dependency is removed.
	 */

	nmemb_prev = l->nmemb;
	i = 0;
	ll_delete(l, iptr);
	assert(l->nmemb == nmemb_prev-1);

	nmemb_prev = l->nmemb;
	i = INITIAL_INS_COUNT-1;
	ll_delete(l, iptr);
	assert(l->nmemb == nmemb_prev-1);

	nmemb_prev = l->nmemb;
	i = INITIAL_INS_COUNT/2;
	ll_delete(l, iptr);
	assert(l->nmemb == nmemb_prev-1);

	nmemb_prev = l->nmemb;
	i = INITIAL_INS_COUNT;
	ll_delete(l, iptr);
	assert(l->nmemb == nmemb_prev);

	nmemb_prev = l->nmemb;
	i = -1;
	ll_delete(l, iptr);
	assert(l->nmemb == nmemb_prev);

	return 1;
}

/* Test int linked list */
int test_int(void)
{
	struct ll *l;

	l = ll_create(cpy_i, cmp_i, print_int_lln);
	assert(test_int_create(l) == 1);
	assert(test_int_insert(l) == 1);
	ll_print(l);
	assert(test_int_search(l) == 1 );
	assert(test_int_delete(l) == 1 );

	ll_destroy(l);
	l = NULL;

	return 1;
}

/*
 *******************************************************************************
 * Str Test Section
 *******************************************************************************
 */

/*
 * Make a copy of a string 
 *
 * @src: source ; whose copy is to be made
 */
void *cpy_s(void *src)
{
	char *dest; 

	/* TODO: Make this function agnostic of empty src, 
	 * which means, that the below assert should go away
	 */
	assert(src);

	dest = NULL;
	dest = strdup(src);
	assert(dest);

	return dest;
}

/*
 * Compare two strings. Return 1, 0 or -1  if str1 is greater
 * than, equal, or smaller than str2. 
 *
 * @str1: First string
 * @str2: Second string
 */
int cmp_s(void *str1, void *str2)
{
	int retval;

	assert(str1);
	assert(str2);

	retval = strncmp(str1, str2, strlen(str1)+1);

	return retval;
}

/*
 * Test create function for str linked list 
 *
 * @l: pointer to linked list
 */
int test_str_create(struct ll *l)
{
	char *str1;
	char *str2;
	char *img;

	assert(l);

	/* Test head */
	assert(s->head == NULL);

	/* Test cpy */
	str1 = "Hello_world!";
	img = l->cpy(str1);
	assert(strncmp(img, str1, strlen(str1)+1) == 0);
	free(img);

	/* Test cmp */
	str1 = "hello";
	str2 = "gello";
	assert(l->cmp(str1, str2) == 1);

	str1 = "hello";
	str2 = "hello";
	assert(l->cmp(str1, str2) == 0);

	str1 = "hello";
	str2 = "iello";
	assert(l->cmp(str1, str2) == -1);

	/* Test nmemb */
	assert(l->nmemb == 0);

	return 1;
}

/*
 * Test insert in str linked list
 *
 * @l: Pointer to linked list
 */
int test_str_insert(struct ll *l)
{
	int i;
	char str[10] = "hello";
	size_t nmemb_prev;
	void *head_prev;

	head_prev = l->head;
	nmemb_prev = l->nmemb;
	for (i = 0; i < INITIAL_INS_COUNT; i++) {
		str[1] = 'e' + i;
		ll_insert(l, str);
		assert(l->cmp(l->head->val, str) == 0);
		assert(l->nmemb == nmemb_prev + 1);
		assert(l->head->next == head_prev);
		nmemb_prev = l->nmemb;
		head_prev = l->head;
	}

#ifdef VISUAL_PRINT
	ll_print(l);
#endif /* VISUAL_PRINT */

	return 1;
}

/*
 * Test search in a str linked list
 *
 * @l: Pointer to linked list
 */
int test_str_search(struct ll *l)
{
	char str[10] = "hello";

	assert(l);

	/* TODO: This is very much dependent on the knowledge
	 * of what is inserted during `test_int_insert()'. It will
	 * be better if this knowledge dependency is removed.
	 */

	assert(ll_search(l, str) == 1);
	str[1] = 'e' + 1;
	assert(ll_search(l, str) == 1);
	str[1] = 'e' + INITIAL_INS_COUNT-1;
	assert(ll_search(l, str) == 1);
	str[1] = 'd';
	assert(ll_search(l, str) == 0);
	str[0] = 'a';
	assert(ll_search(l, str) == 0);

	return 1;
}

/*
 * Test delete in str linked list
 *
 * @l: Pointer to linked list
 */
int test_str_delete(struct ll *l)
{
	int i;
	char str[10] = "hello";
	int nmemb_prev;

	assert(l);

	/* TODO: This is very much dependent on the knowledge
	 * of what is inserted during `test_int_insert()'. It will
	 * be better if this knowledge dependency is removed.
	 */

	nmemb_prev = l->nmemb;
	i = 0;
	str[1] = 'e' + i;
	ll_delete(l, str);
	assert(l->nmemb == nmemb_prev-1);

	nmemb_prev = l->nmemb;
	i = INITIAL_INS_COUNT-1;
	str[1] = 'e' + i;
	ll_delete(l, str);
	assert(l->nmemb == nmemb_prev-1);

	nmemb_prev = l->nmemb;
	i = INITIAL_INS_COUNT/2;
	str[1] = 'e' + i;
	ll_delete(l, str);
	assert(l->nmemb == nmemb_prev-1);

	nmemb_prev = l->nmemb;
	i = INITIAL_INS_COUNT;
	str[1] = 'e' + i;
	ll_delete(l, str);
	assert(l->nmemb == nmemb_prev);

	nmemb_prev = l->nmemb;
	i = -1;
	str[1] = 'e' + i;
	ll_delete(l, str);
	assert(l->nmemb == nmemb_prev);

	return 1;
}

/* Test str linked list */
int test_str(void)
{
	struct ll *l;

	l = ll_create(cpy_s, cmp_s, NULL);
	assert(test_str_create(l) == 1);
	assert(test_str_insert(l) == 1);
	ll_print(l);
	assert(test_str_search(l) == 1);
	assert(test_str_delete(l) == 1);

	ll_destroy(l);
	l = NULL;

	return 1;
}

/* main: start */
int main(void)
{
	test_int();
	test_str();
	return 0;
}

