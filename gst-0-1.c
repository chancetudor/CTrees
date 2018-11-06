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
    srandom(13);
    printf("REAL test of GST, simple\n");
    GST *p = newGST(compareREAL);
    setGSTdisplay(p,displayREAL);
    setGSTfree(p,freeREAL);
    insertGST(p,newREAL(3));
    insertGST(p,newREAL(2));
    insertGST(p,newREAL(1));
    insertGST(p,newREAL(2));
    debugGST(p,0);
    printf("GST:\n");
    displayGST(p,stdout);
    debugGST(p,1);
    printf("debug: ");
    displayGST(p,stdout);
    printf("\n");
    REAL *q = newREAL(2);
    printf("is ");
    displayREAL(q,stdout);
    printf(" present? %s\n",findGST(p,q) == 0? "no" : "yes");
    REAL *r = newREAL(3);
    printf("deleting %f\n",getREAL(r));
    if (freqGST(p,r) > 1)
        deleteGST(p,r);
    else
        {
        REAL *x = findGST(p,r);
        deleteGST(p,r);
        freeREAL(x);
        }
    debugGST(p,0);
    printf("GST:\n");
    displayGST(p,stdout);
    debugGST(p,1);
    printf("debug: ");
    displayGST(p,stdout);
    printf("\n");
    printf("is ");
    displayREAL(q,stdout);
    printf(" present? %s\n",findGST(p,q) == 0? "no" : "yes");
    setREAL(r,2);
    printf("deleting %f\n",getREAL(r));
    if (freqGST(p,r) > 1)
        deleteGST(p,r);
    else
        {
        REAL *x = findGST(p,r);
        deleteGST(p,r);
        freeREAL(x);
        }
    debugGST(p,0);
    printf("GST:\n");
    displayGST(p,stdout);
    debugGST(p,1);
    printf("debug: ");
    displayGST(p,stdout);
    printf("\n");
    printf("is ");
    displayREAL(q,stdout);
    printf(" present? %s\n",findGST(p,q) == 0? "no" : "yes");
    printf("size: %d\n",sizeGST(p));
    statisticsGST(p,stdout);
    freeREAL(q);
    freeREAL(r);
    freeGST(p);
    return 0;
    }
