/*
 * test/htTest.c:
 *
 * St: 2016-09-27 Tue 01:50 PM
 * Up: 2016-09-27 Tue 11:38 PM
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
#define TOT_SLOTS 5

/*
 * Different hash table types
 */
#define KINT_VINT 10      /* int key and int val */
#define KINT_VSTR 11      /* int key and str val */
#define KSTR_VINT 12      /* str key and int val */
#define KSTR_VSTR 13      /* str key and str val */

/*
 * Test ht_create function for hash table.
 *
 * Following tests are performed:
 *
 * 1. h->tot_slots is correct
 * 2. Agnostic of h->table, as there is no straight forward
 *    way to test its correctness.
 * 3. h->k_cpy is correct
 * 4. h->k_cmp is correct
 * 5. h->v_cpy is correct
 * 6. h->v_cmp is correct
 * 7. h->get_key_size is correct
 *
 * @h:    Pointer to the hash table structure
 * @type: Type of hash table
 */
int test_ht_create(struct ht *h, int type)
{
	assert(h->tot_slots == TOT_SLOTS);

	switch(type) {
	case KINT_VINT:
		assert(test_cpy_i(h->k_cpy) == 1);
		assert(test_cmp_i(h->k_cmp) == 1);
		assert(test_cpy_i(h->v_cpy) == 1);
		assert(test_cmp_i(h->v_cmp) == 1);
		assert(test_get_size_i(h->get_key_size) == 1);
		break;
	case KINT_VSTR:
		assert(test_cpy_i(h->k_cpy) == 1);
		assert(test_cmp_i(h->k_cmp) == 1);
		assert(test_cpy_s(h->v_cpy) == 1);
		assert(test_cmp_s(h->v_cmp) == 1);
		assert(test_get_size_i(h->get_key_size) == 1);
		break;
	case KSTR_VINT:
		assert(test_cpy_s(h->k_cpy) == 1);
		assert(test_cmp_s(h->k_cmp) == 1);
		assert(test_cpy_i(h->v_cpy) == 1);
		assert(test_cmp_i(h->v_cmp) == 1);
		assert(test_get_size_s(h->get_key_size) == 1);
		break;
	case KSTR_VSTR:
		assert(test_cpy_s(h->k_cpy) == 1);
		assert(test_cmp_s(h->k_cmp) == 1);
		assert(test_cpy_s(h->v_cpy) == 1);
		assert(test_cmp_s(h->v_cmp) == 1);
		assert(test_get_size_s(h->get_key_size) == 1);
		break;
	default:
		assert(TEST_ERR == 0);
	}

	return 1;
}

/* Test hash table with int key and int val */
int test_ht_kint_vint(void)
{
	struct ht *h;

	int k;
	int v;
	int *kptr;
	int *vptr;

	h = ht_create(TOT_SLOTS, cpy_i, cpy_i, cmp_i, cmp_i, get_int_size);
	test_ht_create(h, KINT_VINT);

	kptr = &k;
	vptr = &v;

	k = 100; v = 300;
	ht_insert(h, kptr, vptr);
	k = 101; v = 301;
	ht_insert(h, kptr, vptr);
	k = 102; v = 302;
	ht_insert(h, kptr, vptr);
	k = 103; v = 303;
	ht_insert(h, kptr, vptr);
	k = 104; v = 304;
	ht_insert(h, kptr, vptr);
	k = 105; v = 305;
	ht_insert(h, kptr, vptr);

	ht_print(h, KINT_VINT);
	printf("<_____________________________________________________>\n\n");
	ht_destroy(h);
	return 1;
}

/* Test hash table with int key and str val */
int test_ht_kint_vstr(void)
{
	struct ht *h;
	int k;
	int *kptr;

	h = ht_create(TOT_SLOTS, cpy_i, cpy_s, cmp_i, cmp_s, get_int_size);
	test_ht_create(h, KINT_VSTR);

	kptr = &k;

	k = 100;
	ht_insert(h, kptr, "Tinker");
	k = 101; 
	ht_insert(h, kptr, "Tinker");
	k = 102; 
	ht_insert(h, kptr, "Tinker");
	k = 103; 
	ht_insert(h, kptr, "Tinker");
	k = 104; 
	ht_insert(h, kptr, "Tinker");
	k = 105; 
	ht_insert(h, kptr, "Tinker");
	ht_print(h, KINT_VSTR);
	printf("<_____________________________________________________>\n\n");
	ht_destroy(h);

	return 1;
}

/* Test hash table with str key and int val */
int test_ht_kstr_vint(void)
{
	struct ht *h;
	int v;
	int *vptr;

	h = ht_create(TOT_SLOTS, cpy_s, cpy_i, cmp_s, cmp_i, get_str_size);
	test_ht_create(h, KSTR_VINT);

	vptr = &v;

	v = 100;
	ht_insert(h, "Tinker", vptr);
	v = 101; 
	ht_insert(h, "Uinker", vptr);
	v = 102; 
	ht_insert(h, "Vinker", vptr);
	v = 103; 
	ht_insert(h, "Winker", vptr);
	v = 104; 
	ht_insert(h, "Xinker", vptr);
	v = 105; 
	ht_insert(h, "Yinker", vptr);
	ht_print(h, KSTR_VINT);
	printf("<_____________________________________________________>\n\n");

	ht_destroy(h);

	return 1;
}

/* Test hash table with str key and str val */
int test_ht_kstr_vstr(void)
{
	struct ht *h;

	h = ht_create(TOT_SLOTS, cpy_s, cpy_s, cmp_s, cmp_s, get_str_size);
	test_ht_create(h, KSTR_VSTR);

	ht_insert(h, "name", "lahm");
	ht_insert(h, "game", "football");
	ht_insert(h, "place", "munich");
	ht_insert(h, "country", "germany");
	ht_insert(h, "team", "bayern");
	ht_insert(h, "name", "phillip");
	ht_insert(h, "name", "phillip");
	ht_insert(h, "name", "thomas");

	ht_insert(h, "mame", "thomas");
	ht_insert(h, "aame", "thomas");
	ht_insert(h, "oame", "thomas");
	ht_insert(h, "pame", "thomas");
	ht_insert(h, "qame", "thomas");
	ht_insert(h, "rame", "thomas");
	ht_insert(h, "same", "thomas");
	ht_insert(h, "tame", "thomas");
	ht_insert(h, "uame", "thomas");
	ht_insert(h, "vame", "thomas");
	ht_print(h, KSTR_VSTR);
	printf("<_____________________________________________________>\n\n");

	assert(ht_search(h, "name") == 1);
	assert(ht_search(h, "tameee") == 0);
	assert(ht_search(h, "same") == 1);
	ht_delete(h, "same");
	assert(ht_search(h, "same") == 0);

	ht_destroy(h);

	return 1;
}

/* main: start */
int main(void)
{
	test_ht_kint_vint();
	test_ht_kint_vstr();
	test_ht_kstr_vint();
	test_ht_kstr_vstr();

	return 0;
}

