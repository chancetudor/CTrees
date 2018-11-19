/*File: main.c
 *Author: Chance Tudor
 *Creates a red black or green search tree based on a user-provided file's contents
 *Manipulates the tree based on a user provided command file
 *Outputs the tree and various tree statistics to stdout
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gst.h"
#include "rbt.h"
#include "string.h"
#include "interpreter.h"

int main(int argc, char **argv) {
  int author = 0, green = 0, rbt = 0;
  if (argc != 4) {
    printf("Error! Usage: %s -flag corpus_file command_file\n", argv[0]);
  }

  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "-v") == 0) { author = 1; }
    if (strcmp(argv[i], "-g") == 0) { green = i; }
    if (strcmp(argv[i], "-r") == 0) { rbt = i; }
  }

  if (author) {
    printf("Written by Chance Tudor\n");
    exit(0);
  }

  // FIXME
  if (green) {
    GST * tree = newGST(compareSTRING);
    GSTInterpreter(tree, argv, stdout);
    freeGST(tree);
  }
  else if (rbt == 1 || (rbt == 0 && green == 0)) {
    RBT * tree = newRBT(compareSTRING);
    RBTInterpreter(tree, argv, stdout);
    freeRBT(tree);
  }
  else {
    printf("Error: invalid flag. Valid flags are: '-v' | '-g' | '-r'\n");
  }

  return 0;
}
