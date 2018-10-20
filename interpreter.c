/*File: interpreter.c
 *Author: Emily Turner
 *Implements functions found in interpreter.h in order to populate a binary tree with the text found in the corpus text file
 *Also reads in a command text file in order to manipulate and display statistics about the tree
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rbt.h"
#include "vbst.h"
#include "scanner.h"

rbt *readRBTCorpus(char *filename, rbt *tree);
void readRBTCommands(char *filename, rbt *tree, FILE *outfp);
vbst *readVBSTCorpus(char *filename, vbst *tree);
void readVBSTCommands(char *filename, vbst *tree, FILE *outfp);

void RBTInterpreter(char **argv, FILE *outfp, rbt *tree)
{
    tree = readRBTCorpus(argv[2], tree);
    readRBTCommands(argv[3], tree, outfp);
}

rbt *readRBTCorpus(char *filename, rbt *tree)
{
    char *str;
    FILE *fp = fopen(filename, "r");
    if (fp == 0)
    {
        fprintf(stderr, "Error: %s could not be opened for reading.\n", filename);
        exit(1);
    }
    str = readString(fp);
    while (!feof(fp))
    {
        if (strlen(str) > 0)
        {
            insertRBT(tree, str);
        }
        str = readString(fp);
    }
    fclose(fp);

    return tree;
}

void readRBTCommands(char *filename, rbt *tree, FILE *outfp)
{
    char *str;
    int freq;
    FILE *fp = fopen(filename, "r");
    if (fp == 0)
    {
        fprintf(stderr, "Error: %s could not be opened for reading.\n", filename);
        exit(1);
    }
    int c = fgetc(fp); ////////
    while (!feof(fp))
    {
        switch (c)
        {
        case 's':
            displayRBT(outfp, tree);
            break;
        case 'r':
            statisticsRBT(tree, outfp);
            break;
        case 'f':
            str = readString(fp);
            freq = findRBT(tree, str);
            fprintf(outfp, "Frequency of \"%s\": %d\n", str, freq);
            break;
        case 'd':
            str = readString(fp);
            deleteRBT(tree, str);
            break;
        case 'i':
            str = readString(fp);
            if (strlen(str) > 0) //error check
            {
                insertRBT(tree, str);
            }
            break;
        }
        c = fgetc(fp); ////
    }
    fclose(fp);
}

void VBSTInterpreter(char **argv, FILE *outfp, vbst *tree)
{
    tree = readVBSTCorpus(argv[2], tree);
    readVBSTCommands(argv[3], tree, outfp);
}

vbst *readVBSTCorpus(char *filename, vbst *tree)
{
    char *str;
    FILE *fp = fopen(filename, "r");
    if (fp == 0)
    {
        fprintf(stderr, "Error: %s could not be opened for reading.\n", filename);
        exit(1);
    }
    str = readString(fp);
    while (!feof(fp))
    {
        if (strlen(str) > 0)
        {
            insertVBST(tree, str);
        }
        str = readString(fp);
    }
    fclose(fp);

    return tree;
}

void readVBSTCommands(char *filename, vbst *tree, FILE *outfp)
{
    char *str;
    int freq;
    FILE *fp = fopen(filename, "r");
    if (fp == 0)
    {
        fprintf(stderr, "Error: %s could not be opened for reading.\n", filename);
        exit(1);
    }
    int c = fgetc(fp);
    while (!feof(fp))
    {
        switch (c)
        {
        case 's':
            displayVBST(outfp, tree);
            break;
        case 'r':
            statisticsVBST(tree, outfp);
            break;
        case 'f':
            str = readString(fp);
            freq = findVBST(tree, str);
            fprintf(outfp, "Frequency of \"%s\": %d\n", str, freq);
            break;
        case 'd':
            str = readString(fp);
            deleteVBST(tree, str);
            break;
        case 'i':
            str = readString(fp);
            if (strlen(str) > 0)
            {
                insertVBST(tree, str);
            }
            break;
        }
        c = fgetc(fp);
    }
    fclose(fp);
}
