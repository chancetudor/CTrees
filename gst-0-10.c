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
    srandom(22);
    printf("INTEGER test of GST, mixed insertions and deletions\n");
    int i;
    GST *p = newGST(compareINTEGER);
    setGSTdisplay(p,displayINTEGER);
    setGSTfree(p,freeINTEGER);
    for (i = 0; i < 20000; ++i)
        {
        int j = random() % 3333;
        INTEGER *a = newINTEGER(j);
        if (random() % 10 < 6)
            {
            if (freqGST(p,a) > 1)
                deleteGST(p,a);
            else
                {
                INTEGER *x = findGST(p,a);
                deleteGST(p,a);
                freeINTEGER(x);
                }
            freeINTEGER(a);
            }
        else
             insertGST(p,a);
        }
    if (sizeGST(p) < 200)
        {
        debugGST(p,0);
        printf("GST:\n");
        displayGST(p,stdout);
        debugGST(p,1);
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
    while (sizeGST(p) > 200)
        {
        for (i = 0; i < 10000; ++i)
            {
            int j = random() % 5000;
            INTEGER *a = newINTEGER(j);
            if (freqGST(p,a) > 1)
                deleteGST(p,a);
            else
                {
                INTEGER *x = findGST(p,a);
                deleteGST(p,a);
                freeINTEGER(x);
                }
            freeINTEGER(a);
            }
        }
    if (sizeGST(p) < 200)
        {
        debugGST(p,0);
        printf("GST:\n");
        displayGST(p,stdout);
        debugGST(p,1);
        printf("debug: ");
        displayGST(p,stdout);
        printf("\n");
        debugGST(p,2);
        printf("debug: ");
        displayGST(p,stdout);
        printf("\n");
        debugGST(p,3);
        printf("debug: ");
        displayGST(p,stdout);
        printf("\n");
        }
    printf("size is %d\n",sizeGST(p));
    statisticsGST(p,stdout);
    freeGST(p);
    return 0;
    }
