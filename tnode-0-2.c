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
    srandom(14);
    printf("STRING test of TNODE, simple\n");
    TNODE *p = newTNODE(newSTRING("three"),0,0,0);
    setTNODEdisplay(p,displaySTRING);
    setTNODEfree(p,freeSTRING);
    printf("tnode's value: ");
    displayTNODE(p,stdout);
    printf("\n");
    freeTNODE(p);
    return 0;
    }
