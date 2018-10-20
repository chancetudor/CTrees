#include <stdio.h>
#include "rbt.h"
#include "vbst.h"

#ifndef _INTERPRETER_INCLUDED_
#define _INTERPRETER_INCLUDED_

void RBTInterpreter(char **argv, FILE *outfp, rbt *tree);
void VBSTInterpreter(char **argv, FILE *outfp, vbst *tree);

#endif // _INTERPRETER_INCLUDED_
