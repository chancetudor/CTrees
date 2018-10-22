/*File: bst.c
 *Author: Chance Tudor
 *Implements functions found in bst.h in order to implement a binary search tree
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"
#include "bst.h"

static int isLeftChild(bstNode *);
static int isRoot(bstNode *);
static int isRightChild(bstNode *);
static int isLeaf(bstNode *);
static int findMinDepth(bstNode *);
static int findMaxDepth(bstNode *);
static void displayNode(FILE *, bstNode *, bst *);
static bstNode *newBSTNode(void *);
static void deleteNode(bstNode *n);

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
  if (tree->compare(getTNODEvalue(getTNODEparent(newNode)), value) == 1) {
    setTNODEleft(getTNODEparent(newNode), newNode)
  }
  else {
    setTNODEright(getTNODEparent(newNode), newNode)
  }

  return newNode;
}
// returns the value with the searched-for key
// if key is not in the tree, the method returns null
extern void * findBST(BST *t, void *key) {
  TNODE * temp = findBSTNode(tree, value);
  if () return getTNODEvalue(temp);
  return NULL;
}

extern bstNode *findBSTNode(bst *tree, void *value) {
  bstNode *temp = tree->root;
  while (temp && tree->compare(temp->value, value) != 0) {
    if (tree->compare(temp->value, value) > 0) temp = temp->left;
    else temp = temp->right;
  }
  return temp;
}



static bstNode *newBSTNode(void *value)
{
    bstNode *n = (bstNode *)malloc(sizeof(bstNode));
    if (n == 0)
    {
        fprintf(stderr,"Error: out of memory\n");
        exit(-1);
    }
    n->left = NULL;
    n->right = NULL;
    n->parent = n;
    n->value = value;
    return n;
}

extern int sizeBST(BST * t) {
  return t->size;
}

static void deleteNode(bstNode *n)
{
    free(n);
}

extern void pruneBSTNode(bst *tree, bstNode *n)
{
    tree->size--;
    if (isRoot(n))
    {
        tree->root = NULL;
    }
    else
    {
        if (isLeftChild(n)) n->parent->left = NULL;
        else n->parent->right = NULL;
    }
    deleteNode(n);
}

bstNode *swapValues(bstNode *x, bstNode *y) {
  void *temp = x->value;
  x->value = y->value;
  y->value = temp;

  return y;
}

bstNode *findPredecessor(bstNode *n)
{
    bstNode *temp = n->left;
    while (temp->right) temp = temp->right;
    return temp;
}

bstNode *findSuccessor(bstNode *n)
{
    bstNode *temp = n->right;
    while (temp->left) temp = temp->left;
    return temp;
}
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

static int findMinDepth(bstNode *n)
{
    if (n == NULL) return 0;

    int rdepth = findMinDepth(n->right);
    int ldepth = findMinDepth(n->left);

    if (rdepth < ldepth) return rdepth + 1;
    else return ldepth + 1;
}

static int findMaxDepth(bstNode *n)
{
    if (n == NULL) return 0;

    int rdepth = findMaxDepth(n->right);
    int ldepth = findMaxDepth(n->left);

    if (rdepth > ldepth) return rdepth + 1;
    else return ldepth + 1;
}

extern void statisticsBST(bst *tree,FILE *fp)
{
    fprintf(fp, "Nodes: %d\n", sizeBST(tree));
    fprintf(fp, "Minimum depth: %d\n", findMinDepth(tree->root));
    fprintf(fp, "Maximum depth: %d\n", findMaxDepth(tree->root));
}

extern void displayBST(FILE *fp,bst *t)
{
    int level = 0;
    queue *tree = newQueue(t->display);
    //If tree is not null its first element is enqueued onto the queue
    fprintf(fp, "%d:", level);
    if (t->root != NULL)
    {
        enqueue(tree, t->root);
        enqueue(tree, NULL);
    }
    while (sizeQueue(tree))
    {
        //The first element in the queue is dequeued and displayed
        bstNode *temp = dequeue(tree);
        if (temp == NULL)
        {
            if (sizeQueue(tree))
            {
                fprintf(fp, "\n");
                fprintf(fp, "%d:", ++level);
                enqueue(tree, NULL);
            }
        }
        else
        {
            fprintf(fp, " ");
            displayNode(fp, temp, t);
            //Its children, if they exist, are enqueued onto the queue, left child first then right child
            if (temp->left) enqueue(tree, temp->left);
            if (temp->right) enqueue(tree, temp->right);
        }
    }
    fprintf(fp, "\n");
}

static void displayNode(FILE *fp, bstNode *n, bst *tree)
{
    if (n == NULL) return; //error
    if (isLeaf(n)) fprintf(fp, "=");
    tree->display(fp, n->value);

    fprintf(fp, "(");
    tree->display(fp, n->parent->value);
    fprintf(fp, ")-");
    if (isLeftChild(n)) fprintf(fp, "l");
    else if (isRightChild(n)) fprintf(fp, "r");
}

static int isRightChild(bstNode *n)
{
    if (n->parent->right == n) return 1;
    return 0;
}

static int isLeftChild(bstNode *n) {
  if (n->parent->left == n) return 1;
  return 0;
}

static int isLeaf(bstNode *n) {
  if (!n->left && !n->right) return 1;
  return 0;
}

static int isRoot(bstNode *n) {
  if (n->parent == n) return 1;
  return 0;
}
