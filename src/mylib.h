/* mylib.h: Header file (interface) for my C library
 *
 * St: 2016-09-26 Mon 01:47 AM
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


#ifndef MYLIB_H
#define MYLIB_H


/*
 singly linked list stuff
 */

struct ll_node {
	void *val;
	struct ll_node *next;
};

/*
 * @cpy: Pointer to a function which copies data from
 * a node and returns pointer to the new copy.
 *
 * @cmp: Pointer to a function which compares two nodes.
 */
struct ll {
	struct ll_node *head;              /* head of linked list */
	void *(*cpy)(void *);              /* copy function */
	int (*cmp)(void *, void *);        /* compare function */
	void (*printl) (struct ll_node *); /* print function */
	size_t nmemb;                      /* total nodes */   
};

/* Singly linked list functions */
struct ll *ll_create(void *(*cpy)(void *),
                     int (*cmp)(void *, void *),
		     void (*printl)(struct ll_node *));
void ll_insert(struct ll *l, void *val);
int ll_search(struct ll *l, void *val);
void ll_delete(struct ll *l, void *val);
void ll_destroy(struct ll *l);
void ll_print(struct ll *l);


/*
 * Stack stuff
 */

struct st_node {
	void *val;
	struct st_node *next;
};

struct st {
	struct st_node *head;              /* head(top) of the stack */
	void *(*cpy)(void *);              /* copy function */
	int (*cmp)(void *, void *);        /* compare function */
	void (*printn) (struct st_node *); /* print function */
	size_t nmemb;                      /* total nodes */   

};

/* Stack functions */
struct st *st_create(void *(*cpy)(void *),
                     int (*cmp)(void *, void *),
		     void (*printn)(struct st_node *));
void st_push(struct st *s, void *val);
void *st_pop(struct st *s);
void st_destroy(struct st *s);
int st_is_empty(struct st *s);
void st_print(struct st *s);


/*
 * Queue stuff
 */

struct q_node {
	void *val;
	struct q_node *next;
};

struct queue {
	struct q_node *head;               /* head of the queue */
	struct q_node *tail;               /* tail of the queue */
	void *(*cpy)(void *);              /* copy function */
	int (*cmp)(void *, void *);        /* compare function */
	void (*printn) (struct q_node *);  /* print function */
	size_t nmemb;                      /* total nodes */   

};

/* Queue functions */
struct queue *q_create(void *(*cpy)(void *),
                       int (*cmp)(void *, void *),
                       void (*printn)(struct q_node *));
void q_push(struct queue *q, void *val);
void *q_pop(struct queue *q);
void q_destroy(struct queue *q);
int q_is_empty(struct queue *q);
void q_print(struct queue *q);


/*
 * Heap stuff
 */

struct heap {
	void **hparr;                 /* Array representing heap */
	char type;                    /* Type of heap (min or max) */
	size_t nmemb;                 /* Number of members */
	size_t cap;                   /* Capacity -> max nmemb */
	void *(*cpy)(void *);         /* Copy funciton */
	int (*cmp)(void *, void *);   /* Compare funciton */
};

#define MIN_HEAP 1
#define MAX_HEAP 2

#define GROWTH_RATE 2

#define Parent(x)  (((x) - 1) / 2)
#define Child(x, dir)  (2 * (x) + 1 + (dir))

/* Heap functions */
struct heap *hp_create(size_t cap,
                       char type,
		       void *(*cpy)(void *),
		       int (*cmp)(void *, void *));
void hp_insert(struct heap *h, void *val);
void *hp_extract_m(struct heap *h);
void *hp_find_m(struct heap *h);
void hp_destroy(struct heap *h);
int hp_is_empty(struct heap *h);
size_t hp_get_size(struct heap *h);
void hp_print(struct heap *h);
void **get_sorted_arr(struct heap *h);
int hp_arr_is_sorted(struct heap *h,
                     void **arr,
		     size_t nmemb);

#endif  /* MYLIB_H */

