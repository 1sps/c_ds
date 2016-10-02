/*
 * src/graph.c:
 *
 * St: 2016-10-01 Sat 06:24 PM
 * Up: 2016-10-02 Sun 05:25 AM
 *
 * Author: SPS
 */

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

#include"mylib.h"

#define SKIP

/* Function prototype declaration of helper functions */
static int conv_to_int(int (*get_size)(void *), void *val);
static void *conv_from_int(void *val);
static struct hlpr_ht *hlpr_ht_create(int nvert, int (*get_size)(void *));
void *cpy_i(void *src);
int cmp_i(void *val1, void *val2);
void printn_i(struct ll_node *lln);
void dval_i(void *);

/*
 * Create a graph.
 *
 * @nvert: Total number of vertices
 */
struct graph *graph_create(int nvert, int type)
{
	int i;
	struct graph *g;

	/* Allocate memory for graph structure */
	g = malloc(sizeof(struct graph));
	assert(g);

	/* Allocate memory for adjacency list */
	g->alist = malloc(sizeof(struct adjlist*) * nvert);
	assert(g->alist);

	/* Allocate memory for sucessors list for each vertex */
	for (i = 0; i < nvert; i++) {
		/* Memory for sucessor list metadata */
		g->alist[i] = malloc(sizeof(struct adjlist));
		assert(g->alist[i]);

		/* Create the sucessor list */
		g->alist[i]->l = ll_create(cpy_i, cmp_i, dval_i, printn_i);
	}

	/* Initialise properties */
	g->nvert = nvert;
	g->type = type;
	g->nedge = 0;

	/*
	 * Search graph will be created later when
	 * a search is done in graph
	 */
	g->search_graph = NULL;

	/*
	 * TODO: Create a preprocessing structure
	 *       if type is not int
	 */
	/* g->ht = hlpr_ht_create(nvert, get_size); */

	return g;
}

/*
 * Destroy a graph.
 *
 * @g: Pointer to the graph structure
 */
void graph_destroy(struct graph *g)
{
	int i;

	/* Destroy sucessor list for each vertex */
	for (i = 0; i < g->nvert; i++) {
		ll_destroy(g->alist[i]->l);
		free(g->alist[i]);
	}

	/* Free any memory occupied by search graph */
	free(g->search_graph);

	/* Free memory occupied by alist */
	free(g->alist);

	/* Free memomry occupied by graph structure */
	free(g);
}

/*
 * Add an edge to graph.
 *
 * @g:    Pointer to the graph structure
 * @src:  Pointer to source node label
 * @dest: Pointer to destination node label
 */
void graph_add_edge(struct graph *g, void *src, void *dest)
{
	int isrc;
	int idest;
	int new_edge;

	/* Assume this edge is already present */
	new_edge = 0;

	/* Get the int value for the vertices */
	if (g->type != GRAPH_INT) {
		/* TODO */
		;
	} else {
		isrc = *(int *) src;
		idest = *(int *) dest;
	}

	/* Add dest in src's sucessors list */
	if (ll_search(g->alist[isrc]->l, dest) == 0) {
		new_edge = 1;
		ll_insert(g->alist[isrc]->l, dest);
	
	}
	/* Add src in dest's sucessors list */
	if (ll_search(g->alist[idest]->l, src) == 0)
		ll_insert(g->alist[idest]->l, src);

	/* Updage edge count if needed */
	if (new_edge == 1)
		g->nedge++;
}

/*
 * Return total number of vertices in graph.
 *
 * @g: Pointer to the graph structure
 */
int graph_tot_vertex(struct graph *g)
{
	return g->nvert;
}

/*
 * Return total number of edgges in graph.
 *
 * @g: Pointer to the graph structure
 */
int graph_tot_edge(struct graph *g)
{
	return g->nedge;
}

/*
 * Check if graph contains an edge. 
 *
 * @g:    Pointer to graph structure
 * @src:  Pointer to source label
 * @dest: Pointer to destination label
 */
int graph_has_edge(struct graph *g, void *src, void *dest)
{
	int isrc;
	int idest;
	int retval;

	/* Assume no edge */
	retval = 0;

	/* Convert labels to int if needed */
	if (g->type != GRAPH_INT) {
		/* isrc = todo(); */
		/* idest = todo(); */
		;
	} else {
		isrc = *(int * ) src;
		idest = *(int * ) dest;
	}

	/* Search for the edge */
	retval = ll_search(g->alist[isrc]->l, dest);

	return retval;
}

/*
 * Print a graph.
 *
 * @g: Pointer to the graph structure
 */
void graph_print(struct graph *g)
{
	int i;

	printf("Total vertices: %d\n", g->nvert);
	printf("Total edges: %d\n", g->nedge);
	for (i = 0; i < g->nvert; i++) {
		if (!ll_is_empty(g->alist[i]->l)) {
			printf("Succesor list of vertex %d:\n", i);
			ll_print(g->alist[i]->l);
			putchar('\n');
		}
	}
}

/* Used in graph searches */
#define NIL -1
#define INFINITE -1

/*
 * Breadth first search the graph.
 *
 * @g:    Pointer to the graph structure
 * @src:  Pointer to the label of source vertex
 * @dest: Pointer to the label of destination vertex
 */
int graph_bfs(struct graph *g, void *src, void *dest)
{
	int i;
	void *isrc;
	void *idest;
	int *cur;
	struct queue *q;
	int retval;

	/* Allocate space for search_graph if needed */
	if (g->search_graph == NULL) {
		g->search_graph = malloc(sizeof(struct search_node) * g->nvert);
		assert(g->search_graph);
	}

	/* Initialise search_graph */
	for (i = 0; i < g->nvert; i++) {
		g->search_graph[i].vtx = i;
		g->search_graph[i].par = NIL;
		g->search_graph[i].dist = INFINITE;
	}

	/* Create queue */
	q = q_create(cpy_i, cmp_i, NULL);

	/* Change labels to int equivalent if needed */
	if (g->type != GRAPH_INT) {
		/* isrc = todo(src); */
		/* idest = todo(dest); */
		;
	} else {
		isrc = src;
		idest = dest;
	}

	/* Set par and distance for root (src) */
	g->search_graph[*(int *)isrc].par = *(int *)isrc;
	g->search_graph[*(int *)isrc].dist = 0;

	/* Enqueue root (src) */
	q_push(q, isrc);

	while (!q_is_empty(q)) {
		cur = q_pop(q);

		/*
		 * foreach adjacent node of cur
		 *     if not visited then
		 *         update parent, distance and enqueue
		 */

		/* 
		 * TODO: Linked list should provide an interface
		 * to iterate through the list. For now, as seen below
		 * we are writing code which is aware of linked-list
		 * implementation. This knowledge dependency is not
		 * good and should be avoided.
		 */

		struct ll_node *lln;
		struct ll *cur_list;

		cur_list = g->alist[*cur]->l;

		lln = cur_list->head;
		while (lln != NULL) {
			if (g->search_graph[*(int *)lln->val].par == NIL) {
				g->search_graph[*(int *)lln->val].par = *cur;
				g->search_graph[*(int *)lln->val].dist =
				               g->search_graph[*cur].dist + 1;
				q_push(q, lln->val);
			}
			lln = lln->next;
		}

		/* TODO: Until there. See above TODO  */

		free(cur);
	}

	/* Check if src and dest are connected */
	if (g->search_graph[*(int *)idest].par != NIL)
		retval = 1;
	else 
		retval = 0;

	/* Destroy queue */
	q_destroy(q);

	/* Destroy search_graph */
	free(g->search_graph);
	g->search_graph = NULL;

	return retval;
}

/*******************************************************************************
 * Helper functions
 ******************************************************************************/

/*
 * Make a copy of an int 
 *
 * @src: source ; whose copy is to be made
 */
void *cpy_i(void *src)
{
	int *dest; 

	/* TODO: Make this function agnostic of empty src, 
	 * which means, that the below assert should go away
	 */
	assert(src);

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
 * Print an int val to stdout
 *
 * @val: Pointer to the value to print
 */
void printn_i(struct ll_node *lln)
{
	printf("%d, ", *(int *)lln->val);
}

/*
 * Destroy a int value.
 *
 * @val: Pointer to the value to destroy
 */
void dval_i(void *val)
{
	free(val);
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
 * Convert a data to a unique int
 *
 * @get_size: Pointer to a function to give the size of data
 * @src:      Pointer to data
 */
static int conv_to_int(int (*get_size)(void *), void *src)
{
	int retval;
	int size;

	size = get_size(src);

	/* retval = todo(); */

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
