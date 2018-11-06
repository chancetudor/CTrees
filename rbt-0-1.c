#include <stdio.h>
#include <stdlib.h>
#include "gst.h"
#include "rbt.h"
#include "integer.h"
#include "real.h"
#include "string.h"

void srandom(unsigned int);
long int random(void);

int
main(void)
    {
    srandom(13);
    printf("REAL test of RBT, simple\n");
    RBT *p = newRBT(compareREAL);
    setRBTdisplay(p,displayREAL);
    setRBTfree(p,freeREAL);
    insertRBT(p,newREAL(3));
    insertRBT(p,newREAL(2));
    insertRBT(p,newREAL(1));
    insertRBT(p,newREAL(2));
    debugRBT(p,0);
    printf("RBT:\n");
    displayRBT(p,stdout);
    debugRBT(p,1);
    printf("debug: ");
    displayRBT(p,stdout);
    printf("\n");
    REAL *q = newREAL(2);
    printf("is ");
    displayREAL(q,stdout);
    printf(" present? %s\n",findRBT(p,q) == 0? "no" : "yes");
    REAL *r = newREAL(3);
    printf("deleting %f\n",getREAL(r));
    if (freqRBT(p,r) > 1)
        deleteRBT(p,r);
    else
        {
        REAL *x = findRBT(p,r);
        deleteRBT(p,r);
        freeREAL(x);
        }
    debugRBT(p,0);
    printf("RBT:\n");
    displayRBT(p,stdout);
    debugRBT(p,1);
    printf("debug: ");
    displayRBT(p,stdout);
    printf("\n");
    printf("is ");
    displayREAL(q,stdout);
    printf(" present? %s\n",findRBT(p,q) == 0? "no" : "yes");
    setREAL(r,2);
    printf("deleting %f\n",getREAL(r));
    if (freqRBT(p,r) > 1)
        deleteRBT(p,r);
    else
        {
        REAL *x = findRBT(p,r);
        deleteRBT(p,r);
        freeREAL(x);
        }
    debugRBT(p,0);
    printf("RBT:\n");
    displayRBT(p,stdout);
    debugRBT(p,1);
    printf("debug: ");
    displayRBT(p,stdout);
    printf("\n");
    printf("is ");
    displayREAL(q,stdout);
    printf(" present? %s\n",findRBT(p,q) == 0? "no" : "yes");
    printf("size: %d\n",sizeRBT(p));
    statisticsRBT(p,stdout);
    freeREAL(q);
    freeREAL(r);
    freeRBT(p);
    return 0;
    }
