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
    srandom(13);
    //simple REAL test of BST
    BST *p = newBST(compareREAL);
    setBSTdisplay(p,displayREAL);
    setBSTfree(p,freeREAL);
    insertBST(p,newREAL(3));
    insertBST(p,newREAL(2));
    debugBST(p,1);
    printf("debug (in-order): ");
    displayBST(p,stdout);
    printf("\n");
    debugBST(p,0);
    printf("BST:\n");
    displayBST(p,stdout);
    REAL *q = newREAL(2);
    printf("is ");
    displayREAL(q,stdout);
    printf(" present? %s\n",findBST(p,q) == 0? "no" : "yes");
    REAL *r = newREAL(3);
    if (locateBST(p,r) != 0)
        {
        REAL *x = findBST(p,r);
        deleteBST(p,r);
        freeREAL(x);
        }
    debugBST(p,1);
    printf("debug (in-order): ");
    displayBST(p,stdout);
    printf("\n");
    debugBST(p,0);
    printf("BST:\n");
    displayBST(p,stdout);
    printf("is ");
    displayREAL(q,stdout);
    printf(" present? %s\n",findBST(p,q) == 0? "no" : "yes");
    setREAL(r,2);
    if (locateBST(p,r) != 0)
        {
        REAL *x = findBST(p,r);
        deleteBST(p,r);
        freeREAL(x);
        }
    debugBST(p,1);
    printf("debug (in-order): ");
    displayBST(p,stdout);
    debugBST(p,0);
    printf("BST:\n");
    displayBST(p,stdout);
    printf("is ");
    displayREAL(q,stdout);
    printf(" present? %s\n",findBST(p,q) == 0? "no" : "yes");
    printf("size: %d\n",sizeBST(p));
    statisticsBST(p,stdout);
    freeREAL(q);
    freeREAL(r);
    freeBST(p);
    return 0;
    }
