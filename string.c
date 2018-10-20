#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string.h"


void
displayString(FILE *fp,void *v)
    {
    fprintf(fp,"\"%s\"",(char *) v);
    }

int stringComparator(void *v, void *w)
{
    /*if (strcmp((char *) v,(char *) w) > 0) return 1;
    else if (strcmp((char *) v,(char *) w) < 0) return -1;
    else return 0;*/
    return strcmp((char *) v,(char *) w);
}
