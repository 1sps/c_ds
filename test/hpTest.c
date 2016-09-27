/*
 * test/hpTest.c: Test heap.c implementation
 *
 * St: 2016-09-26 Mon 09:21 PM
 * Up: 2016-09-27 Tue 02:29 AM
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
#define INIT_HEAP_CAP 5
#define INIT_INSERT_COUNT 10

/*
 * Check if there is a TEST ERROR
 *
 * @q: Pointer to the queue
 */
int is_test_err(struct heap *h)
{
	int test_err;

	test_err = 0;

	if (h == NULL)
		test_err = TEST_ERR;
	
	return test_err;
}

/*
 * Print a node of int heap
 *
 * @ptr: Pointer to the int to print
 */
void printn_i(void *ptr)
{
	printf("%d, ", *(int *) ptr);
}

/*
 * Print a node of str heap
 *
 * @ptr: Pointer to the str to print
 */
void printn_s(void *ptr)
{
	printf("%s, ", (char *) ptr);
}

/*
 * Test if integer array is sorted.
 *
 * @arr:
 * @nmemb:
 */
int iarr_is_sorted(void **arr, size_t nmemb)
{
	return 1;
}

/*
 *******************************************************************************
 * Str Test Section
 *******************************************************************************
 */


/*
 * Test hp_create for int heap.
 *
 * @h: Pointer to the heap structure
 */
int test_int_create(struct heap *h)
{
	int val1;
	int val2;
	int *val1_ptr;
	int *val2_ptr;
	int *img;

	/* Assert that there is no TEST ERROR */
	assert(is_test_err(h) != TEST_ERR);

	val1_ptr = &val1;
	val2_ptr = &val2;

	/* Test cpy */
	val1 = 10;
	img = h->cpy(val1_ptr);
	assert(*img = val1);
	free(img);

	/* Test cmp */
	val1 = 10;
	val2 = 5;
	assert(h->cmp(val1_ptr, val2_ptr) == 1);

	val1 = -10;
	val2 = -10;
	assert(h->cmp(val1_ptr, val2_ptr) == 0);

	val1 = -10;
	val2 = 5;
	assert(h->cmp(val1_ptr, val2_ptr) == -1);

	/* Test cap and nmemb */
	assert(h->cap == INIT_HEAP_CAP);
	assert(h->nmemb == 0);

	/* Test hparr */
	/* TODO: What if INIT_HEAP_CAP is 0? */
	assert(h->hparr != NULL);
	assert(h->hparr[INIT_HEAP_CAP-1] == NULL);

	return 1;
}

/*
 * Test hp_insert for int heap.
 *
 * @h: Pointer to the heap structure
 */
int test_int_insert(struct heap *h)
{
	int i;
	int j;
	int *jptr;
	void **sarr;
	int prev_nmemb;

	/* Assert that there is no TEST ERROR */
	assert(is_test_err(h) != TEST_ERR);

	jptr = &j;

	/* Insert items into heap */
	for (i = 0; i < INIT_INSERT_COUNT; i++) {
		j = INIT_INSERT_COUNT - i;
		prev_nmemb = h->nmemb;
		hp_insert(h, jptr);
		assert(h->nmemb == prev_nmemb+1);
	}

	/* Get a sorted array from the heap */
	sarr = get_sorted_arr(h);

	/* Assert that the array is sorted */
	assert(hp_arr_is_sorted(h, sarr, h->nmemb) == 1);

	/* Free the array and its contents */
	for (i = 0; i < INIT_INSERT_COUNT; i++)
		free(sarr[i]);
	free(sarr);

	return 1;
}

/* Test int heap */
int test_heap_int(void)
{
	struct heap *h;

	h = hp_create(INIT_HEAP_CAP, MIN_HEAP, cpy_i, cmp_i);
	test_int_create(h);
	test_int_insert(h);

	hp_destroy(h);

	return 1;
}


/*
 *******************************************************************************
 * Str Test Section
 *******************************************************************************
 */


/*
 * Test create function for str heap
 *
 * @h: pointer to heap
 */
int test_str_create(struct heap *h)
{
	char *str1;
	char *str2;
	char *img;

	/* Assert that there is no TEST ERROR */
	assert(is_test_err(h) != TEST_ERR);

	/* Test cpy */
	str1 = "Hello_world!";
	img = h->cpy(str1);
	assert(strncmp(img, str1, strlen(str1)+1) == 0);
	free(img);

	/* Test cmp */
	str1 = "hello";
	str2 = "gello";
	assert(h->cmp(str1, str2) == 1);

	str1 = "hello";
	str2 = "hello";
	assert(h->cmp(str1, str2) == 0);

	str1 = "hello";
	str2 = "iello";
	assert(h->cmp(str1, str2) == -1);

	/* Test cap and nmemb */
	assert(h->cap == INIT_HEAP_CAP);
	assert(h->nmemb == 0);

	/* Test hparr */
	/* TODO: What if INIT_HEAP_CAP is 0? */
	assert(h->hparr != NULL);
	assert(h->hparr[INIT_HEAP_CAP-1] == NULL);

	return 1;
}

/*
 * Test hp_insert for str heap.
 *
 * @h: Pointer to the heap structure
 */
int test_str_insert(struct heap *h)
{
	int i;
	int j;
	char str[10] = "test";
	void **sarr;
	int prev_nmemb;

	/* Assert that there is no TEST ERROR */
	assert(is_test_err(h) != TEST_ERR);

	/* Insert items into heap */
	for (i = 0; i < INIT_INSERT_COUNT; i++) {
		j = INIT_INSERT_COUNT - i;
		prev_nmemb = h->nmemb;
		str[0] = 'h' + j;
		hp_insert(h, str);
		assert(h->nmemb == prev_nmemb+1);
	}

	/* Get a sorted array from the heap */
	sarr = get_sorted_arr(h);

	/* Assert that the array is sorted */
	assert(hp_arr_is_sorted(h, sarr, h->nmemb) == 1);

	/* Free the array and its contents */
	for (i = 0; i < INIT_INSERT_COUNT; i++)
		free(sarr[i]);
	free(sarr);

	return 1;
	
}

/* Test str heap */
int test_heap_str(void)
{
	struct heap *h;

	h = hp_create(INIT_HEAP_CAP, MIN_HEAP, cpy_s, cmp_s);
	test_str_create(h);
	test_str_insert(h);

	hp_destroy(h);

	return 1;
}


/* main: start */
int main(void)
{
	test_heap_int();
	test_heap_str();
	return 0;
}


/*
 * NOTES:
 *
 * This test code is not really complete for the following reasons:
 *
 * 1. Test for hp_insert is minimal.
 * 2. There is no test functions for functions other than hp_create and
 *    hp_insert.
 * 3. There is no test funciton for max heap.
 */
