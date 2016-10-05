/*
 * heap.c: Heap implementation
 *
 * St: 2016-09-26 Mon 09:20 PM
 * Up: 2016-10-04 Tue 08:55 PM
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

#include"mylib.h"

#define SKIP


/*
 * Swap two elements of an array of struct hp_data pointers.
 *
 * @arr:  Array of void pointers
 * @pos1: Position of first element
 * @pos2: Position of second element 
 */
static void swap(struct hp_data **arr, int pos1, int pos2)
{
	struct hp_data *tmp;

	tmp = arr[pos1];
	arr[pos1] = arr[pos2];
	arr[pos2] = tmp;
}

/*
 * Create a heap.
 *
 * @cap:  Capacity of the heap. This is maximum number of members 
 *        this heap can hold.       
 * @type: Type of heap. It can be either MAX_HEAP or MIN_HEAP.
 * @cpy:  Pointer to copy function.
 * @k_cmp:  Pointer to compare function.
 */
struct heap *hp_create(size_t cap, char type,
                       void *(*k_cpy)(void *),
                       void *(*v_cpy)(void *),
		       int (*k_cmp)(void *, void *),
		       int (*v_cmp)(void *, void *),
		       void (*k_dval)(void *),
		       void (*v_dval)(void *))
{
	struct heap *h;

	h = malloc(sizeof(struct heap));
	assert(h);

	/*
	 * TODO: Is it better to assert that the arguments passed
	 *       look sane?
	 */

	/* TODO: calloc or malloc and memset? */
	h->hparr = calloc(cap, sizeof(struct hp_data *));
	assert(h->hparr);

	h->cap = cap;
	h->nmemb = 0;
	h->type = type;
	h->k_cpy = k_cpy;
	h->v_cpy = v_cpy;
	h->k_cmp = k_cmp;
	h->v_cmp = v_cmp;
	h->k_dval = k_dval;
	h->v_dval = v_dval;

	return h;
}

/* Helper of hp_float_down */
static void hp_float_down_min(struct heap *h, int pos)
{
	int smallest;

	for (;;) {
		smallest = pos;
		if (Child(pos, 0) < h->nmemb &&
		    h->k_cmp(h->hparr[Child(pos, 0)]->key, h->hparr[smallest]->key) < 0)
		 	smallest = Child(pos, 0);
		if (Child(pos, 1) < h->nmemb &&
		    h->k_cmp(h->hparr[Child(pos, 1)]->key, h->hparr[smallest]->key) < 0)
		 	smallest = Child(pos, 1);
		if (smallest != pos) {
			swap(h->hparr, pos, smallest);
			pos = smallest;
		} else {
			break;
		}
	}
}

/* Helper of hp_float_down */
static void hp_float_down_max(struct heap *h, int pos)
{
	int largest;

	for (;;) {
		largest = pos;
		if (Child(pos, 0) < h->nmemb &&
		    h->k_cmp(h->hparr[Child(pos, 0)]->key, h->hparr[largest]->key) > 0)
		 	largest = Child(pos, 0);
		if (Child(pos, 1) < h->nmemb &&
		    h->k_cmp(h->hparr[Child(pos, 1)]->key, h->hparr[largest]->key) > 0)
		 	largest = Child(pos, 1);
		if (largest != pos) {
			swap(h->hparr, pos, largest);
			pos = largest;
		} else {
			break;
		}
	}
}

/*
 * Float a node down the heap.
 *
 * @h:   Pointer to the heap structure
 * @pos: Postition of the node to float down
 */
static void hp_float_down(struct heap *h, int pos)
{
	if (h->type == MIN_HEAP)
		hp_float_down_min(h, pos);
	else
		hp_float_down_max(h, pos);
}

/*
 * Float up a node in heap.
 *
 * @h:   Pointer to the heap structure
 * @pos: Position of the node to float up
 */
static void hp_float_up(struct heap *h, int pos)
{
	struct hp_data **hparr;

	hparr = h->hparr;

	if (h->type == MIN_HEAP) {
		while (pos > 0 && h->k_cmp(hparr[Parent(pos)]->key, hparr[pos]->key) > 0) {
			swap(hparr, Parent(pos), pos);
			pos = Parent(pos);
		}
	} else {
		while (pos > 0 && h->k_cmp(hparr[Parent(pos)]->key, hparr[pos]->key) < 0) {
			swap(hparr, Parent(pos), pos);
			pos = Parent(pos);
		}
	}
}

/*
 * Grow the heap. Increase its capacity
 * by GROWTRH_RATE.
 *
 * @h: Pointer to the heap structure
 */
static void hp_grow(struct heap *h)
{
	h->hparr = realloc(h->hparr, GROWTH_RATE * h->cap * sizeof(struct hp_data *));
	h->cap *= GROWTH_RATE;
}

/*
 * Insert a new element to heap.
 *
 * @h: Pointer to the heap structure
 * @k_val: Pointer to the key of new element to be inserted
 * @v_val: Pointer to the value new element to be inserted
 */
void hp_insert(struct heap *h, void *k_val, void *v_val)
{
	struct hp_data *hpd_new;

	assert(h);

	/* Grow heap if already full */
	if (h->nmemb == h->cap)
		hp_grow(h);

	hpd_new = malloc(sizeof(struct hp_data));
	assert(hpd_new);

	hpd_new->key = h->k_cpy(k_val);
	hpd_new->val = h->v_cpy(v_val);

	h->hparr[h->nmemb] = hpd_new;

	hp_float_up(h, h->nmemb);

	h->nmemb++;
}

/*
 * Return the min/max value from heap. This does not
 * remove the min/max value from the heap, rather just
 * makes a copy of min/max value and returns it (a pointer
 * to it ofcourse). So the total number of members in
 * heap remains the same.
 *
 * @h: Pointer to the heap structure
 */
void *hp_find_m(struct heap *h)
{
	struct hp_data *retval;

	if (hp_is_empty(h) == 1) {
		retval = NULL;
	} else {
		retval = malloc(sizeof(struct hp_data));
		assert(retval);
		retval->key = h->k_cpy(h->hparr[0]->key);
		retval->val = h->k_cpy(h->hparr[0]->val);
	}

	return retval;
}

/*
 * Extract the min/max value from heap. This removes
 * the min/max value from heap, and as a result of
 * which, the total number of members reduces by 1.
 *
 * @h: Pointer to the heap structure
 */
void *hp_extract_m(struct heap *h)
{
	struct hp_data *retval;

	if (hp_is_empty(h) == 1) {
		retval = NULL;
	} else {
		/* Make a copy of the min/max val */
		retval = malloc(sizeof(struct hp_data));
		assert(retval);
		retval->key = h->k_cpy(h->hparr[0]->key);
		retval->val = h->k_cpy(h->hparr[0]->val);

		/* 
		 * Remove the min/max val from heap.
		 *
		 * 1. Swap the min/max val with last 
		 *    memb.
		 * 2. Free the memory occupied by 
		 *    min/max val.
		 * 3. Update h->nmemb by reducing 1.
		 * 4. Float down from index 0 to
		 *    maintain heap property.
		 */
		swap(h->hparr, 0, h->nmemb-1);
		h->k_dval(h->hparr[h->nmemb-1]->key);
		h->v_dval(h->hparr[h->nmemb-1]->val);
		free(h->hparr[h->nmemb-1]);
		h->nmemb--;
		hp_float_down(h, 0);
	}

	return retval;
}

/*
 * Find out if heap is empty.
 *
 * @h: Pointer to the heap structure
 */
int hp_is_empty(struct heap *h)
{
	int retval;

	if (h->nmemb == 0)
		retval = 1;
	else
		retval = 0;

	return retval;
}

/*
 * Return the size of heap.
 *
 * @h: Pointer to the heap structure
 */
size_t hp_get_size(struct heap *h)
{
	return h->nmemb;
}

/*
 * Print heap contents.
 *
 * @h: Pointer to the heap structure
 */
void hp_print(struct heap *h)
{
	/*
	 * TODO: Some design issues:
	 * 
	 * 1. Should the heap structure have a
	 *    pointer to print function as well?
	 *
	 * 2. If not, then how to print for any
	 *    type of data?
	 */
}

/*
 * Destroy heap, by freeing all/any memory occupied
 * by heap.
 *
 * This is done by performing following steps:
 *
 * 1. free memory occupied by any valid element of hparr
 * 2. free memory occupied by the hparr itself
 * 3. free memory occupied by the heap structure
 *
 * @h: Pointer to the heap structure
 */
void hp_destroy(struct heap *h)
{
	int i;

	assert(h);

	for (i = 0; i < h->nmemb; i++) {
		if (h->hparr[i] != NULL) {
			h->k_dval(h->hparr[i]->key);
			h->v_dval(h->hparr[i]->val);
			free(h->hparr[i]);
		}
	}

	free(h->hparr);
	free(h);
}

/*
 * Make a copy of heap, and return a pointer to this copy.
 * This makes a copy of the hparr as well.
 * This makes a copy of elements of hparr as well.
 *
 * @h: Pointer to heap structure whose copy is to be made
 */
static struct heap *hp_copy(struct heap *hsrc) 
{
	int i;
	struct hp_data **hpcpyarr;
	struct heap *hcpy;
	
	/* Make a copy of the structure. This is a shallow copy. */
	hcpy = malloc(sizeof(struct heap));
	assert(hcpy);
	hcpy = memcpy(hcpy, hsrc, sizeof(struct heap));

	/* Make hcpy->hparr point to its own array in memory */
	hcpy->hparr = calloc(hcpy->nmemb, sizeof(struct hp_data *));
	assert(hcpy->hparr);
	hpcpyarr = hcpy->hparr;

	/* Make copies of each element in hsrc->hparr */
	for (i = 0; i < hcpy->nmemb; i++) {
		hpcpyarr[i] = malloc(sizeof(struct hp_data));
		assert(hpcpyarr[i]);
		hpcpyarr[i]->key = hcpy->k_cpy(hsrc->hparr[i]->key);
		hpcpyarr[i]->val = hcpy->v_cpy(hsrc->hparr[i]->val);
	}

	return hcpy;
}

/*
 * Make a sorted array using heap contents. Return pointer
 * to this sorted array.
 *
 * To do this, first a copy of original heap is made. Then 
 * this copy-heap is used to create the sorted array. This
 * is done by continuously extracting the min/max element
 * from the copy-heap untill the copy-heap is empty.
 *
 * Copy of the original heap is made so that the original
 * heap is not altered in any way.
 *
 * @h: Pointer to heap structure
 */
struct hp_data **get_sorted_arr(struct heap *h)
{
	int i;
	struct hp_data **arr;
	struct heap *hcpy;

	/* Allocate memory for array */
	arr = malloc(h->nmemb * sizeof(struct hp_data *));
	assert(arr);
	
	/* Make a temporary copy of original heap */
	hcpy = hp_copy(h);

	/*
	 * Fill the array from the copy-heap such
	 * that the array is sorted
	 */
	i = 0;
	while (!hp_is_empty(hcpy)) {
		arr[i] = hp_extract_m(hcpy);
		i++;
	}

	/* Destroy the temporary copy-heap */
	hp_destroy(hcpy);

	return arr;
}

/*
 * Test if array is sorted.
 *
 * @h:     Pointer to heap structure. Heap structure is needed
 *         because, it will have the k_cmp function. 
 *         TODO: Alternative design would be to pass the k_cmp
 *         function instead of heap structure.
 * @arr:   Array to test 
 * @nmemb: Total members in the array
 */
int hp_arr_is_sorted(struct heap *h, struct hp_data **arr, size_t nmemb)
{
	int i;
	void *prev;
	int retval;

	/* Assume array is sorted */
	retval = 1;

	/*
	 * Get the first element. However be
	 * careful and test that there is at-least
	 * one element, before trying to access
	 * the first element of the array.
	 */
	if (nmemb > 0)
		prev = arr[0]->key;

	/*
	 * Check that array is in non-descreasing order.
	 * Update retval if this is not the case.
	 */
	for (i = 0; i < nmemb; i++) {
		if (h->k_cmp(prev, arr[i]->key) > 0) {
			/* Indicate array is not sorted */
			retval = 0;
			break;
		}
		prev = arr[i]->key;
	}

	return retval;
}

/*
 * Decrease key of an element in heap
 *
 * @h:      Pointer to heap structure
 * @pos:    Position of the element in heap
 * @newval: New key value at the position
 */
void hp_decrease_key(struct heap *h, int pos, void *newkey)
{
	/* Check sanity of request */
	if (h->k_cmp(newkey, h->hparr[pos]->key) > 0)
		return;

	/* Free old val and copy new val */
	h->k_dval(h->hparr[pos]->key);
	h->hparr[pos]->key = h->k_cpy(newkey);

	/* Float up to preserve heap property */
	hp_float_up(h, pos);
}

/*
 * Get index of an element with a given key.
 *
 * Returns index of first match if multiple elements
 * have same key. Returns -1 if no match found.
 *
 * @h:   Pointer to the heap structure
 * @key: Pointer to the key to be searched
 */
int hp_get_index_key(struct heap *h, void *key)
{
	int i;
	int retval;

	retval = -1;

	for (i = 0; i < h->nmemb; i++) {
		if (h->v_cmp(h->hparr[i]->key, key) == 0) {
			retval = i;
			break;
		}
	}

	return retval;
}

/*
 * Get index of an element with a given value.
 *
 * Returns index of first match if multiple elements
 * have same value. Returns -1 if no match found.
 *
 * @h:   Pointer to the heap structure
 * @val: Pointer to the value to be searched
 */
int hp_get_index_val(struct heap *h, void *val)
{
	int i;
	int retval;

	retval = -1;

	for (i = 0; i < h->nmemb; i++) {
		if (h->v_cmp(h->hparr[i]->val, val) == 0) {
			retval = i;
			break;
		}
	}

	return retval;
}

