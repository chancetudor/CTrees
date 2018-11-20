#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "integer.h"
#include "real.h"
#include "string.h"
#include "cda.h"

void srandom(unsigned int);
long int random(void);

static void
display(CDA *p,char *intro)
    {
    printf("%s",intro);
    if (sizeCDA(p) < 200)
        {
        displayCDA(p,stdout);
        }
    else
        {
        int k = 100;
        int j;
        printf("(displaying first and last %d elements)\n",k);
        for (j = 0; j < k; ++j)
            {
            displayINTEGER(getCDA(p,j),stdout);
            printf(" ");
            }
        printf("... ");
        for (j = 0; j < k; ++j)
            {
            displayINTEGER(getCDA(p,sizeCDA(p)-k+j),stdout);
            if (j < k - 1) printf(" ");
            }
        printf("\n");
        }
    printf("\n");
    }

int
main(void)
    {
    srandom(2039);
    int i,r,rounds=2,size=1177,dist=10;
    printf("INTEGER test of CDA, delete all\n");
    CDA *p = newCDA();
    setCDAdisplay(p,displayINTEGER);
    setCDAfree(p,freeINTEGER);
    debugCDA(p,1);
    display(p,"items: ");
    for (r = 0; r < rounds; ++r)
        {
        for (i = 0; i < size; ++i)
            {
            int k = sizeCDA(p) < dist? 0 : random() % dist;
            if (random() % 2 == 1) //back
                k = sizeCDA(p) - k;
            insertCDA(p,k,newINTEGER(i));
            }
        display(p,"items: ");
        printf("size is %d\n",sizeCDA(p));
        while (sizeCDA(p) - r > 0)
            {
            int k = sizeCDA(p) < dist? 0 : random() % dist;
            if (random() % 2 == 1) //back
                k = (sizeCDA(p) - 1) - k;
            freeINTEGER(removeCDA(p,k));
            }
        insertCDA(p,0,newINTEGER(1));
        display(p,"items: ");
        printf("size is %d\n",sizeCDA(p));
        }
    freeCDA(p);
    return 0;
    }
