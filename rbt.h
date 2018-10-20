/*** red-black binary search tree class ***/

#include <stdio.h>
#include "bst.h"

#ifndef __RBT_INCLUDED__
#define __RBT_INCLUDED__

typedef struct rbt
        {
        bst *tree;
        void (*display)(FILE *,void *);
        int (*compare)(void *,void *);
        int size; //redundant
        int words;
        } rbt;

extern rbt *newRBT(void (*)(FILE *,void *),int (*)(void *,void *));
extern void insertRBT(rbt *,void *);
extern int findRBT(rbt *,void *);
extern void deleteRBT(rbt *,void *);
extern int sizeRBT(rbt *);
extern int wordsRBT(rbt *);
extern void statisticsRBT(rbt *,FILE *);
extern void displayRBT(FILE *,rbt *);
extern void checkRBT(rbt *);               //optional

#endif
