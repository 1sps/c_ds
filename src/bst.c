/*
 * src/bst.c: Binary search tree implementation in C
 *
 * St: 2016-09-29 Thu 02:48 AM
 * Up: 2016-09-30 Fri 05:07 AM
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
#include<assert.h>

#include"mylib.h"

#define SKIP

/*
 * Function prototype declaration of helper functions
 */

static struct bst_node *bst_node_create(struct bst *t, void *val);
static void dval_helper_stack(void *bstn);
static void *cpy_val_helper_stack(void *bstn);
static void bst_hlpr_push_node_to_stack(struct st *s, struct bst_node *bstn);
static struct bst_node *bst_hlpr_pop_and_destroy(struct st *s, struct bst *t);
static struct bst_node **bst_get_predessor(struct bst *t,
                                           struct bst_node *bstn);
static void *bst_hlpr_remove_node(struct bst *t,
                                  struct bst_node *cur,
                                  struct bst_node **par)

/*
 * Create a binary search tree.
 *
 * @cpy:    Pointer to copy function 
 * @cmp:    Pointer to compare function
 * @dval:   Pointer to function which destroys a nodes val
 * @printn: Pointer to function which prints a node
 */
struct bst *bst_create(void *(*cpy)(void *), int (*cmp)(void *, void *),
                       void (*dval)(void *),
		       void (*printn) (struct bst_node *))
{
	struct bst *t;

	t = malloc(sizeof(struct bst));
	assert(t);

	t->root = NULL;
	t->nmemb = 0;

	t->cpy = cpy;
	t->cmp = cmp;
	t->dval = dval;
	t->printn = printn;

	return t;
}

/*
 * Insert a new value to BST
 *
 * @t:   Pointer to the bst structure
 * @val: Pointer to the value to be inserted
 */
int bst_insert(struct bst *t, void *newval)
{
	struct bst_node *bstn;
	struct bst_node **prevn;
	struct bst_node *curn;

	bstn = bst_node_create(t, newval);

	prevn = &t->root;

	while (*prevn != NULL) {
		curn = *prevn;
		if (t->cmp(newval, curn->val) <= 0)
			prevn = &curn->left; 
		else
			prevn = &curn->right; 
	}

	*prevn = bstn;
	t->nmemb++;

	/*
	 * TODO: Make checks within the function to 
	 * see if insert fails, and return 0 if so.
	 */
	return 1;
}

/*
 * Search for a val in bst.
 *
 * @t:          Pointer to the bst tree structure
 * @search_val: Pointer to the value to search
 */
struct bst_node *bst_search(struct bst *t, void *search_val)
{
	struct bst_node *cur;
	struct bst_node *retval;

	retval = NULL;

	cur = t->root;
	
	while (cur != NULL) {
		if (t->cmp(cur->val, search_val) == 0) {
			/*
			 * search_val found. Update retval
			 * and break out of loop
			 */
			retval = cur;
			break;
		} else if (t->cmp(cur->val, search_val) > 0) {
			/*
			 * Cur val is greatr than search_val, so
			 * search in the left subtree
			 */
			cur = cur->left;
		} else {
			/*
			 * Cur val is smaller than search_val, so
			 * search in the right subtree
			 */
			cur = cur->right;
		}
	}

	return retval;
}

/*
 * Delete a node from BST. Has no effect if the value
 * is not found in BST.
 *
 * @t:      Pointer to the bst tree structure
 * @delval: value to be deleted
 */
void *bst_delete(struct bst *t, void *delval)
{
	void *retval;
	struct bst_node *cur;
	struct bst_node **par;

	retval = NULL;

	par = &t->root;

	while (*par != NULL) {
		cur = *par;
		if (t->cmp(cur->val, delval) == 0) {
		/* Node to delete is found. */
			retval = bst_hlpr_remove_node(t, cur, par);
			t->nmemb--;
			break;
		}
		else if (t->cmp(cur->val, delval) > 0) {
			par = &cur->left;
		} else {
			par = &cur->right;
		}
	}

	return retval;
}

#ifdef RECURSIVE_DESTROY
/*
 * Destroy a bst - recursive version.
 *
 * @t: Pointer to the bst structure
 */
void bst_destroy_rec(struct bst *t)
{
	struct bst_node *bstn;

	if (bstn == NULL)
		return;

	bst_destroy(bstn->left);
	bst_destroy(bstn->right);

	t->dval(bstn->val);
	free(bstn);
}
#endif  /* RECURSIVE_DESTROY */

/*
 * Destroy a bst.
 *
 * Traverse through each node of the bst and destroy it.
 * Itertative post-order traversal is done. We create a 
 * helper stack to store the pointer to bst nodes.
 *
 * @t: Pointer to the bst structure
 */
void bst_destroy(struct bst *t)
{
	struct bst_node *bstn;
	struct st *s;

	/* Create helper stack */
	s = st_create(cpy_val_helper_stack, NULL, dval_helper_stack, NULL);

	/* 
	 * Do an interative post order traversal,
	 * destroying nodes along the way
	 */

	bstn = t->root;
	while (bstn != NULL || !st_is_empty(s)) {
		if (bstn != NULL) {
			bst_hlpr_push_node_to_stack(s, bstn);
			/* Move on to next left node */
			bstn = bstn->left;
		} else {
			bstn = bst_hlpr_pop_and_destroy(s, t);
		}
	}

	/* Destroy helper stack */
	st_destroy(s);

	/* Free tree strcutre */
	free(t);
}

void bst_print(struct bst *t);

/* 
 ******************************************************************************
 * Helper functions
 ******************************************************************************
 */


/*
 * Create a bst_node
 *
 * @t:   Pointer to the tree structure
 * @val: Pointer to the value to be inserted
 */
static struct bst_node *bst_node_create(struct bst *t, void *val)
{
	struct bst_node *bstn; 

	bstn = malloc(sizeof(struct bst_node));
	assert(bstn);

	bstn->val = t->cpy(val);
	bstn->left = NULL;
	bstn->right = NULL;

	return bstn;
}

/*
 * TODO: Comment
 *
 * @bstn: 
 */
static void dval_helper_stack(void *bstn)
{
	return;
}

/*
 * TODO: Comment
 *
 * @bstn: 
 */
static void *cpy_val_helper_stack(void *bstn)
{
	return bstn;
}

/*
 * Push a (pointer to) bst_node to stack
 *
 * @s:    Stack of bst_node pointers
 * @bstn: Pointer to a bst_node
 */
static void bst_hlpr_push_node_to_stack(struct st *s, struct bst_node *bstn)
{
	/* If right child present, push it first */
	if (bstn->right != NULL)
		st_push(s, bstn->right);
	/* Push current node */
	st_push(s, bstn);
}

/*
 * Pop a node from helper stack and either destroy the
 * popped node, or inform the caller function to move on
 * with the right child of popped node.
 *
 * @s: Stack of bst_node pointers
 * @t: Bst structure. Needed because it gives us the
 *     t->dval() function.
 */
static struct bst_node *bst_hlpr_pop_and_destroy(struct st *s, struct bst *t)
{
	struct bst_node *btn;
	struct bst_node *maybe_rchild;
	int rchild_first;
	struct bst_node *retval;

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
		maybe_rchild = st_pop(s);
		if (maybe_rchild == btn->right)
			rchild_first = 1;
		else
			/* Not right child! Push it back to stack */
			st_push(s, maybe_rchild);
	}

	if (rchild_first == 1) {
		/* Move on with right child */
		st_push(s, btn);
		retval = maybe_rchild;
	} else {
		/* Destroy currently popped node */
		t->dval(btn->val);
		free(btn);
		retval = NULL;
	}

	return retval;
}

/*
 * Get predessor of a node.
 *
 * Note: It is an error to call this for a bst_node whose left
 *       child is not present.
 *
 * @t:    Pointer to the BST structure
 * @bstn: node whose predessor is to be found
 */
static struct bst_node **bst_get_predessor(struct bst *t, struct bst_node *bstn)
{
	struct bst_node **predecessor;

	predecessor = &bstn->left;

	while ((*predecessor)->right != NULL)
		predecessor = &(*predecessor)->right;

	return predecessor;
}

/*
 * Remove a node from BST.
 *
 * This is algorithmn roughly:
 *
 * 1. If delnode is a leaf node free its memory and update
 *    parents pointer to nullpointer.
 *
 * 2. If it has only one child (left or right but not both),
 *    then free its memory and make parents pointer point to 
 *    that child.
 *
 * 3. If it has both children, then, swap the node with its
 *    predesessor along with updating parent's pointer.
 *    Once swap is done, free the node as if it were a leaf
 *    node (which is true now, because it is in it's
 *    predecessors position after the swap).
 *
 * Note that t-nmemb is not updated. Caller funciton should 
 * update it. 
 *
 * @t:   Pointer to the BST structure
 * @cur: Pointer to the node to remove
 * @par: Pointer to the parent node's left
 *       or right child pointer
 */
static void *bst_hlpr_remove_node(struct bst *t, struct bst_node *cur,
                                                 struct bst_node **par)
{
	void *retval;
	struct bst_node **predecessor;

	if (cur->left == NULL && cur->right == NULL) {
	/* If leaf node  */
		*par = NULL;
	} else if (cur->left == NULL || cur->right == NULL) {
	/* If only one child is present */
		if (cur->left != NULL)
			*par = cur->left;
		else
			*par = cur->right;
	} else {
	/* If both children present */

		/* Get the predecessor and make needed changes */
		predecessor = bst_get_predessor(t, cur);
		*par = *predecessor;
		if (cur->left != *predecessor)
			(*predecessor)->left = cur->left;
		(*predecessor)->right = cur->right;
		*predecessor = NULL;
	}

	retval = t->cpy(cur->val);

	/* Destroy the node */
	t->dval(cur->val);
	free(cur);

	return retval;
}

