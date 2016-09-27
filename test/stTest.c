/*
 * test/stTest.c: Test stack implementation
 *
 * St: 2016-09-26 Mon 03:39 PM
 * Up: 2016-09-26 Mon 05:45 PM
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

#define SKIP
#define TEST_ERR 1

/* Total nodes to insert while doing insert test */
#define INITIAL_INS_COUNT 10


/*
 * Check if there is a TEST ERROR
 *
 * @s: Pointer to the stack
 */
int is_test_err(struct st *s)
{
	int test_err;

	test_err = 0;

	if (s == NULL)
		test_err = TEST_ERR;
	
	return test_err;
}


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
 * Print a node of int linked list
 *
 * @l: Pointer to linked list
 */
void printn_i(struct st_node *stn)
{
	printf("%d, ", *(int *)stn->val);
}

/*
 * Test st_create function for int stack
 *
 * @s: Pointer to stack
 */
int test_int_create(struct st *s)
{
	int val1;
	int val2;
	int *val1_ptr;
	int *val2_ptr;
	int *img;

	/* Assert that there is no TEST ERROR */
	assert(is_test_err(s) != TEST_ERR);

	val1_ptr = &val1;
	val2_ptr = &val2;

	/* Test head */
	assert(s->head == NULL);

	/* Test cpy */
	val1 = 10;
	img = s->cpy(val1_ptr);
	assert(*img = val1);
	free(img);

	/* Test cmp */
	val1 = 10;
	val2 = 5;
	assert(s->cmp(val1_ptr, val2_ptr) == 1);

	val1 = -10;
	val2 = -10;
	assert(s->cmp(val1_ptr, val2_ptr) == 0);

	val1 = -10;
	val2 = 5;
	assert(s->cmp(val1_ptr, val2_ptr) == -1);

	/* Test nmemb */
	assert(s->nmemb == 0);

	return 1;
}

/*
 * Test st_push for int stack
 *
 * @s: Pointer to stack
 */
int test_int_push(struct st *s)
{
	int i;
	int *iptr;
	size_t nmemb_prev;
	void *head_prev;

	/* Assert that there is no TEST ERROR */
	assert(is_test_err(s) != TEST_ERR);

	head_prev = s->head;
	nmemb_prev = s->nmemb;
	iptr = &i;
	for (i = 0; i < INITIAL_INS_COUNT; i++) {
		st_push(s, iptr);
		assert(s->cmp(s->head->val, iptr) == 0);
		assert(s->nmemb == nmemb_prev + 1);
		assert(s->head->next == head_prev);
		nmemb_prev = s->nmemb;
		head_prev = s->head;
	}

#ifdef VISUAL_PRINT
	st_print(s);
#endif /* VISUAL_PRINT */

	return 1;
}

/*
 * Test st_pop for int stack 
 *
 * @s: Pointer to stack
 */
int test_int_pop(struct st *s)
{
	int prev_nmemb;
	void *pop_val;
	void *prev_head_val;
	struct st_node *prev_head_next;

	/* Assert that there is no TEST ERROR */
	assert(is_test_err(s) != TEST_ERR);

	/* Test pop from non-empty stack */
	while (!st_is_empty(s)) {
		prev_nmemb = s->nmemb;
		prev_head_val = s->cpy(s->head->val);
		prev_head_next = s->head->next;
		pop_val = st_pop(s);
		assert(s->cmp(pop_val, prev_head_val) == 0);
		assert(s->nmemb == prev_nmemb-1);
		assert(s->head == prev_head_next);
		free(prev_head_val);
		free(pop_val);
	}
	assert(s->nmemb == 0);

	/* Test pop from non-empty stack */
	pop_val = st_pop(s);
	assert(pop_val == NULL);

	return 1;
}

/* Test int stack */
int test_int(void)
{
	struct st *s;

	s = st_create(cpy_i, cmp_i, printn_i);
	assert(test_int_create(s) == 1);
	assert(test_int_push(s) == 1);
	st_print(s);
	assert(test_int_pop(s) == 1);

	st_destroy(s);

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
 * Print a node of str linked list
 *
 * @l: Pointer to linked list
 */
void printn_s(struct st_node *stn)
{
	printf("%s, ", (char *)stn->val);
}

/*
 * Test create function for str linked list 
 *
 * @l: pointer to linked list
 */
int test_str_create(struct st *s)
{
	char *str1;
	char *str2;
	char *img;

	/* Assert that there is no TEST ERROR */
	assert(is_test_err(s) != TEST_ERR);

	/* Test head */
	assert(s->head == NULL);

	/* Test cpy */
	str1 = "Hello_world!";
	img = s->cpy(str1);
	assert(strncmp(img, str1, strlen(str1)+1) == 0);
	free(img);

	/* Test cmp */
	str1 = "hello";
	str2 = "gello";
	assert(s->cmp(str1, str2) == 1);

	str1 = "hello";
	str2 = "hello";
	assert(s->cmp(str1, str2) == 0);

	str1 = "hello";
	str2 = "iello";
	assert(s->cmp(str1, str2) == -1);

	/* Test nmemb */
	assert(s->nmemb == 0);

	return 1;
}

/*
 * Test insert in str linked list
 *
 * @l: Pointer to linked list
 */
int test_str_push(struct st *s)
{
	int i;
	char str[10] = "hello";
	size_t nmemb_prev;
	void *head_prev;

	/* Assert that there is no TEST ERROR */
	assert(is_test_err(s) != TEST_ERR);

	head_prev = s->head;
	nmemb_prev = s->nmemb;
	for (i = 0; i < INITIAL_INS_COUNT; i++) {
		str[1] = 'e' + i;
		st_push(s, str);
		assert(s->cmp(s->head->val, str) == 0);
		assert(s->nmemb == nmemb_prev + 1);
		assert(s->head->next == head_prev);
		nmemb_prev = s->nmemb;
		head_prev = s->head;
	}

#ifdef VISUAL_PRINT
	st_print(s);
#endif /* VISUAL_PRINT */

	return 1;
}

/*
 * Test st_pop for str stack 
 *
 * @s: Pointer to stack
 */
int test_str_pop(struct st *s)
{
	int prev_nmemb;
	void *pop_val;
	void *prev_head_val;
	struct st_node *prev_head_next;

	/* Assert that there is no TEST ERROR */
	assert(is_test_err(s) != TEST_ERR);

	/* Test pop from non-empty stack */
	while (!st_is_empty(s)) {
		prev_nmemb = s->nmemb;
		prev_head_val = s->cpy(s->head->val);
		prev_head_next = s->head->next;
		pop_val = st_pop(s);
		assert(s->cmp(pop_val, prev_head_val) == 0);
		assert(s->nmemb == prev_nmemb-1);
		assert(s->head == prev_head_next);
		free(prev_head_val);
		free(pop_val);
	}
	assert(s->nmemb == 0);

	/* Test pop from non-empty stack */
	pop_val = st_pop(s);
	assert(pop_val == NULL);

	return 1;
}

/* Test str stack */
int test_str(void)
{
	struct st *s;

	s = st_create(cpy_s, cmp_s, printn_s);
	assert(test_str_create(s) == 1);
	assert(test_str_push(s) == 1);
	st_print(s);
	assert(test_str_pop(s) == 1);

	st_destroy(s);

	return 1;
}

/* main: perform testing of stack implementation */
int main(void)
{
	test_int();
	test_str();
	return 0;
}

