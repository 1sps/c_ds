/*
 * src/avl.c: Implementation of AVL tree
 *
 * St: 2016-10-05 Wed 07:07 PM
 * Up: 2016-10-06 Thu 12:22 AM
 *
 * Author: SPS
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

#include"mylib.h"

#define SKIP


/* Function prototype declaration of helper functions */
static struct avl_node *avl_hlpr_pop_and_destroy(struct st *s, struct avl *avlt);
static void avl_hlpr_push_node_to_stack(struct st *s, struct avl_node *bstn);
static void dval_helper_stack(void *avln);
static void *cpy_val_helper_stack(void *avln);


/*
 * Create an AVL tree.
 *
 * @cpy:    Pointer to copy function
 * @cmp:    Ponter to compare function
 * @dval:   Pointer to value destroy function
 * @printn: Pointer to node print function
 */
struct avl *avl_create(void *(*cpy)(void *), int (*cmp)(void *, void *),
                       void (*dval)(void *),
		       void (*printn) (void *))
{
	struct avl *avlt;

	avlt = malloc(sizeof(struct avl));
	assert(avlt);

	avlt->root = NULL;

	avlt->cpy = cpy;
	avlt->cmp = cmp;
	avlt->dval = dval;
	avlt->printn = printn;

	avlt->nmemb = 0;

	return avlt;
}

/*
 * Rotate a node in AVL tree
 *
 * @avln: Pointer to the AVL node pointer
 * @dir:  Direction of child which will come to current node's place
 */
static void avl_rotate(struct avl_node **avln, int dir)
{
	struct avl_node *root;
	struct avl_node *child;
	struct avl_node *grand_child;

	/* Get the init positions */
	root = *avln;
	assert(root);

	if (dir == AVL_LEFT) {
		child = root->left;
		assert(child);
		grand_child = child->right;
	} else if (dir == AVL_RIGHT) {
		child = root->right;
		assert(child);
		grand_child = child->left;
	}

	/* Rotate */
	*avln = child;
	if (dir == LEFT) {
		child->right = root;
		root->left = grand_child;
	} else if (dir == AVL_RIGHT) {
		child->left = root;
		root->right = grand_child;
	}
}

/*
 * Update an AVL node's balance info.  
 *
 * @root: Node whose balance info has to be updated
 */
static int avl_update_bal(struct avl_node *root)
{
	int left;
	int right;

	left = 0;
	right = 0;

	if (root->left != NULL)
		left = -1;
	if (root->right != NULL)
		right = 1;

	left += avl_get_bal(root->left);
	right += avl_get_bal(root->right);

	root->bal = left + right;
}

/*
 * Return the balance info if an AVL node.
 *
 * @root: Node whose balance info is to be returned
 */
static int avl_get_bal(struct avl_node *root)
{
	if (root == NULL)
		return 0;
	else
		return root->bal;
}

/*
 * Insert a new node to AVL tree.
 *
 * @avlt: Pointer to the AVL tree
 * @val:  Pointer to the value to be inserted
 */
int avl_insert(struct avl *avlt, void *val)
{
	struct avl_node **avln;

	avln = &avlt->root;

	avl_insert_hlpr(avlt, avln, val);

	return 1;
}

/*
 * Insert a new node to AVL tree without losing balance.
 *
 * @root: Pointer to the current node pointer 
 * @val:  Pointer to the value to be inserted
 */
int avl_insert_hlpr(struct avl *t, struct avl_node **root, void *val)
{
	struct avl_node *avln;
	struct avl_node *cur;

	if (*root == NULL) {
	/* Slot found */
		avln = avl_create_node(val);
		*root = avln;
	} else {
		cur = *root;

		if (t->cmp(val, cur->val) <= 0) {
			cur->left = avl_insert_hlpr(t, &cur->left, val);
		} else {
			cur->right = avl_insert_hlpr(t, &cur->right, val);
		}

		avl_fix(*root);
		avl_rebalance(root);
	}
}

struct avl_node *avl_search(struct avl *avlt, void *val);
void *avl_delete(struct avl *avlt, void *val);


/*
 * Destroy an AVL tree.
 *
 * Traverse through each node of the avl and destroy it.
 * Itertative post-order traversal is done. We create a 
 * helper stack to store the pointer to avl nodes.
 *
 * @avlt: Pointer to the avl structure
 */
void avl_destroy(struct avl *avlt)
{
	struct avl_node *avln;
	struct st *s;

	/* Create helper stack */
	s = st_create(cpy_val_helper_stack, NULL, dval_helper_stack, NULL);

	/* 
	 * Do an iterative post order traversal,
	 * destroying nodes along the way
	 */

	avln = avlt->root;
	while (avln != NULL || !st_is_empty(s)) {
		if (avln != NULL) {
			avl_hlpr_push_node_to_stack(s, avln);
			/* Move on to next left node */
			avln = avln->left;
		} else {
			avln = avl_hlpr_pop_and_destroy(s, avlt);
		}
	}

	/* Destroy helper stack */
	st_destroy(s);

	/* Free tree strcutre */
	free(avlt);
}

void avl_print(struct avl *avlt);


/*
 *******************************************************************************
 * Helper functions
 *******************************************************************************
 */


/*
 * Push a (pointer to) avl_node to stack
 *
 * @s:    Stack of avl_node pointers
 * @avln: Pointer to a avl_node
 */
static void avl_hlpr_push_node_to_stack(struct st *s, struct avl_node *avln)
{
	/* If right child present, push it first */
	if (avln->right != NULL)
		st_push(s, avln->right);
	/* Push current node */
	st_push(s, avln);
}

/*
 * Pop a node from helper stack and either destroy the
 * popped node, or inform the caller function to move on
 * with the right child of popped node.
 *
 * @s: Stack of avl_node pointers
 * @t: Bst structure. Needed because it gives us the
 *     t->dval() function.
 */
static struct avl_node *avl_hlpr_pop_and_destroy(struct st *s, struct avl *t)
{
	struct avl_node *btn;
	void *maybe_rchild;
	struct avl_node *rchild;
	int rchild_first;
	struct avl_node *retval;

	/* Pop a node from helper stack */
	btn = st_pop(s);

	/*
	 * Check if currently popped node's right child
	 * is still waiting to be processed. If yes, then
	 * push back the current node, and move on with
	 * rchild. Else destroy the currently popped node. 
	 */

	rchild_first = 0;
	if (!st_is_empty(s)) {
		maybe_rchild = st_peek(s);
		if (maybe_rchild == btn->right)
			rchild_first = 1;
	}

	if (rchild_first == 1) {
		/* Move on with right child */
		rchild = st_pop(s);
		st_push(s, btn);
		retval = rchild;
	} else {
		/* Destroy currently popped node */
		t->dval(btn->val);
		free(btn);
		retval = NULL;
	}

	return retval;
}

/*
 * TODO: Comment
 *
 * @bstn: 
 */
static void dval_helper_stack(void *avln)
{
	return;
}

/*
 * TODO: Comment
 *
 * @bstn: 
 */
static void *cpy_val_helper_stack(void *avln)
{
	return avln;
}

