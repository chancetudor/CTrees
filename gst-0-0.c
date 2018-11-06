#include <stdio.h>
#include <stdlib.h>
#include "bst.h"
#include "gst.h"
#include "integer.h"
#include "real.h"
#include "string.h"

void srandom(unsigned int);
long int random(void);

int
main(void)
    {
    srandom(12);
    printf("INTEGER test of GST, simple\n");
    GST *p = newGST(compareINTEGER);
    setGSTdisplay(p,displayINTEGER);
    setGSTfree(p,freeINTEGER);
    insertGST(p,newINTEGER(3));
    insertGST(p,newINTEGER(2));
    insertGST(p,newINTEGER(1));
    insertGST(p,newINTEGER(2));
    debugGST(p,0);
    printf("GST:\n");
    displayGST(p,stdout);
    debugGST(p,1);
    printf("debug: ");
    displayGST(p,stdout);
    printf("\n");
    INTEGER *q = newINTEGER(2);
    printf("is ");
    displayINTEGER(q,stdout);
    printf(" present? %s\n",findGST(p,q) == 0? "no" : "yes");
    INTEGER *r = newINTEGER(3);
    printf("deleting %d\n",getINTEGER(r));
    if (freqGST(p,r) > 1)
        deleteGST(p,r);
    else
        {
        INTEGER *x = findGST(p,r);
        deleteGST(p,r);
        freeINTEGER(x);
        }
    debugGST(p,0);
    printf("GST:\n");
    displayGST(p,stdout);
    debugGST(p,1);
    printf("debug: ");
    displayGST(p,stdout);
    printf("\n");
    printf("is ");
    displayINTEGER(q,stdout);
    printf(" present? %s\n",findGST(p,q) == 0? "no" : "yes");
    setINTEGER(r,2);
    printf("deleting %d\n",getINTEGER(r));
    if (freqGST(p,r) > 1)
        deleteGST(p,r);
    else
        {
        INTEGER *x = findGST(p,r);
        deleteGST(p,r);
        freeINTEGER(x);
        }
    debugGST(p,0);
    printf("GST:\n");
    displayGST(p,stdout);
    debugGST(p,1);
    printf("debug: ");
    displayGST(p,stdout);
    printf("\n");
    printf("is ");
    displayINTEGER(q,stdout);
    printf(" present? %s\n",findGST(p,q) == 0? "no" : "yes");
    printf("deleting %d\n",getINTEGER(r));
    if (freqGST(p,r) > 1)
        deleteGST(p,r);
    else
        {
        INTEGER *x = findGST(p,r);
        deleteGST(p,r);
        freeINTEGER(x);
        }
    debugGST(p,0);
    printf("GST:\n");
    displayGST(p,stdout);
    debugGST(p,1);
    printf("debug: ");
    displayGST(p,stdout);
    printf("\n");
    printf("is ");
    displayINTEGER(q,stdout);
    printf(" present? %s\n",findGST(p,q) == 0? "no" : "yes");
    printf("size: %d\n",sizeGST(p));
    statisticsGST(p,stdout);
    freeINTEGER(q);
    freeINTEGER(r);
    freeGST(p);
    return 0;
    }
