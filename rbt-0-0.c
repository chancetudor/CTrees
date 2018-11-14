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
    srandom(12);
    printf("INTEGER test of RBT, simple\n");
    RBT *p = newRBT(compareINTEGER);
    setRBTdisplay(p,displayINTEGER);
    setRBTfree(p,freeINTEGER);
    insertRBT(p,newINTEGER(3));
    insertRBT(p,newINTEGER(2));
    insertRBT(p,newINTEGER(1));
    insertRBT(p,newINTEGER(2));
    debugRBT(p,0);
    printf("RBT:\n");
    displayRBT(p,stdout);
    debugRBT(p,1);
    printf("debug: ");
    displayRBT(p,stdout);
    printf("\n");
    INTEGER *q = newINTEGER(2);
    printf("is ");
    displayINTEGER(q,stdout);
    printf(" present? %s\n",findRBT(p,q) == 0? "no" : "yes");
    INTEGER *r = newINTEGER(3);
    printf("deleting %d\n",getINTEGER(r));
    if (freqRBT(p,r) > 1) {
        deleteRBT(p,r);
      }
    else {
        INTEGER *x = findRBT(p,r);
        deleteRBT(p,r);
        freeINTEGER(x);
    }
    debugRBT(p,0);
    printf("RBT:\n");
    displayRBT(p,stdout);
    debugRBT(p,1);
    printf("debug: ");
    displayRBT(p,stdout);
    printf("\n");
    printf("is ");
    displayINTEGER(q,stdout);
    printf(" present? %s\n",findRBT(p,q) == 0? "no" : "yes");
    setINTEGER(r,2);
    printf("deleting %d\n",getINTEGER(r));
    if (freqRBT(p,r) > 1) {
        deleteRBT(p,r);
      }
    else
        {
        INTEGER *x = findRBT(p,r);
        deleteRBT(p,r);
        freeINTEGER(x);
        }
    debugRBT(p,0);
    printf("RBT:\n");
    displayRBT(p,stdout);
    debugRBT(p,1);
    printf("debug: ");
    displayRBT(p,stdout);
    printf("\n");
    printf("is ");
    displayINTEGER(q,stdout);
    printf(" present? %s\n",findRBT(p,q) == 0? "no" : "yes");
    printf("deleting %d\n",getINTEGER(r));
    if (freqRBT(p,r) > 1)
        deleteRBT(p,r);
    else
        {
        INTEGER *x = findRBT(p,r);
        deleteRBT(p,r);
        freeINTEGER(x);
        }
    debugRBT(p,0);
    printf("RBT:\n");
    displayRBT(p,stdout);
    debugRBT(p,1);
    printf("debug: ");
    displayRBT(p,stdout);
    printf("\n");
    printf("is ");
    displayINTEGER(q,stdout);
    printf(" present? %s\n",findRBT(p,q) == 0? "no" : "yes");
    printf("size: %d\n",sizeRBT(p));
    statisticsRBT(p,stdout);
    freeINTEGER(q);
    freeINTEGER(r);
    freeRBT(p);
    return 0;
    }
