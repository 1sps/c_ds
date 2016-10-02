/*
 * test/graphTest.c: Test graph implementation.
 *
 * St: 2016-10-02 Sun 12:47 AM
 * Up: 2016-10-02 Sun 05:24 AM
 *
 * Author: SPS
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<time.h>

#include"funcutils.h"
#include"../src/mylib.h"

#define INITITAL_VERTEX_COUNT 20

#define SKIP

/* Test graph_has_edge() for int graph */
int test_graph_int_has_edge(void)
{
	int src;
	int dest;
	int *src_ptr;
	int *dest_ptr;
	struct graph *g;

	src_ptr = &src;
	dest_ptr = &dest;

	/* Create an int graph */
	g = graph_create(INITITAL_VERTEX_COUNT, GRAPH_INT);

	/* Add edges */
	src = 0;
	dest = 3;
	graph_add_edge(g, src_ptr, dest_ptr);

	src = 0;
	dest = 5;
	graph_add_edge(g, src_ptr, dest_ptr);

	src = 0;
	dest = 8;
	graph_add_edge(g, src_ptr, dest_ptr);

	src = 0;
	dest = 8;
	graph_add_edge(g, src_ptr, dest_ptr);

	src = 2;
	dest = 3;
	graph_add_edge(g, src_ptr, dest_ptr);

	src = 2;
	dest = 6;
	graph_add_edge(g, src_ptr, dest_ptr);

	src = 7;
	dest = 1;
	graph_add_edge(g, src_ptr, dest_ptr);

	/* Print the graph */
	/* graph_print(g); */

	src = 2;
	dest = 6;
	assert(graph_has_edge(g, src_ptr, dest_ptr) == 1);

	src = 2;
	dest = 7;
	assert(graph_has_edge(g, src_ptr, dest_ptr) == 0);

	/* Test graph_bfs_search */
	src = 2;
	dest = 6;
	assert(graph_bfs(g, src_ptr, dest_ptr) == 1);

	src = 2;
	dest = 7;
	assert(graph_bfs(g, src_ptr, dest_ptr) == 0);


	/* Destroy the graph */
	graph_destroy(g);

	return 1;
}
/* Test graph_add_edge() for int graph */
/* TODO: No assertions are done, need to do it */
int test_graph_int_add_edge(void)
{
	int src;
	int dest;
	int *src_ptr;
	int *dest_ptr;
	struct graph *g;

	src_ptr = &src;
	dest_ptr = &dest;

	/* Create an int graph */
	g = graph_create(INITITAL_VERTEX_COUNT, GRAPH_INT);

	/* Add edges */
	src = 0;
	dest = 3;
	graph_add_edge(g, src_ptr, dest_ptr);

	src = 0;
	dest = 5;
	graph_add_edge(g, src_ptr, dest_ptr);

	src = 0;
	dest = 8;
	graph_add_edge(g, src_ptr, dest_ptr);

	src = 0;
	dest = 8;
	graph_add_edge(g, src_ptr, dest_ptr);

	src = 2;
	dest = 3;
	graph_add_edge(g, src_ptr, dest_ptr);

	src = 2;
	dest = 6;
	graph_add_edge(g, src_ptr, dest_ptr);

	src = 7;
	dest = 1;
	graph_add_edge(g, src_ptr, dest_ptr);

	/* Destroy the graph */
	graph_destroy(g);

	return 1;
}

/* Test graph_create() for int graph */
int test_graph_int_create(void)
{
	struct graph *g;

	/* Create an int graph */
	g = graph_create(INITITAL_VERTEX_COUNT, GRAPH_INT);

	/* Test other settings */
	/* TODO */

	/* Destroy the graph */
	graph_destroy(g);

	return 1;
}

/* Test graph where vertices labels are int */
int test_graph_int(void)
{
	test_graph_int_create();
	test_graph_int_add_edge();
	test_graph_int_has_edge();

	return 1;
}

/* main: perform graph test */
int main(void)
{
	test_graph_int();
	/* test_graph_str(); */

	return 0;
}

