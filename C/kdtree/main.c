#include "kdtree.h"
#include <stdio.h>

typedef struct test {
    int p1;
    int p2;
} test;

#define N 7

void printNode(kdnode *root) {
    test *data = root->data;
    test *left = NULL;
    test *right = NULL;
    if (root->left) {
        left = root->left->data;
    }
    if (root->right) {
        right = root->right->data;
    }

    printf("(%d, %d); L: (%d, %d); R: (%d, %d);\n",
        data->p1, data->p2,
        left ? left->p1 : 0, left ? left->p2 : 0,
        right ? right->p1 : 0, right ? right->p2 : 0);
    if (root->left) {
        printNode(root->left);
    }
    if (root->right) {
        printNode(root->right);
    }
}

int main() {
    printf("Hello, world!\n");

    test arr[N] = {
        {3,6},
        {17,15},
        {13,15},
        {6,12},
        {9,1},
        {2,7},
        {10,19}
    };

    kdnode *root = NULL;
    for (int i = 0; i < N; i++) {
        root = kdnode_insert(root, arr + i, arr[i].p1, arr[i].p2);
    }

    printNode(root);

    kdnode *best = kdnode_nearestNeighbor(root, 15, 14);
    test *bestData = best->data;
    printf("Best: (%d, %d)\n", bestData->p1, bestData->p2);

    kdnode_free(root);

    return 0;
}