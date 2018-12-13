//
//  cda.c
//  A generic circular dynamic array class with corresponding methods to alter the array
//  project0
//
//  Created by Chance Tudor on 9/3/18.
//  Copyright © 2018 Chance Tudor. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "cda.h"

typedef void (*FM)(void * ptr); // typedef declaration to store a freeMethod function pointer in CDA struct
typedef void (*DM)(void * ptr, FILE *fp); // typedef declaration to store a displayMethod function pointer in CDA struct
static int getCapacityCDA(CDA * items);
static void setCapacityCDA(CDA * items, int cap);
static int correctIndex(CDA *items, int oldIndex);
static int getStartCDA(CDA * items);
static int getEndCDA(CDA * items);
static int getIndex(CDA * items, int oldIndex);
static bool isFull(CDA * items);
static void doubleArray(CDA * items);
static void halveArray(CDA * items);

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

// constructor; returns new, initialized CDA object
extern CDA * newCDA(void) {
  CDA * array = malloc(sizeof(CDA));
  assert(array != 0);
  array->capacity = 1;
  array->storage = malloc(sizeof(void *) * array->capacity);
  assert(array->storage != 0);
  array->size = 0;
  array->startIndex = 0;
  array->endIndex = 0;
  array->debugVal = 0;
  array->freeMethod = 0;
  array->displayMethod = 0;

  return array;
}

extern void setCDAdisplay(CDA *items, void (*displayMeth)(void *ptr, FILE *fp)) { items->displayMethod = displayMeth; }

extern void setCDAfree(CDA *items, void (*freeMeth)(void *ptr)) { items->freeMethod = freeMeth; }

static bool isFull(CDA * items) {
  if (sizeCDA(items) == getCapacityCDA(items)) { return true; }
  else { return false; }
}

static void doubleArray(CDA * items) {
  int newCap = getCapacityCDA(items) * 2;
  void * (*temp) = malloc(sizeof(void *) * newCap);
  assert(temp != 0);
  for (int i = 0; i < sizeCDA(items); i++) { temp[i] = getCDA(items, i); }
  free(items->storage);
  items->storage = temp;
  items->startIndex = 0;
  items->endIndex = sizeCDA(items);
  setCapacityCDA(items, newCap);
}

static void halveArray(CDA * items) {
  int newCap = 0;
  if (getCapacityCDA(items) != 1) {
    newCap = getCapacityCDA(items) / 2;
  }
  else { newCap = getCapacityCDA(items); }
  //int newCap = getCapacityCDA(items) / 2;
  void * (*temp) = malloc(sizeof(void*) * newCap);
  assert(temp != 0);
  for (int i = 0; i < sizeCDA(items); i++) { temp[i] = getCDA(items, i); }
  free(items->storage);
  items->storage = temp;
  items->startIndex = 0;
  items->endIndex = sizeCDA(items);
  setCapacityCDA(items, newCap);
}

static int correctIndex(CDA *items, int oldIndex) {
  int index = (oldIndex + getCapacityCDA(items) + getStartCDA(items)) % getCapacityCDA(items);
  return index;
}

// places value in slot named by given index
// previous item at that slot shifts to the next higher slot (and so on)
// if no room for insertion, array grows by doubling
extern void insertCDA(CDA *items, int index, void *value) {
  assert(index >= 0 && index <= sizeCDA(items));
  // if full, then double
  if (isFull(items) == true) { doubleArray(items); }

  if (index == 0) { // insert at front of CDA
    if (sizeCDA(items) != 0) { items->startIndex = correctIndex(items, -1); }

    if (sizeCDA(items) == 0) { items->endIndex = 1; }

    items->storage[getStartCDA(items)] = value;
    items->size += 1;
  }

  else if (index == sizeCDA(items)) { // insert at back of CDA
    items->storage[getEndCDA(items)] = value;
    items->endIndex = correctIndex(items, sizeCDA(items) + 1);
    items->size += 1;
  }

  else { // insert in the middle of the CDA
    int decisionPt = sizeCDA(items) / 2; // determines whether array shifts left or right for insertion
    int trueIndex = correctIndex(items, index);
    if (index <= decisionPt) { // shift left
      for (int i = 0; i < index; i++) {
        items->storage[correctIndex(items, i - 1)] = items->storage[correctIndex(items, i)];
      }
      items->storage[correctIndex(items, index - 1)] = value;
  	  items->startIndex = (items->startIndex + items->capacity - 1) % items->capacity;
    }
    else { // shift right
      for (int i = items->size - 1; i >= index; i--) {
        items->storage[correctIndex(items, i + 1)] = items->storage[correctIndex(items, i)];
      }
      items->storage[trueIndex] = value;
      items->endIndex = (items->endIndex + 1) % items->capacity;
    }
    items->size += 1;
  }
}

// removes and returns the item named by the given index
// item at the next higher slot shifts to that slot (and so on)
// if ratio of size to capacity < .25 array shrinks by half
// array should never shrink below a capacity of one
extern void *removeCDA(CDA * items, int index) {
  assert(index >= 0 && index <= sizeCDA(items) - 1);
  assert(sizeCDA(items) > 0);
  void * value = getCDA(items, index);

  if (index == 0) {
    items->startIndex = correctIndex(items, 1);
    items->size -= 1;
  }
  else if (index == sizeCDA(items) - 1) {
    items->endIndex = (items->endIndex - 1 + items->capacity) % items->capacity;
    items->size -= 1;
  }
  else {
    int decisionPt = sizeCDA(items) / 2; // determines whether array shifts left or right for removal
    if (index <= decisionPt) { // shift right
      for (int i = index; i > 0; i--) {
        items->storage[correctIndex(items, i)] = items->storage[correctIndex(items, i - 1)];
      }
      items->startIndex = (items->startIndex + 1) % items->capacity;
    }

    else { // shift left
      for (int i = index; i < sizeCDA(items) - 1; i++) {
        items->storage[correctIndex(items, i)] = items->storage[correctIndex(items, i + 1)];
      }
      items->endIndex = (items->endIndex - 1 + items->capacity) % items->capacity;
    }

    items->size -= 1;
  }

  if ((sizeCDA(items)/(double)getCapacityCDA(items)) < .25) {
    halveArray(items);
    if ((sizeCDA(items)/(double)getCapacityCDA(items)) < .25) {
      halveArray(items);
    }
  }

  return value;
}

static int getStartCDA(CDA * items) { return items->startIndex; }

static int getEndCDA(CDA * items) { return items->endIndex; }

static int getIndex(CDA * items, int oldIndex) {
  int trueIndex = correctIndex(items, oldIndex);
  return trueIndex;
}

// takes two arrays and moves all the items in the donor array to the recipient arrays
extern void unionCDA(CDA *recipient, CDA *donor) {
  for (int i = 0; i < sizeCDA(donor); i++) {
    insertCDAback(recipient, getCDA(donor, i));
  }
  donor->size = 0;
  donor->capacity = 1;
  free(donor->storage);
  donor->storage = malloc(sizeof(void *) * 1);
}

// method returns the value at the given index, from user's perspective
// In the user's view, the first item is at index zero, the second item at index 1, and so on
// routine has to translate between the users view
// and the internal view (where the first item can be anywhere in the underlying array)
extern void *getCDA(CDA *items, int index) {
  assert(index >= 0 && index < sizeCDA(items));
  int trueIndex = correctIndex(items, index);
  return items->storage[trueIndex];
}

// updates the value at the given index, from user's perspective
// if given index == size of the array, value is inserted at back of array
// if given index == -1 value is inserted at front of array
extern void *setCDA(CDA *items, int index, void *value) {
  assert(index >= -1 && index <= sizeCDA(items));
  int trueIndex = correctIndex(items, index);
  if (index == sizeCDA(items)) {
    insertCDAback(items, value);
    return 0; /*val;*/
  }
  else if (index == -1) {
    insertCDAfront(items, value);
    return 0; /*val;*/
  }
  else {
    void * val = getCDA(items, trueIndex);
    items->storage[trueIndex] = value;
    return val;
  }
}

// method returns the size of array
extern int sizeCDA(CDA *items) { return items->size; }

// method returns the capacity of array
static int getCapacityCDA(CDA * items) { return items->capacity; }

static void setCapacityCDA(CDA * items, int c) { items->capacity = c; }

extern void displayCDA(CDA *items, FILE *fp) {
  if (sizeCDA(items) == 0) {
    if (items->debugVal > 0) { fprintf(fp, "((%d))", items->capacity); } // empty array and method should display num. empty indeces
    else { fprintf(fp, "()"); } // empty array and method should not display num. empty indeces
  }

  else if (items->displayMethod == 0) {
    if (items->debugVal > 0) { // no display method set and method should display num. empty indeces
      fprintf(fp, "(");
      for (int i = 0; i < sizeCDA(items); i++) {
        fprintf(fp, "@%p,", &items->storage[getIndex(items, i)]); // no set display method forces addresses of each item to be printed
      }
      fprintf(fp, "(%d))", (getCapacityCDA(items) - sizeCDA(items)));
    }
    else { // no display method set and method should not display num. empty indeces
      fprintf(fp, "(");
      for (int i = 0; i < sizeCDA(items); i++) {
        fprintf(fp, "@%p,", &items->storage[getIndex(items, i)]); // no set display method forces addresses of each item to be printed
      }
      fprintf(fp, ")");
    }
  }
  else {
    if (items->debugVal > 0) { // display method set and method should display num. empty indeces
      fprintf(fp, "(");
      for (int i = 0; i < sizeCDA(items); i++) {
        items->displayMethod(getCDA(items, i), fp);
        if (i != (sizeCDA(items))) { fprintf(fp, ","); }
      }
      fprintf(fp, "(%d))", (getCapacityCDA(items) - sizeCDA(items)));
    }
    else { // display method set and method should not display num. empty indeces
      fprintf(fp, "(");
      for (int i = 0; i < sizeCDA(items); i++) {
        items->displayMethod(getCDA(items, i), fp);
        if (i != (sizeCDA(items) - 1)) { fprintf(fp, ","); }
      }
      fprintf(fp, ")");
    }
  }
}

extern int debugCDA(CDA *items, int level) {
  int prevVal = items->debugVal;
  items->debugVal = level;

  return prevVal;
}

extern void freeCDA(CDA *items) {
  if (items->freeMethod != 0) { // individual items are only freed if a freeMethod is set
    for (int i = 0; i < sizeCDA(items); i++) { items->freeMethod(getCDA(items, i)); }
  }
  free(items->storage);
  free(items);
}
