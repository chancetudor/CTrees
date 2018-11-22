/*File: gst.c
 *Author: Chance Tudor
 *Implements functions found in gst.h in order to implement a green tree
 */
#include <stdio.h>
#include <stdlib.h>
#include "gst.h"
#include "bst.h"
#include "tnode.h"
// stores a comparator function pointer in GST struct
typedef int (*CM)(void * one, void * two);
// stores a displayMethod function pointer in GST struct
typedef void (*DM)(void * ptr, FILE *fp);
// stores a swapper function pointer in GST struct
typedef void (*SM)(TNODE * one, TNODE * two);
// stores a freeMethod function pointer in GST struct
typedef void (*FM)(void * ptr);

typedef struct gstval GSTVAL;

struct gst {
  BST * tree;
  int duplicates;
  DM display;
  CM compare;
  FM freeMethod;
  SM swap;
};

struct gstval {
  void * val;
  int freq;
  DM display;
  CM compare;
  FM freeMethod;
};

static TNODE * findGSTNode(GST *t, void *key);

static int getGSTduplicates(GST *t);
static int compareGVAL(void * x, void * y);

static void displayGVAL(GSTVAL *v, FILE *fp);
static void freeGVAL(GSTVAL *v);
static void setGSTduplicates(GST *t, int i);
static void setGSTFreq(GSTVAL * v, int f);
static void swapVals(TNODE *x, TNODE *y);
////////////////////////////////////////////////////////////////////////////////
// creates new GST object and sets default comparator function
extern GST * newGST(int (*c)(void * one, void * two)) {
  GST * gTree = malloc(sizeof(GST));
  gTree->tree = newBST((void *)compareGVAL);
  gTree->duplicates = 0;
  gTree->display = 0;
  gTree->compare = c;
  gTree->freeMethod = 0;
  gTree->swap = (void *)swapVals;

  return gTree;
}
static GSTVAL * newGSTVAL(GST *t, void *v) {
  GSTVAL * value = malloc(sizeof(GSTVAL));
  value->val = v;
  value->freq = 1;
  value->display = t->display;
  value->compare = t->compare;
  value->freeMethod = t->freeMethod;

  return value;
}
// sets underlying BST diplay function
extern void setGSTdisplay(GST *t, void (*d)(void * ptr, FILE *fp)) {
  t->display = d;
  BST * tree = t->tree;
  setBSTdisplay(tree, (void *)displayGVAL);
}
// sets underlying BST swapper function
extern void setGSTswapper(GST *t, void (*s)(TNODE * one, TNODE * two)) {
  t->swap = s;
  BST * tree = t->tree;
  setBSTswapper(tree, s);
}
// sets underlying BST free function
extern void setGSTfree(GST *t, void (*f)(void * ptr)) {
  t->freeMethod = f;
  BST * tree = t->tree;
  setBSTfree(tree, (void *)freeGVAL);
}
// returns root of GST
extern TNODE *getGSTroot(GST *t) {
  BST * tree = t->tree;
  return getBSTroot(tree);
}
// sets root of GST
extern void setGSTroot(GST *t, TNODE *replacement) {
  BST * tree = t->tree;
  setTNODEparent(replacement, replacement);
  setBSTroot(tree, replacement);
}
// sets size of GST
extern void setGSTsize(GST *t, int s) {
  BST * tree = t->tree;
  setBSTsize(tree, s);
}
// attempts to inserts a generic value in the tree.
// If value is already in tree, freq count of value++,
// value passed is perhaps freed & method returns null
// If value is not in tree, it's inserted and method returns new tree node
extern TNODE *insertGST(GST *t, void *value) {
  GSTVAL * newVal = newGSTVAL(t, value); // FIXME: Free
  TNODE * temp = findGSTNode(t, newVal);
  if (temp) {
    freeGVAL(newVal);
    GSTVAL * ptr = (GSTVAL *)getTNODEvalue(temp);
    setGSTFreq(ptr, ptr->freq + 1);
    setGSTduplicates(t, getGSTduplicates(t) + 1);
    return 0;
  }
  BST * tree = t->tree;
  return insertBST(tree, newVal);
}
// returns the value stored with the given key
// returns null if the key is not in the tree.
extern void * findGST(GST *t, void *key) {
  GSTVAL * v = newGSTVAL(t, key);
  TNODE * temp = findGSTNode(t, v);
  if (temp == 0) {
    free(v);
    return 0;
  }
  void * val = unwrapGST(temp);
  free(v);
  return val;
}
// returns the tree node holding the searched-for key.
// If the key is not in the tree, the method returns null.
extern TNODE *locateGST(GST *t, void *key) {
  GSTVAL * v = newGSTVAL(t, key);
  BST * tree = t->tree;
  TNODE * ptr = locateBST(tree, v);
  free(v);
  return ptr;
}
/*
* finds the generic value stored in the tree that matches the given value.
* If value is not in the tree, -1 is returned.
* Otherwise the resulting frequency is returned.
* If freq count of the stored value > one, method reduces freq
* If freq count == 1, GST value is removed from tree (i.e. zero is returned).
*/
extern int deleteGST(GST *t, void *key) {
  int freq = freqGST(t, key); // var to hold freq count of GSTVAL
  BST * tree = t->tree;
  if (freq == 0) { // GSTVAL is not in tree
    return -1;
  }
  else if (freq > 1) { // multiple GSTVALs w/ same value in tree
    GSTVAL * newVal = newGSTVAL(t, key);
    TNODE * node = findGSTNode(t, newVal);
    GSTVAL * ptr = (GSTVAL *)getTNODEvalue(node);
    --freq;
    setGSTFreq(ptr, freq);
    setGSTduplicates(t, getGSTduplicates(t) - 1);
    free(newVal);
    return freq;
  }
  GSTVAL * v = getTNODEvalue(locateGST(t, key));
  int result = deleteBST(tree, v);
  free(v);
  return result;
}
/* recursively swaps a node's value with its predecessor's (preferred)
* or its successor's until a leaf node holds the original value.
* calls the BST’s swapper function to actually accomplish the swap,
* sending the two nodes whose values need to be swapped.
* The method returns the leaf node holding the swapped value.
*/
extern TNODE *swapToLeafGST(GST *t, TNODE *node) {
  BST * tree = t->tree;
  return swapToLeafBST(tree, node);
}
/* method detaches given node from the tree
* does not free the node nor decrement size
*/
extern void pruneLeafGST(GST *t, TNODE *leaf) {
  BST * tree = t->tree;
  pruneLeafBST(tree, leaf);
  free(getTNODEvalue(leaf));
}
// returns number of nodes currently in tree
extern int sizeGST(GST *t) {
  BST * tree = t->tree;
  return sizeBST(tree);
}
// displays number of duplicates
// == # GST duplicates - # nodes in underlying BST
// Then calls BST statistics method
extern void statisticsGST(GST *t, FILE *fp) {
  BST * tree = t->tree;
  fprintf(fp, "Duplicates: %d\n", duplicatesGST(t));
  statisticsBST(tree, fp);
}
// calls the tree using a level-order traversal, via display method of BST
extern void displayGST(GST *t, FILE *fp) {
  BST * tree = t->tree;
  displayBST(tree, fp);
}
// sets the debug level of the underlying data structure
extern int debugGST(GST *t, int level) {
  BST * tree = t->tree;
  return debugBST(tree, level);
}
extern void freeGST(GST *t) {
  BST * tree = t->tree;
  freeBST(tree);
  free(t);
}
// extracts the GST value from a node
// From that extracted value, returns generic value wrapped by GST value
extern void * unwrapGST(TNODE *n) {
  GSTVAL * v = (GSTVAL *)getTNODEvalue(n);
  return v->val;
}
// returns the frequency of the searched-for key.
// If key is not in tree, method returns zero
extern int freqGST(GST *g, void *key) {
  GSTVAL * v = newGSTVAL(g, key); // FIXME: free
  TNODE * n = findGSTNode(g, v);
  if (n) {
    GSTVAL * ptr = (GSTVAL *)getTNODEvalue(n);
    free(v);
    return ptr->freq;
  }
  free(v);
  return 0;
}
// returns the number of duplicate values currently in the tree
// == # GST duplicates - # nodes in underlying BST
extern int duplicatesGST(GST *g) {
  return getGSTduplicates(g);
}
////////////////////////////////////////////////////////////////////////////////
static void swapVals(TNODE *x, TNODE *y) {
  void *one = getTNODEvalue(x);
  void *two = getTNODEvalue(y);
  setTNODEvalue(x, two);
  setTNODEvalue(y, one);
}

static TNODE * findGSTNode(GST *t, void *key) {
  BST * tree = t->tree;
  TNODE *temp = getBSTroot(tree);
  if (temp == 0 || sizeBST(tree) == 0) {
    return 0;
  }
  while (temp && compareGVAL((GSTVAL *)getTNODEvalue(temp), key) != 0) {
    if (compareGVAL((GSTVAL *)getTNODEvalue(temp), key) > 0) {
      temp = getTNODEleft(temp);
    }
    else {
      temp = getTNODEright(temp);
    }
  }
  return temp;
}

static void displayGVAL(GSTVAL *v, FILE *fp) {
  void *val = v->val;
  v->display(val, fp);
  if (v->freq > 1) {
    fprintf(fp, "<%d>", v->freq);
  }
}

static int compareGVAL(void *x, void *y) {
  GSTVAL *a = (GSTVAL *)x;
  GSTVAL *b = (GSTVAL *)y;
  return a->compare(a->val, b->val);
}

static void freeGVAL(GSTVAL *v) {
  if (v->freeMethod) {
    v->freeMethod(v->val);
  }
  free(v);
}

static int getGSTduplicates(GST * t) {
  return t->duplicates;
}

static void setGSTduplicates(GST * t, int i) {
  t->duplicates = i;
}

static void setGSTFreq(GSTVAL *v, int f) {
  v->freq = f;
}
