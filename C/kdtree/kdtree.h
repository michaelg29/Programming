#ifndef KDTREE_H
#define KDTREE_H

#define NUMARGS(type, ...) (sizeof((type[]){0, ##__VA_ARGS__}) / sizeof(type) - 1)

typedef struct kdnode {
    int *point;
    void *data;
    int d;
    struct kdnode *left;
    struct kdnode *right;
} kdnode;

typedef struct {
    int *min;
    int *max;
    int d;
} bounds;

kdnode* kdnode_newArr(void *data, int dim, int *point);
kdnode* kdnode_newDim(void *data, int dim, ...);
#define kdnode_new(data, ...) kdnode_newDim(data, NUMARGS(int, __VA_ARGS__), ##__VA_ARGS__);

void kdnode_free(kdnode *root);
void kdnode_freeDeep(kdnode *root);

kdnode *kdnode_insertArr(kdnode *root, void *data, int depth, int dim, int *point);
kdnode *kdnode_insertDim(kdnode *root, void *data, int dim, ...);
#define kdnode_insert(root, data, ...) kdnode_insertDim(root, data, NUMARGS(int, __VA_ARGS__), ##__VA_ARGS__)

void kdnode_nearestNeighborArr(kdnode *root, int depth, int dim, int *point, kdnode **best, int *bestDistSquared, bounds b);
kdnode *kdnode_nearestNeighborDim(kdnode *root, int dim, ...);
#define kdnode_nearestNeighbor(root, ...) kdnode_nearestNeighborDim(root, NUMARGS(int, __VA_ARGS__), ##__VA_ARGS__)

int pointsDistanceSquared(int dim, int *p1, int *p2);
int distanceToRect(int *p, bounds b);

#endif