//
//  queue.c
//  A generic queue class based on a generic circular dynamic array class
//  with corresponding methods to alter the array
//
//  project0
//
//  Created by Chance Tudor on 9/15/18.
//  Copyright © 2018 Chance Tudor. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"
#include "cda.h"

typedef void (*FM)(void * ptr); // typedef declaration to store a freeMethod function pointer in queue struct
typedef void (*DM)(void * ptr, FILE *fp); // typedef declaration to store a displayMethod function pointer in queue struct
static int getIndex(CDA * items, int oldIndex);

struct cda {
  void * (*storage);
  int capacity;
  int size;
  int startIndex;
  int endIndex;
  int debugVal;
  FM freeMethod;
  DM displayMethod;
};

struct queue {
  CDA * array;
  int debugVal;
  FM freeMethod;
  DM displayMethod;
};

extern QUEUE *newQUEUE(void) {
  QUEUE * queue = malloc(sizeof(QUEUE));
  assert(queue != 0);
  queue->array = newCDA();
  assert(queue->array != 0);
  queue->debugVal = 0;
  queue->freeMethod = 0;
  queue->displayMethod = 0;

  return queue;
}


extern void setQUEUEdisplay(QUEUE *items, void (*displayMeth)(void * ptr, FILE *fp)) {
  items->displayMethod = displayMeth;
}


extern void setQUEUEfree(QUEUE *items, void (*freeMeth)(void *ptr)) {
  items->freeMethod = freeMeth;
}


extern void enqueue(QUEUE *items, void *value) {
  insertCDAback(items->array, value);
}


extern void *dequeue(QUEUE *items) {
  assert(sizeQUEUE(items) > 0);
  void * temp = removeCDAfront(items->array);
  return temp;
}


extern void *peekQUEUE(QUEUE *items) {
  assert(sizeQUEUE(items) > 0);
  void * temp = getCDA(items->array, 0);
  return temp;
}


extern void displayQUEUE(QUEUE *items,FILE *fp) {
  if (items->debugVal == 0) { // use QUEUE display method
    if (items->displayMethod == 0) {
      fprintf(fp, "<");
      for (int i = 0; i < sizeQUEUE(items); i++) {
        fprintf(fp, "@%p,", &items->array->storage[getIndex(items->array, i)]);
        if (i != sizeQUEUE(items) - 1) { fprintf(fp, ","); }
      }
      fprintf(fp, ">");
    }
    else {
      fprintf(fp, "<");
      for (int i = 0; i < sizeQUEUE(items); i++) {
        items->displayMethod(getCDA(items->array, i), fp);
        if (i != sizeQUEUE(items) - 1) { fprintf(fp, ","); }
      }
      fprintf(fp, ">");
    }
  }
  else if (items->debugVal == 1) { // use CDA display method
    debugCDA(items->array, 0);
    setCDAdisplay(items->array, items->displayMethod);
    displayCDA(items->array, fp);
  }
  else { // use CDA display method w/ debugVal > 0
    debugCDA(items->array, 1);
    setCDAdisplay(items->array, items->displayMethod);
    displayCDA(items->array, fp);
  }
}


extern int debugQUEUE(QUEUE *items, int level) {
  int prevVal = items->debugVal;
  items->debugVal = level;

  return prevVal;
}


extern void freeQUEUE(QUEUE *items) {
  setCDAfree(items->array, items->freeMethod);
  freeCDA(items->array);
  free(items);
}


extern int sizeQUEUE(QUEUE *items) { return sizeCDA(items->array); }

static int getIndex(CDA * items, int oldIndex) {
  int trueIndex = (oldIndex + items->capacity) % items->capacity;
  return trueIndex;
}
