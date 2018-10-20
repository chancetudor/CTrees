#include <stdio.h>
#include "bst.h"

#ifndef __VBST__INCLUDED__
#define __VBST__INCLUDED__

typedef struct vbst
{
    bst *tree;
    void (*display)(FILE *,void *);
    int (*compare)(void *,void *);
    int size; //redundant
    int words;
} vbst;

extern vbst *newVBST(void (*)(FILE *,void *),int (*)(void *,void *));
extern void insertVBST(vbst *,void *);
extern int findVBST(vbst *,void *);
extern void deleteVBST(vbst *,void *);
extern void statisticsVBST(vbst *,FILE *);
extern void displayVBST(FILE *,vbst *);
extern int sizeVBST(vbst *);
extern int wordsVBST(vbst *);
//extern void checkVBST(vbst *);               //optional

#endif // __VBST__INCLUDED__
