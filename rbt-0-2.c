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
    srandom(14);
    printf("STRING test of RBT, simple\n");
    RBT *p = newRBT(compareSTRING);
    setRBTdisplay(p,displaySTRING);
    setRBTfree(p,freeSTRING);
    insertRBT(p,newSTRING("a3"));
    insertRBT(p,newSTRING("a2"));
    insertRBT(p,newSTRING("a1"));
    insertRBT(p,newSTRING("a2"));
    debugRBT(p,0);
    printf("RBT:\n");
    displayRBT(p,stdout);
    debugRBT(p,1);
    printf("debug: ");
    displayRBT(p,stdout);
    printf("\n");
    STRING *q = newSTRING("a2");
    printf("is ");
    displaySTRING(q,stdout);
    printf(" present? %s\n",findRBT(p,q) == 0? "no" : "yes");
    STRING *r = newSTRING("a3");
    printf("deleting %s\n",getSTRING(r));
    if (freqRBT(p,r) > 1)
        deleteRBT(p,r);
    else
        {
        STRING *x = findRBT(p,r);
        deleteRBT(p,r);
        freeSTRING(x);
        }
    debugRBT(p,0);
    printf("RBT:\n");
    displayRBT(p,stdout);
    debugRBT(p,1);
    printf("debug: ");
    displayRBT(p,stdout);
    printf("\n");
    printf("is ");
    displaySTRING(q,stdout);
    printf(" present? %s\n",findRBT(p,q) == 0? "no" : "yes");
    setSTRING(r,"a2");
    printf("deleting %s\n",getSTRING(r));
    if (freqRBT(p,r) > 1)
        deleteRBT(p,r);
    else
        {
        STRING *x = findRBT(p,r);
        deleteRBT(p,r);
        freeSTRING(x);
        }
    debugRBT(p,0);
    printf("RBT:\n");
    displayRBT(p,stdout);
    debugRBT(p,1);
    printf("debug: ");
    displayRBT(p,stdout);
    printf("\n");
    printf("is ");
    displaySTRING(q,stdout);
    printf(" present? %s\n",findRBT(p,q) == 0? "no" : "yes");
    printf("size: %d\n",sizeRBT(p));
    statisticsRBT(p,stdout);
    freeSTRING(q);
    freeSTRING(r);
    freeRBT(p);
    return 0;
    }
