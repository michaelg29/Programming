#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "btree.h"

void testSearch();
void testInsert();
void testDelete();

void printBtree(btree_node* root)
{
    if (root)
    {
        printf("( ");
        for (int i = 0; i < root->n; i++) {
            if (root->noChildren) {
                printBtree(root->children[i]);
            }
            printf(" %d ", root->keys[i]);
        }
        if (root->noChildren)
        {
            printBtree(root->children[root->n]);
        }
        printf(" )");
    }
    else
    {
        printf("<>");
    }
    

    
}

int main()
{
    printf("Hello, world!\n");

    testDelete();

    return 0;
}

void testSearch()
{
    btree tree = btree_new(4);
    tree.root = btree_newNode(tree);
    tree.root->keys[0] = (int)'e'; tree.root->vals[0] = (void*)(int)'e';
    tree.root->keys[1] = (int)'k'; tree.root->vals[1] = (void*)(int)'k';
    tree.root->keys[2] = (int)'p'; tree.root->vals[2] = (void*)(int)'p';
    tree.root->n = 3;
    tree.root->noChildren = 4;

    tree.root->children[0] = btree_newNode(tree);
    tree.root->children[0]->keys[0] = (int)'b'; tree.root->children[0]->vals[0] = (void*)(int)'b';
    tree.root->children[0]->keys[1] = (int)'d'; tree.root->children[0]->vals[1] = (void*)(int)'d';
    tree.root->children[0]->n = 2;

    tree.root->children[1] = btree_newNode(tree);
    tree.root->children[1]->keys[0] = (int)'g'; tree.root->children[1]->vals[0] = (void*)(int)'g';
    tree.root->children[1]->n = 1;

    tree.root->children[2] = btree_newNode(tree);
    tree.root->children[2]->keys[0] = (int)'m'; tree.root->children[2]->vals[0] = (void*)(int)'m';
    tree.root->children[2]->keys[1] = (int)'n'; tree.root->children[2]->vals[1] = (void*)(int)'n';
    tree.root->children[2]->n = 2;

    tree.root->children[3] = btree_newNode(tree);
    tree.root->children[3]->keys[0] = (int)'q'; tree.root->children[3]->vals[0] = (void*)(int)'q';
    tree.root->children[3]->keys[1] = (int)'t'; tree.root->children[3]->vals[1] = (void*)(int)'t';
    tree.root->children[3]->keys[2] = (int)'w'; tree.root->children[3]->vals[2] = (void*)(int)'w';
    tree.root->children[3]->n = 3;

    printBtree(tree.root);
    printf("\n");

    btree_free(&tree);
}

void testInsert()
{
    btree tree = btree_new(3);

#define LEN 16
    int elements[LEN] = { 25, 8, 37, 55, 95, 27, 88, 13, 29, 42, 51, 72, 100, 105, 90, 92 };

    int i;
    for (i = 0; i < 9 - 1; i++)
    {
        btree_insert(&tree, elements[i], NULL);
        printf("+%04d: ", elements[i]);
        printBtree(tree.root);
        printf("\n");
    }

    btree_insert(&tree, elements[i], NULL);
    printf("+%04d: ", elements[i]);
    printBtree(tree.root);
    printf("\n");

    btree_free(&tree);
}

void testDelete()
{
    btree tree = btree_new(3);

    #define INSERTLEN 20
    int insertElements[INSERTLEN] = {25, 8, 37, 55, 95, 27, 88, 13, 29, 42, 51, 72, 100, 105, 90, 92, 50, 49, 45, 47};

    int i;
    for (i = 0; i < INSERTLEN; i++)
    {
        btree_insert(&tree, insertElements[i], NULL);
    }
    printBtree(tree.root);
    printf("\n");

    #define DELETELEN 16
    int deleteElements[DELETELEN] = {27, 29, 88, 100, 51, 90, 37, 95, 72, 42, 92, 8, 55, 13, 105, 25};
    for (i = 0; i < DELETELEN - 1; i++)
    {
        btree_delete(&tree, deleteElements[i]);
        printf("-%04d: ", deleteElements[i]);
        printBtree(tree.root);
        printf("\n");
    }

    btree_delete(&tree, deleteElements[i]);
    printf("-%04d: ", deleteElements[i]);
    printBtree(tree.root);
    printf("\n");

    btree_free(&tree);
}