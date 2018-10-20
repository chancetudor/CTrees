/*File: main.c
 *Author: Emily Turner
 *Creates a red black or binary search tree based on a user-provided files contents
 *Manipulates the tree based on a user provided command file
 *Outputs the tree and various tree statistics to stdout or to a user provided file
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vbst.h"
#include "rbt.h"
#include "string.h"
#include "interpreter.h"

int main(int argc, char **argv)
{
    if (argc != 4 && argc != 5)
    {
        fprintf(stderr, "Usage: %s -flag corpus commands output\n", argv[0]);
        exit(1);
    }
    FILE *outfp = stdout;
    if (argc == 5)
    {
        outfp = fopen(argv[4], "w");
    }
    if (outfp == 0) fprintf(stderr, "Error: could not open output file for writing, will write to stdin.\n");
    if (strcmp(argv[1], "-v") == 0)
    {
        vbst *tree = newVBST(displayString, stringComparator);
        VBSTInterpreter(argv, outfp, tree);
    }
    else if (strcmp(argv[1], "-r") == 0)
    {
        rbt *tree = newRBT(displayString, stringComparator);
        RBTInterpreter(argv, outfp, tree);
    }
    else
    {
        fprintf(stderr, "Error: invalid flag, valid flags are '-v' and '-r'\n");
    }
    fclose(outfp);
    return 0;
}
