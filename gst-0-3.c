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
    srandom(15);
    printf("INTEGER test of GST, insertions\n");
    int i;
    GST *p = newGST(compareINTEGER);
    setGSTdisplay(p,displayINTEGER);
    setGSTfree(p,freeINTEGER);
    for (i = 0; i < 13; ++i)
        {
        int j = random() % 6;
        INTEGER *a = newINTEGER(j);
        insertGST(p,a);
        }
    if (sizeGST(p) < 200)
        {
        printf("GST:\n");
        debugGST(p,0);
        displayGST(p,stdout);
        printf("debug (in-order): ");
        displayGST(p,stdout);
        printf("\n");
        debugGST(p,2);
        printf("debug (pre-order): ");
        displayGST(p,stdout);
        printf("\n");
        debugGST(p,3);
        printf("debug (post-order): ");
        displayGST(p,stdout);
        printf("\n");
        }
    printf("size is %d\n",sizeGST(p));
    statisticsGST(p,stdout);
    freeGST(p);
    return 0;
    }
