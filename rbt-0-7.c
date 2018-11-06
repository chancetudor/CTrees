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
    srandom(19);
    printf("INTEGER test of RBT, mixed insertions and deletions\n");
    int i;
    RBT *p = newRBT(compareINTEGER);
    setRBTdisplay(p,displayINTEGER);
    setRBTfree(p,freeINTEGER);
    for (i = 0; i < 20000; ++i)
        {
        int j = random() % 3333;
        INTEGER *a = newINTEGER(j);
        if (random() % 10 < 6)
            {
            if (freqRBT(p,a) > 1)
                deleteRBT(p,a);
            else
                {
                INTEGER *x = findRBT(p,a);
                deleteRBT(p,a);
                freeINTEGER(x);
                }
            freeINTEGER(a);
            }
        else
             insertRBT(p,a);
        }
    if (sizeRBT(p) < 200)
        {
        debugRBT(p,0);
        printf("RBT:\n");
        displayRBT(p,stdout);
        debugRBT(p,1);
        printf("debug (in-order): ");
        displayRBT(p,stdout);
        printf("\n");
        debugRBT(p,2);
        printf("debug (pre-order): ");
        displayRBT(p,stdout);
        printf("\n");
        debugRBT(p,3);
        printf("debug (post-order): ");
        displayRBT(p,stdout);
        printf("\n");
        }
    printf("size is %d\n",sizeRBT(p));
    statisticsRBT(p,stdout);
    while (sizeRBT(p) > 200)
        {
        for (i = 0; i < 10000; ++i)
            {
            int j = random() % 5000;
            INTEGER *a = newINTEGER(j);
            if (freqRBT(p,a) > 1)
                deleteRBT(p,a);
            else
                {
                INTEGER *x = findRBT(p,a);
                deleteRBT(p,a);
                freeINTEGER(x);
                }
            freeINTEGER(a);
            }
        }
    if (sizeRBT(p) < 200)
        {
        debugRBT(p,0);
        printf("RBT:\n");
        displayRBT(p,stdout);
        debugRBT(p,1);
        printf("debug: ");
        displayRBT(p,stdout);
        printf("\n");
        debugRBT(p,2);
        printf("debug: ");
        displayRBT(p,stdout);
        printf("\n");
        debugRBT(p,3);
        printf("debug: ");
        displayRBT(p,stdout);
        printf("\n");
        }
    printf("size is %d\n",sizeRBT(p));
    statisticsRBT(p,stdout);
    freeRBT(p);
    return 0;
    }
