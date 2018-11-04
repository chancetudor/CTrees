/*** common binary search tree class ***/

#ifndef __BST_INCLUDED__
#define __BST_INCLUDED__

#include <stdio.h>
#include "tnode.h"

// stores a comparator function pointer in BST struct
typedef int (*CM)(void * one, void * two);
// stores a displayMethod function pointer in BST struct
//typedef void (*DM)(void * ptr, FILE *fp);
// stores a swapper function pointer in BST struct
typedef void (*SM)(TNODE * one, TNODE * two);
// stores a freeMethod function pointer in BST struct
//typedef void (*FM)(void * ptr);
typedef struct bst BST;
struct bst {
  TNODE * root;
  int size;
  CM comparator;
  DM displayMethod;
  SM swapper;
  FM freeMethod;
};

extern BST *newBST(int (*c)(void *,void *));
extern void   setBSTdisplay(BST *t, void (*d)(void *,FILE *));
extern void   setBSTswapper(BST *t, void (*s)(TNODE *,TNODE *));
extern void   setBSTfree(BST *t, void (*f)(void * ptr));
extern TNODE *getBSTroot(BST *t);
extern void   setBSTroot(BST *t, TNODE *replacement);
extern void   setBSTsize(BST *t, int s);
extern TNODE *insertBST(BST *t, void *value);
extern void  *findBST(BST *t, void *key);
extern TNODE *locateBST(BST *t, void *key);
extern int    deleteBST(BST *t, void *key);
extern TNODE *swapToLeafBST(BST *t, TNODE *node);
extern void   pruneLeafBST(BST *t, TNODE *leaf);
extern int    sizeBST(BST *t);
extern void   statisticsBST(BST *t, FILE *fp);
extern void   displayBST(BST *t, FILE *fp);
extern int    debugBST(BST *t, int level);
extern void   freeBST(BST *t);

#endif /* bst.h */
