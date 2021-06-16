#include "kdtree.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <string.h>

kdnode *kdnode_newArr(void *data, int dim, int *point) {
    kdnode *ret = malloc(sizeof(kdnode));

    // copy coordinates
    ret->point = malloc(dim * sizeof(int));
    memcpy(ret->point, point, dim * sizeof(int));

    ret->data = data;
    ret->d = dim;
    ret->left = NULL;
    ret->right = NULL;

    return ret;
}

kdnode* kdnode_newDim(void *data, int dim, ...) {
    // copy coordinates
    int *point = malloc(dim * sizeof(int));
    va_list list;
    va_start(list, dim);

    for (int i = 0; i < dim; i++) {
        point[i] = va_arg(list, int);
    }

    va_end(list);

    kdnode *ret = kdnode_newArr(data, dim, point);

    free(point);

    return ret;
}

void kdnode_free(kdnode *root) {
    if (root) {
        kdnode_free(root->left);
        kdnode_free(root->right);

        free(root);
    }
}

void kdnode_freeDeep(kdnode *root) {
    if (root) {
        kdnode_freeDeep(root->left);
        kdnode_freeDeep(root->right);

        free(root->data);
        free(root);
    }
}

kdnode *kdnode_insertArr(kdnode *root, void *data, int depth, int dim, int *point) {
    if (!root) {
        return kdnode_newArr(data, dim, point);
    }

    int currentDepth = depth % dim;

    if (point[currentDepth] < root->point[currentDepth]) {
        root->left = kdnode_insertArr(root->left, data, depth + 1, dim, point);
    }
    else {
        root->right = kdnode_insertArr(root->right, data, depth + 1, dim, point);
    }

    return root;
}

kdnode *kdnode_insertDim(kdnode *root, void *data, int dim, ...) {
    // copy coordinates
    int *point = malloc(dim * sizeof(int));
    va_list list;
    va_start(list, dim);

    for (int i = 0; i < dim; i++) {
        point[i] = va_arg(list, int);
    }

    va_end(list);

    kdnode *ret = kdnode_insertArr(root, data, 0, dim, point);

    free(point);

    return ret;
}


void kdnode_nearestNeighborArr(kdnode *root, int depth, int dim, int *point, kdnode **best, int *bestDistSquared, bounds b) {
    if (!root || distanceToRect(point, b) > *bestDistSquared) {
        return;
    }

    int currentDepth = depth % dim;

    int distSq = pointsDistanceSquared(dim, point, root->point);
    if (distSq < *bestDistSquared) {
        *bestDistSquared = distSq;
        *best = root;
    }

    // visit subtrees in order
    int oldVal;
    if (point[currentDepth] < root->point[currentDepth]) {
        // trim left and visit left
        oldVal = b.max[currentDepth];
        b.max[currentDepth] = root->point[currentDepth];
        kdnode_nearestNeighborArr(root->left, depth + 1, dim, point, best, bestDistSquared, b);
        b.max[currentDepth] = oldVal;

        // trim right and visit right
        oldVal = b.min[currentDepth];
        b.min[currentDepth] = root->point[currentDepth];
        kdnode_nearestNeighborArr(root->right, depth + 1, dim, point, best, bestDistSquared, b);
        b.min[currentDepth] = oldVal;
    }
    else {
        // trim right and visit right
        oldVal = b.min[currentDepth];
        b.min[currentDepth] = root->point[currentDepth];
        kdnode_nearestNeighborArr(root->right, depth + 1, dim, point, best, bestDistSquared, b);
        b.min[currentDepth] = oldVal;

        // trim left and visit left
        oldVal = b.max[currentDepth];
        b.max[currentDepth] = root->point[currentDepth];
        kdnode_nearestNeighborArr(root->left, depth + 1, dim, point, best, bestDistSquared, b);
        b.max[currentDepth] = oldVal;
    }
}

kdnode *kdnode_nearestNeighborDim(kdnode *root, int dim, ...) {
    // setup best value placeholders
    kdnode *best = NULL;
    int bestDist = INT_MAX;

    // setup bounding rectangle
    bounds b = { NULL, NULL, root->d };
    b.max = malloc(root->d * sizeof(int));
    b.min = malloc(root->d * sizeof(int));

    // copy coordinates
    int *point = malloc(dim * sizeof(int));
    va_list list;
    va_start(list, dim);

    // populate arrays
    for (int i = 0; i < dim; i++) {
        point[i] = va_arg(list, int);

        b.max[i] = INT_MAX;
        b.min[i] = INT_MIN;
    }

    va_end(list);
    
    kdnode_nearestNeighborArr(root, 0, dim, point, &best, &bestDist, b);

    free(point);
    free(b.min);
    free(b.max);

    return best;
}

int pointsDistanceSquared(int dim, int *p1, int *p2) {
    int distSquared = 0.0f;

    for (int i = 0; i < dim; i++) {
        int diff = p2[i] - p1[i];
        distSquared += (diff * diff);
    }

    return distSquared;
}

int distanceToRect(int *p, bounds b) {
    int distSquared = 0;

    for (int i = 0; i < b.d; i++) {
        if (p[i] < b.min[i]) {
            int diff = b.min[i] - p[i];
            distSquared += diff * diff;
        }
        else if (p[i] > b.max[i]) {
            int diff = p[i] - b.max[i];
            distSquared += diff * diff;
        }
    }

    return distSquared;
}