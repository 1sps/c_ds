/* mylib.h: Header file (interface) for my C library
 *
 * St: 2016-09-26 Mon 01:47 AM
 * Up: 2016-10-03 Mon 12:02 AM
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
 *
 * @dval: Pointer to a function which destroys a node's
 *        `val' 
 */
struct ll {
	struct ll_node *head;              /* head of linked list */
	void *(*cpy)(void *);              /* copy function */
	int (*cmp)(void *, void *);        /* compare function */
	void (*dval)(void *);              /* node val destroy function */
	void (*printl) (struct ll_node *); /* print function */
	size_t nmemb;                      /* total nodes */   
};

/* Singly linked list functions */
struct ll *ll_create(void *(*cpy)(void *), int (*cmp)(void *, void *),
		     void (*dval)(void *), void (*printl)(struct ll_node *));
void ll_insert(struct ll *l, void *val);
int ll_search(struct ll *l, void *val);
void ll_delete(struct ll *l, void *val);
void ll_destroy(struct ll *l);
void ll_print(struct ll *l);
int ll_is_empty(struct ll *l);
void* ll_first(struct ll *l);
void* ll_next(struct ll *l, void **itrp);
int ll_done(void *itr);


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
	void (*dval) (void *);             /* value destroy function */
	void (*printn) (struct st_node *); /* print function */
	size_t nmemb;                      /* total nodes */   

};

/* Stack functions */
struct st *st_create(void *(*cpy)(void *), int (*cmp)(void *, void *),
		     void (*dval)(void *), void (*printn)(struct st_node *));
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
struct queue *q_create(void *(*cpy)(void *), int (*cmp)(void *, void *),
                       void (*printn)(struct q_node *));
void q_push(struct queue *q, void *val);
void *q_pop(struct queue *q);
void q_destroy(struct queue *q);
int q_is_empty(struct queue *q);
void q_print(struct queue *q);

/* 
 * Circular linked list stuff 
 */

struct cll_node {
	void *val;
	struct cll_node *next;
};

struct cll {
	struct cll_node *head;              /* head of linked list */
	struct cll_node *tail;              /* tail of linked list */
	void *(*cpy)(void *);               /* copy function */
	int (*cmp)(void *, void *);         /* compare function */
	void (*dval)(void *);               /* node val destroy function */
	void (*printl) (struct cll_node *); /* print function */
	size_t nmemb;                       /* total nodes */   
};

/* Circular linked list functions */
struct cll *cll_create(void *(*cpy)(void *), int (*cmp)(void *, void *),
		     void (*dval)(void *), void (*printl)(struct cll_node *));
void cll_insert(struct cll *l, void *val);
int cll_search(struct cll *l, void *val);
void cll_delete(struct cll *l, void *val);
void cll_destroy(struct cll *l);
void cll_print(struct cll *l);

/*
 * Doubly linked list stuff
 */

struct dll_node {
	void *val;
	struct dll_node *next;
	struct dll_node *prev;
};

/*
 * @cpy: Pointer to a function which copies data from
 * a node and returns pointer to the new copy.
 *
 * @cmp: Pointer to a function which compares two nodes.
 *
 * @dval: Pointer to a function which destroys a node's
 *        `val' 
 */
struct dll {
	struct dll_node *head;              /* head of linked list */
	void *(*cpy)(void *);               /* copy function */
	int (*cmp)(void *, void *);         /* compare function */
	void (*dval)(void *);               /* node val destroy function */
	void (*printl) (struct dll_node *); /* print function */
	size_t nmemb;                       /* total nodes */   
};

/* Doubly linked list functions */
struct dll *dll_create(void *(*cpy)(void *), int (*cmp)(void *, void *),
		     void (*dval)(void *), void (*printl)(struct dll_node *));
void dll_insert(struct dll *l, void *val);
int dll_search(struct dll *l, void *val);
void dll_delete(struct dll *l, void *val);
void dll_destroy(struct dll *l);
void dll_print(struct dll *l);

/*
 * Deque stuff
 */

struct deque_node {
	void *val;
	struct deque_node *next;
	struct deque_node *prev;
};

/*
 * @cpy: Pointer to a function which copies data from
 * a node and returns pointer to the new copy.
 *
 * @cmp: Pointer to a function which compares two nodes.
 *
 * @dval: Pointer to a function which destroys a node's
 *        `val' 
 */
struct deque {
	struct deque_node *head;              /* head of deque */
	struct deque_node *tail;              /* tail of deque */
	void *(*cpy)(void *);                 /* copy function */
	int (*cmp)(void *, void *);           /* compare function */
	void (*dval)(void *);                 /* node val destroy function */
	void (*printl) (struct deque_node *); /* print function */
	size_t nmemb;                         /* total nodes */   
};

/* Directions for deque insert */
#define DEQUE_HEAD 50
#define DEQUE_TAIL 51

/* Deque functions */
struct deque *deque_create(void *(*cpy)(void *), int (*cmp)(void *, void *),
		           void (*dval)(void *),
			   void (*printl)(struct deque_node *));
int deque_insert(struct deque *dek, int dir, void *val);
void *deque_pop(struct deque *dek, int dir);
int deque_is_empty(struct deque *dek);
int deque_search(struct deque *dek, void *val);
void deque_delete(struct deque *dek, void *val);
void deque_destroy(struct deque *dek);
void deque_print(struct deque *dek);

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
struct heap *hp_create(size_t cap, char type, void *(*cpy)(void *),
		       int (*cmp)(void *, void *));
void hp_insert(struct heap *h, void *val);
void *hp_extract_m(struct heap *h);
void *hp_find_m(struct heap *h);
void hp_destroy(struct heap *h);
int hp_is_empty(struct heap *h);
size_t hp_get_size(struct heap *h);
void hp_print(struct heap *h);
void **get_sorted_arr(struct heap *h);
int hp_arr_is_sorted(struct heap *h, void **arr, size_t nmemb);

/*
 * Hash Table Stuff
 */

struct ht_data {
	void *key;
	void *val;
};

struct ht {
	struct ll **table;                 /* table of linked lists */
	size_t tot_slots;                  /* total slots in table */
	void *(*k_cpy) (void *);           /* function to copy key */
	void *(*v_cpy) (void *);           /* function to copy val */
	int (*k_cmp)(void *, void *);      /* function to compare keys */
	int (*v_cmp)(void *, void *);      /* function to compare vals */
	int (*get_key_size) (void *);      /* function to get size of key */
	int (*hash_func) (struct ht *, void *);   
	                                   /* Hash function */
};

/* Hash Table functions */
struct ht *ht_create(size_t tot_slots, void *(*k_cpy) (void *),
	             void *(*v_cpy) (void *), int (*k_cmp) (void *, void *),
	             int (*v_cmp)(void *, void *), int (*get_key_size)(void *));
int ht_insert(struct ht *h, void *key, void *val);
int ht_search(struct ht *h, void *key);
void ht_delete(struct ht *h, void *key);
void ht_destroy(struct ht *h);
void ht_print(struct ht *h, int type);

/* 
 * Binary Search Tree Stuff 
 */

struct bst_node {
	void *val;
	struct bst_node *left;
	struct bst_node *right;
};

struct bst {
	struct bst_node *root; 
	void *(*cpy)(void *);                 /* copy function */
	int (*cmp)(void *, void *);           /* compare function */
	void (*dval)(void *);                 /* node val destroy function */
	void (*printn) (struct bst_node *);   /* print function */
	size_t nmemb;                         /* total nodes */   
};

#define PRINT_STDOUT 100
#define PRINT_STRING 101
#define PRINT_FILE   102

#define MAX_NODE_PRINT_LEN 100

/* Binary Search Tree Functions */ 
struct bst *bst_create(void *(*cpy)(void *), int (*cmp)(void *, void *),
                       void (*dval)(void *),
		       void (*printn) (struct bst_node *));
int bst_insert(struct bst *t, void *val);
struct bst_node *bst_search(struct bst *t, void *val);
void *bst_delete(struct bst *t, void *val);
void bst_destroy(struct bst *t);
void bst_print(struct bst *t);

/* 
 * Graph stuff 
 */

/* Hash table for preprocessing */
struct hlpr_ht {
	int *table;
	size_t nmemb;
	size_t cap;
	int (*get_size)(void *);
};

#define GRAPH_INT    100
#define GRAPH_NO_INT 101

struct adjlist {
	struct ll *l;
};

/* Graph structure */
struct graph {
	int type;
	/* struct hlpr_ht *ht; */
	int nvert;
	int nedge;
	struct adjlist **alist;

	/* For BFS/DFS search */
	struct search_node {
		int vtx;
		int par;
		int dist;
	} *search_graph;
};

/* Graph functions */
struct graph *graph_create(int nvert, int type);
void graph_destroy(struct graph *g);
void graph_add_edge(struct graph *g, void *src, void *dest);
int graph_tot_vertex(struct graph *g);
int graph_tot_edge(struct graph *g);
int graph_has_edge(struct graph *g, void *src, void *dest);
void graph_print(struct graph *g);
int graph_out_degree(struct graph *g, void *src);
int graph_bfs(struct graph *g, void *src, void *dest);
int graph_dfs(struct graph *g, void *src, void *dest);

#endif  /* MYLIB_H */

