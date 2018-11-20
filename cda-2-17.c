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
    srandom(2041);
    int i,r,rounds=2,size=181471;
    printf("INTEGER test of CDA, insertions at the tail\n");
    CDA *p = newCDA();
    setCDAdisplay(p,displayINTEGER);
    setCDAfree(p,freeINTEGER);
    debugCDA(p,1);
    display(p,"items: ");
    for (r = 0; r < rounds; ++r)
        {
        for (i = 0; i < size; ++i)
            {
            int j = random() % size;
            if (random() % 2 == 0)
                insertCDA(p,sizeCDA(p),newINTEGER(j));
            else
                assert(setCDA(p,sizeCDA(p),newINTEGER(j)) == 0);
            }
        display(p,"items: ");
        printf("size is %d\n",sizeCDA(p));
        }
    freeCDA(p);
    return 0;
    }
