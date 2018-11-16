/* File: tnode.c
* implements functions in tnode.h for altering a tree node for either a
* BST, GST, or RBT
*/
#include <stdio.h>
#include <stdlib.h>
#include "tnode.h"

struct tnode {
  void * value;
  int debugVal;
  TNODE * left;
  TNODE * right;
  TNODE * parent;
  FM freeMethod;
  DM displayMethod;
};

extern TNODE * newTNODE(void * val, TNODE * l, TNODE * r, TNODE * p) {
  TNODE * node = malloc(sizeof(TNODE));
  if (node == NULL) {
    printf("Error: memory for node not allocated.\n");
  }
  node->value = val;
  //node->key = 0; // FIXME: need?
  //node->freq = 0; // FIXME: need?
  node->left = l;
  node->right = r;
  node->parent = p;
  node->freeMethod = 0;
  node->displayMethod = 0;
  node->debugVal = 0;

  return node;
}
// sets a node's display method
extern void setTNODEdisplay(TNODE * n, void (*d)(void *value, FILE *fp)) {
  n->displayMethod = d;
}
// sets a node's free method
extern void setTNODEfree(TNODE * n, void (*f)(void *value)) {
  n->freeMethod = f;
}
// gets the value of a node
extern void * getTNODEvalue(TNODE * n) {
  return n->value;
}
// sets a node's value
extern void setTNODEvalue(TNODE * n, void * replacement) {
  n->value = replacement;
}
// returns a node's left pointer
extern TNODE * getTNODEleft(TNODE * n) {
  return n->left;
}
// sets a node's left pointer
extern void setTNODEleft(TNODE * n, TNODE * replacement) {
  n->left = replacement;
}
// returns a node's right pointer
extern TNODE * getTNODEright(TNODE * n) {
  return n->right;
}
// sets a node's right pointer
extern void setTNODEright(TNODE * n, TNODE * replacement) {
  n->right = replacement;
}
// returns a node's parent
extern TNODE * getTNODEparent(TNODE * n) {
  return n->parent;
}
// sets a node's parent pointer
extern void setTNODEparent(TNODE * n, TNODE * replacement) {
  n->parent = replacement;
}
// If node's displayMethod == 0, address of the value is printed with a preceding ampersand
// if debug level > 0, method prints an ampersand and then
// address of the node immediately after displaying the value (with no intervening/following spaces)
extern void displayTNODE(TNODE * n, FILE * fp) {
  // case 1: displayMethod == 0 && debugVal == 0
  // case 2: displayMethod == 0 && debugVal > 0
  // case 3: displayMethod != 0 && debugVal == 0
  // case 4: displayMethod != 0 && debugVal > 0
  if (n->displayMethod == 0) {
    if (n->debugVal == 0) {
      // print "&valueAddress"
      fprintf(fp, "&%p\n", &n->value);
    }
    else {
      // print "&valueAddress&nodeAddress"
      fprintf(fp, "&%p&%p\n", &n->value, &n);
    }
  }
  else if (n->displayMethod != 0) {
    if (n->debugVal == 0) {
      // pass value to node's displayMethod
      n->displayMethod(n->value, fp);
    }
    else {
      // pass value to node's displayMethod
      // print "&nodeAddress"
      n->displayMethod(n->value, fp);
      fprintf(fp, "&%p\n", &n);
    }
  }
}
// method sets the node's new debugVal and
// returns prev. debugVal
extern int debugTNODE(TNODE * n, int level) {
  int prevVal = n->debugVal;
  n->debugVal = level;

  return prevVal;
}
// frees a node
extern void freeTNODE(TNODE * n) {
  if (n->freeMethod) {
    n->freeMethod(n->value);
  }
  free(n);
}
