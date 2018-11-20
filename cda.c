#include "cda.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct cda {
    int curSize;
    int capacity;
    int start;
    int end;
    int debug;
    void **array;
    void(*display)(void *v, FILE *fp);
    void(*free)(void *v);
};

CDA *newCDA(void) {
    CDA *items = malloc(sizeof(CDA));
    assert(items != 0);
    items->array = malloc(sizeof(void *));
    items->curSize = 0;
    items->capacity = 1;
    items->start = 0;
    items->end = 0;
    items->display = 0;
    items->free = 0;
    items->debug = 0;
    return items;
}

void setCDAdisplay(CDA *items, void(*d)(void *, FILE *)) {
    items->display = d;
    return;
}

void setCDAfree(CDA *items, void(*f)(void *)) {
    items->free = f;
    return;
}

int translate(CDA *items, int i) {
    return (items->start + items->capacity + i) % items->capacity;
}

void customRealloc(CDA *items, int newSize) {
    void **temp = items->array;
    items->array = malloc(sizeof(void *) * newSize);
    assert(items->array != 0);
    for (int i = 0; i < items->curSize; ++i)
        items->array[i] = temp[translate(items, i)];// [(items->start + i) % items->capacity];
    items->start = 0;
    items->end = items->curSize;
    free(temp);
    return;
}

void insertCDA(CDA *items, int index, void *value) {
    assert(items->array != 0);
    assert(index >= 0 && index <= items->curSize);
    if (items->curSize >= items->capacity || items->capacity == 1) { //dynamic allocation
        customRealloc(items, (items->capacity * 2));
        items->capacity *= 2;
    }
    if (items->curSize == 0) { //first element
        items->array[0] = value;
        items->start = 0;
        items->end = 1;
    }
    else if (index == 0) { //insertFront
        items->start = ((items->start - 1) + items->capacity) % items->capacity;
        items->array[items->start] = value;
    }
    else if (index == items->curSize) { //insertBack
        items->array[items->end] = value;
        items->end = (items->end + 1) % items->capacity;
    }
    else if (index > 0 && index < items->curSize) { //insertMiddle
        if (index > items->curSize / 2) { //shift right
            for (int i = items->curSize - 1; i >= index; --i)
                items->array[translate(items, i + 1)] = items->array[translate(items, i)];
            setCDA(items, index, value);
            items->end = (items->end + 1) % items->capacity;
        }
        else { //shift left
            for (int i = 0; i < index; ++i)
                items->array[translate(items, i - 1)] = items->array[translate(items, i)];
            setCDA(items, index - 1, value);
            items->start = (items->start + items->capacity - 1) % items->capacity;
        }
    }
    items->curSize++;
    return;
}

void *removeCDA(CDA *items, int index) {
    assert(items->curSize != 0);
    assert(index >= 0 && index <= (items->curSize - 1));
    //int newI = (items->start + index) % items->capacity;
    void *removed = getCDA(items, index);// items->array[newI];
    if (items->curSize == 1)
        items->start = items->end = 0;
    else if (index == 0) //removeFront WRONG PERSPECTIVE, CHANGED TO 0
        items->start = (items->start + 1) % items->capacity;
    else if (index == items->curSize - 1) //removeBack
        items->end = ((items->end - 1) + items->capacity) % items->capacity;
    else if (index > 0 && index < items->curSize - 1) { //removeMiddle
        if (index > items->curSize / 2) { //shift from the right
            for (int i = index; i < items->curSize - 1; ++i)
                items->array[translate(items, i)] = items->array[translate(items, i + 1)];
            items->end = ((items->end - 1) + items->capacity) % items->capacity;
        }
        else { //shift form the left
            for (int i = index; i > 0; --i)
                items->array[translate(items, i)] = items->array[translate(items, i - 1)];
            items->start = (items->start + 1) % items->capacity;
        }
    }
    items->curSize--;
    if (items->curSize == 0) { //added this if statement
        customRealloc(items, 1);
        items->capacity = 1;
    }
    if ((items->curSize) * 4 < items->capacity && items->capacity != 1) {
        customRealloc(items, items->capacity / 2); //changed order of this line and the one below
        items->capacity /= 2;
    }
    return removed;
}

void unionCDA(CDA *recipient, CDA *donor) {
    //customRealloc(donor, donor->curSize);
    //void **temp = donor->array;
    int j = donor->curSize;
    for (int i = 0; i < j; ++i)
        insertCDAback(recipient, getCDA(donor, i));
    free(donor->array);
    donor->array = malloc(sizeof(void *));
    donor->curSize = 0;
    donor->capacity = 1;
    return;
}

void *getCDA(CDA *items, int index) {
    assert(index >= 0 && index < items->capacity);
    return items->array[(items->start + index + items->capacity) % items->capacity];
}

void *setCDA(CDA *items, int index, void *value) {
    assert(index >= -1 && index <= items->capacity);
    void *replaced = 0;
    if (index == -1) insertCDAfront(items, value);
    else if (index == items->capacity) insertCDAback(items, value);
    else {
        replaced = items->array[(items->start + index) % items->capacity];
        items->array[(items->start + index + items->capacity) % items->capacity] = value;
    }
    return replaced;
}

void **extractCDA(CDA *items) {
    customRealloc(items, items->curSize);
    void **returned = items->array;
    items->array = malloc(sizeof(void *));
    items->curSize = 0;
    items->capacity = 1;
    return returned;
}

int sizeCDA(CDA *items) {
    return items->curSize;
}

void displayCDA(CDA *items, FILE *fp) {
    fprintf(fp, "(");
    for (int i = 0; i < items->curSize; ++i) {
        items->display ?
            items->display(items->array[(items->start + i) % items->capacity], fp):
            fprintf(fp, "@%p", items->array[(items->start + i) % items->capacity]);
        if (i != items->curSize - 1)
            fprintf(fp, ",");
    }
    if (items->debug)
        items->curSize ?
        fprintf(fp, ",(%d)", items->capacity - items->curSize) :
        fprintf(fp, "(%d)", items->capacity - items->curSize);
    fprintf(fp, ")");
    return;
}

int debugCDA(CDA *items, int level) {
    int previous = items->debug;
    items->debug = level;
    return previous;
}

void freeCDA(CDA *items) {
    if (items->free)
        for (int i = 0; i < items->curSize; ++i)
            items->free(items->array[(items->start + i) % items->capacity]);
    free(items->array);
    free(items);
    return;
}
