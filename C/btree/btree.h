#ifndef BTREE_H
#define BTREE_H

#define BTREE_MIN_M 3

typedef struct btree_node {
    int n;
    int noChildren;
    int *keys;
    void **vals;
    struct btree_node **children;
} btree_node;

typedef struct {
    int key;
    void *val;
} keyValPair;

typedef struct btree {
    int m;
    int t;

    btree_node *root;
} btree;

// BTREE============================
btree btree_new(int m);

void *btree_search(btree tree, int key);

void btree_insert(btree *tree, int key, void *val);
void btree_delete(btree *tree, int key);

void btree_free(btree *b);

// BTREE_NODE==========================
btree_node *btree_newNode(btree tree);
btree_node *btree_newNodeItem(btree tree, int key, void *val);

void btree_moveKeyVal(btree_node *in, int inIdx, btree_node *out, int outIdx);

btree_node *btree_node_search(btree_node *root, btree tree, int key, int *idx);

keyValPair btree_node_get_inorderSuccessor(btree_node *root, btree tree, int i);
keyValPair btree_node_get_inorderPredecessor(btree_node *root, btree tree, int i);

btree_node *btree_node_split(btree_node *root, btree tree, btree_node *new_node, int i);
btree_node *btree_node_insert(btree_node *root, btree tree, int key, void *val);

void btree_node_merge_children(btree_node *root, btree tree, int leftChild);
void btree_node_borrow(btree_node *root, btree tree, int base, int offset);
int btree_node_delete(btree_node *root, btree tree, int key);

void btree_node_free(btree_node *root, btree tree);

#endif