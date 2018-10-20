/*File: bst.c
 *Author: Emily Turner
 *Implements functions found in bst.h in order to implement a binary search tree
 */

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "bst.h"

static int isLeftChild(bstNode *);
static int isRoot(bstNode *);
static int isRightChild(bstNode *);
static int isLeaf(bstNode *);
static int findMinDepth(bstNode *);
static int findMaxDepth(bstNode *);
static void displayNode(FILE *, bstNode *, bst *);
static bstNode *newBSTNode(void *);
static void deleteNode(bstNode *n);

static bstNode *newBSTNode(void *value)
{
    bstNode *n = (bstNode *)malloc(sizeof(bstNode));
    if (n == 0)
    {
        fprintf(stderr,"Error: out of memory\n");
        exit(-1);
    }
    n->left = NULL;
    n->right = NULL;
    n->parent = n;
    n->value = value;
    return n;
}

extern bst *newBST(void (*d)(FILE *,void *),int (*c)(void *,void *))
{
    bst *tree = (bst *)malloc(sizeof(bst));
    if (tree == 0)
    {
        fprintf(stderr,"Error: out of memory\n");
        exit(-1);
    }
    tree->root = NULL;
    tree->size = 0;
    tree->display = d;
    tree->compare = c;
    return tree;
}

extern int sizeBST(bst *tree)
{
    return tree->size;
}

extern bstNode *insertBST(bst *tree,void *value)
{
    tree->size++;
    bstNode *n = newBSTNode(value);
    bstNode *temp = tree->root;

    //If the tree is empty set the root, return
    if (temp == NULL)
    {
        tree->root = n;
        return n;
    }
    while (temp)
    {
        n->parent = temp;
        if (tree->compare(temp->value, value) == 1)
        {
            temp = temp->left;
        }
        else
        {
            temp = temp->right;
        }
    }
    if (tree->compare(n->parent->value, value) == 1)
    {
        n->parent->left = n;
    }
    else
    {
        n->parent->right = n;
    }
    return n;
}

extern int findBST(bst *tree,void *value)
{
    if (findBSTNode(tree, value)) return 1;
    return 0;
}

extern bstNode *findBSTNode(bst *tree,void *value)
{
    bstNode *temp = tree->root;
    while (temp && tree->compare(temp->value, value) != 0)
    {
        if (tree->compare(temp->value, value) > 0) temp = temp->left;
        else temp = temp->right;
    }
    return temp;
}

static void deleteNode(bstNode *n)
{
    free(n);
}

extern void pruneBSTNode(bst *tree, bstNode *n)
{
    tree->size--;
    if (isRoot(n))
    {
        tree->root = NULL;
    }
    else
    {
        if (isLeftChild(n)) n->parent->left = NULL;
        else n->parent->right = NULL;
    }
    deleteNode(n);
}

bstNode *swapValues(bstNode *x, bstNode *y)
{
    void *temp = x->value;
    x->value = y->value;
    y->value = temp;

    return y;
}

bstNode *findPredecessor(bstNode *n)
{
    bstNode *temp = n->left;
    while (temp->right) temp = temp->right;
    return temp;
}

bstNode *findSuccessor(bstNode *n)
{
    bstNode *temp = n->right;
    while (temp->left) temp = temp->left;
    return temp;
}

extern bstNode *swapToLeafBSTNode(bstNode *n)
{
    if (n == NULL) return n; //error
    if (isLeaf(n)) return n;
    bstNode *temp;
    if (n->left)
    {
        temp = swapValues(n, findPredecessor(n));
    }
    else
    {
        temp = swapValues(n, findSuccessor(n));
    }
    return swapToLeafBSTNode(temp);
}

int findMinDepth(bstNode *n)
{
    if (n == NULL) return 0;

    int rdepth = findMinDepth(n->right);
    int ldepth = findMinDepth(n->left);

    if (rdepth < ldepth) return rdepth + 1;
    else return ldepth + 1;
}

int findMaxDepth(bstNode *n)
{
    if (n == NULL) return 0;

    int rdepth = findMaxDepth(n->right);
    int ldepth = findMaxDepth(n->left);

    if (rdepth > ldepth) return rdepth + 1;
    else return ldepth + 1;
}

extern void statisticsBST(bst *tree,FILE *fp)
{
    fprintf(fp, "Nodes: %d\n", sizeBST(tree));
    fprintf(fp, "Minimum depth: %d\n", findMinDepth(tree->root));
    fprintf(fp, "Maximum depth: %d\n", findMaxDepth(tree->root));
}

extern void displayBST(FILE *fp,bst *t)
{
    int level = 0;
    queue *tree = newQueue(t->display);
    //If tree is not null its first element is enqueued onto the queue
    fprintf(fp, "%d:", level);
    if (t->root != NULL)
    {
        enqueue(tree, t->root);
        enqueue(tree, NULL);
    }
    while (sizeQueue(tree))
    {
        //The first element in the queue is dequeued and displayed
        bstNode *temp = dequeue(tree);
        if (temp == NULL)
        {
            if (sizeQueue(tree))
            {
                fprintf(fp, "\n");
                fprintf(fp, "%d:", ++level);
                enqueue(tree, NULL);
            }
        }
        else
        {
            fprintf(fp, " ");
            displayNode(fp, temp, t);
            //Its children, if they exist, are enqueued onto the queue, left child first then right child
            if (temp->left) enqueue(tree, temp->left);
            if (temp->right) enqueue(tree, temp->right);
        }
    }
    fprintf(fp, "\n");
}

static void displayNode(FILE *fp, bstNode *n, bst *tree)
{
    if (n == NULL) return; //error
    if (isLeaf(n)) fprintf(fp, "=");
    tree->display(fp, n->value);

    fprintf(fp, "(");
    tree->display(fp, n->parent->value);
    fprintf(fp, ")-");
    if (isLeftChild(n)) fprintf(fp, "l");
    else if (isRightChild(n)) fprintf(fp, "r");
}

static int isRightChild(bstNode *n)
{
    if (n->parent->right == n) return 1;
    return 0;
}

static int isLeftChild(bstNode *n)
{
    if (n->parent->left == n) return 1;
    return 0;
}

static int isLeaf(bstNode *n)
{
    if (!n->left && !n->right) return 1;
    return 0;
}

static int isRoot(bstNode *n)
{
    if (n->parent == n) return 1;
    return 0;
}
