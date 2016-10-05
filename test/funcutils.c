/*
 * test/funcutils.c: Contains utility functions
 *
 * St: 2016-09-26 Mon 06:22 PM
 * Up: 2016-10-04 Tue 09:40 PM
 *
 * Author: SPS
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

#include "funcutils.h"
#include "../src/mylib.h"

#define SKIP

/*
 *******************************************************************************
 * Str Section
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

	if (src == NULL)
		return NULL;

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
 * Return size of an int
 *
 * @vptr: Pointer to an int. However this is not used
 *        here, but included for syntantic reasons.
 */
int get_int_size(void *vptr)
{
	return sizeof(int);
}

/*
 * Destroy an int allocated in memory
 *
 * @ival: Pointer to the int in memory
 */
void dval_i(void *ival)
{
	free(ival);
}

/*
 * Test correctness of int copy function `cpy'.
 *
 * @cpy: Pointer to copy funtion to be tested
 */
int test_cpy_i(void *(*cpy)(void *))
{
	int val1;
	int *val1_ptr;
	int *img;

	val1_ptr = &val1;

	/* Test for positive value */
	val1 = 1000;
	img = cpy(val1_ptr);
	assert(*img == val1);
	free(img);

	/* Test for 0 */
	val1 = 0;
	img = cpy(val1_ptr);
	assert(*img == val1);
	free(img);

	/* Test for negative value */
	val1 = -1000;
	img = cpy(val1_ptr);
	assert(*img == val1);
	free(img);

	return 1;
}

/*
 * Test int compare function.
 *
 * @cmp: Pointer to int compare function to test
 */
int test_cmp_i(int (*cmp)(void *, void *))
{
	int val1;
	int val2;
	int *val1_ptr;
	int *val2_ptr;

	val1_ptr = &val1;
	val2_ptr = &val2;

	/* Test cmp */
	val1 = 10;
	val2 = 5;
	assert(cmp(val1_ptr, val2_ptr) == 1);

	val1 = -10;
	val2 = -10;
	assert(cmp(val1_ptr, val2_ptr) == 0);

	val1 = -10;
	val2 = 5;
	assert(cmp(val1_ptr, val2_ptr) == -1);

	return 1;
}

/*
 * Test get_key_size function for int
 *
 * @get_key_size: Pointer to the funciton to test
 */
int test_get_size_i(int (*get_key_size)(void *))
{
	int i;
	int *iptr;

	iptr = &i;
	i = 1;

	assert(get_key_size(iptr) == sizeof(int));

	return 1;
}


/*
 *******************************************************************************
 * Str Section
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

	if (src == NULL)
		return NULL;

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
 * Return size of a stringtval.
 *
 * @vptr: Pointer to string whose size 
 *        is to be determined
 */
int get_str_size(void *vptr)
{
	int i;
	int retval;
	char *str;

	str = (char *) vptr;

	for (i = 0; str[i] != '\0'; i++)
		;

	retval = i;

	return retval;
}

/*
 * Destroy an str allocated in memory
 *
 * @ival: Pointer to the str in memory
 */
void dval_s(void *sval)
{
	free(sval);
}


/*
 * Test correctness of str copy function `cpy'.
 *
 * @cpy: Pointer to str copy funtion to be tested
 */
int test_cpy_s(void *(*cpy)(void *))
{
	char *str;
	char *img;

	/* Test simple string */
	str = "Hello_world!";
	img = cpy(str);
	assert(strncmp(img, str, strlen(str)+1) == 0);
	free(img);

	/* Test empty string */
	str = "";
	img = cpy(str);
	assert(strncmp(img, str, strlen(str)+1) == 0);
	free(img);

	/* Test string of length 1 */
	str = "1";
	img = cpy(str);
	assert(strncmp(img, str, strlen(str)+1) == 0);
	free(img);

	/* Test not so simple string */
	str = "!@#$%^&*()1234567890asdfmnbLKJHjhj____";
	img = cpy(str);
	assert(strncmp(img, str, strlen(str)+1) == 0);
	free(img);

	return 1;
}

/*
 * Test str compare function.
 *
 * @cmp: Pointer to str compare function to test
 */
int test_cmp_s(int (*cmp)(void *, void *))
{
	char *str1;
	char *str2;

	/* Test cmp */
	str1 = "hello";
	str2 = "gello";
	assert(cmp(str1, str2) > 0);

	str1 = "hello";
	str2 = "hello";
	assert(cmp(str1, str2) == 0);

	str1 = "hello";
	str2 = "iello";
	assert(cmp(str1, str2) < 0);

	return 1;
}

/*
 * Test get_key_size function for str
 *
 * @get_key_size: Pointer to the funciton to test
 */
int test_get_size_s(int (*get_key_size)(void *))
{
	char str[30] = "!@_test_test_123##";

	assert(get_key_size(str) == strlen(str));

	str[1] = 0;
	assert(get_key_size(str) == strlen(str));

	str[0] = 0;
	assert(get_key_size(str) == strlen(str));

	return 1;
}

