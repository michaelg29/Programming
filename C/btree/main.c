#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "btree.h"

void testSearch();
void testInsert();

void printBtree(btree_node* root)
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

int main()
{
    printf("Hello, world!\n");

    testInsert();

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
    btree tree = btree_new(5);

#define LEN 17
    int elements[LEN] = { 200, 0, 1000, 2000, 600, 5000, 6000, 3500, 2100, 2090, 2080, 2010, 2020, 2005, 2040, 2050, 2030 };

    int i;
    for (i = 0; i < LEN - 1; i++)
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

//void test()
//{
//    int i;
//    btree tree;
//    btree_node* root;
//    int key;
//    void* val;
//    btree_node* ret;
//
//    // full branch, split
//            // determine what element is moved up and insert key as needed
//    if (i < tree.t)
//    {
//        // key inserted before median (account for shift)
//        ret = btree_newNodeItem(tree, root->keys[tree.t - 1], root->vals[tree.t - 1]);
//
//        // right shift to fill gap of removing the median
//        for (int j = tree.t; j > i; j--)
//        {
//            btree_moveKeyVal(root, j - 1, root, j);
//        }
//        // insert new element
//        root->keys[i] = key;
//        root->vals[i] = val;
//    }
//    else if (i == tree.t)
//    {
//        // key will be the upshift element
//        ret = btree_newNodeItem(tree, key, val);
//    }
//    else
//    {
//        ret = btree_newNodeItem(tree, root->keys[tree.t], root->vals[tree.t]);
//
//        // left shift to fill gap of removing the median
//        for (int j = tree.t; j < i && j < root->n - 1; j++)
//        {
//            btree_moveKeyVal(root, j + 1, root, j);
//        }
//        // insert new element
//        root->keys[i - 1] = key;
//        root->vals[i - 1] = val;
//    }
//
//    // split elements
//    ret->children[0] = root;
//    ret->children[1] = btree_newNode(tree);
//    for (int j = tree.t; j < tree.m - 1; j++)
//    {
//        btree_moveKeyVal(root, j, ret->children[1], j - tree.t);
//        //root->keys[j] = INT_MAX;
//        root->vals[j] = NULL;
//    }
//
//    // update counts
//    ret->children[0]->n = tree.t;
//    ret->children[1]->n = tree.m - tree.t - 1;
//    ret->n = 1;
//    ret->noChildren = 2;
//}