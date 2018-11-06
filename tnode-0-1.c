#include <stdio.h>
#include <stdlib.h>
#include "bst.h"
#include "integer.h"
#include "real.h"
#include "string.h"
#include "queue.h"

void srandom(unsigned int);
long int random(void);

int
main(void)
    {
    srandom(13);
    printf("REAL test of TNODE, simple\n");
    TNODE *p = newTNODE(newREAL(2.0),0,0,0);
    setTNODEdisplay(p,displayREAL);
    setTNODEfree(p,freeREAL);
    printf("tnode's value: ");
    displayTNODE(p,stdout);
    printf("\n");
    freeTNODE(p);
    return 0;
    }
