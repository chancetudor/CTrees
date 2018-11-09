/*File: bst.c
 *Author: Chance Tudor
 *Implements functions found in bst.h in order to implement a binary search tree
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"
#include "bst.h"
#include "tnode.h"

static int isLeftChild(TNODE *n);
static int isRoot(TNODE *n);
static int isRightChild(TNODE *n);
static int isLeaf(TNODE *n);
static int findMinDepth(TNODE *n);
static int findMaxDepth(TNODE *n);
static int getDebugVal(BST *t);
//static int min(int x, int y);
static void displayNode(BST *t, TNODE *n, FILE *fp);
static void displayLevel(BST *t, FILE *fp);
static void displayInOrder(BST *t, TNODE *n, FILE *fp);
static void displayPreOrder(BST *t, TNODE *n, FILE *fp);
static void displayPostOrder(BST *t, TNODE *n, FILE *fp);
static void freeSubTree(TNODE *n);
static TNODE * swapVals(TNODE *x, TNODE *y);
static TNODE *getPred(TNODE *n);
static TNODE *getSucc(TNODE *n);
static TNODE *findBSTNode(BST *tree, void *key);
////////////////////////////////////////////////////////////////////////////////
// BST constructor
extern BST * newBST(int (*c)(void * one, void * two)) {
  BST * tree = malloc(sizeof(BST));
  assert(tree != NULL);
  tree->root = 0;
  tree->size = 0;
  tree->debugVal = 0;
  tree->comparator = c;
  tree->displayMethod = 0;
  tree->swapper = (void *)swapVals;
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
  setTNODEparent(replacement, replacement);
}
// sets size of a tree
extern void setBSTsize(BST *t, int s) {
  t->size = s;
}
// inserts a new node into the BST and returns inserted node
extern TNODE *insertBST(BST *t, void * value) {
  TNODE * newNode = newTNODE(value, 0, 0, 0);
  assert(newNode != NULL);
  setTNODEfree(newNode, (void *)t->freeMethod);
  setTNODEdisplay(newNode, (void *)t->displayMethod);
  TNODE * temp = getBSTroot(t);

  // If tree is empty: set root, return
  if (temp == 0) {
    setBSTroot(t, newNode);
    setBSTsize(t, sizeBST(t) + 1);
    return newNode;
  }
  while (temp != 0) {
    setTNODEparent(newNode, temp);
    if (t->comparator(getTNODEvalue(getTNODEparent(newNode)), value) >= 1) {
      // value of newNode's parent > value being inserted, so go left
      temp = getTNODEleft(temp);
    }
    else {
      // value of newNode's parent < value being inserted, so go right
      temp = getTNODEright(temp);
    }
  }
  if (t->comparator(getTNODEvalue(getTNODEparent(newNode)), value) >= 1) {
    // value of parent > value being inserted
    // so newNode is left child of its parent
    setTNODEleft(getTNODEparent(newNode), newNode);
  }
  else {
    // value of parent < value being inserted
    // so newNode is right child of its parent
    setTNODEright(getTNODEparent(newNode), newNode);
  }

  setBSTsize(t, sizeBST(t) + 1);
  return newNode;
}
// returns the value with the searched-for key
// if key is not in the tree, the method returns null
extern void * findBST(BST *t, void *key) {
  TNODE * temp = findBSTNode(t, key);
  if (temp != NULL) { return getTNODEvalue(temp); }
  else { return 0; }
}
/*
* returns the tree node holding the searched-for key.
* If the key is not in the tree, the method should return null.
*/
extern TNODE * locateBST(BST *t, void *key) {
  TNODE * temp = findBSTNode(t, key);
  if (temp != NULL) { return temp; }
  else { return 0; }
}
/* method returns -1 if given value is not in the tree; 0 otherwise
* if present, tree node holding given val is removed from tree
* node is removed by swapping value to a leaf and then pruning the leaf
* size is decremented
*/
extern int deleteBST(BST *t, void *key) {
  TNODE * temp = findBSTNode(t, key);
  if (temp) {
    temp = swapToLeafBST(t, temp); // temp now a leaf
    pruneLeafBST(t, temp); // prune leaf
    free(temp);
    setBSTsize(t, sizeBST(t) - 1); // decrement size
    return 0;
  }
  else {
    printf("Error: node with key not found\n");
    return -1; // temp not in tree
  }
}
/* takes a node and recursively swaps its value with its predecessor's (preferred)
* or its successor's until a leaf node holds the original value.
* calls the BST’s swapper function to actually accomplish the swap,
* sending the two nodes whose values need to be swapped.
* The method returns the leaf node holding the swapped value.
*/
extern TNODE *swapToLeafBST(BST *t, TNODE *node) {
  if (t->swapper == 0) {
    setBSTswapper(t, (void*)swapVals);
  }
  if (node == 0) return node; // error
  if (isLeaf(node)) return node;
  TNODE *temp = node;
  if (getTNODEleft(temp)) {
    //t->swapper(temp, getPred(temp));
    temp = swapVals(temp, getPred(temp));
  }
  else {
    //t->swapper(temp, getSucc(temp));
    temp = swapVals(temp, getSucc(temp));
  }
  return swapToLeafBST(t, temp);
}
/* method detaches given node from the tree
* does not free the node nor decrement size
*/
extern void pruneLeafBST(BST *t, TNODE *leaf) {
  if (t == 0) {
    return;
  }
  else if (isRoot(leaf)) {
    return;
  }
  TNODE * parent = getTNODEparent(leaf);
  if (getTNODEleft(parent) == leaf) { // leaf is left of parent
    setTNODEleft(parent, 0); // detaches leaf from parent
    setTNODEparent(leaf, 0); // detaches parent from leaf
  }
  else { // leaf is right of parent
    setTNODEright(parent, 0); // detaches leaf from parent
    setTNODEparent(leaf, 0); // detaches parent from leaf
  }
}
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
  int minDepth = 0;
  int maxDepth = 0;
  if (sizeBST(t) == 0) {
    minDepth = -1;
    maxDepth = -1;
  }
  else if (sizeBST(t) == 1) {
    minDepth = 0;
    maxDepth = 0;
  }
  else {
    minDepth = findMinDepth(getBSTroot(t));
    maxDepth = findMaxDepth(getBSTroot(t));
  }
  fprintf(fp, "Minimum depth: %d\n", minDepth);
  fprintf(fp, "Maximum depth: %d\n", maxDepth);
}
/*
* debugLevel == 0 : displayMethod prints a level-order traversal
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
extern void displayBST(BST *t, FILE *fp) {
  // level order
  if (getDebugVal(t) == 0) {
    displayLevel(t, fp);
  }
  // in-order
  else if (getDebugVal(t) == 1) {
    TNODE * temp = getBSTroot(t);
    displayInOrder(t, temp, fp);
  }
  // pre-order
  else if (getDebugVal(t) == 2) {
    TNODE * temp = getBSTroot(t);
    displayPreOrder(t, temp, fp);
  }
  // post-order
  else {
    TNODE * temp = getBSTroot(t);
    displayPostOrder(t, temp, fp);
  }
}
/*
* sets debugVal for tree
* returns previous debugVal
*/
extern int debugBST(BST *t, int level) {
  int prevVal = t->debugVal;
  t->debugVal = level;

  return prevVal;
}
/* walks through the tree, freeing the nodes & their values if necessary
* then tree object itself is freed
*/
extern void freeBST(BST *t) {
  TNODE * temp = getBSTroot(t);
  if (temp == 0 || sizeBST(t) == 0) {
    free(t);
    //setBSTroot(t, 0);
    return;
  }
  freeSubTree(temp);
  free(t);
  //setBSTroot(t, 0);
}
////////////////////////////////////////////////////////////////////////////////
static int getDebugVal(BST *t) {
  return t->debugVal;
}

static TNODE * findBSTNode(BST *tree, void *key) {
  TNODE *temp = getBSTroot(tree);
  if (temp == 0 || sizeBST(tree) == 0) {
    return 0;
  }
  while (temp && tree->comparator(getTNODEvalue(temp), key) != 0) {
    if (tree->comparator(getTNODEvalue(temp), key) > 0) {
      temp = getTNODEleft(temp);
    }
    else {
      temp = getTNODEright(temp);
    }
  }
  return temp;
}

static TNODE * swapVals(TNODE *x, TNODE *y) {
  void *temp = getTNODEvalue(x);
  setTNODEvalue(x, getTNODEvalue(y));
  setTNODEvalue(y, temp);

  return y;
}

static TNODE *getPred(TNODE *n) {
  TNODE *temp = getTNODEleft(n);
  while (getTNODEright(temp)) {
    temp = getTNODEright(temp);
  }
  return temp;
}

static TNODE *getSucc(TNODE *n) {
  TNODE *temp = getTNODEright(n);
  while (getTNODEleft(temp)) temp = getTNODEleft(temp);
  return temp;
}
// looks for a node with at least one null child
static int findMinDepth(TNODE *n) {
  if (n == 0) {
    return 0;
  }
  /*if (getTNODEleft(n) == 0 && getTNODEright(n) == 0) {
    return 1;
  }
  if (getTNODEleft(n) == 0) {
    return findMinDepth(getTNODEright(n)) + 1;
  }
  if (getTNODEright(n) == 0) {
    return findMinDepth(getTNODEleft(n)) + 1;
  }

  return min(findMinDepth(getTNODEleft(n)), findMinDepth(getTNODEright(n))) + 1;*/

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
  if (n == 0) {
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

static void freeSubTree(TNODE *n) {
  if (n == NULL) {
    return;
  }
  TNODE *ptr = n;
  freeSubTree(getTNODEleft(ptr));
  freeSubTree(getTNODEright(ptr));
  freeTNODE(ptr);

  return;
}
/* debugLevel == 0 : prints a level-order traversal; each level on its own line
* each line starts with level number
* single space separates entries on a line
* root val tagged w/ 'X,' left child w/ 'L,' right child w/ 'R,' leaves w/ '='
* '=' should precede the node value. example:
* 0: 20(20)X
* 1: =7(20)L =33(20)R
* empty tree prints '0:' followed by newline
*/
static void displayLevel(BST *t, FILE *fp) {
  int level = 0;
  if (t == 0 || sizeBST(t) == 0) {
    fprintf(fp, "0:\n");
    return;
  }
  else {
    QUEUE *q = newQUEUE();
    setQUEUEdisplay(q, t->displayMethod);
    // If tree != null its first element is enqueued
    fprintf(fp, "%d:", level);
    if (getBSTroot(t) != 0) {
      enqueue(q, getBSTroot(t));
      enqueue(q, NULL);
    }
    while (sizeQUEUE(q) != 0) {
      // 1st elem in queue is dequeued and displayed
      TNODE *temp = (TNODE *)dequeue(q);
      if (temp == 0) {
        if (sizeQUEUE(q)) {
          fprintf(fp, "\n");
          fprintf(fp, "%d:", ++level);
          enqueue(q, NULL);
        }
      }
      else {
        fprintf(fp, " ");
        displayNode(t, temp, fp);
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
* @ any given node, method displays left and right subtrees, each enclosed by
* brackets, but only if they exist
* space is printed by displayBST to separate any existing subtrees/node values
* empty tree displayed as []
* example:
* [[7] 20 [33]]
* [20 [7] [33]]
* [[7] [33] 20]
*/
static void displayInOrder(BST * t, TNODE * n, FILE * fp) {
  if (t == 0 || sizeBST(t) == 0) { // empty tree
    fprintf(fp, "[]");
    return;
  }
  if (n == 0) {
    return;
  }
  TNODE * left = getTNODEleft(n);
  TNODE * right = getTNODEright(n);
  displayInOrder(t, left, fp); // recur left subtree first

  if (left != 0 && right != 0) { // left and right subtree exists
    fprintf(fp, "["); // outer bracket
    fprintf(fp, "[");
    t->displayMethod(getTNODEvalue(left), fp); // left subtree
    fprintf(fp, "] ");
    t->displayMethod(getTNODEvalue(n), fp); // curr node
    fprintf(fp, " [");
    t->displayMethod(getTNODEvalue(right), fp); // right subtree
    fprintf(fp, "]");
    fprintf(fp, "]"); // outer bracket
  }

  else if (left != 0 && right == 0) { // only left subtree exists
    fprintf(fp, "["); // outer bracket
    fprintf(fp, "[");
    t->displayMethod(getTNODEvalue(left), fp); // left subtree
    fprintf(fp, "] ");
    t->displayMethod(getTNODEvalue(n), fp); // curr node
    fprintf(fp, "]"); // outer bracket
  }

  else if (left == 0 && right != 0) { // only right subtree exists
    fprintf(fp, "["); // outer bracket
    t->displayMethod(getTNODEvalue(n), fp); // curr node
    fprintf(fp, "[");
    t->displayMethod(getTNODEvalue(right), fp); // right subtree
    fprintf(fp, "]");
    fprintf(fp, "]"); // outer bracket
  }

  else { // no left nor right subtree
    fprintf(fp, "["); // outer bracket
    t->displayMethod(getTNODEvalue(n), fp); // curr node
    fprintf(fp, "]"); // outer bracket
  }

  displayInOrder(t, right, fp); // then right subtree
}
/*
* @ any given node, method displays left and right subtrees, each enclosed by
* brackets, but only if they exist
* space is printed by displayBST to separate any existing subtrees/node values
* empty tree displayed as []
* example:
* [[7] 20 [33]]
* [20 [7] [33]]
* [[7] [33] 20]
*/
static void displayPreOrder(BST *t, TNODE *n, FILE *fp) {
  if (t == 0 || sizeBST(t) == 0) { // empty tree
    fprintf(fp, "[]\n");
    return;
  }
  if (n == 0) {
    return;
  }
  TNODE * left = getTNODEleft(n);
  TNODE * right = getTNODEright(n);

  if (left != 0 && right != 0) { // left and right subtree exists
    fprintf(fp, "["); // outer bracket
    fprintf(fp, "[");
    t->displayMethod(fp, getTNODEvalue(left)); // left subtree
    fprintf(fp, "] ");
    t->displayMethod(fp, getTNODEvalue(n)); // curr node
    fprintf(fp, " [");
    t->displayMethod(fp, getTNODEvalue(right)); // right subtree
    fprintf(fp, "]");
    fprintf(fp, "]"); // outer bracket
  }

  if (left != 0 && right == 0) { // only left subtree exists
    fprintf(fp, "["); // outer bracket
    fprintf(fp, "[");
    t->displayMethod(fp, getTNODEvalue(left)); // left subtree
    fprintf(fp, "] ");
    t->displayMethod(fp, getTNODEvalue(n)); // curr node
    fprintf(fp, "]"); // outer bracket
  }

  if (left == 0 && right != 0) { // only right subtree exists
    fprintf(fp, "["); // outer bracket
    t->displayMethod(fp, getTNODEvalue(n)); // curr node
    fprintf(fp, "[");
    t->displayMethod(fp, getTNODEvalue(right)); // right subtree
    fprintf(fp, "]");
    fprintf(fp, "]"); // outer bracket
  }

  else { // no left nor right subtree
    fprintf(fp, "["); // outer bracket
    t->displayMethod(fp, getTNODEvalue(n)); // curr node
    fprintf(fp, "]"); // outer bracket
  }
  displayPreOrder(t, left, fp); // recur left subtree first
  displayPreOrder(t, right, fp); // then right subtree
}
/*
* @ any given node, method displays left and right subtrees, each enclosed by
* brackets, but only if they exist
* space is printed by displayBST to separate any existing subtrees/node values
* empty tree displayed as []
* example:
* [[7] 20 [33]]
* [20 [7] [33]]
* [[7] [33] 20]
*/
static void displayPostOrder(BST *t, TNODE *n, FILE *fp) {
  if (t == 0 || sizeBST(t) == 0) { // empty tree
    fprintf(fp, "[]\n");
    return;
  }
  if (n == 0) {
    return;
  }
  TNODE * left = getTNODEleft(n);
  TNODE * right = getTNODEright(n);
  displayPostOrder(t, left, fp); // recur left subtree first
  displayPostOrder(t, right, fp); // then right subtree

  if (left != 0 && right != 0) { // left and right subtree exists
    fprintf(fp, "["); // outer bracket
    fprintf(fp, "[");
    t->displayMethod(fp, getTNODEvalue(left)); // left subtree
    fprintf(fp, "] ");
    t->displayMethod(fp, getTNODEvalue(n)); // curr node
    fprintf(fp, " [");
    t->displayMethod(fp, getTNODEvalue(right)); // right subtree
    fprintf(fp, "]");
    fprintf(fp, "]"); // outer bracket
  }

  if (left != 0 && right == 0) { // only left subtree exists
    fprintf(fp, "["); // outer bracket
    fprintf(fp, "[");
    t->displayMethod(fp, getTNODEvalue(left)); // left subtree
    fprintf(fp, "] ");
    t->displayMethod(fp, getTNODEvalue(n)); // curr node
    fprintf(fp, "]"); // outer bracket
  }

  if (left == 0 && right != 0) { // only right subtree exists
    fprintf(fp, "["); // outer bracket
    t->displayMethod(fp, getTNODEvalue(n)); // curr node
    fprintf(fp, "[");
    t->displayMethod(fp, getTNODEvalue(right)); // right subtree
    fprintf(fp, "]");
    fprintf(fp, "]"); // outer bracket
  }

  else { // no left nor right subtree
    fprintf(fp, "["); // outer bracket
    t->displayMethod(fp, getTNODEvalue(n)); // curr node
    fprintf(fp, "]"); // outer bracket
  }
}

static void displayNode(BST *t, TNODE *n, FILE *fp) {
  if (n == 0) { // error
    return;
  }
  if (isLeaf(n)) {
    fprintf(fp, "=");
  }
  t->displayMethod(getTNODEvalue(n), fp);

  fprintf(fp, "(");
  t->displayMethod(getTNODEvalue(getTNODEparent(n)), fp);
  fprintf(fp, ")");
  if (getBSTroot(t) == n) {
    fprintf(fp, "X");
  }
  if (isLeftChild(n)) {
    fprintf(fp, "L");
  }
  else if (isRightChild(n)) {
    fprintf(fp, "R");
  }
}

/*static int min(int x, int y) {
  if (x < y) {
    return x;
  }
  else { return y; }
}*/

static int isRightChild(TNODE *n) {
  if (getTNODEright(getTNODEparent(n)) == n) {
    return 1;
  }
  return 0;
}

static int isLeftChild(TNODE *n) {
  if (getTNODEleft(getTNODEparent(n)) == n) {
    return 1;
  }
  return 0;
}

static int isLeaf(TNODE *n) {
  if (getTNODEleft(n) == 0 && getTNODEright(n) == 0) {
    return 1;
  }
  return 0;
}

static int isRoot(TNODE *n) {
  if (getTNODEparent(n) == 0) {
    return 1;
  }
  return 0;
}
