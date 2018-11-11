/*File: RBT.c
 *Author: Chance Tudor
 *Implements functions found in RBT.h in order to implement a red black tree
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "gst.h"
#include "rbt.h"
#include "tnode.h"

typedef struct rbtval RBTVAL;

struct rbt {
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
  SM swap;
  FM freeMethod;
};

static RBTVAL *newRBTVAL(RBT *t, void *v);

static TNODE *grandparent(TNODE *n);
static TNODE *parent(TNODE *n);
static TNODE *uncle(TNODE *n);
static TNODE *niece(TNODE *n);
static TNODE *nephew(TNODE *n);
static TNODE *sibling(TNODE *n);
static TNODE *findRBTNode(RBT *t, void *key);

static int compareRBTVAL(void *a, void *b);
static int isBlack(TNODE *n);
static int isRed(TNODE *n);
static int getColor(RBTVAL *v);
static int linearWithParent(TNODE *n);
static int isLeftChild(TNODE *n);
//static int isRoot(TNODE *n);
static int isRightChild(TNODE *n);
static int getRBTitems(RBT *t);

static void displayRBTVAL(RBTVAL *v, FILE *fp);
static void insertionFixUp(GST *tree, TNODE * n);
static void deletionFixUp(GST * tree, TNODE * n);
static void swapVals(TNODE *a, TNODE *b);
static void freeRBTVAL(RBTVAL *v);
static void colorBlack(TNODE *n);
static void colorRed(TNODE *n);
static void setColor(RBTVAL *v, int c);
static void rightRotate(GST *tree, TNODE *n);
static void leftRotate(GST *tree, TNODE * n);
static void setRBTFreq(RBTVAL *v, int f);
static void setRBTitems(RBT *t, int i);
////////////////////////////////////////////////////////////////////////////////
extern RBT * newRBT(int (*c)(void * x, void * y)) {
  RBT * t = malloc(sizeof(RBT));
  assert(t != NULL);
  t->tree = newGST((void *)compareRBTVAL);
  t->display = 0;
  t->compare = c;
  t->freeMethod = 0;
  t->swap = (void *)swapVals;
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
  v->swap = t->swap;
  v->freeMethod = t->freeMethod;

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
  colorRed(temp);
  insertionFixUp(tree, temp);
  return temp;
}

extern void * findRBT(RBT *t, void *value) {
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

extern int deleteRBT(RBT *t, void *key) { // FIXME
  setRBTswapper(t, (void *)swapVals);
  int freq = freqRBT(t, key);
  GST * tree = t->tree;
  if (freq == 0) {
    return -1;
  }
  else if (freq > 1) {
    RBTVAL * v = newRBTVAL(t, key);
    --freq;
    setRBTFreq(v, freq);
    return deleteGST(tree, v);
  }
  RBTVAL * newVal = newRBTVAL(t, key); // FIXME: Free
  TNODE * node = findRBTNode(t, newVal);
  node = swapToLeafRBT(t, node);
  deletionFixUp(tree, node);
  pruneLeafGST(tree, node);
  setRBTitems(t, getRBTitems(t) - 1);
  free(node);
  return 0;
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
  int duplicates = getRBTitems(t) - sizeGST(tree);
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
  RBTVAL *rVal = (RBTVAL *)getTNODEvalue(n);
  return rVal->value;
}

extern int freqRBT(RBT *g, void *key) {
  RBTVAL * newVal = newRBTVAL(g, key);
  TNODE * n = findRBTNode(g, newVal);
  if (n != 0) {
    RBTVAL * v = (RBTVAL *)getTNODEvalue(n);
    return v->freq;
  }
  return 0;
}

extern int duplicatesRBT(RBT *g) {
  GST * tree = g->tree;
  return getRBTitems(g) - sizeGST(tree);
}
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
  RBTVAL *x = unwrapGST(a);
  RBTVAL *y = unwrapGST(b);
  int color = getColor(x);
  //x->color = y->color;
  setColor(x, getColor(y));
  //y->color = color;
  setColor(y, color);
}

static TNODE * findRBTNode(RBT *t, void *key) {
  GST * tree = t->tree;
  TNODE * temp = getGSTroot(tree);
  if (temp == 0 || sizeGST(tree) == 0) {
    return 0;
  }
  while (temp && compareRBTVAL(unwrapGST(temp), key) != 0) {
    if (compareRBTVAL(unwrapGST(temp), key) > 0) {
      temp = getTNODEleft(temp);
    }
    else {
      temp = getTNODEright(temp);
    }
  }
  return temp;
}

static void displayRBTVAL(RBTVAL *v, FILE *fp) {
  void * val = v->value;
  v->display(val, fp);
  if (v->freq > 1) {
    fprintf(fp, "<%d>", v->freq);
  }
  if (getColor(v) == 0) {
    fprintf(fp, "*"); // color is red
  }
}

static int compareRBTVAL(void *x, void *y) {
  RBTVAL *a = x;
  RBTVAL *b = y;
  return a->compare(a->value, b->value);
}

static void freeRBTVAL(RBTVAL *v) {
  v->freeMethod(v->value);
  free(v);
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

static void insertionFixUp(GST *tree, TNODE * n) {
  while (1) {
    if (getGSTroot(tree) == n) {
      break;
    }
    if (isBlack(parent(n))) {
      break;
    }
    if (isRed(uncle(n))) {
      colorBlack(parent(n));
      colorBlack(uncle(n));
      colorRed(grandparent(n));
      n = grandparent(n);
    }
    else {
      if (!linearWithParent(n)) {
        if (isRightChild(n)) {
          leftRotate(tree, n);
          //n = n->left;
          n = getTNODEleft(n);
        }
        else {
          rightRotate(tree, n);
          //n = n->right;
          n = getTNODEright(n);
        }
      }
      colorBlack(parent(n));
      colorRed(grandparent(n));
      if (isLeftChild(parent(n))) {
        rightRotate(tree, parent(n));
      }
      else {
        leftRotate(tree, parent(n));
      }
      break;
    }
  }
  colorBlack(getGSTroot(tree));
}
// FIXME: Segfaulting
static void deletionFixUp(GST * tree, TNODE * n) {
  while (1) {
    if (getGSTroot(tree) == n) {
      break;
    }
    if (isRed(n)) {
      break;
    }
    if (isRed(sibling(n))) {
      colorRed(parent(n)); // color parent red
      colorBlack(sibling(n)); // color sibling black
      // rotate sibling to parent
      if (isLeftChild(sibling(n))) {
        //rightRotate(tree, parent(n));
        rightRotate(tree, sibling(n));
      }
      else {
        //leftRotate(tree, parent(n));
        leftRotate(tree, sibling(n));
      }
    }
    else if (isRed(nephew(n))) {
      int color = getColor(unwrapGST(parent(n)));
      setColor(unwrapGST(sibling(n)), color);
      colorBlack(parent(n));
      colorBlack(nephew(n));
      // rotate sibling to parent
      if (isLeftChild(sibling(n))) {
        //rightRotate(tree, parent(n));
        rightRotate(tree, sibling(n));
      }
      else {
        //leftRotate(tree, parent(n));
        leftRotate(tree, sibling(n));
      }
      break;
    }
    else if (isRed(niece(n))) {
      colorBlack(niece(n));
      colorRed(sibling(n));
      // rotate niece to sibling
      if (isLeftChild(niece(n))) {
        //rightRotate(tree, sibling(n));
        rightRotate(tree, niece(n));
      }
      else {
        //leftRotate(tree, sibling(n));
        leftRotate(tree, niece(n));
      }
    }
    else { // sibling, niece, and nephew must be black
      colorRed(sibling(n));
      setTNODEparent(n, parent(n));
    }
  }
  colorBlack(n);
  if (!isBlack(getGSTroot(tree))) {
    colorBlack(getGSTroot(tree));
  }
}

static int getColor(RBTVAL *v) {
  return v->color;
}

static void setColor(RBTVAL *v, int c) {
  v->color = c;
}

static int isBlack(TNODE *n) {
  if (n) {
    RBTVAL *v = unwrapGST(n);
    if (getColor(v) == 1) {
      return 1;
    }
  }
  return 0;
}

static int isRed(TNODE *n) {
  if (n) {
    RBTVAL *v = unwrapGST(n);
    if (getColor(v) == 0) {
      return 1;
    }
  }
  return 0;
}

static void colorBlack(TNODE *n) {
  //RBTVAL *v = getTNODEvalue(n);
  RBTVAL *v = unwrapGST(n);
  setColor(v, 1);
}

static void colorRed(TNODE *n) {
  //RBTVAL *v = getTNODEvalue(n);
  RBTVAL *v = unwrapGST(n);
  setColor(v, 0);
}

static void rightRotate(GST *tree, TNODE * n) {
  TNODE *oldParent = parent(n);
  if (getGSTroot(tree) == oldParent) {
    setGSTroot(tree, n);
  }
  else if (isLeftChild(oldParent)) {
    //grandparent(n)->left = n;
    setTNODEleft(grandparent(n), n);
  }
  else {
    //grandparent(n)->right = n;
    setTNODEright(grandparent(n), n);
  }
  if (getGSTroot(tree) == oldParent) {
    //n->parent = n;
    setTNODEparent(n, n);
  }
  else {
    //n->parent = grandparent(n);
    setTNODEparent(n, grandparent(n));
  }
  //oldParent->left = n->right;
  setTNODEleft(oldParent, getTNODEright(n));
  if (getTNODEleft(oldParent)) {
    //oldParent->left->parent = oldParent;
    setTNODEparent(getTNODEleft(oldParent), oldParent);
  }
  //n->right = oldParent;
  setTNODEright(n, oldParent);
  setTNODEparent(oldParent, n);
  //oldParent->parent = n;
}

static void leftRotate(GST *tree, TNODE *n) {
  TNODE *oldParent = parent(n);
  if (getGSTroot(tree) == oldParent) {
    setGSTroot(tree, n);
  }
  else if (isLeftChild(oldParent)) {
    setTNODEleft(grandparent(n), n);
  }
  else {
    setTNODEright(grandparent(n), n);
  }
  if (getGSTroot(tree) == oldParent) {
    setTNODEparent(n, n);
  }
  else {
    setTNODEparent(n, grandparent(n));
  }
  setTNODEright(oldParent, getTNODEleft(n));
  if (getTNODEright(oldParent)) {
    setTNODEparent(getTNODEright(oldParent), oldParent);
  }
  setTNODEleft(n, oldParent);
  setTNODEparent(oldParent, n);
}

static int isLeftChild(TNODE *n) {
  if (getTNODEleft(parent(n)) == n) {
    return 1;
  }
  return 0;
}

static int isRightChild(TNODE *n) {
  if (getTNODEright(parent(n)) == n) {
    return 1;
  }
  return 0;
}

static TNODE *grandparent(TNODE *n) {
  return parent(parent(n));
}

static TNODE *parent(TNODE *n) {
  return getTNODEparent(n);
}

static TNODE *uncle(TNODE *n) {
  if (isLeftChild(parent(n))) {
    return getTNODEright(grandparent(n));
  }
  else {
    return getTNODEleft(grandparent(n));
  }
}

static TNODE *niece(TNODE *n) {
  if (isLeftChild(n)) {
    return getTNODEleft(sibling(n));
  }
  else {
    return getTNODEright(sibling(n));
  }
}

static TNODE *nephew(TNODE *n) {
  
  if (isLeftChild(n)) {
    return getTNODEright(sibling(n));
  }
  else {
    return getTNODEleft(sibling(n));
  }
}

static TNODE *sibling(TNODE * n) {
  if (parent(n) == 0) {
    return 0;
  }
  if (isLeftChild(n)) {
    return getTNODEright(parent(n));
  }
  else {
    return getTNODEleft(parent(n));
  }
}

static int linearWithParent(TNODE *n) {
  if (isRightChild(n) && isRightChild(parent(n))) {
    return 1;
  }
  else if (isLeftChild(n) && isLeftChild(parent(n))) {
    return 1;
  }

  return 0;
}

/*static int isRoot(TNODE *n) {
  if (parent(n) == n) {
    return 1;
  }
  return 0;
}*/
