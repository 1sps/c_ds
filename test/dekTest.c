/*
 * dekTest.c: Test code for deque (deque.c)
 *
 * St: 2016-09-29 Thu 12:17 AM
 * Up: 2016-09-29 Thu 01:24 AM
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
 * Destroy a nodes val.
 *
 * @val:
 */
void dval(void *val)
{
	free(val);
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
 * Print int deque
 *
 * @l: Pointer to deque
 */
void print_int_dekn(struct deque_node *dekn)
{
	printf("%d, ", *(int *)dekn->val);
}

/*
 * Test create function for int deque 
 *
 * @l: pointer to deque
 */
int test_int_create(struct deque *dek)
{
	int val1;
	int val2;
	int *val1_ptr;
	int *val2_ptr;
	int *img;

	assert(dek);

	val1_ptr = &val1;
	val2_ptr = &val2;

	/* Test head */
	assert(dek->head == NULL);

	/* Test cpy */
	val1 = 10;
	img = dek->cpy(val1_ptr);
	assert(*img = val1);
	free(img);

	/* Test cmp */
	val1 = 10;
	val2 = 5;
	assert(dek->cmp(val1_ptr, val2_ptr) == 1);

	val1 = -10;
	val2 = -10;
	assert(dek->cmp(val1_ptr, val2_ptr) == 0);

	val1 = -10;
	val2 = 5;
	assert(dek->cmp(val1_ptr, val2_ptr) == -1);

	/* Test nmemb */
	assert(dek->nmemb == 0);

	return 1;
}

/*
 * Test insert in int deque
 *
 * @l: Pointer to deque
 */
int test_int_insert(struct deque *dek)
{
	int i;
	int *iptr;
	size_t nmemb_prev;
	void *head_prev;
	void *tail_prev;

	/*
	 * Test insert to TAIL side
	 */
	head_prev = dek->head;
	nmemb_prev = dek->nmemb;
	iptr = &i;
	for (i = 0; i < INITIAL_INS_COUNT; i++) {
		deque_insert(dek, DEQUE_HEAD, iptr);
		assert(dek->cmp(dek->head->val, iptr) == 0);
		assert(dek->nmemb == nmemb_prev + 1);
		assert(dek->head->next == head_prev);
		nmemb_prev = dek->nmemb;
		head_prev = dek->head;
	}

	/*
	 * Test insert to TAIL side
	 */
	tail_prev = dek->tail;
	nmemb_prev = dek->nmemb;
	for (i = 200; i < 200+INITIAL_INS_COUNT; i++) {
		deque_insert(dek, DEQUE_TAIL, iptr);
		assert(dek->cmp(dek->tail->val, iptr) == 0);
		assert(dek->nmemb == nmemb_prev + 1);
		assert(dek->tail->prev == tail_prev);
		nmemb_prev = dek->nmemb;
		tail_prev = dek->tail;
	}

	return 1;
}

/*
 * Test search in a int deque
 *
 * @l: Pointer to deque
 */
int test_int_search(struct deque *dek)
{
	int i;
	int *iptr;

	assert(dek);

	iptr = &i;

	/* TODO: This is very much dependent on the knowledge
	 * of what is inserted during `test_int_insert()'. It will
	 * be better if this knowledge dependency is removed.
	 */

	i = 0;
	assert(deque_search(dek, iptr) == 1);
	i = INITIAL_INS_COUNT-1;
	assert(deque_search(dek, iptr) == 1);
	i = INITIAL_INS_COUNT/2;
	assert(deque_search(dek, iptr) == 1);
	i = INITIAL_INS_COUNT;
	assert(deque_search(dek, iptr) == 0);
	i = -1;
	assert(deque_search(dek, iptr) == 0);

	return 1;
}

/*
 * Test delete in int deque
 *
 * @l: Pointer to deque
 */
int test_int_delete(struct deque *dek)
{
	int i;
	int *iptr;
	int nmemb_prev;

	assert(dek);

	iptr = &i;

	/* TODO: This is very much dependent on the knowledge
	 * of what is inserted during `test_int_insert()'. It will
	 * be better if this knowledge dependency is removed.
	 */

	nmemb_prev = dek->nmemb;
	i = 0;
	deque_delete(dek, iptr);
	assert(dek->nmemb == nmemb_prev-1);

	nmemb_prev = dek->nmemb;
	i = INITIAL_INS_COUNT-1;
	deque_delete(dek, iptr);
	assert(dek->nmemb == nmemb_prev-1);

	nmemb_prev = dek->nmemb;
	i = INITIAL_INS_COUNT/2;
	deque_delete(dek, iptr);
	assert(dek->nmemb == nmemb_prev-1);

	nmemb_prev = dek->nmemb;
	i = INITIAL_INS_COUNT;
	deque_delete(dek, iptr);
	assert(dek->nmemb == nmemb_prev);

	nmemb_prev = dek->nmemb;
	i = -1;
	deque_delete(dek, iptr);
	assert(dek->nmemb == nmemb_prev);

	return 1;
}

/*
 * Test deuque_pop for int deque
 *
 * @dek: Pointer to the deque structure
 */
int test_int_pop(struct deque *dek)
{
	void *popval;
	int prev_nmemb;

	/*
	 * Try to pop with wrong direction, and assert
	 * that deque is unchanged, and NULL is returned
	 */
	prev_nmemb = dek->nmemb;
	popval = deque_pop(dek, !DEQUE_HEAD);
	assert(popval == NULL);
	assert(prev_nmemb == dek->nmemb);
	free(popval);

	/* Delete until deque is empty */
	while (dek->nmemb != 0) {
		popval = deque_pop(dek, DEQUE_HEAD);
		free(popval);
	}

	/* This is not needed, but anyaways */
	assert(dek->nmemb == 0);

	/* Assert head and tail are updated */
	assert(dek->head == NULL);
	assert(dek->tail == NULL);

	return 1;
}

/* Test int deque */
int test_int(void)
{
	struct deque *dek;

	dek = deque_create(cpy_i, cmp_i, dval, print_int_dekn);
	assert(test_int_create(dek) == 1);
	assert(test_int_insert(dek) == 1);
	deque_print(dek);
	assert(test_int_search(dek) == 1 );
	assert(test_int_delete(dek) == 1 );
	assert(test_int_pop(dek) == 1 );

	deque_destroy(dek);
	dek = NULL;

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
 * Test create function for str deque 
 *
 * @l: pointer to deque
 */
int test_str_create(struct deque *dek)
{
	char *str1;
	char *str2;
	char *img;

	assert(dek);

	/* Test head */
	assert(dek->head == NULL);

	/* Test cpy */
	str1 = "Hello_world!";
	img = dek->cpy(str1);
	assert(strncmp(img, str1, strlen(str1)+1) == 0);
	free(img);

	/* Test cmp */
	str1 = "hello";
	str2 = "gello";
	assert(dek->cmp(str1, str2) == 1);

	str1 = "hello";
	str2 = "hello";
	assert(dek->cmp(str1, str2) == 0);

	str1 = "hello";
	str2 = "iello";
	assert(dek->cmp(str1, str2) == -1);

	/* Test nmemb */
	assert(dek->nmemb == 0);

	return 1;
}

/*
 * Test insert in str deque
 *
 * @l: Pointer to deque
 */
int test_str_insert(struct deque *dek)
{
	int i;
	char str[10] = "hello";
	size_t nmemb_prev;
	void *head_prev;

	/*
	 * Test insert to HEAD side
	 */
	head_prev = dek->head;
	nmemb_prev = dek->nmemb;
	for (i = 0; i < INITIAL_INS_COUNT; i++) {
		str[1] = 'e' + i;
		deque_insert(dek, DEQUE_HEAD, str);
		assert(dek->cmp(dek->head->val, str) == 0);
		assert(dek->nmemb == nmemb_prev + 1);
		assert(dek->head->next == head_prev);
		nmemb_prev = dek->nmemb;
		head_prev = dek->head;
	}


	return 1;
}

/*
 * Test search in a str deque
 *
 * @l: Pointer to deque
 */
int test_str_search(struct deque *dek)
{
	char str[10] = "hello";

	assert(dek);

	/* TODO: This is very much dependent on the knowledge
	 * of what is inserted during `test_int_insert()'. It will
	 * be better if this knowledge dependency is removed.
	 */

	assert(deque_search(dek, str) == 1);
	str[1] = 'e' + 1;
	assert(deque_search(dek, str) == 1);
	str[1] = 'e' + INITIAL_INS_COUNT-1;
	assert(deque_search(dek, str) == 1);
	str[1] = 'd';
	assert(deque_search(dek, str) == 0);
	str[0] = 'a';
	assert(deque_search(dek, str) == 0);

	return 1;
}

/*
 * Test delete in str deque
 *
 * @l: Pointer to deque
 */
int test_str_delete(struct deque *dek)
{
	int i;
	char str[10] = "hello";
	int nmemb_prev;

	assert(dek);

	/* TODO: This is very much dependent on the knowledge
	 * of what is inserted during `test_int_insert()'. It will
	 * be better if this knowledge dependency is removed.
	 */

	nmemb_prev = dek->nmemb;
	i = 0;
	str[1] = 'e' + i;
	deque_delete(dek, str);
	assert(dek->nmemb == nmemb_prev-1);

	nmemb_prev = dek->nmemb;
	i = INITIAL_INS_COUNT-1;
	str[1] = 'e' + i;
	deque_delete(dek, str);
	assert(dek->nmemb == nmemb_prev-1);

	nmemb_prev = dek->nmemb;
	i = INITIAL_INS_COUNT/2;
	str[1] = 'e' + i;
	deque_delete(dek, str);
	assert(dek->nmemb == nmemb_prev-1);

	nmemb_prev = dek->nmemb;
	i = INITIAL_INS_COUNT;
	str[1] = 'e' + i;
	deque_delete(dek, str);
	assert(dek->nmemb == nmemb_prev);

	nmemb_prev = dek->nmemb;
	i = -1;
	str[1] = 'e' + i;
	deque_delete(dek, str);
	assert(dek->nmemb == nmemb_prev);

	return 1;
}

/*
 * Test deuque_pop for str deque
 *
 * @dek: Pointer to the deque structure
 */
int test_str_pop(struct deque *dek)
{
	void *popval;
	int prev_nmemb;

	/*
	 * Try to pop with wrong direction, and assert
	 * that deque is unchanged, and NULL is returned
	 */
	prev_nmemb = dek->nmemb;
	popval = deque_pop(dek, !DEQUE_HEAD);
	assert(popval == NULL);
	assert(prev_nmemb == dek->nmemb);
	free(popval);

	/* Delete until deque is empty */
	while (dek->nmemb != 0) {
		popval = deque_pop(dek, DEQUE_HEAD);
		free(popval);
	}

	/* This is not needed, but anyaways */
	assert(dek->nmemb == 0);

	/* Assert head and tail are updated */
	assert(dek->head == NULL);
	assert(dek->tail == NULL);

	return 1;
}

/* Test str deque */
int test_str(void)
{
	struct deque *dek;

	dek = deque_create(cpy_s, cmp_s, dval, NULL);
	assert(test_str_create(dek) == 1);
	assert(test_str_insert(dek) == 1);
	deque_print(dek);
	assert(test_str_search(dek) == 1);
	assert(test_str_delete(dek) == 1);
	assert(test_str_pop(dek) == 1);

	deque_destroy(dek);
	dek = NULL;

	return 1;
}

/* main: start */
int main(void)
{
	test_int();
	test_str();
	return 0;
}

