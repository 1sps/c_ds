/*
 * src/bst.c: Binary search tree implementation in C
 * St: 2016-09-29 Thu 02:48 AM
 * Up: 2016-10-08 Sat 05:37 AM
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


/*
 * Note: bst_print function has to be completed. Other funcitons are
 *       completed - although can be improved.
 *
 * Note: Good descriptive names are to be given for some functions with
 *       ibscure names.
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
static void *cpy_val_helper_queue(void *bstn);
static void dval_helper_queue(void *val);
static void *cpy_i_hlpr_st(void *val);
static void dval_i_hlpr_st(void *val);
static void printn_i_st(void *val);
static void bst_hlpr_push_node_to_stack(struct st *s, struct bst_node *bstn);
static struct bst_node *bst_hlpr_pop_and_destroy(struct st *s, struct bst *t);
static struct bst_node **bst_get_predessor(struct bst *t,
                                           struct bst_node *bstn);
static void *bst_hlpr_remove_node(struct bst *t,
                                  struct bst_node *cur, struct bst_node **par);
int bst_hlpr_print(struct bst_node *tree,
                   int is_left, int offset, int depth, char s[20][255]);

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

#ifdef RECURSIVE
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
	 * Do an iterative post order traversal,
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

/*
 * Note: This can only be called for BST with int nodes
 * TODO: Make it generic
 *
 * This is code is taken from stackoverflow with minor modifications. This code
 * can be found in below url:
 * http://stackoverflow.com/questions/801740/c-how-to-draw-a-binary-tree-to-the-console
 * and is NOT copyrighted by Author of this file. So this code can be used only
 * as allowed by stackoverflow license. Refer to www.stackoverflow.com for
 * stackoverflow licensing details.
 */
void bst_print(struct bst *t)
{
    struct bst_node *root;

    root = t->root;

    char s[20][255];
    for (int i = 0; i < 20; i++)
        sprintf(s[i], "%80s", " ");

    bst_hlpr_print(root, 0, 0, 0, s);

    printf("\n");
    for (int i = 0; i < 10; i++)
        printf("%s\n", s[i]);
    printf("\n");
}

#ifdef RECURSIVE
/*
 * Helper function to get the height of a tree - recursive.
 *
 * @bstn: Root node of the tree
 */
static int bst_get_height_hlpr_rec(struct bst_node *bstn)
{
	int height;
	int lheight;
	int rheight;

	/* Base case */
	if (bstn == NULL)
		return 0;

	/* Get height of two subtrees */
	lheight = bst_get_height_hlpr_rec(bstn->left);
	rheight = bst_get_height_hlpr_rec(bstn->right);

	/* Calculate height of current node */
	height = 1 + (lheight > rheight ? lheight : rheight);

	return height;
}
#endif /* RECURSIVE */

/*
 * Helper function to get the height of a tree - iterative.
 *
 * @bstn: Root node of the tree
 */
static int bst_get_height_hlpr(struct bst_node *root)
{
	int i;
	int height;
	int lev;
	int lev_node_count;
	int new_count;
	struct bst_node *bstn;
	struct queue *q;
	
	/* Create helper queue */
	q = q_create(cpy_val_helper_queue, NULL, dval_helper_queue, NULL);

	/*
	 * Calculate height
	 */

	lev = 0;
	lev_node_count = 0;

	/* Start calculation with root */
	if (root != NULL) {
		q_push(q, root);
		lev_node_count = 1;
	}

	/* If there is at least one node at this level */
	while (lev_node_count > 0) {
		new_count = 0;

		/* Dequeue all node of this level and enqueue their children */
		for (i = 0; i < lev_node_count; i++) {
			bstn = q_pop(q);
			if (bstn->left != NULL) {
				q_push(q, bstn->left);
				new_count++;
			}
			if (bstn->right != NULL) {
				q_push(q, bstn->right);
				new_count++;
			}
		}
		
		/* Update level */
		lev++;

		/* Update count for the new level that will be processed next */
		lev_node_count = new_count;
	}

	/* Height of the tree is the total levels in tree */
	height = lev;

	/* Destroy helper queue */
	q_destroy(q);

	return height;
}

/*
 * Get height of a BST tree.
 *
 * @t: Pointer to the BST structure
 */
int bst_get_height(struct bst *t)
{
	int height;

	height = bst_get_height_hlpr(t->root);

	return height;
}

/*
 * Check if BST is balanced at a given node
 *
 * @t:    Pointer to the tree structure
 * @bstn: Cureent node 
 */
static int bst_is_balanced_hlpr(struct bst_node *bstn, int *height)
{
	int lbal;
	int rbal;
	int lheight;
	int rheight;
	int retval;

	/* Base case */
	if (bstn == NULL) {
		*height = 0;
		return 1;
	}

	/* Check if left subtree is balanced */
	lbal = bst_is_balanced_hlpr(bstn->left, &lheight);
	if (lbal != 1) {
		retval = 0;
	}

	/* Check if right subtree is balanced - only if left is balanced */
	if (lbal == 1) {
		rbal = bst_is_balanced_hlpr(bstn->right, &rheight);
		if (rbal != 1) {
			retval = 0;
		}
	}

	/* If both subtrees balanced, compare height of subtees */
	if (lbal == 1 && rbal == 1) {
		if (lheight - rheight > 1 || rheight - lheight > 1) {
			retval = 0;
		} else {
		/* Tree is balanced at this node */
			*height = 1 + (lheight > rheight ? lheight : rheight);
			retval = 1;
		}
	}

	return retval;
}

#ifdef SKIP

/*
 * Push nodes to helper stack to do iterative post order traversal. This
 * function pushes all the nodes along the left edge to the stack.
 *
 * @sn:   Pointer to stack of nodes
 * @bstn: Pointer to a bst_node 
 */
static struct bst_node *bst_push_node_to_stack(struct st *sn,
                                               struct bst_node *bstn)
{
	while (bstn != NULL) {
		if (bstn->right != NULL)
			st_push(sn, bstn->right);
		st_push(sn, bstn);
		bstn = bstn->left;
	}
	return bstn;
}

/*
 * Pop nodes and heights from stack and test if heights are balanced.
 *
 * @sn: Pointer to the stack containing post order traversal nodes
 * @sh: Pointer to the stack containing heights of subtrees
 * @bstnp: Pointer to the node used to push to stack
 */
static int bst_pop_and_process(struct st *sn, struct st *sh,
                                              struct bst_node **bstnp)
{
	int balanced;
	int height;
	int *lheight;
	int *rheight;
	struct bst_node *cur;

	balanced = 1;
	while (*bstnp == NULL && !st_is_empty(sn)) {

		cur = st_pop(sn);
		if (cur->right != NULL && cur->right == st_peek(sn)) {
			*bstnp = st_pop(sn);
			st_push(sn, cur);
		} else {
			if (cur->right == NULL) {
				height = 0;
				st_push(sh, &height);
			}
			lheight = st_pop(sh);
			rheight = st_pop(sh);
			if (abs(*lheight - *rheight) > 1) {
				free(lheight);
				free(rheight);
				balanced = 0;
				break;
			} else {
				height = 1 + (*lheight > *rheight ?
				              *lheight : *rheight);
				st_push(sh, &height);
			}
			free(lheight);
			free(rheight);
		}
	}
	return balanced;
}

/*
 * Find out if bst is balanced at a given root node.
 *
 * @root: Pointer to the given root bst_node
 */
static int bst_is_balanced_hlpr_itr_new(struct bst_node *root)
{
	int int_push;
	int balanced; 
	struct st *sn;
	struct st *sh;
	struct bst_node *bstn;

	sn = st_create(cpy_val_helper_stack, NULL, dval_helper_stack, NULL);
	sh = st_create(cpy_i_hlpr_st, NULL, dval_i_hlpr_st, printn_i_st);

	balanced = 1;
	bstn = root;
	while (bstn != NULL || !st_is_empty(sn)) {
		if (bstn != NULL)
			bstn = bst_push_node_to_stack(sn, bstn);

		int_push = 0;
		st_push(sh, &int_push);

		balanced = bst_pop_and_process(sn, sh, &bstn);

		if (balanced == 0)
			break;
	}
	st_destroy(sn);
	st_destroy(sh);

	return balanced;
}

#endif

/*
 * Check if BST is balanced.
 *
 * @t: Pointer to the BST structure
 */
int bst_is_balanced(struct bst *t)
{
	int retval;
	/* int height; */

	/* retval = bst_is_balanced_hlpr(t->root, &height); */
	retval = bst_is_balanced_hlpr_itr_new(t->root);

	return retval;
}

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
 * TODO: Comment
 *
 * @bstn: 
 */
static void *cpy_val_helper_queue(void *bstn)
{
	return bstn;
}

/*
 * dval function for helper queue.
 *
 * @val: Pointer to value to be destroyed
 */
static void dval_helper_queue(void *val)
{
	return;
}

/*
 * copy function for helper stack which stores heights.
 *
 * @val: Pointer to value to be copied
 */
static void *cpy_i_hlpr_st(void *val)
{
	int *retval;

	retval = malloc(sizeof(int));
	assert(retval);

	*retval = *(int *) val;

	return retval;
}

/*
 * dval function for helper stack which stores heights.
 *
 * @val: Pointer to value to be destroyed
 */
static void dval_i_hlpr_st(void *val)
{
	free(val);
}

static void printn_i_st(void *val)
{
	printf("%d, ", *(int *)val);
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
	void *maybe_rchild;
	struct bst_node *rchild;
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

/*
 * This is code is taken from stackoverflow with minor modifications. This code
 * can be found in below url:
 * http://stackoverflow.com/questions/801740/c-how-to-draw-a-binary-tree-to-the-console
 * and is NOT copyrighted by Author of this file. So this code can be used only
 * as allowed by stackoverflow license. Refer to www.stackoverflow.com for
 * stackoverflow licensing details.
 */
int bst_hlpr_print(struct bst_node *tree,
                   int is_left, int offset, int depth, char s[20][255])
{
    char b[20];
    int width = 5;

    if (!tree) return 0;

    sprintf(b, "(%03d)", *(int *) tree->val);

    int left  = bst_hlpr_print(tree->left,  1, offset,                depth + 1, s);
    int right = bst_hlpr_print(tree->right, 0, offset + left + width, depth + 1, s);

#define COMPACT
#ifdef COMPACT
    for (int i = 0; i < width; i++)
        s[depth][offset + left + i] = b[i];

    if (depth && is_left) {

        for (int i = 0; i < width + right; i++)
            s[depth - 1][offset + left + width/2 + i] = '-';

        s[depth - 1][offset + left + width/2] = '.';

    } else if (depth && !is_left) {

        for (int i = 0; i < left + width; i++)
            s[depth - 1][offset - width/2 + i] = '-';

        s[depth - 1][offset + left + width/2] = '.';
    }
#else
    for (int i = 0; i < width; i++)
        s[2 * depth][offset + left + i] = b[i];

    if (depth && is_left) {

        for (int i = 0; i < width + right; i++)
            s[2 * depth - 1][offset + left + width/2 + i] = '-';

        s[2 * depth - 1][offset + left + width/2] = '+';
        s[2 * depth - 1][offset + left + width + right + width/2] = '+';

    } else if (depth && !is_left) {

        for (int i = 0; i < left + width; i++)
            s[2 * depth - 1][offset - width/2 + i] = '-';

        s[2 * depth - 1][offset + left + width/2] = '+';
        s[2 * depth - 1][offset - width/2 - 1] = '+';
    }
#endif

    return left + width + right;
}

