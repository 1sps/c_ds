/*
 * test/qTest.c: Test queue implementation
 *
 * St: 2016-09-26 Mon 06:20 PM
 * Up: 2016-09-26 Mon 08:35 PM
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

#include"funcutils.h"
#include"../src/mylib.h"

#define SKIP
#define TEST_ERR 1

/* Total nodes to insert while doing insert test */
#define INITIAL_INS_COUNT 10


/*
 * Check if there is a TEST ERROR
 *
 * @q: Pointer to the queue
 */
int is_test_err(struct queue *q)
{
	int test_err;

	test_err = 0;

	if (q == NULL)
		test_err = TEST_ERR;
	
	return test_err;
}

/*
 * Print a node of an int queue
 *
 * @qn: queue node
 */
void printn_i(struct q_node *qn)
{
	printf("%d, ", *(int *) qn->val);
}

/*
 * Print a node of a str queue
 *
 * @qn: queue node
 */
void printn_s(struct q_node *qn)
{
	printf("%s ", (char *) qn->val);
}


/*
 *******************************************************************************
 * Int Test Section
 *******************************************************************************
 */


/*
 * Test st_create function for int stack
 *
 * @s: Pointer to stack
 */
int test_int_create(struct queue *q)
{
	int val1;
	int val2;
	int *val1_ptr;
	int *val2_ptr;
	int *img;

	/* Assert that there is no TEST ERROR */
	assert(is_test_err(q) != TEST_ERR);

	val1_ptr = &val1;
	val2_ptr = &val2;

	/* Test head and tail */
	assert(q->head == NULL);
	assert(q->tail == NULL);

	/* Test cpy */
	val1 = 10;
	img = q->cpy(val1_ptr);
	assert(*img = val1);
	free(img);

	/* Test cmp */
	val1 = 10;
	val2 = 5;
	assert(q->cmp(val1_ptr, val2_ptr) == 1);

	val1 = -10;
	val2 = -10;
	assert(q->cmp(val1_ptr, val2_ptr) == 0);

	val1 = -10;
	val2 = 5;
	assert(q->cmp(val1_ptr, val2_ptr) == -1);

	/* Test nmemb */
	assert(q->nmemb == 0);

	return 1;
}

/*
 * Test q_push for int queue.
 *
 * @q: Pointer to the queue
 */
int test_int_push(struct queue *q)
{
	int i;
	int *iptr;
	size_t nmemb_prev;
	struct q_node *head_prev;
	struct q_node *tail_prev;

	/* Assert that there is no TEST ERROR */
	assert(is_test_err(q) != TEST_ERR);

	head_prev = q->head;
	tail_prev = q->tail;
	nmemb_prev = q->nmemb;
	iptr = &i;
	for (i = 0; i < INITIAL_INS_COUNT; i++) {
		q_push(q, iptr);
		assert(q->cmp(q->tail->val, iptr) == 0);
		assert(q->nmemb == nmemb_prev + 1);
		/* Assert head is updated correctly */
		if (head_prev == NULL)
			assert(q->head != NULL &&
			       q->cmp(q->head->val, iptr) == 0);
		else
			assert(q->head = head_prev);
		/* Assert tail is updated correctly */
		if (tail_prev == NULL)
			assert(q->tail != NULL &&
			       q->cmp(q->tail->val, iptr) == 0);
		else
			assert(tail_prev->next = q->tail);
		nmemb_prev = q->nmemb;
		head_prev = q->head;
		tail_prev = q->tail;
	}

#ifdef VISUAL_PRINT
	q_print(q);
#endif /* VISUAL_PRINT */

	return 1;
}

/*
 * Test q_pop for int queue.
 *
 * @q: Pointer to the queue.
 */
int test_int_pop(struct queue *q)
{
	int prev_nmemb;
	void *pop_val;
	void *prev_head_val;
	struct q_node *prev_head_next;

	/* Assert that there is no TEST ERROR */
	assert(is_test_err(q) != TEST_ERR);

	/* Test pop from non-empty queue */
	while (!q_is_empty(q)) {
		prev_nmemb = q->nmemb;
		prev_head_val = q->cpy(q->head->val);
		prev_head_next = q->head->next;
		pop_val = q_pop(q);
		assert(q->cmp(pop_val, prev_head_val) == 0);
		assert(q->nmemb == prev_nmemb-1);
		assert(q->head == prev_head_next);
		free(prev_head_val);
		free(pop_val);
	}
	assert(q->nmemb == 0);
	assert(q->head == NULL);
	assert(q->tail == NULL);

	/* Test pop from non-empty queue */
	pop_val = q_pop(q);
	assert(pop_val == NULL);

	return 1;
}

/* Test queue implementation for int queue */
int test_q_int(void)
{
	struct queue *q;

	q = q_create(cpy_i, cmp_i, printn_i);
	assert(test_int_create(q) == 1);
	assert(test_int_push(q) == 1);
	q_print(q);
	assert(test_int_pop(q) == 1);

	q_destroy(q);

	return 1;
}

/*
 *******************************************************************************
 * Str Test Section
 *******************************************************************************
 */

/*
 * Test create function for str linked list 
 *
 * @l: pointer to linked list
 */
int test_str_create(struct queue *q)
{
	char *str1;
	char *str2;
	char *img;

	/* Assert that there is no TEST ERROR */
	assert(is_test_err(q) != TEST_ERR);

	/* Test head and tail */
	assert(q->head == NULL);
	assert(q->tail == NULL);

	/* Test cpy */
	str1 = "Hello_world!";
	img = q->cpy(str1);
	assert(strncmp(img, str1, strlen(str1)+1) == 0);
	free(img);

	/* Test cmp */
	str1 = "hello";
	str2 = "gello";
	assert(q->cmp(str1, str2) == 1);

	str1 = "hello";
	str2 = "hello";
	assert(q->cmp(str1, str2) == 0);

	str1 = "hello";
	str2 = "iello";
	assert(q->cmp(str1, str2) == -1);

	/* Test nmemb */
	assert(q->nmemb == 0);

	return 1;
}

/*
 * Test insert in str linked list
 *
 * @l: Pointer to linked list
 */
int test_str_push(struct queue *q)
{
	int i;
	char str[10] = "hello";
	size_t nmemb_prev;
	struct q_node *head_prev;
	struct q_node *tail_prev;;

	/* Assert that there is no TEST ERROR */
	assert(is_test_err(q) != TEST_ERR);

	head_prev = q->head;
	tail_prev = q->tail;
	nmemb_prev = q->nmemb;
	for (i = 0; i < INITIAL_INS_COUNT; i++) {
		str[1] = 'e' + i;
		q_push(q, str);
		assert(q->cmp(q->tail->val, str) == 0);
		assert(q->nmemb == nmemb_prev + 1);
		/* Assert head is updated correctly */
		if (head_prev == NULL)
			assert(q->head != NULL &&
			       q->cmp(q->head->val, str) == 0);
		else
			assert(q->head = head_prev);
		/* Assert tail is updated correctly */
		if (tail_prev == NULL)
			assert(q->tail != NULL &&
			       q->cmp(q->tail->val, str) == 0);
		else
			assert(tail_prev->next = q->tail);
		nmemb_prev = q->nmemb;
		head_prev = q->head;
		tail_prev = q->tail;
	}

#ifdef VISUAL_PRINT
	q_print(q);
#endif /* VISUAL_PRINT */

	return 1;
}

/*
 * Test q_pop for str queue.
 *
 * @q: Pointer to the queue.
 */
int test_str_pop(struct queue *q)
{
	int prev_nmemb;
	void *pop_val;
	void *prev_head_val;
	struct q_node *prev_head_next;

	/* Assert that there is no TEST ERROR */
	assert(is_test_err(q) != TEST_ERR);

	/* Test pop from non-empty queue */
	while (!q_is_empty(q)) {
		prev_nmemb = q->nmemb;
		prev_head_val = q->cpy(q->head->val);
		prev_head_next = q->head->next;
		pop_val = q_pop(q);
		assert(q->cmp(pop_val, prev_head_val) == 0);
		assert(q->nmemb == prev_nmemb-1);
		assert(q->head == prev_head_next);
		free(prev_head_val);
		free(pop_val);
	}
	assert(q->nmemb == 0);
	assert(q->head == NULL);
	assert(q->tail == NULL);

	/* Test pop from non-empty stack */
	pop_val = q_pop(q);
	assert(pop_val == NULL);

	return 1;
}

/* Test queue implementation for str queue */
int test_q_str(void)
{
	struct queue *q;

	q = q_create(cpy_s, cmp_s, printn_s);
	assert(test_str_create(q) == 1);
	assert(test_str_push(q) == 1);
	q_print(q);
	assert(test_str_pop(q) == 1);

	q_destroy(q);

	return 1;
}

/* main: perform testing of queue implementation */
int main(void)
{
	test_q_int();
	test_q_str();
	return 0;
}

