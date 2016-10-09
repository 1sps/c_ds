/*
 * test/avl_test.c: Test src/avl.c
 *
 * St: 2016-10-05 Wed 08:42 PM
 * Up: 2016-10-06 Thu 12:20 AM
 *
 * Author: SPS
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

#include"../src/mylib.h"
#include"funcutils.h"

/* Function prototype declaration for utility functions */
void printn_i(void *);
void printn_s(void *);

/* Test avl_create() for int avl */
int test_avl_int_create(void)
{
	struct avl *avlt;

	avlt = avl_create(cpy_i, cmp_i, dval_i, printn_i);

	assert(avlt->root == NULL);
	assert(avlt->cmp == cmp_i);
	assert(avlt->cpy == cpy_i);
	assert(avlt->dval == dval_i);
	assert(avlt->printn == printn_i);
	assert(avlt->nmemb == 0);

	avl_destroy(avlt);

	return 1;
}

/* Test AVL implementaion for int AVL */
int test_avl_int(void)
{
	test_avl_int_create();

	return 1;
}

/* main: start */
int main(void)
{
	test_avl_int();
	return 0;
}


/*
 *******************************************************************************
 * Functions passed while creating AVL 
 *******************************************************************************
 */


/*
 * Print an str data 
 *
 * @data: Pointer to data print
 */
void printn_s(void *data)
{
	printf("%s, ", (char *) data);
}

/*
 * Print an int data 
 *
 * @data: Pointer to data to print
 */
void printn_i(void *data)
{
	printf("%d, ", *(int *) data);
}

