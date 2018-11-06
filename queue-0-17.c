#include <stdio.h>
#include <stdlib.h>
#include "integer.h"
#include "real.h"
#include "string.h"
#include "cda.h"
#include "queue.h"

void srandom(unsigned int);
long int random(void);

static void
display(QUEUE *p,char *intro)
    {
    printf("%s",intro);
    if (sizeQUEUE(p) < 200)
        {
        displayQUEUE(p,stdout);
        }
    else
        {
        printf("(displaying top element)\n");
        displayINTEGER(peekQUEUE(p),stdout);
        printf("\n");
        }
    printf("\n");
    }

int
main(void)
    {
    srandom(29);
    int i,r,rounds=3,size=2076667;
    printf("INTEGER test of QUEUE, insertions and deletions at front\n");
    QUEUE *p = newQUEUE();
    setQUEUEdisplay(p,displayINTEGER);
    setQUEUEfree(p,freeINTEGER);
    debugQUEUE(p,0);
    display(p,"items: ");
    for (r = 0; r < rounds; ++r)
        {
        for (i = 0; i < size; ++i)
            {
            int j = random() % size;
            enqueue(p,newINTEGER(j));
            }
        int limit = r == rounds - 1? size - random() % size / 2 : 0;
        while (sizeQUEUE(p) > limit)
            {
            freeINTEGER(dequeue(p));
            }
        debugQUEUE(p,r);
        display(p,"items: ");
        printf("size is %d\n",sizeQUEUE(p));
        }
    freeQUEUE(p);
    return 0;
    }
