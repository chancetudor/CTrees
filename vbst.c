/*File: vbst.c
 *Author: Emily Turner
 *Implements functions found in vbst.h in order to implement a vanilla binary search tree
 */
#include <stdio.h>
#include <stdlib.h>
#include "vbst.h"
#include "bst.h"

typedef struct vbstValue {
    void *value;
    int freq;
    void(* display)(FILE *, void *);
    int(* compare)(void *, void *);
} vbstValue;

int vbstCompare(void *, void *);
void vbstDisplay(FILE *, void *);
static vbstValue *newVBSTValue(void *, vbst *);

extern vbst *newVBST(void (*display)(FILE *,void *),int (*compare)(void *,void *))
{
    vbst *t = (vbst *)malloc(sizeof(vbst));
    if (t == 0)
    {
        fprintf(stderr,"Error: out of memory\n");
        exit(-1);
    }
    t->tree = newBST(vbstDisplay,vbstCompare);
    t->display = display;
    t->compare = compare;
    t->size = 0;
    t->words = 0;
    return t;
}

extern int findVBST(vbst *t,void *value)
{
    vbstValue *v = newVBSTValue(value, t);
    bstNode *x = findBSTNode(t->tree, v);
    if (x == NULL) return 0;
    else
    {
        v = x->value;
        return v->freq;
    }
}

extern void deleteVBST(vbst *t,void *value)
{
    vbstValue *v = newVBSTValue(value, t);
    bstNode *temp = findBSTNode(t->tree, v);
    if (temp == NULL)
    {
        fprintf(stderr, "Value ");
        v->display(stderr, v->value);
        fprintf(stderr, " not found.\n");
        return;
    }
    t->words--;
    v = temp->value;
    v->freq--;
    if (v->freq == 0)
    {
        temp = swapToLeafBSTNode(temp);
        pruneBSTNode(t->tree, temp);
    }
}

extern void insertVBST(vbst *t,void *value)
{
    t->words++;

    vbstValue *v = newVBSTValue(value, t);
    bstNode *temp = findBSTNode(t->tree, v);
    if (temp == NULL)
    {
        t->size++;
        insertBST(t->tree, v);
    }
    else
    {
        ((vbstValue *) (temp->value))->freq++;
    }
}

extern void statisticsVBST(vbst *t,FILE *fp)
{
    fprintf(fp, "Words/Phrases: %d\n", t->words);
    statisticsBST(t->tree, fp);
}

extern void displayVBST(FILE *fp,vbst *t)
{
    displayBST(fp, t->tree);
}

int vbstCompare(void *a, void *b)
{
    vbstValue *x = a;
    vbstValue *y = b;
    return x->compare(x->value,y->value);
}

void vbstDisplay(FILE *fp, void *n)
{
    vbstValue *v = n;
    v->display(fp, v->value);
    if (v->freq > 1) fprintf(fp, "-%d", v->freq);
}

vbstValue *newVBSTValue(void *v, vbst *t)
{
    vbstValue *x = (vbstValue *)malloc(sizeof(vbstValue));
    if (x == 0)
    {
        fprintf(stderr,"Error: out of memory\n");
        exit(-1);
    }
    x->freq = 1;
    x->value = v;
    x->display = t->display;
    x->compare = t->compare;

    return x;
}

extern int sizeVBST(vbst *t)
{
    return t->size;
}

extern int wordsVBST(vbst *t)
{
    return t->words;
}
