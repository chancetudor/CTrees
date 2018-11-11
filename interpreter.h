#include <stdio.h>
#include "rbt.h"
#include "gst.h"

#ifndef _INTERPRETER_INCLUDED_
#define _INTERPRETER_INCLUDED_

void RBTInterpreter(RBT *tree, char **argv, FILE *outfp);
void GSTInterpreter(GST *tree, char **argv, FILE *outfp);

#endif // _INTERPRETER_INCLUDED_
