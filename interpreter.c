
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rbt.h"
#include "gst.h"
#include "scanner.h"

RBT *readRBTCorpus(RBT *tree, char *filename);
void readRBTCommands(RBT *tree, char *filename, FILE *outfp);
GST *readGSTCorpus(GST *tree, char *filename);
void readGSTCommands(GST *tree, char *filename, FILE *outfp);

void RBTInterpreter(RBT *tree, char **argv, FILE *outfp) {
  tree = readRBTCorpus(tree, argv[2]);
  readRBTCommands(tree, argv[3], outfp);
}

RBT *readRBTCorpus(RBT *tree, char *filename) {
  char *str;
  FILE *fp = fopen(filename, "r");
  if (fp == 0) {
    fprintf(stdout, "Error: %s could not be opened for reading.\n", filename);
  }
  str = readString(fp);
  while (!feof(fp)) {
    if (strlen(str) > 0) {
      insertRBT(tree, str);
    }
    str = readString(fp);
  }
  fclose(fp);

  return tree;
}

void readRBTCommands(RBT *tree, char *filename, FILE *outfp) {
  char *str;
  int freq;
  FILE *fp = fopen(filename, "r");
  if (fp == 0) {
    fprintf(stderr, "Error: %s could not be opened for reading.\n", filename);
  }
  int c = fgetc(fp); ////////
  while (!feof(fp)) {
    switch (c) {
      case 's':
        displayRBT(tree, outfp);
        break;
      case 'r':
        statisticsRBT(tree, outfp);
        break;
      case 'f':
        str = readString(fp);
        freq = freqRBT(tree, str);
        fprintf(outfp, "Frequency of \"%s\": %d\n", str, freq);
        break;
      case 'd':
        str = readString(fp);
        deleteRBT(tree, str);
        break;
      case 'i':
        str = readString(fp);
        if (strlen(str) > 0) {
          insertRBT(tree, str);
        }
        break;
    }
    c = fgetc(fp);
  }
  fclose(fp);
}

void GSTInterpreter(GST *tree, char **argv, FILE *outfp) {
  tree = readGSTCorpus(tree, argv[2]);
  readGSTCommands(tree, argv[3], outfp);
}

GST *readGSTCorpus(GST *tree, char *filename) {
  char *str;
  FILE *fp = fopen(filename, "r");
  if (fp == 0) {
    fprintf(stdout, "Error: %s could not be opened for reading.\n", filename);
  }
  str = readString(fp);
  while (!feof(fp)) {
    if (strlen(str) > 0) {
      insertGST(tree, str);
    }
    str = readString(fp);
  }
  fclose(fp);

  return tree;
}

void readGSTCommands(GST *tree, char *filename, FILE *outfp) {
  char *str;
  int freq;
  FILE *fp = fopen(filename, "r");
  if (fp == 0) {
    fprintf(stdout, "Error: %s could not be opened for reading.\n", filename);
  }
  int c = fgetc(fp);
  while (!feof(fp)) {
    switch (c) {
      case 's':
        displayGST(tree, outfp);
        break;
      case 'r':
        statisticsGST(tree, outfp);
        break;
      case 'f':
        str = readString(fp);
        freq = freqGST(tree, str);
        fprintf(outfp, "Frequency of \"%s\": %d\n", str, freq);
        break;
      case 'd':
        str = readString(fp);
        deleteGST(tree, str);
        break;
      case 'i':
        str = readString(fp);
        if (strlen(str) > 0) {
          insertGST(tree, str);
        }
        break;
      }
    c = fgetc(fp);
  }
  fclose(fp);
}
