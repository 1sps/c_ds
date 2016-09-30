/*
 * test/bstTest.c: Test BST implementation
 *
 * St: 2016-09-29 Thu 04:43 AM
 * Up: 2016-09-30 Fri 04:57 AM
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
#include<time.h>

#include"funcutils.h"
#include"../src/mylib.h"

#define SKIP

/* Function prototype declarations of some utility functions  */
void printn_i(struct bst_node *bstn);
void printn_s(struct bst_node *bstn);
void dval_int_str(void *val);

/*******************************************************************************
 * Int Test Section
 ******************************************************************************/

int test_int_bst_insert(void)
{
	int i;
	int j;
	int *jptr;
	int prev_nmemb;
	struct bst *t;

	/* Create a bst */
	t = bst_create(cpy_i, cmp_i, dval_int_str, printn_i);

	/* Insert random values */

	srand(time(NULL));
	jptr = &j;

	for (i= 0; i < 1000; i++) {
		prev_nmemb = t->nmemb;
		j = rand();
		bst_insert(t, jptr);
		assert(t->nmemb == prev_nmemb+1);
	}

	/*
	 * Do an in-order walk of the tree. This should result
	 * in a sorted array. Assert this.
	 * TODO:
	 */
	
	/* Destroy bst */
	bst_destroy(t);

	return 1;
}

/* Test bst_create function for an int bst */
int test_int_bst_create(void) 
{
	struct bst *t;

	/* Create a bst */
	t = bst_create(cpy_i, cmp_i, dval_int_str, printn_i);

	/* Test copy and compare functions */
	assert(test_cpy_i(t->cpy) == 1);
	assert(test_cmp_i(t->cmp) == 1);

	/* Test root is NULL at beginning */
	assert(t->root == NULL);

	/* Test number of members is zero at beginning */
	assert(t->nmemb == 0);

	/* TODO */
	/* Test dval and printn functions? */

	/* Destroy bst */
	bst_destroy(t);

	return 1;
}

/* Test bst_delete for int bst */
int test_int_bst_delete(void)
{
	int j;
	int *jptr;
	int prev_nmemb;
	void *tmp;
	struct bst *t;

	/* Create a bst */
	t = bst_create(cpy_i, cmp_i, dval_int_str, printn_i);

	/*
	 * Insert definite values to make following tree
	 *
	 *                   10
	 *              5          15
	 *            2   6     12    18
	 *                    11         19
	 */

	jptr = &j;

	j = 10;
	bst_insert(t, jptr);

	j = 5;
	bst_insert(t, jptr);

	j = 6;
	bst_insert(t, jptr);

	j = 2;
	bst_insert(t, jptr);

	j = 15;
	bst_insert(t, jptr);

	j = 12;
	bst_insert(t, jptr);

	j = 18;
	bst_insert(t, jptr);

	j = 11;
	bst_insert(t, jptr);

	j = 19;
	bst_insert(t, jptr);

	/* Delete a non-existent node */
	j = 100;
	prev_nmemb = t->nmemb; 
	tmp = bst_delete(t, jptr); 
	free(tmp); 
	assert(t->nmemb == prev_nmemb); 

	/* Delete a leaf node */
	j = 2;
	prev_nmemb = t->nmemb; 
	tmp = bst_delete(t, jptr); 
	free(tmp); 
	assert(t->nmemb == prev_nmemb-1); 

	/* Delete a node with only left child */
	j = 12;
	prev_nmemb = t->nmemb; 
	tmp = bst_delete(t, jptr); 
	free(tmp); 
	assert(t->nmemb == prev_nmemb-1); 

	/* Delete a node with only right child */
	j = 18;
	prev_nmemb = t->nmemb; 
	tmp = bst_delete(t, jptr); 
	free(tmp); 
	assert(t->nmemb == prev_nmemb-1); 

	/*
	 * Delete a node with both children
	 * and also root node
	 */
	j = 10;
	prev_nmemb = t->nmemb; 
	tmp = bst_delete(t, jptr); 
	free(tmp); 
	assert(t->nmemb == prev_nmemb-1); 

	/*
	 * TODO: Assert after each delete BST property is
	 * preserved
	 */

	/* Destroy bst */
	bst_destroy(t);

	return 1;
}

/* Test int bst */
int test_int_bst(void)
{
	test_int_bst_create();
	test_int_bst_insert();
	/* test_int_bst_search(); */
	test_int_bst_delete(); 
	return 1;
}

/*******************************************************************************
 * Str Test Section
 ******************************************************************************/

/* Test bst_create function for an int bst */
int test_str_bst_create(void) 
{
	struct bst *t;

	/* Create a bst */
	t = bst_create(cpy_s, cmp_s, dval_int_str, printn_s);

	/* Test copy and compare functions */
	assert(test_cpy_s(t->cpy) == 1);
	assert(test_cmp_s(t->cmp) == 1);

	/* Test root is NULL at beginning */
	assert(t->root == NULL);

	/* Test number of members is zero at beginninh */
	assert(t->nmemb == 0);

	/* TODO */
	/* Test dval and printn functions? */

	bst_destroy(t);
	
	return 1;
}

/* Test str bst */
int test_str_bst(void)
{
	test_str_bst_create();
	return 1;
}

/*******************************************************************************
 * Main Section
 ******************************************************************************/

/* main: Perform bst testing */
int main(void)
{
	test_int_bst();
	/* test_str_bst(); */
	return 0;
}

/*******************************************************************************
 * Utility Functions Section
 ******************************************************************************/
/*
 * Print a node of int bst
 *
 * @bstn: Pointer to a bst node
 */
void printn_i(struct bst_node *bstn)
{
	printf("%d, ", *(int *) bstn->val);
}

/*
 * Print a node of str bst
 *
 * @bstn: Pointer to a bst node
 */
void printn_s(struct bst_node *bstn)
{
	printf("%s, ", (char *) bstn->val);
}


/*
 * Destroy a val
 *
 * @val: Pointer the val
 */
void dval_int_str(void *val)
{
	free(val);
}

