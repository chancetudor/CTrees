/*File: RBT.c
 *Author: Chance Tudor
 *Implements functions found in RBT.h in order to implement a red black tree
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "gst.h"
#include "RBT.h"
#include "tnode.h"

typedef struct rbtval RBTVAL;

struct RBT {
  GST * tree;
  int items;
  DM display;
  CM compare;
  FM freeMethod;
  SM swap;
};

struct rbtval {
  void *value;
  int freq;
  int color;
  DM display;
  CM compare;
};

static RBTVAL *newRBTVAL(void *v, RBT *t);

static TNODE *grandparent(TNODE *n);
static TNODE *parent(TNODE *n);
static TNODE *uncle(TNODE *n);
static TNODE * findRBTNode(RBT *t, void *key);

static int compareRBTVAL(void *a, void *b);
static int isBlack(TNODE *n);
static int isRed(TNODE *n);
static int linearWithParent(TNODE *n);
static int isLeftChild(TNODE *n);
static int isRoot(TNODE *n);
static int isRightChild(TNODE *n);
static int getRBTitems(RBT *t);

static void displayRBTVAL(FILE *fp, void *n);
static void insertionFixUp(TNODE *n, BST *tree);
static void swapVals(TNODE *a, TNODE *b);
static void freeRBTVAL(RBTVAL *v);
static void colorBlack(TNODE *n);
static void colorRed(TNODE *n);
static void rightRotate(TNODE *n, BST *tree);
static void leftRotate(TNODE *n, BST *tree);
static void setRBTFreq(RBTVAL *v, int f);
static void setRBTitems(RBT *t, int i);
////////////////////////////////////////////////////////////////////////////////
extern RBT * newRBT(int (*c)(void * x, void * y)) {
  RBT * t = malloc(sizeof(RBT));
  assert(t != NULL);
  t->tree = newGST(compareRBTVAL);
  t->display = 0;
  t->compare = c;
  t->freeMethod = 0;
  t->swap = (void *)swapVals;
  t->size = 0;
  t->items = 0;

  return t;
}

static RBTVAL *newRBTVAL(RBT *t, void * val) {
  RBTVAL *v = malloc(sizeof(RBTVAL));
  assert(v != NULL);
  v->color = 0; // set color to red
  v->freq = 0;
  v->value = val;
  v->display = t->display;
  v->compare = t->compare;

  return v;
}

extern void setRBTdisplay(RBT *t, void (*d)(void * ptr, FILE * fp)) {
  t->display = d;
  GST * tree = t->tree;
  setGSTdisplay(tree, (void *)displayRBTVAL);
}

extern void setRBTswapper(RBT *t, void (*s)(TNODE *x, TNODE *y)) {
  t->swap = s;
  GST * tree = t->tree;
  setGSTswapper(tree, s);
}

extern void setRBTfree(RBT *t, void (*f)(void * ptr)) {
  t->freeMethod = f;
  GST * tree = t->tree;
  setGSTfree(tree, (void *)freeRBTVAL);
}

extern TNODE * getRBTroot(RBT *t) {
  GST * tree = t->tree;
  return getGSTroot(tree);
}

extern void setRBTroot(RBT *t, TNODE *replacement) {
  GST * tree = t->tree;
  setGSTroot(tree, replacement);
}

extern void setRBTsize(RBT *t, int s) {
  GST * tree = t->tree;
  setGSTsize(tree, s);
}

extern TNODE * insertRBT(RBT *t, void *value) {
  RBTVAL * newVal = newRBTVAL(t, value);
  TNODE * temp = findRBTNode(t, newVal);
  if (temp != 0) {
    newVal = (RBTVAL *)getTNODEvalue(temp);
    setRBTFreq(newVal, newVal->freq + 1);
    if (t->freeMethod != 0) {
      t->freeMethod(value);
    }
    return 0;
  }
  setRBTitems(t, getRBTitems(t) + 1);
  setRBTFreq(newVal, newVal->freq + 1);
  GST * tree = t->tree;
  temp = insertGST(tree, newVal);
  insertionFixUp(temp, tree);
  return temp;
}

extern int findRBT(RBT *t, void *value) {
  RBTVAL * newVal = newRBTVAL(t, value);
  TNODE * n = findRBTNode(t, newVal);
  if (n == 0) {
    return 0;
  }
  void * val = unwrapRBT(n);
  free(newVal);
  return val;
}

extern TNODE *locateRBT(RBT *t, void *key) {
  GST * tree = t->tree;
  return locateGST(tree, key);
}

extern void deleteRBT(RBT *t, void *value) { // FIXME
  RBTVAL *v = newRBTVAL(value, t);
  RBTVAL *rv = t->tree->root->value;
  fprintf(stderr, "Error: cannot delete value ");
  v->display(stderr, v->value);
  fprintf(stderr, " from tree rooted at ");
  rv->display(stderr, rv->value);
  fprintf(stderr, "\n");
}

extern TNODE *swapToLeafRBT(RBT *t, TNODE *node) {
  GST * tree = t->tree;
  return swapToLeafGST(tree, node);
}

extern void pruneLeafRBT(RBT *t, TNODE *leaf) {
  GST * tree = t->tree;
  pruneLeafGST(tree, leaf);
}

extern int sizeRBT(RBT *t) {
  GST * tree = t->tree;
  return sizeGST(tree);
}

extern void statisticsRBT(RBT *t, FILE *fp) {
  GST * tree = t->tree;
  int duplicates = getGSTitems(t) - sizeGST(tree);
  fprintf(fp, "Duplicates: %d\n", duplicates);
  statisticsGST(tree, fp);
}

extern void displayRBT(RBT *t, FILE *fp) {
  GST * tree = t->tree;
  displayGST(tree, fp);
}

extern int debugRBT(RBT *t, int level) {
  GST * tree = t->tree;
  return debugGST(tree, level);
}

extern void freeRBT(RBT *t) {
  GST * tree = t->tree;
  freeGST(tree);
}

extern void * unwrapRBT(TNODE *n) {
  GSTVAL *v = (GSTVAL *)getTNODEvalue(n);
  RBTVAL * rVal = (RBTVAL *)unwrapGST(v);
  return rVal->val;
}

extern int freqRBT(RBT *g, void *key);

extern int duplicatesRBT(RBT *g);
////////////////////////////////////////////////////////////////////////////////
static void swapVals(TNODE *a, TNODE *b) {
  void *va = getTNODEvalue(a); // get the GST value
  void *vb = getTNODEvalue(b); // get the GST value
  // swap the GST values
  setTNODEvalue(a, vb);
  setTNODEvalue(b, va);
  // the above swap swapped the colors,
  // but the colors should stay with the nodes,
  // so swap the colors back to the original nodes
  RBTVALUE *x = unwrapGST(a);
  RBTVALUE *y = unwrapGST(b);
  int color = x->color;
  x->color = y->color;
  y->color = color;
}

static TNODE * findRBTNode(RBT *t, void *key) {
  GST * tree = t->tree;
  TNODE * temp = getGSTroot(tree);
  if (temp == 0 || sizeGST(tree) == 0) {
    return 0;
  }
  while (temp && compareRBTVAL(getTNODEvalue(temp), key) != 0) {
    if (compareRBTVAL(getTNODEvalue(temp), key) > 0) {
      temp = getTNODEleft(temp);
    }
    else {
      temp = getTNODEright(temp);
    }
  }
  return temp;
}

static void displayRBTVAL(void *n, FILE *fp) {
  RBTVAL *v = n;
  v->display(fp, v->value);
  if (v->freq > 1) fprintf(fp, "-%d", v->freq);
  if (v->color == 1) fprintf(fp, "-B");
  else fprintf(fp, "-R");
}

static int compareRBTVAL(void *x, void *y) {
  RBTVAL *a = x;
  RBTVAL *b = y;
  return a->compare(a->value, b->value);
}

static void setRBTFreq(RBTVAL * v, int f) {
  v->freq = f;
}

static void setRBTitems(RBT *t, int i) {
  t->items = i;
}

static int getRBTitems(RBT *t) {
  return t->items;
}

static void insertionFixUp(TNODE *n, BST *tree)
{
    while (1)
    {
        if (isRoot(n)) break;
        if (isBlack(parent(n))) return;
        if (isRed(uncle(n)))
        {
            colorBlack(parent(n));
            colorBlack(uncle(n));
            colorRed(grandparent(n));
            n = grandparent(n);
        }
        else
        {
            if (!linearWithParent(n))
            {
                if (isRightChild(n))
                {
                    leftRotate(n, tree);
                    n = n->left;
                }
                else
                {
                    rightRotate(n, tree);
                    n = n->right;
                }
            }
            colorBlack(parent(n));
            colorRed(grandparent(n));
            if (isLeftChild(n->parent))
            {
                rightRotate(n->parent, tree);
            }
            else
            {
                leftRotate(n->parent, tree);
            }
            break;
        }
    }
    if (isRed(tree->root)) colorBlack(tree->root);
}

static int isBlack(TNODE *n)
{
    if (n)
    {
        RBTVAL *v = n->value;
        return v->color;
    }
    return 1;
}

static int isRed(TNODE *n)
{
    if (n)
    {
        RBTVAL *v = n->value;
        if (v->color == 0) return 1;
    }
    return 0;
}

static void colorBlack(TNODE *n)
{
    RBTVAL *v = n->value;
    v->color = 1;
}

static void colorRed(TNODE *n)
{
    RBTVAL *v = n->value;
    v->color = 0;
}

static void rightRotate(TNODE *n, BST *tree)
{
    TNODE *oldParent = parent(n);
    if (isRoot(oldParent))
    {
        tree->root = n;
    }
    else if (isLeftChild(oldParent))
    {
        grandparent(n)->left = n;
    }
    else
    {
        grandparent(n)->right = n;
    }
    if (isRoot(oldParent)) n->parent = n;
    else n->parent = grandparent(n);
    oldParent->left = n->right;
    if (oldParent->left)
    {
        oldParent->left->parent = oldParent;
    }
    n->right = oldParent;
    oldParent->parent = n;
}

static void leftRotate(TNODE *n, BST *tree)
{
    TNODE *oldParent = n->parent;
    if (isRoot(oldParent))
    {
        tree->root = n;
    }
    else if (isLeftChild(oldParent))
    {
        grandparent(n)->left = n;
    }
    else
    {
        grandparent(n)->right = n;
    }
    if (isRoot(oldParent)) n->parent = n;
    else n->parent = grandparent(n);
    oldParent->right = n->left;
    if (oldParent->right)
    {
        oldParent->right->parent = oldParent;
    }
    n->left = oldParent;
    oldParent->parent = n;
}

static int isLeftChild(TNODE *n)
{
    if (n->parent->left == n) return 1;
    return 0;
}

static int isRightChild(TNODE *n)
{
    if (n->parent->right == n) return 1;
    return 0;
}

static TNODE *grandparent(TNODE *n)
{
    return n->parent->parent;
}
static TNODE *parent(TNODE *n)
{
    return n->parent;
}
static TNODE *uncle(TNODE *n)
{
    if (isLeftChild(n->parent)) return grandparent(n)->right;
    else return grandparent(n)->left;
}

static int linearWithParent(TNODE *n)
{
    if ((isRightChild(n) && isRightChild(parent(n))) || (isLeftChild(n) && isLeftChild(parent(n)))) return 1;
    else return 0;
}

static int isRoot(TNODE *n)
{
    if (n->parent == n) return 1;
    return 0;
}
