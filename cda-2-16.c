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
    srandom(2040);
    int i,r,rounds=2,size=1507;
    printf("INTEGER test of CDA, unions\n");
    printf("test size is %d\n",size);
    CDA *p = newCDA();
    setCDAdisplay(p,displayINTEGER);
    setCDAfree(p,freeINTEGER);
    debugCDA(p,1);
    CDA *q = newCDA();
    setCDAdisplay(q,displayINTEGER);
    setCDAfree(q,freeINTEGER);
    debugCDA(q,1);
    display(p,"p items: ");
    display(q,"q items: ");
    for (r = 0; r < rounds; ++r)
        {
        int j,k;
        for (i = 0; i < size; ++i)
            {
            j = random() % size;
            k = random() % (sizeCDA(p) + 1);
            insertCDA(p,k,newINTEGER(j));
            j = random() % size;
            k = random() % (sizeCDA(q) + 1);
            insertCDA(q,k,newINTEGER(j));
            }
        for (i = 0; i < size - random() % size / 2; ++i)
            {
            unionCDA(p,q);
            unionCDA(q,p);
            }
        display(p,"p items: ");
        display(q,"q items: ");
        printf("p size is %d\n",sizeCDA(p));
        printf("q size is %d\n",sizeCDA(q));
        }
    freeCDA(p);
    freeCDA(q);
    return 0;
    }
