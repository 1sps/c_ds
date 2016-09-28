/*
 * ht.c: Hash table implementaion in C
 *
 * St: 2016-09-27 Tue 01:23 PM
 * Up: 2016-09-27 Tue 11:22 PM
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
 * Different hash table types
 */
#define KINT_VINT 10      /* int key and int val */
#define KINT_VSTR 11      /* int key and str val */
#define KSTR_VINT 12      /* str key and int val */
#define KSTR_VSTR 13      /* str key and str val */

/*
 * Structure which contains a pointer to hash table,
 * and pointer to ht_data.
 */
struct ht_and_data {
	struct ht *h;
	struct ht_data *data;
};

/*
 * Destroy a val in hash table linked list node.
 *
 * @val: Val of a node in hash table linked list
 */
static void ht_ll_dval(void *val)
{
	struct ht_data *data;

	data = (struct ht_data *) val;

	free(data->key);
	free(data->val);

	free(data);
}

/*
 * Copy a data to hash table linked list. This will be the copy
 * function for hash table linked list.
 *
 * @h_n_d: Pointer to a structure which contains pointers
 *         to hash table struture and ht_data.
 *         It has to be a `void *' because this function
 *         is passed as a copy function for linked list,
 *         and linked list requires the prototype of the
 *         copy function be `void *cpy(void *)'.
 */
static void *ht_data_cpy(void *h_n_d) 
{
	struct ht_and_data *htnd;
	struct ht *h;
	struct ht_data *data;
	struct ht_data *dest;

	htnd = (struct ht_and_data *) h_n_d;

	h = htnd->h;
	data = htnd->data;

	dest = malloc(sizeof(struct ht_data));
	assert(dest);

	dest->key = h->k_cpy(data->key);
	dest->val = h->v_cpy(data->val);

	return dest;
}

/*
 * Compare a key from hash table linked list. This will be the
 * compare function for hash table linked list.
 *
 * @val:   First value among the two to be compared.
 * @h_n_d: Hash table and Data. Data is the second value among
 *         the two to be compared.
 */
static int ht_data_cmp(void *val, void *h_n_d)
{
	int retval;
	struct ht *h;
	struct ht_and_data *htnd;
	struct ht_data *data1;
	struct ht_data *data2;

	htnd = (struct ht_and_data *) h_n_d;

	h = htnd->h;

	data1 = (struct ht_data *) val;
	data2 = htnd->data;

	retval = h->k_cmp(data1->key, data2->key);

	return retval;
}

/*
 * Hash function.
 *
 * @h:   Pointer to the hash table structure. This is needed
 *       because it has pointer to the function to get the 
 *       key size. TODO: Change design where passing of hash
 *       table structure is not needed.
 *
 * @key: Key whose hash value is to be calculated
 */
static int hash_func(struct ht *h, void *key)
{
	int i;
	int hash_val;
	int key_size;
	char *key_char;

	key_size = h->get_key_size(key);
	key_char = (char *) key;

	hash_val = 0;

	/*
	 * TODO: Is it better not to do a modulo divisin with
	 * h->tot_slots here and do it in the caller instead? 
	 */

	for (i = 0; i < key_size; i++) {
		hash_val += ((*key_char) * 31) % h->tot_slots;
		key_char++;
	}

	return hash_val % h->tot_slots;
}

/*
 * Create a hash table.
 *
 * @tot_slots:    Total slots in the hash table
 * @k_cpy:        Function to copy key
 * @v_cpy:        Function to copy val 
 * @k_cmp:        Function to compare key
 * @v_cmp:        Function to compare val
 * @get_key_size: Function to get key size
 */
struct ht *ht_create(size_t tot_slots,
                     void *(*k_cpy) (void *),
		     void *(*v_cpy) (void *),
		     int (*k_cmp) (void *, void *), 
		     int (*v_cmp) (void *, void *), 
		     int (*get_key_size) (void *))
{
	struct ht *h;

	/* TODO: Is it good to assert that params are good? */

	h = malloc(sizeof(struct ht));
	assert(h);

	h->table = calloc(tot_slots, sizeof(struct ll *));
	assert(h->table);

	h->tot_slots = tot_slots;

	h->k_cpy = k_cpy;
	h->v_cpy = v_cpy;

	h->k_cmp = k_cmp;
	h->v_cmp = v_cmp;

	h->get_key_size = get_key_size;

	h->hash_func = hash_func;

	return h;
}

/*
 * Encapsulate hash table structure and data (key and val) in a single
 * structure.
 *
 * @h:   Pointer to hash table structure
 * @key: Pointer to key
 * @val: Pointer to val 
 */
static struct ht_and_data *ht_n_d_encap(struct ht *h, void *key, void *val)
{
	struct ht_data *data;
	struct ht_and_data *htnd;

	/* Encapsulate key and val in ht_data structure */
	data = malloc(sizeof(struct ht_data));
	assert(data);
	data->key = key;
	data->val = val;

	/*
	 * Encapsulate hash table structure and insert/search 
	 * ht_data in ht_and_data structure
	 */
	htnd = malloc(sizeof(struct ht_and_data));
	assert(htnd);
	htnd->h = h;
	htnd->data = data;

	return htnd;
}


/*
 * Destroy temporary encapsulation structure 
 *
 * @htnd: Encapsulation structure to destory
 */
static void ht_n_d_dencap(struct ht_and_data *htnd)
{
	free(htnd->data);
	free(htnd);
}

/*
 * Insert a new data to Hash table.
 *
 * @h:   Pointer to the hash table structure
 * @key: Key of the data to be inserted
 * @val: Val of the data to be inserted
 */
int ht_insert(struct ht *h, void *key, void *val)
{
	int idx;
	struct ht_and_data *htnd;

	/* Encapsulate hash table and search data in single struct */
	htnd = ht_n_d_encap(h, key, val);

	/* Get the hash value for key */
	idx = h->hash_func(h, key);

	/* 
	 * Insert new data to correct slot
	 */

	/* Create a linked list if none at this slot */
	if (h->table[idx] == NULL) {
		h->table[idx] =
		ll_create(ht_data_cpy, ht_data_cmp, ht_ll_dval, NULL);
	}

	/* Insert data to the list */
	ll_insert(h->table[idx], htnd);

	/* Destroy the temporary encapsulation structure */
	ht_n_d_dencap(htnd);

	return idx;
}

/*
 * Search for a data in hash table.
 *
 * @h:   Pointer to the hash table structure
 * @key: key of the data to be searched
 */
int ht_search(struct ht *h, void *key)
{
	int idx;
	int retval;
	struct ht_and_data *htnd;

	/* Encapsulate hash table and search data in single struct */
	htnd = ht_n_d_encap(h, key, NULL);

	/* Get the hash value for key */
	idx = h->hash_func(h, key);

	/* Search for the key in linked list in that slot */
	if (h->table[idx] == NULL)
		retval = 0;
	else
		retval = ll_search(h->table[idx], htnd);

	/* Destroy the temporary encapsulation structure */
	ht_n_d_dencap(htnd);

	return retval;
}

/*
 * Delete an item from hash table. Has no effect if itemn is not
 * found in the hash table.
 *
 * @h:   Pointer to the hash table structure
 * @key: Pointer to key of the item to delete
 */
void ht_delete(struct ht *h, void *key)
{
	int idx;
	struct ht_and_data *htnd;

	/* Encapsulate hash table and search data in single struct */
	htnd = ht_n_d_encap(h, key, NULL);

	/* Get the hash value for key */
	idx = h->hash_func(h, key);
	
	/*
	 * Delete item from the list. This will delete the item only
	 * if it is present in the list. Else there is no effect.
	 */
	if (h->table[idx] != NULL)
		ll_delete(h->table[idx], htnd);

	/* Destroy the temporary encapsulation structure */
	ht_n_d_dencap(htnd);
}

/*
 * Destroy a hash table.
 *
 * Free any memory occupied by the hash table by doing the following:
 *
 * 1. Destroy all the linked list whose pointers are in h->table.
 *    i.e: for all i do destroy(h->table[i])
 * 
 * 2. Free memory occupied by h->table.
 *    i.e: free(h->table)
 *
 * 3. Free memory occupied by hash table structure itself.
 *    i.e: free(h)
 *
 * @h: Pointer to the hash table structure
 */
void ht_destroy(struct ht *h)
{
	int i;

	for (i = 0; i < h->tot_slots; i++)
		if (h->table[i] != NULL) {
			ll_destroy(h->table[i]);
		}

	free(h->table);

	free(h);
}

/*
 * Print hash table.
 *
 * @h: Pointer to the hash table structure
 */
void ht_print(struct ht *h, int type)
{
	int i;
	struct ht_data *data;
	struct ll_node *lln;

	int *key_i;
	int *val_i;
	char *key_s;
	char *val_s;

	for (i = 0; i < h->tot_slots; i++) {
		printf("==> Linked list in slot [%d] <==\n", i);
		if (h->table[i] != NULL) {
			lln = h->table[i]->head;
			while (lln != NULL) {
				data = (struct ht_data *) (lln->val);

				switch(type) {
				case KINT_VINT:
					key_i = (int *) (data->key);	
					val_i = (int *) (data->val);	
					printf("[k] %d <=> [v] %d\n", *key_i,
					                              *val_i);
					break;
				case KINT_VSTR:
					key_i = (int *) (data->key);	
					val_s = (char *) (data->val);	
					printf("[k] %d <=> [v] %s\n", *key_i,
					                              val_s);
					break;
				case KSTR_VINT:
					key_s = (char *) (data->key);	
					val_i = (int *) (data->val);	
					printf("[k] %s <=> [v] %d\n", key_s,
					                              *val_i);
					break;
				case KSTR_VSTR:
					key_s = (char *) (data->key);	
					val_s = (char *) (data->val);	
					printf("[k] %s <=> [v] %s\n", key_s,
					                              val_s);
					break;
				}

				lln = lln->next; 
			}
			putchar('\n');
		} 
		putchar('\n');
	}
}

