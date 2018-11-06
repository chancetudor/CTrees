#include <stdio.h>
#include <stdlib.h>
#include "bst.h"
#include "integer.h"
#include "real.h"
#include "string.h"
#include "queue.h"

void srandom(unsigned int);
long int random(void);

int
main(void)
    {
    srandom(14);
    //simple STRING test of BST
    BST *p = newBST(compareSTRING);
    setBSTdisplay(p,displaySTRING);
    setBSTfree(p,freeSTRING);
    insertBST(p,newSTRING("a3"));
    insertBST(p,newSTRING("a2"));
    debugBST(p,1);
    printf("debug (in-order): ");
    displayBST(p,stdout);
    printf("\n");
    debugBST(p,0);
    printf("BST:\n");
    displayBST(p,stdout);
    STRING *q = newSTRING("a2");
    printf("is ");
    displaySTRING(q,stdout);
    printf(" present? %s\n",findBST(p,q) == 0? "no" : "yes");
    STRING *r = newSTRING("a3");
    if (locateBST(p,r) != 0)
        {
        STRING *x = findBST(p,r);
        deleteBST(p,r);
        freeSTRING(x);
        }
    debugBST(p,1);
    printf("debug (in-order): ");
    displayBST(p,stdout);
    printf("\n");
    debugBST(p,0);
    printf("BST:\n");
    displayBST(p,stdout);
    printf("is ");
    displaySTRING(q,stdout);
    printf(" present? %s\n",findBST(p,q) == 0? "no" : "yes");
    setSTRING(r,"a2");
    if (locateBST(p,r) != 0)
        {
        STRING *x = findBST(p,r);
        deleteBST(p,r);
        freeSTRING(x);
        }
    debugBST(p,1);
    printf("debug (in-order): ");
    displayBST(p,stdout);
    printf("\n");
    debugBST(p,0);
    printf("BST:\n");
    displayBST(p,stdout);
    printf("is ");
    displaySTRING(q,stdout);
    printf(" present? %s\n",findBST(p,q) == 0? "no" : "yes");
    printf("size: %d\n",sizeBST(p));
    statisticsBST(p,stdout);
    freeSTRING(q);
    freeSTRING(r);
    freeBST(p);
    return 0;
    }
