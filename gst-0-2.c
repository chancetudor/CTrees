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
    srandom(14);
    printf("STRING test of GST, simple\n");
    GST *p = newGST(compareSTRING);
    setGSTdisplay(p,displaySTRING);
    setGSTfree(p,freeSTRING);
    insertGST(p,newSTRING("a3"));
    insertGST(p,newSTRING("a2"));
    insertGST(p,newSTRING("a1"));
    insertGST(p,newSTRING("a2"));
    debugGST(p,0);
    printf("GST:\n");
    displayGST(p,stdout);
    debugGST(p,1);
    printf("debug: ");
    displayGST(p,stdout);
    printf("\n");
    STRING *q = newSTRING("a2");
    printf("is ");
    displaySTRING(q,stdout);
    printf(" present? %s\n",findGST(p,q) == 0? "no" : "yes");
    STRING *r = newSTRING("a3");
    printf("deleting %s\n",getSTRING(r));
    if (freqGST(p,r) > 1)
        deleteGST(p,r);
    else
        {
        STRING *x = findGST(p,r);
        deleteGST(p,r);
        freeSTRING(x);
        }
    debugGST(p,0);
    printf("GST:\n");
    displayGST(p,stdout);
    debugGST(p,1);
    printf("debug: ");
    displayGST(p,stdout);
    printf("\n");
    printf("is ");
    displaySTRING(q,stdout);
    printf(" present? %s\n",findGST(p,q) == 0? "no" : "yes");
    setSTRING(r,"a2");
    printf("deleting %s\n",getSTRING(r));
    if (freqGST(p,r) > 1)
        deleteGST(p,r);
    else
        {
        STRING *x = findGST(p,r);
        deleteGST(p,r);
        freeSTRING(x);
        }
    debugGST(p,0);
    printf("GST:\n");
    displayGST(p,stdout);
    debugGST(p,1);
    printf("debug: ");
    displayGST(p,stdout);
    printf("\n");
    printf("is ");
    displaySTRING(q,stdout);
    printf(" present? %s\n",findGST(p,q) == 0? "no" : "yes");
    printf("size: %d\n",sizeGST(p));
    statisticsGST(p,stdout);
    freeSTRING(q);
    freeSTRING(r);
    freeGST(p);
    return 0;
    }
