/*File: bst.c
 *Author: Chance Tudor
 *Implements functions found in bst.h in order to implement a binary search tree
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"
#include "bst.h"

static int isLeftChild(TNODE *);
static int isRoot(TNODE *);
static int isRightChild(TNODE *);
static int isLeaf(TNODE *);
static int findMinDepth(TNODE *);
static int findMaxDepth(TNODE *);
static void displayNode(BST *, TNODE *, FILE *);
static TNODE *swapVals(TNODE *x, TNODE *y);
static TNODE *getPred(TNODE *n);
static TNODE *getSucc(TNODE *n);
////////////////////////////////////////////////////////////////////////////////
// BST constructor
extern BST * newBST(int (*c)(void * one, void * two)) {
  BST * tree = malloc(sizeof(BST));
  assert(tree != NULL);
  tree->root = NULL;
  tree->size = 0;
  tree->comparator = c;
  tree->displayMethod = 0;
  tree->swapper = 0;
  tree->freeMethod = 0;

  return tree;
}
// sets displayMethod
extern void setBSTdisplay(BST * t, void (*d)(void * ptr, FILE * fp)) {
  t->displayMethod = d;
}
// sets swapMethod
extern void setBSTswapper(BST *t, void (*s)(TNODE * one, TNODE * two)) {
  t->swapper = s;
}
// sets freeMethod
extern void setBSTfree(BST *t, void (*f)(void * ptr)) {
  t->freeMethod = f;
}
// returns root of a tree
extern TNODE * getBSTroot(BST *t) {
  return t->root;
}
// sets root of a tree
extern void setBSTroot(BST *t, TNODE *replacement) {
  t->root = replacement;
}
// sets size of a tree
extern void setBSTsize(BST *t, int s) {
  t->size = s;
}
// inserts a new node into the BST and returns inserted node
extern TNODE *insertBST(BST *t, void * value) {
  setBSTsize(t, ++sizeBST(t));
  TNODE * newNode = newTNODE(value, NULL, NULL, NULL);
  assert(newNode != NULL);
  TNODE * temp = getBSTroot(t);

  //If the tree is empty set the root, return
  if (temp == NULL) {
    setBSTroot(t, newNode);
    return newNode;
  }
  while (temp) {
    newNode->parent = temp;
    if (t->comparator(getTNODEvalue(temp), value) == 1) {
      temp = temp->left;
    }
    else {
      temp = temp->right;
    }
  }
  if (tree->comparator(getTNODEvalue(getTNODEparent(newNode)), value) == 1) {
    setTNODEleft(getTNODEparent(newNode), newNode)
  }
  else {
    setTNODEright(getTNODEparent(newNode), newNode)
  }

  return newNode;
}
// returns the value with the searched-for key
// if key is not in the tree, the method returns null
extern void * findBST(BST *t, void *key) { // FIXME: write function
  TNODE * temp = findBSTNode(tree, value);
  if () return getTNODEvalue(temp);
  return NULL;
}
// FIXME: complete function
static TNODE * findBSTNode(bst *tree, void *value) {
  bstNode *temp = tree->root;
  while (temp && tree->comparator(temp->value, value) != 0) {
    if (tree->comparator(temp->value, value) > 0) temp = temp->left;
    else temp = temp->right;
  }
  return temp;
}
/* method returns -1 if given value is not in the tree; 0 otherwise
* if present, tree node holding given val is removed from tree
* node is removed by swapping value to a leaf and then pruning the leaf
* size is decremented
*/
extern int deleteBST(BST *t, void *key); // FIXME: write function
/* takes a node and recursively swaps its value with its predecessor's (preferred)
* or its successor's until a leaf node holds the original value.
* calls the BST’s swapper function to actually accomplish the swap,
* sending the two nodes whose values need to be swapped.
* The method returns the leaf node holding the swapped value.
*/
extern TNODE *swapToLeafBST(BST *t, TNODE *node) {
  if (node == NULL) return node; //error
  if (isLeaf(node)) return node;
  TNODE *temp;
  if (getTNODEleft(temp)) {
    temp = swapValues(node, findPredecessor(node));
  }
  else {
    temp = swapValues(node, findSuccessor(node));
  }
  return swapToLeafBSTNode(temp);
}
/* method detaches given node from the tree
* does not free the node nor decrement size
*/
extern void pruneLeafBST(BST *t, TNODE *leaf);
// returns the number of nodes currently in the tree
extern int sizeBST(BST * t) {
  return t->size;
}
/* displays:
* number of nodes in the tree
* minimum height
* maximum height
*/
extern void statisticsBST(BST *t, FILE *fp) {
  fprintf(fp, "Nodes: %d\n", sizeBST(t));
  fprintf(fp, "Minimum depth: %d\n", findMinDepth(t->root));
  fprintf(fp, "Maximum depth: %d\n", findMaxDepth(t->root));
}
/* prints a level-order traversal of the tree; each level on its own line
* each line starts with the level number
* single space separates entries on a line
* root val tagged w/ 'X,' left child w/ 'L,' right child w/ 'R,' leaves w/ '='
* '=' should precede the node value. example:
* 0: 20(20)X
* 1: =7(20)L =33(20)R
* empty tree prints 0: followed by newline
*/
extern void displayBST(BST *t, FILE *fp) {
  int level = 0;
  if (sizeBST(t) == 0) {
    fprintf(fp, "0:\n");
  }
  else {
    QUEUE *q = newQUEUE();
    setQUEUEdisplay(q, t->displayMethod);
    //If tree is not null its first element is enqueued onto the queue
    fprintf(fp, "%d:", level);
    if (getBSTroot(t) != NULL) {
      enqueue(q, getBSTroot(t));
      enqueue(q, NULL);
    }
    while (sizeQUEUE(q) != 0) {
      // 1st elem in queue is dequeued and displayed
      TNODE *temp = (TNODE *)dequeue(q);
      if (temp == NULL) {
        if (sizeQUEUE(q)) {
          fprintf(fp, "\n");
          fprintf(fp, "%d:", ++level);
          enqueue(q, NULL);
        }
      }
      else {
        fprintf(fp, " ");
        displayNode(fp, temp, t);
        // Its children, if they exist, are enqueued onto the queue,
        // left child, then right child
        if (getTNODEleft(temp)) { enqueue(q, getTNODEleft(temp)); }
        if (getTNODEright(temp)) { enqueue(q, getTNODEright(temp)); }
      }
    }
    fprintf(fp, "\n");
    freeQUEUE(q);
  }
}
/*
* debugLevel == 1 : displayMethod prints an in-order traversal
* debugLevel == 2 : displayMethod prints a pre-order traversal
* debugLevel == 3 : displayMethod prints a post-order traversal
* @ any given node, method displays left and right subtrees, each enclosed by
* brackets, but only if they exist
* space is printed by displayBST to separate any existing subtrees/node values
* empty tree displayed as []
* example:
* [[7] 20 [33]]
* [20 [7] [33]]
* [[7] [33] 20]
*/
extern int debugBST(BST *t, int level); // FIXME: write function
/* walks through the tree, freeing the nodes and their values if necessary
* then the tree object itself is freed
*/
extern void freeBST(BST *t); // FIXME: write function
////////////////////////////////////////////////////////////////////////////////
extern void pruneBSTNode(bst *tree, bstNode *n) {
  tree->size--;
  if (isRoot(n)) {
    tree->root = NULL;
  }
  else {
    if (isLeftChild(n)) n->parent->left = NULL;
    else n->parent->right = NULL;
  }
  deleteNode(n);
}

static TNODE *swapVals(TNODE *x, TNODE *y) {
  void *temp = getTNODEvalue(x);
  //x->value = y->value;
  setTNODEvalue(x, getTNODEvalue(y));
  //y->value = temp;
  setTNODEvalue(y, getTNODEvalue(temp));

  return y;
}

static TNODE *getPred(TNODE *n) {
  TNODE *temp = getTNODEleft(n);
  while (getTNODEright(temp)) temp = getTNODEright(temp);
  return temp;
}

static TNODE *getSucc(TNODE *n) {
  TNODE *temp = getTNODEright(n);
  while (getTNODEleft(temp)) temp = getTNODEleft(temp);
  return temp;
}

static int findMinDepth(TNODE *n) {
  if (n == NULL) {
    return 0;
  }

  int rightDepth = findMinDepth(getTNODEright(n));
  int leftDepth = findMinDepth(getTNODEleft(n));

  if (rightDepth < leftDepth) {
    return rightDepth + 1;
  }
  else {
    return leftDepth + 1;
  }
}

static int findMaxDepth(TNODE *n) {
  if (n == NULL) {
    return 0;
  }
  int rightDepth = findMaxDepth(getTNODEright(n));
  int leftDepth = findMaxDepth(getTNODEleft(n));

  if (rightDepth > leftDepth) {
    return rightDepth + 1;
  }
  else {
    return leftDepth + 1;
  }
}

static void displayNode(BST *t, TNODE *n, FILE *fp) {
  if (n == NULL) { return; } //error
  if (isLeaf(n)) { fprintf(fp, "="); }
  tree->displayMethod(fp, getTNODEvalue(n));

  fprintf(fp, "(");
  tree->displayMethod(fp, getTNODEvalue(getTNODEparent(n)));
  fprintf(fp, ")");
  if (isLeftChild(n)) {
    fprintf(fp, "L");
  }
  else if (isRightChild(n)) {
    fprintf(fp, "R");
  }
}

static int isRightChild(TNODE *n) {
  if (getTNODEright(getTNODEparent(n)) == n) return 1;
  return 0;
}

static int isLeftChild(TNODE *n) {
  if (getTNODEleft(getTNODEparent(n)) == n) return 1;
  return 0;
}

static int isLeaf(TNODE *n) {
  if (!getTNODEleft(n) && !getTNODEright(n)) return 1;
  return 0;
}

static int isRoot(TNODE *n) {
  if (getTNODEparent(n) == n) return 1;
  return 0;
}
