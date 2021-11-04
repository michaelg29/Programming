#include "btree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

btree btree_new(int m)
{
    if (m < BTREE_MIN_M)
    {
        m = BTREE_MIN_M;
    }
    btree ret = { m, ceilf((float)m / 2.0f), NULL };
    return ret;
}

void* btree_search(btree tree, int key)
{
    int idx;
    btree_node* ret = btree_node_search(tree.root, tree, key, &idx);
    if (ret)
    {
        return ret->vals[idx];
    }
}

void btree_insert(btree* tree, int key, void* val)
{
    if (!tree->root)
    {
        tree->root = btree_newNodeItem(*tree, key, val);
    }
    else
    {
        btree_node* ret = btree_node_insert(tree->root, *tree, key, val);

        if (ret)
        {
            tree->root = ret;
        }
    }
}

void btree_delete(btree *tree, int key)
{
    if (tree->root)
    {
        btree_node_delete(tree->root, *tree, key);

        if (!tree->root->n)
        {
            // no more keys, tree will shrink
            btree_node *prevRoot = tree->root;
            tree->root = NULL;
            if (prevRoot->noChildren)
            {
                tree->root = prevRoot->children[0];
            }
            free(prevRoot);
        }
    }
}

void btree_free(btree* tree)
{
    btree_node_free(tree->root, *tree);
}

btree_node* btree_newNode(btree tree)
{
    btree_node* ret = malloc(sizeof(btree_node));

    ret->children = malloc(tree.m * sizeof(void*));
    memset(ret->children, 0, tree.m * sizeof(void*));

    ret->keys = malloc((tree.m - 1) * sizeof(int));
    memset(ret->keys, 0, (tree.m - 1) * sizeof(int));

    ret->vals = malloc((tree.m - 1) * sizeof(void*));
    memset(ret->vals, 0, (tree.m - 1) * sizeof(void*));

    ret->n = 0;
    ret->noChildren = 0;
    return ret;
}

btree_node* btree_newNodeItem(btree tree, int key, void* val)
{
    btree_node* ret = btree_newNode(tree);
    ret->keys[0] = key;
    ret->vals[0] = val;
    ret->n = 1;
    return ret;
}

void btree_moveKeyVal(btree_node* in, int inIdx, btree_node* out, int outIdx)
{
    out->keys[outIdx] = in->keys[inIdx];
    out->vals[outIdx] = in->vals[inIdx];
}

btree_node* btree_node_search(btree_node* root, btree tree, int key, int* idx)
{
    for (*idx = 0; *idx < root->n && key >= root->keys[*idx]; (*idx)++) {
        if (key == root->keys[*idx]) {
            // found element
            return root;
        }
    }

    return root->n ? btree_node_search(root->children[*idx], tree, key, idx) : NULL;
}

btree_node *btree_node_get_inorderSuccessor(btree_node *root, btree tree, int i)
{
   if (root->noChildren) {
       btree_node *current = root->children[i];
       while (current->noChildren)
       {
           // get first child
           current = current->children[0];
       }
       return current;
   }
   else {
       return NULL;
   }
}

btree_node *btree_node_get_inorderPredecessor(btree_node *root, btree tree, int i)
{
    if (root->noChildren) {
       btree_node *current = root->children[i];
       while (current->noChildren)
       {
           // get last child
           current = current->children[current->noChildren - 1];
       }
       return current;
   }
   else {
       return NULL;
   }
}

btree_node* btree_node_split(btree_node* root, btree tree, btree_node* new_node, int i)
{
    int hasChildren = root->noChildren;
    // returns upshift element in new_node

    btree_node* tmp = btree_newNode(tree);
    memcpy(tmp->children, new_node->children, 2 * sizeof(btree_node*));
    // swap new node with corresponding node to be upshifted
    if (i < tree.t - 1)
    {
        // insert before median
        btree_moveKeyVal(root, tree.t - 2, tmp, 0); // upshift element will be one before median (becomes median after shift)

        // right shift to fill gap of removing the median
        for (int j = tree.t - 2; j > i; j--)
        {
            btree_moveKeyVal(root, j - 1, root, j);
        }
        // insert new node
        btree_moveKeyVal(new_node, 0, root, i);
    }
    else if (i > tree.t - 1)
    {
        // insert after median
        btree_moveKeyVal(root, tree.t - 1, tmp, 0);

        // left shift to fill gap of removing the median
        for (int j = tree.t - 1; j < i && j < root->n - 1; j++)
        {
            btree_moveKeyVal(root, j + 1, root, j);
        }
        // insert new node
        btree_moveKeyVal(new_node, 0, root, i - 1);
    }
    else
    {
        // new node will be the upshift element
        btree_moveKeyVal(new_node, 0, tmp, 0);
    }

    // populate new node with proper upshift element
    btree_moveKeyVal(tmp, 0, new_node, 0);
    //btree_node_free(tmp, tree);

    new_node->children[0] = root;
    new_node->children[1] = btree_newNode(tree);
    for (int j = tree.t - 1; j < tree.m - 1; j++)
    {
        btree_moveKeyVal(root, j, new_node->children[1], j - tree.t + 1);
        root->keys[j] = INT_MAX;
        root->vals[j] = NULL;
    }
    if (hasChildren)
    {
        if (i < tree.t - 1)
        {
            // copy children to right side
            for (int j = tree.t - 1; j < tree.m; j++)
            {
                new_node->children[1]->children[j - tree.t + 1] = new_node->children[0]->children[j];
            }
            // right shift children on left side
            for (int j = tree.t - 1; j > i + 1; j--)
            {
                new_node->children[0]->children[j] = new_node->children[0]->children[j - 1];
            }
            // insert extra child on left side
            new_node->children[0]->children[i + 1] = tmp->children[1];
        }
        //else if (i > tree.t)
        else
        {
            // copy children to right side
            for (int j = tree.t; j < tree.m; j++)
            {
                new_node->children[1]->children[j - tree.t] = new_node->children[0]->children[j];
            }
            // right shift children on right side
            for (int j = tree.t - 1; j > i - tree.t + 1; j--)
            {
                new_node->children[1]->children[j] = new_node->children[1]->children[j - 1];
            }
            // insert extra child on left side
            new_node->children[1]->children[i - tree.t + 1] = tmp->children[1];
        }

        new_node->children[0]->noChildren = tree.t;
        new_node->children[1]->noChildren = tree.m - tree.t + 1;
    }

    // update counts
    new_node->children[0]->n = tree.t - 1;
    new_node->children[1]->n = tree.m - tree.t;
    new_node->n = 1;
    new_node->noChildren = 2;
}

btree_node* btree_node_insert(btree_node* root, btree tree, int key, void* val)
{
    // find position
    int i = 0;
    while (i < root->n && key > root->keys[i]) {
        i++;
    }

    btree_node* ret = NULL;
    if (!root->noChildren)
    {
        // leaf node
        if (root->n == tree.m - 1) {
            ret = btree_newNodeItem(tree, key, val);

            // split elements
            btree_node_split(root, tree, ret, i);
        }
        else {
            // simple insert
            // right shift elements after
            for (int j = root->n; j > i; j--) {
                btree_moveKeyVal(root, j - 1, root, j);
            }
            // insert element
            root->keys[i] = key;
            root->vals[i] = val;

            root->n++;
        }
    }
    else {
        // insert into child
        ret = btree_node_insert(root->children[i], tree, key, val);

        if (ret)
        {
            if (root->n == tree.m - 1)
            {
                // split elements
                btree_node_split(root, tree, ret, i);
            }
            else
            {
                // right shift keys to insert upshifted element
                for (int j = root->n - 1; j >= i; j--)
                {
                    btree_moveKeyVal(root, j, root, j + 1);
                }
                btree_moveKeyVal(ret, 0, root, i);

                // right shift children
                for (int j = root->noChildren - 1; j >= i + 1; j--)
                {
                    root->children[j + 1] = root->children[j];
                }
                root->children[i + 1] = ret->children[1];
                root->children[i] = ret->children[0];
                ret = NULL;

                root->n++;
                root->noChildren++;
            }
        }
    }

    return ret;
}

void btree_rebalance(btree_node *root, btree tree, int key)
{
    if (!root->noChildren)
    {
        // rebalancing takes place in parent of target leaf
        return;
    }

    // find position
    int i = 0;
    while (i < root->n && key > root->keys[i]) {
        i++;
    }

    // rebalance starting at child
    btree_rebalance(root->children[i], tree, key);

    if (root->children[i]->n < tree.t - 1)
    {
        /**
         * @brief must rebalance
         * 
         * CASE 1: right sibling exists (i < noChildren - 1), has more than t - 1 keys
         *      Rotate left
         * 
         * CASE 2: left sibling exists (i > 0), has more than t - 1 keys
         *      Rotate right
         * 
         * CASE 3: both siblings only have t - 1 keys
         *      Merge with sibling and parent separator
         */

        /**
         * @brief terminology
         * 
         * Deficient node: root->children[i]
         * 
         * Left separator: root->keys[i - 1]
         * Right separator: root->keys[i]
         * Left sibling: root->children[i - 1]
         * Right sibling: root->children[i + 1]
         */

        if (i < root->noChildren - 1 && root->children[i + 1]->n > tree.t - 1)
        {
            // CASE 1
            
            // copy right separator to end of deficient node
            btree_moveKeyVal(root, i, root->children[i], root->children[i]->n);
            root->children[i]->n++;

            // copy smallest key in right sibling to right separator
            btree_moveKeyVal(root->children[i + 1], 0, root, i);
            // left shift in right sibling to cover up removal
            for (int j = 0; j < root->children[i + 1]->n - 1; j++)
            {
                btree_moveKeyVal(root->children[i + 1], j + 1, root->children[i + 1], j);
            }
            root->children[i + 1]->n--;

            // CHILDREN
            if (root->children[i]->noChildren)
            {
                // move leftmost child of right sibling to end of deficient node
                root->children[i]->children[root->children[i]->noChildren] = root->children[i + 1]->children[0];
                root->children[i]->noChildren++;

                // left shift children on right sibling
                for (int j = 0; j < root->children[i + 1]->noChildren - 1; j++)
                {
                    root->children[i + 1]->children[j] = root->children[i + 1]->children[j + 1];
                }
                root->children[i + 1]->noChildren--;
            }
        }
        else if (i > 0 && root->children[i - 1]->n > tree.t - 1)
        {
            // CASE 2

            // right shift in deficient node to create space
            for (int j = root->children[i]->n - 1; j > 0; j--)
            {
                btree_moveKeyVal(root->children[i], j - 1, root->children[i], j);
            }
            // copy left separator to start of deficient node
            btree_moveKeyVal(root, i - 1, root->children[i], 0);
            root->children[i]->n++;

            // copy largest key in left sibling to left separator
            btree_moveKeyVal(root->children[i - 1], root->children[i - 1]->n - 1, root, i - 1);
            root->children[i - 1]->n--;

            // CHILDREN
            if (root->children[i]->noChildren)
            {
                // right shift children on deficient node
                for (int j = root->children[i]->noChildren; j > 0; j--)
                {
                    root->children[i]->children[j] = root->children[i]->children[j - 1];
                }
                root->children[i]->noChildren++;

                // move rightmost child of left sibling to beginning of deficient node
                root->children[i]->children[0] = root->children[i - 1]->children[root->children[i - 1]->noChildren - 1];
                root->children[i - 1]->noChildren--;
            }
        }
        else
        {
            // CASE 3

            // if i >= 1, has left sibling
            int leftIdx = i > 0 ? i - 1 : i;

            btree_node *left = root->children[leftIdx];
            btree_node *right = root->children[leftIdx + 1];

            // copy separator to end of left merge node
            btree_moveKeyVal(root, leftIdx, left, left->n);
            left->n++;

            // copy elements of right merge node to left
            for (int j = 0; j < right->n; j++)
            {
                btree_moveKeyVal(right, j, left, left->n);
                left->n++;
            }

            // copy children from right merge node to left
            for (int j = 0; j < right->noChildren; j++)
            {
                left->children[left->noChildren] = right->children[j];
                left->noChildren++;
            }

            // left shift keys in root to cover up removal
            for (int j = leftIdx; j < root->n - 1; j++)
            {
                btree_moveKeyVal(root, j + 1, root, j);
            }
            root->n--;

            // free right merge node
            free(right);

            // left shift children to cover up right
            for (int j = leftIdx + 1; j < root->noChildren - 1; j++)
            {
                root->children[j] = root->children[j + 1];
            }
            root->noChildren--;
        }
    }
}

/**
 * @brief 
 * 
 * @param root 
 * @param tree 
 * @param key 
 * @return int -1 if child not found
 *              0 if successfully deleted
 *              1 if cannot delete because violated minimum condition
 */
int btree_node_delete(btree_node *root, btree tree, int key)
{
    /**
     * CASE 1: Key present in leaf node
     *      Delete key/value pair
     * CASE 2: Key present in internal node
     *      Replace key/value pair with inorder successor
     */

    // find position
    int i = 0;
    char found = 0;
    while (i < root->n && key >= root->keys[i]) {
        if (root->keys[i] == key)
        {
            found = 1;
            break;
        }
        i++;
    }

    if (found)
    {
        if (!root->noChildren)
        {
            // left shift to cover up
            for (int j = i; j < root->n - 1; j++)
            {
                btree_moveKeyVal(root, j + 1, root, j);
            }
            root->n--;

            if (root->n < tree.t - 1)
            {
                // must rebalance starting from root
                btree_rebalance(tree.root, tree, root->keys[0]);
            }
        }
        else
        {
            // get inorder successor
            btree_node *predecessor = btree_node_get_inorderPredecessor(root, tree, i);

            // replace with largest value in predecessor
            btree_moveKeyVal(predecessor, --predecessor->n, root, i);

            if (predecessor->n < tree.t - 1)
            {
                // must rebalance starting from successor
                btree_rebalance(tree.root, tree, predecessor->keys[0]);
            }
        }
    }
    else
    {
        if (root->noChildren)
        {
            return btree_node_delete(root->children[i], tree, key);
        }
    }

    return 0;
}

void btree_node_free(btree_node* root, btree tree)
{
    if (root)
    {
        for (int i = 0; i < root->noChildren; i++)
        {
            btree_node_free(root->children[i], tree);
        }
        free(root->children);

        free(root->keys);
        free(root->vals);
        free(root);
    }
}