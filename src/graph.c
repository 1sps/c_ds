/*
 * src/graph.c:
 *
 * St: 2016-10-01 Sat 06:24 PM
 * Up: 2016-10-05 Wed 01:47 AM
 *
 * Author: SPS
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

/* Function prototype declaration of helper functions */
static int *conv_label_to_int(struct graph *g, void *label);
void *cpy_i(void *src);
int cmp_i(void *val1, void *val2);
void printn_i(struct ll_node *lln);
void dval_i(void *);
static struct search_vtx_info *init_search_info(struct graph *g);
static void bfs_process_cur(struct graph *g, struct queue *q, int *cur,
                            struct search_vtx_info *search_info);
static void dfs_process_cur(struct graph *g,
                            struct st *st, struct graph_edge *e,
                            struct search_vtx_info *search_info);
static void *dfs_cpy_edge(void *val);
static void graph_destroy_edge(void *edge);
static struct graph_edge *graph_create_edge(int src, int sink);
static void init_priq(struct graph *g, struct heap *priq, int src);
void djk_relax(struct hp_data *par, int vtx, struct heap *priq);
void release_dijkstra_search_setup(struct heap *priq, int *visited, int *dist);
void init_dijkstra_search(struct graph *g, struct heap **priqp,
                          int **visitedp, int **distp, int src);

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
	g->alist = malloc(sizeof(struct successors *) * nvert);
	assert(g->alist);

	/* Allocate memory for sucessors list for each vertex */
	for (i = 0; i < nvert; i++) {
		/* Memory for sucessor list metadata */
		g->alist[i] = malloc(sizeof(struct successors));
		assert(g->alist[i]);

		/* Create the sucessor list */
		g->alist[i]->l = ll_create(cpy_i, cmp_i, dval_i, printn_i);
	}

	/* Initialise properties */
	g->nvert = nvert;
	g->type = type;
	g->nedge = 0;

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
 * Get the out degree of a vertex
 *
 * @g:   Pointer to the graph structure
 * @src: Vertex whose out degree is to be returned
 */
int graph_out_degree(struct graph *g, void *src)
{
	int *isrc;
	int retval;

	/* Convert label to int if needed */
	isrc = conv_label_to_int(g, src);

	retval = g->alist[*isrc]->l->nmemb;

	return retval;
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
	int *isrc;
	int *idest;
	int retval;

	/* Assume no edge */
	retval = 0;

	/* Convert labels to int if needed */
	isrc = conv_label_to_int(g, src);
	idest = conv_label_to_int(g, dest);

	/* Search for the edge */
	retval = ll_search(g->alist[*isrc]->l, idest);

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
	int *cur;
	int retval;
	void *isrc;
	void *idest;
	struct queue *q;
	struct search_vtx_info *search_info;

	/* Initialize search_info */
	search_info = init_search_info(g);

	/* Create queue */
	q = q_create(cpy_i, cmp_i, NULL);

	/* Change labels to int equivalent if needed */
	isrc = conv_label_to_int(g, src);
	idest = conv_label_to_int(g, dest);

	/* Set par and distance for root (src) */
	search_info[*(int *)isrc].par = *(int *)isrc;
	search_info[*(int *)isrc].dist = 0;

	/* Enqueue root (src) */
	q_push(q, isrc);

	/* For each vertex update neighbors */
	while (!q_is_empty(q)) {
		cur = q_pop(q);
		bfs_process_cur(g, q, cur, search_info);
		free(cur);
	}

	/* Check if src and dest are connected */
	if (search_info[*(int *)idest].par != NIL)
		retval = 1;
	else 
		retval = 0;

	/* Destroy queue */
	q_destroy(q);

	/* Destroy search_info */
	free(search_info);
	search_info = NULL;

	return retval;
}

/*
 * Depth first search the graph.
 *
 * @g:    Pointer to the graph structure
 * @src:  Pointer to the label of source vertex
 * @dest: Pointer to the label of destination vertex
 */
int graph_dfs(struct graph *g, void *src, void *dest)
{
	int retval;
	int *isrc;
	int *idest;
	struct st *st;
	struct graph_edge *edge;
	int cur_vtx;
	struct search_vtx_info *search_info;

	/* Initialize search_info */
	search_info = init_search_info(g);

	/* Create stack */
	st = st_create(dfs_cpy_edge, NULL, graph_destroy_edge, NULL);

	/* Change labels to int equivalent if needed */
	isrc = conv_label_to_int(g, src);
	idest = conv_label_to_int(g, dest);

	/* Push edge (from src to src) to stack */
	edge = graph_create_edge(*isrc, *isrc);
	st_push(st, edge);
	free(edge);

	/* Visit each vertex */
	while (!st_is_empty(st)) {
		edge = st_pop(st);
		cur_vtx = edge->sink;
		if (search_info[cur_vtx].par == NIL)
		/* If cur vertex not visited */
			dfs_process_cur(g, st, edge, search_info);
		free(edge);
	}

	/* Check if src and dest are connected */
	if (search_info[*idest].par != NIL)
		retval = 1;
	else 
		retval = 0;

	/* Destroy stack */
	st_destroy(st);

	/* Destroy search_info */
	free(search_info);
	search_info = NULL;

	return retval;
}

/*
 * Show a path between src and dest vertex.
 *
 * @g:    Pointer to the graph structure
 * @src:  Sourve vertex
 * @dest: Destination vertex
 */
char *graph_show_path(struct graph *g, void *src, void *dest)
{
	/* TODO */
	return NULL;
}

/* Infinity for Dijkstra search distance */
#define DJK_INF 100000

/*
 * Dijkstra shortest path first search
 *
 * @g:    Pointer to the graph structure
 * @src:  Pointer to source vertex
 * @dest: Pointer to destination vertex 
 */
int graph_dijkstra(struct graph *g, void *src, void *dest)
{
	int retval;
	int *isrc;
	int *idest;
	int *nbr_vtx;
	struct heap *priq;
	int *visited;
	int *dist;
	void *ll_itr;
	struct hp_data *cur;

	/* Change labels to int equivalent if needed */
	isrc = conv_label_to_int(g, src);
	idest = conv_label_to_int(g, dest);

	/* Initialise dijkstra search setup */
	init_dijkstra_search(g, &priq, &visited, &dist, *isrc);

	/* While queue is not empty */
	while (!hp_is_empty(priq)) {
		/* Get node with min dist in priq */
		cur = hp_extract_m(priq);
		/* Mark it as visited */
		visited[*(int *)cur->val] = 1;
		/* Record its distance - it is the minimum distance */
		if (*(int *)cur->key != DJK_INF)
			dist[*(int *)cur->val] = *(int *)cur->key;

		/* for all neighbors of cur */
		ll_itr = ll_first(g->alist[*(int *)cur->val]->l);
		while (!ll_done(ll_itr)) {
			nbr_vtx = ll_next(g->alist[*(int *)cur->val]->l, &ll_itr);
			/* if not already visited */
			if (visited[*(int *)nbr_vtx] == 0)
				djk_relax(cur, *(int *)nbr_vtx, priq);
			free(nbr_vtx); 
		}
		dval_i(cur->val);
		dval_i(cur->key);
		free(cur);
	}

	/* Update retval to show distance */
	retval = dist[*idest];

	/* Destroy memory of helper structures */
	release_dijkstra_search_setup(priq, visited, dist);

	return retval;
}

/*
 *******************************************************************************
 * Helper functions
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
 * Convert a vertex label to a int
 *
 * @g:     Pointer to the graph structure
 * @label: Pointer to label to convert
 */
static int *conv_label_to_int(struct graph *g, void *label)
{
	int *retval;

	if (g->type != GRAPH_INT)
		/* retval = todo(label); */
		;
	else
		retval = label;

	return retval;
}

/*
 * Update the neighbors of a vertex during BFS search.
 *
 * @g:   Pointer to the graph structure
 * @cur:
 */
static void bfs_process_cur(struct graph *g, struct queue *q, int *cur,
                            struct search_vtx_info *search_info)
{
	void *ll_itr;
	void *nbr_vtx;

	/*
	 * foreach adjacent node of cur
	 *     if not visited then
	 *         update parent, distance, and enqueue it
	 */

	ll_itr = ll_first(g->alist[*cur]->l);

	while (!ll_done(ll_itr)) {
		nbr_vtx = ll_next(g->alist[*cur]->l, &ll_itr);
		if (search_info[*(int *)nbr_vtx].par == NIL) {
			search_info[*(int *)nbr_vtx].par = *cur;
			search_info[*(int *)nbr_vtx].dist =
			             search_info[*cur].dist + 1;
			q_push(q, nbr_vtx);
		}
		free(nbr_vtx);
	}
}

/*
 * Update the neighbors of a vertex during DFS search.
 *
 * @g:   Pointer to the graph structure
 * @cur:
 */
static void dfs_process_cur(struct graph *g,
                            struct st *st, struct graph_edge *e,
                            struct search_vtx_info *search_info)
{
	int par;
	int cur;
	void *ll_itr;
	void *nbr_vtx;
	struct graph_edge *out_edge;

	par = e->src;
	cur = e->sink;

	/* Update distance and parent of cur vertex */
	search_info[cur].par = par;
	if (e->sink == e->src)
		search_info[cur].dist = 0;
	else
		search_info[cur].dist = search_info[par].dist + 1;

	/*
	 * Push each outgoing edge (from cur vertex) to stack
	 */

	ll_itr = ll_first(g->alist[cur]->l);

	while (!ll_done(ll_itr)) {
		/* Get neighbor */
		nbr_vtx = ll_next(g->alist[cur]->l, &ll_itr);
		/* Make an edge */
		out_edge = graph_create_edge(cur, *(int *)nbr_vtx);
		/* Push edge to stack */
		st_push(st, out_edge);
		/* Free temp memory used */
		free(out_edge);
		free(nbr_vtx);
	}
}

/*
 * Initialize search_info for BFS search
 *
 * @g: Pointer to the graph structure
 */
static struct search_vtx_info *init_search_info(struct graph *g)
{
	int i;
	struct search_vtx_info *search_info;

	/* Allocate space for search_info if needed */
	search_info = malloc(sizeof(struct search_vtx_info) * g->nvert);
	assert(search_info);

	/* Initially all node is unreachable */
	for (i = 0; i < g->nvert; i++) {
		search_info[i].vtx = i;
		search_info[i].par = NIL;
		search_info[i].dist = INFINITE;
	}

	return search_info;
}

/*
 * Copy function for dfs edge.
 *
 * @val: Pointer to graph_edge to copy
 */
static void *dfs_cpy_edge(void *val)
{
	struct graph_edge *dfse;
	struct graph_edge *retval;

	retval = malloc(sizeof(struct graph_edge));
	assert(retval);

	dfse = (struct graph_edge *) val;

	retval->src =  dfse->src;
	retval->sink =  dfse->sink;

	return retval;
}

/*
 * Create an edge
 *
 * @src:  Vertex one of edge
 * @sink: Vertex two of edge
 */
static struct graph_edge *graph_create_edge(int src, int sink)
{
	struct graph_edge *edge;

	edge = malloc(sizeof(struct graph_edge));
	assert(edge);

	edge->src = src;
	edge->sink = sink;

	return edge;
}

/*
 * Destroy an edge
 *
 * @edge: Pointer to the edge structure
 */
static void graph_destroy_edge(void *edge)
{
	free(edge);
}

/*
 * Initialize necessary setup for dijkstra search.
 *
 * @priqp:    Pointer to the priority queue structure pointer
 * @visitedp: Pointer to visited array
 * @distp:    Pointer to distance array
 */
void init_dijkstra_search(struct graph *g, struct heap **priqp,
                          int **visitedp, int **distp, int src)
{
	int i;

	/* Create and initialize priority queue */
	*priqp = hp_create(g->nvert, MIN_HEAP,
			   cpy_i, cpy_i, cmp_i, cmp_i, dval_i, dval_i);
	init_priq(g, *priqp, src);

	/* Create and initiallize visited array */
	*visitedp = calloc(g->nvert , sizeof(int));
	assert(*visitedp);

	/* Create and initiallize distance array */
	*distp = malloc(g->nvert * sizeof(int));
	assert(*distp);
	for (i = 0; i < g->nvert; i++)
		(*distp)[i] = -1;
}

/*
 * Release memory occupied by helper structures for dijkstra search.
 *
 * @priq:    Pointer to the helper priority queue
 * @visited: Pointer to visited array
 * @dist:    Pointer to distance array
 */
void release_dijkstra_search_setup(struct heap *priq, int *visited, int *dist)
{
	hp_destroy(priq);
	free(visited);
	free(dist);
}

/*
 * Relax for a node during djkstra search.
 *
 * @par:  Pointer to Parent vertex priq_node
 * @vtx:  Cur vertex for which relaxation is needed 
 * @priq: Pointer to priority queue structure
 */
void djk_relax(struct hp_data *par, int vtx, struct heap *priq)
{
	int idx;
	int old_dist;
	int new_dist;

	/* Get the index of node in priq heap */
	idx = hp_get_index_val(priq, &vtx);

	/* Get old distance and new distance */
	old_dist = *(int *)(priq->hparr[idx]->key);
	new_dist = *(int *)par->key + 1;

	/* Check if new distance is shorter */
	if (new_dist < old_dist)
		/* if yes call hp_decrease_key to update distance */
		hp_decrease_key(priq, idx, &new_dist);
}


/*
 * Initialize priority queue for Dijkstras search
 *
 * @g:    Pointer to the graph structure
 * @priq: Pointer to priority queue
 * @src:  Source vertex for search 
 */
static void init_priq(struct graph *g, struct heap *priq, int src)
{
	int i;
	int key;
	int val;

	/*
	 * Put each vertex into queue
	 */

	/* All vertex whose vtx num is less than src */
	for (i = 0; i < src; i++) {
		val = i;
		key = DJK_INF;
		hp_insert(priq, &key, &val);
	}

	/* Src vertex */
	val = src;
	key = 0;
	hp_insert(priq, &key, &val);

	/* All vertex whose vtx num is more than src */
	for (i = 0; i < g->nvert; i++) {
		val = i;
		key = DJK_INF;
		hp_insert(priq, &key, &val);
	}
}


/*
 * Important design TODO:
 *
 * 1. Design of conv_to_int. How is the client supposed to use it?
 *    Issue here is there is no consistency to free-or-not-not-free
 *    the returned int *.
 */

