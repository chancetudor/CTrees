/*File: rbt.c
 *Author: Emily Turner
 *Implements functions found in rbt.h in order to implement a red black tree
 */
#include <stdio.h>
#include <stdlib.h>
#include "bst.h"
#include "rbt.h"

typedef struct rbtValue {
    void *value;
    int freq;
    int color;
    void(* display)(FILE *, void *);
    int(* compare)(void *, void *);
} rbtValue;

static int RBTCompare(void *a, void *b);
static void RBTDisplay(FILE *fp, void *n);
static rbtValue *newRBTValue(void *v, rbt *t);
static void insertionFixUp(bstNode *n, bst *tree);
static int isBlack(bstNode *n);
static int isRed(bstNode *n);
static void colorBlack(bstNode *n);
static void colorRed(bstNode *n);
static void rightRotate(bstNode *n, bst *tree);
static void leftRotate(bstNode *n, bst *tree);
static bstNode *grandparent(bstNode *n);
static bstNode *parent(bstNode *n);
static bstNode *uncle(bstNode *n);
static int linearWithParent(bstNode *n);
static int isLeftChild(bstNode *);
static int isRoot(bstNode *);
static int isRightChild(bstNode *);

extern rbt *newRBT(void (*display)(FILE *,void *),int (*compare)(void *,void *))
{
    rbt *t = (rbt *)malloc(sizeof(rbt));
    if (t == 0)
    {
        fprintf(stderr,"Error: out of memory\n");
        exit(-1);
    }
    t->tree = newBST(RBTDisplay,RBTCompare);
    t->display = display;
    t->compare = compare;
    t->size = 0;
    t->words = 0;
    return t;
}

extern int findRBT(rbt *t,void *value)
{
    rbtValue *v = newRBTValue(value, t);
    bstNode *x = findBSTNode(t->tree, v);
    if (x == NULL) return 0;
    v = x->value;
    return v->freq;
}

static int RBTCompare(void *a, void *b)
{
    rbtValue *x = a;
    rbtValue *y = b;
    return x->compare(x->value,y->value);
}

static void insertionFixUp(bstNode *n, bst *tree)
{
    while (1)
    {
        if (isRoot(n)) break;
        if (isBlack(parent(n))) return;
        if (isRed(uncle(n)))
        {
            colorBlack(parent(n));
            colorBlack(uncle(n));
            colorRed(grandparent(n));
            n = grandparent(n);
        }
        else
        {
            if (!linearWithParent(n))
            {
                if (isRightChild(n))
                {
                    leftRotate(n, tree);
                    n = n->left;
                }
                else
                {
                    rightRotate(n, tree);
                    n = n->right;
                }
            }
            colorBlack(parent(n));
            colorRed(grandparent(n));
            if (isLeftChild(n->parent))
            {
                rightRotate(n->parent, tree);
            }
            else
            {
                leftRotate(n->parent, tree);
            }
            break;
        }
    }
    if (isRed(tree->root)) colorBlack(tree->root);
}

static void RBTDisplay(FILE *fp, void *n)
{
    rbtValue *v = n;
    v->display(fp, v->value);
    if (v->freq > 1) fprintf(fp, "-%d", v->freq);
    if (v->color == 1) fprintf(fp, "-B");
    else fprintf(fp, "-R");
}

static rbtValue *newRBTValue(void *v, rbt *t)
{
    rbtValue *x = (rbtValue *)malloc(sizeof(rbtValue));
    if (x == 0)
    {
        fprintf(stderr,"Error: out of memory\n");
        exit(-1);
    }
    x->color = 0; //set color to red
    x->freq = 1;
    x->value = v;
    x->display = t->display;
    x->compare = t->compare;

    return x;
}

extern void insertRBT(rbt *t,void *value)
{
    t->words++;

    rbtValue *v = newRBTValue(value, t);
    bstNode *temp = findBSTNode(t->tree, v);
    if (temp == 0)
    {
        t->size++;
        temp = insertBST(t->tree, v);
        insertionFixUp(temp, t->tree);
    }
    else
    {
        ((rbtValue *) (temp->value))->freq++;
    }
    /*if (findBST(t->tree, v))
    {
        bstNode *temp = findBSTNode(t->tree, v);
        ((rbtValue *) (temp->value))->freq++;
    }
    else
    {
        t->size++;
        bstNode *temp = insertBST(t->tree, v);
        insertionFixUp(temp, t->tree);
    }*/
}

extern void deleteRBT(rbt *t,void *value)
{
    rbtValue *v = newRBTValue(value, t);
    rbtValue *rv = t->tree->root->value;
    fprintf(stderr, "Error: cannot delete value ");
    v->display(stderr, v->value);
    fprintf(stderr, " from tree rooted at ");
    rv->display(stderr, rv->value);
    fprintf(stderr, "\n");
}

extern void statisticsRBT(rbt *t,FILE *fp)
{
    fprintf(fp, "Words/Phrases: %d\n", wordsRBT(t));
    statisticsBST(t->tree, fp);
}

extern void displayRBT(FILE *fp,rbt *t)
{
    displayBST(fp, t->tree);
}

static int isBlack(bstNode *n)
{
    if (n)
    {
        rbtValue *v = n->value;
        return v->color;
    }
    return 1;
}

static int isRed(bstNode *n)
{
    if (n)
    {
        rbtValue *v = n->value;
        if (v->color == 0) return 1;
    }
    return 0;
}

static void colorBlack(bstNode *n)
{
    rbtValue *v = n->value;
    v->color = 1;
}

static void colorRed(bstNode *n)
{
    rbtValue *v = n->value;
    v->color = 0;
}

static void rightRotate(bstNode *n, bst *tree)
{
    bstNode *oldParent = parent(n);
    if (isRoot(oldParent))
    {
        tree->root = n;
    }
    else if (isLeftChild(oldParent))
    {
        grandparent(n)->left = n;
    }
    else
    {
        grandparent(n)->right = n;
    }
    if (isRoot(oldParent)) n->parent = n;
    else n->parent = grandparent(n);
    oldParent->left = n->right;
    if (oldParent->left)
    {
        oldParent->left->parent = oldParent;
    }
    n->right = oldParent;
    oldParent->parent = n;
}

static void leftRotate(bstNode *n, bst *tree)
{
    bstNode *oldParent = n->parent;
    if (isRoot(oldParent))
    {
        tree->root = n;
    }
    else if (isLeftChild(oldParent))
    {
        grandparent(n)->left = n;
    }
    else
    {
        grandparent(n)->right = n;
    }
    if (isRoot(oldParent)) n->parent = n;
    else n->parent = grandparent(n);
    oldParent->right = n->left;
    if (oldParent->right)
    {
        oldParent->right->parent = oldParent;
    }
    n->left = oldParent;
    oldParent->parent = n;
}

static int isLeftChild(bstNode *n)
{
    if (n->parent->left == n) return 1;
    return 0;
}

static int isRightChild(bstNode *n)
{
    if (n->parent->right == n) return 1;
    return 0;
}

static bstNode *grandparent(bstNode *n)
{
    return n->parent->parent;
}
static bstNode *parent(bstNode *n)
{
    return n->parent;
}
static bstNode *uncle(bstNode *n)
{
    if (isLeftChild(n->parent)) return grandparent(n)->right;
    else return grandparent(n)->left;
}

static int linearWithParent(bstNode *n)
{
    if ((isRightChild(n) && isRightChild(parent(n))) || (isLeftChild(n) && isLeftChild(parent(n)))) return 1;
    else return 0;
}

static int isRoot(bstNode *n)
{
    if (n->parent == n) return 1;
    return 0;
}

extern int sizeRBT(rbt *t)
{
    return t->size;
}
extern int wordsRBT(rbt *t)
{
    return t->words;
}
