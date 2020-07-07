#ifndef BTREE_HPP
#define BTREE_HPP

#include <vector>
#include <iostream>

namespace btree {
    template <typename V>
    void shiftRight(V* arr, int i, int& size) {
        for (int idx = size - 1; idx >= i; idx--) {
            arr[idx + 1] = arr[idx];
        }
        size++;
    }

    template <typename V>
    void shiftLeft(V* arr, int i, int& size) {
        for (; i < size - 1; i++) {
            arr[i] = arr[i + 1];
        }
        size--;
    }

    template <typename T>
    class node {
    public:
        node<T>** children;		// array of child pointers
        T** data;				// array of data (has keys)

        int m;			// maximum tree degree
        int divisor;	// index divisor
        int n;			// current number of keys
        int min;		// min no keys = ceil(m / 2) - 1

        bool leaf;				// true if no children

        node(int m, bool leaf = true)
            : m(m), divisor(m / 2), min(ceil(m / 2.) - 1), leaf(leaf), n(0) {
            // allocate memory for arrays
            children = new node<T> * [m + 1];
            data = new T * [m];
        }

        node<T>* split(T*& upshiftElement) {
            node<T>* right = new node<T>(m, leaf);

            upshiftElement = data[divisor];
            data[divisor] = nullptr;

            // data
            for (int idx = divisor + 1; idx < n; idx++) {
                T* b = data[idx];
                right->data[idx - divisor - 1] = data[idx];
                right->n++;
                data[idx] = nullptr;
            }
            n -= right->n + 1; // to account for divisor

            // children
            if (!leaf) {
                // m + 1 because overflow creates one additional child
                for (int idx = divisor + 1; idx < m + 1; idx++) {
                    node<T>* b = children[idx];
                    right->children[idx - divisor - 1] = children[idx];
                    children[idx] = nullptr;
                }
            }

            return right;
        }

        node<T>* insert(T* element, T*& upshiftElement) {
            /*
                if full, return branch to right
            */

            /*
                find ordered position for element
                - start from left and iterate until is less than an element
                - its location will be i for data after shift, i for children
            */
            int i = 0;
            while (i < n && element->key > data[i]->key) {
                i++;
            }

            if (leaf) {
                shiftRight<T*>(data, i, n);
                data[i] = element;

                if (n == m) {
                    // too many elements
                    return split(upshiftElement);
                }
            }
            else {
                // pass down to child
                node<T>* right = children[i]->insert(element, upshiftElement);

                if (right) {
                    // child overflowed, returned divisor and right branch

                    // set data
                    shiftRight<T*>(data, i, n);
                    data[i] = upshiftElement;

                    // add child
                    int j = n;
                    shiftRight<node<T>*>(children, i + 1, j);
                    children[i + 1] = right;

                    if (n == m) {
                        // too many elements
                        return split(upshiftElement);
                    }
                }
            }

            upshiftElement = nullptr;
            return nullptr;
        }

        /*
            return cases
            -1: child not found
            0: successful deletion
            1: leaf node, cannot delete because violates min
        */
        int eraseIndex(int i) {
            int childIdx = -1;
            int keyIdx = -1;

            if (leaf) {
                // leaf node
                /*
                    - delete data
                    - determine if need to shift
                */
                data[i] = nullptr;
                //shiftLeft<T*>(data, i, n);
                /*
                    - if doesn't violate min number of keys, successful deletion
                    - else, need to borrow
                */
                return (n > min) ? 0 : 1;
            }
            else {
                if (children[i]->n > min) {
                    // left child can give key
                    /*
                        - get greatest key in left child
                        - delete the greatest key in left child recursively
                        - replace target in this with that key
                    */
                }
                else if (children[i + 1] > min) {
                    // right child can give key
                    /*
                        - get smallest key in right child
                        - delete smallest key in right child recursively
                        - replace target in this with that key
                    */
                }
                else {
                    // both children have min number of keys
                    /*
                        - merge left and right children with target key
                        - left shift children to right
                        - recursively delete target key from merged child
                    */
                }
            }

        childDeletion:
            if (childIdx != -1 && keyIdx != -1) {
                int res = children[childIdx]->eraseIdx(keyIdx);
                switch (res) {
                case -1:
                    return -1;
                case 0:
                    // successful deletion
                    // shift left
                    shiftLeft<T*>(children[childIdx]->data, keyIdx, children[childIdx]->n);
                    return 0;
                case 1:
                    // leaf node violates minimum number of keys
                    /*
                        - if left sibling has more than min, borrow from left
                        - if right sibling has more than min, borrow from right
                        - else merge with left parent and sibling
                        note: no need for recursion because all leaves on same level
                        borrow: take parent from left/right and insert, take smallest/largest key from left/right and move up
                    */

                    int _n = -1;

                    if (childIdx > 0) {
                        _n = children[childIdx - 1]->n;
                    }
                    if (_n > min) {
                        // make space in target
                        //shiftRight<T*>(children[childIdx]->data, 0, children[childIdx]->n);
                        // insert data in parent to target
                        children[childIdx]->data[0] = data[childIdx - 1];
                        // bring data from left child up
                        data[childIdx - 1] = children[childIdx - 1]->data[_n - 1];
                        // remove data from left child
                        children[childIdx - 1]->data[_n - 1] = nullptr;
                        children[childIdx - 1]->n--;
                        return 0;
                    }

                    if (childIdx < m - 1) {
                        _n = children[childIdx + 1];
                    }
                    if (_n > min) {
                        // make space in target
                        // insert data in parent to target
                        children[childIdx]->data[_n] = data[childIdx];
                        // bring data from right child up
                        data[childIdx] = children[childIdx + 1]->data[0];
                        // remove data from right child
                        children[childIdx + 1]->data[0] = nullptr;
                        shiftLeft<T*>(children[childIdx - 1]->data, 0, children[childIdx - 1]->n);
                        return 0;
                    }

                    // merge otherwise
                    // shift left
                    shiftLeft<T*>(children[childIdx]->data, keyIdx, children[childIdx]->n);
                    if (childIdx < m - 1) {
                        // merge with right child
                        _n = children[childIdx]->n++;
                        //children[childIdx]->data[_n - 1] = 
                    }
                    else {
                        // merge with left child
                    }
                }
            }
        }


















































        T* getPredecessor(int idx) {
            node<T>* current = children[idx];
            while (!current->leaf) {
                // get last child
                current = current->children[current->leaf];
            }

            return current->data[current->n - 1];
        }

        T* getSuccessor(int idx) {
            node<T>* current = children[idx];
            while (!current->leaf) {
                // get first child
                current = current->children[0];
            }

            return current->data[0];
        }

        void mergeChildren(int leftIdx) {
            node<T>* child = children[leftIdx];
            node<T>* sibling = children[leftIdx + 1];

            int ogN = child->n;

            // data
            for (int i = 0; i < children[leftIdx + 1]->n; i++) {
                child->data[child->n++] = sibling->data[i];
            }

            // children
            if (!child->leaf) {
                for (int i = 0; i <= sibling->n; i++) {
                    child->children[ogN + i] = sibling->children[i];
                }
            }

            // left shift other children
            int _n = n + 1;
            shiftLeft<node<T>*>(children, leftIdx + 1, _n);
        }

        /*
            return vals
            -1:	child not found
            0:	successful deletion
            1:	cannot delete because violates min
        */
        int erase(int key, int idx = -1) {
            // find location
            bool found = false;
            int i = -1;
            if (idx == -1) {
                i++;
                while (i < n && key >= data[i]->key) {
                    if (key == data[i]->key) {
                        // found element
                        found = true;
                        break;
                    }
                    i++;
                }
            }

            if (found) {
                // key in data
                if (leaf) {
                    // remove item
                    data[i] = nullptr;
                    shiftLeft<T*>(data, i, n);

                    // if violates minimum condition, return that case
                    return n >= min ? 0 : 1;
                }
                else {
                    // left child: children[i], right child: children[i + 1]

                    int _n = children[i]->n;
                    if (_n > children[i]->min) {
                        T* repl = getPredecessor(i);
                        // recursively delete largest key from left
                        children[i]->erase(repl->key);
                        // borrow largest key from left
                        data[i] = repl;
                        return 0;
                    }

                    _n = children[i + 1]->n;
                    if (_n > children[i]->min) {
                        T* repl = getSuccessor(i + 1);
                        // recursively delete smallest key from right
                        children[i]->erase(repl->key);
                        // borrow smallest key from right
                        data[i] = repl;
                        return 0;
                    }

                    // both children have min no keys
                    /*
                        - if more than min keys in this node, merge children
                        - 
                    */
                    if (n > min) {
                        // deletion doesn't violate condition
                        shiftLeft<T*>(data, i, n);
                        mergeChildren(i);
                    }
                    else {

                    }
                }
            }
            else {
                if (leaf) {
                    // no more children to search
                    return -1;
                }

                // goto child
                int res = children[i]->erase(key);
                if (res < 1) {
                    // successful or not found
                    return res;
                }
                else {
                    // TODO
                    return -1;
                }
            }
        }

















































        T* search(int key) {
            int i = 0;
            while (i < n && key >= data[i]->key) {
                if (key == data[i]->key) {
                    // found element
                    return data[i];
                }
                i++;
            }

            // if no children, no other places to look for the element
            return leaf ? nullptr : children[i]->search(key);
        }

        std::vector<T*> traverse() {
            std::vector<T*> ret;
            for (int i = 0; i < n; i++) {
                if (!leaf) {
                    std::vector<T*> res = children[i]->traverse();
                    ret.insert(ret.end(), res.begin(), res.end());
                }
                ret.push_back(data[i]);
            }
            if (!leaf) {
                std::vector<T*> res = children[n]->traverse();
                ret.insert(ret.end(), res.begin(), res.end());
            }

            return ret;
        }

        void clear() {
            for (int i = 0; i < n; i++) {
                if (!leaf) {
                    children[i]->clear();
                    children[i] = nullptr;
                }
                data[i] = nullptr;
            }
            if (!leaf) {
                children[n]->clear();
                children[n] = nullptr;
            }
            children = nullptr;
            data = nullptr;
        }
    };

    template <typename T>
    class BTree {
    public:
        int m;

        node<T>* root;

        BTree(int m)
            : m(m) {
            root = new node<T>(m, true);
            root->min = 1;
        }

        std::vector<T*> traverse() {
            return root->traverse();
        }

        void insert(std::vector<T*> elements) {
            for (T* element : elements) {
                insert(element);
            }
        }

        void insert(T* element) {
            T* upshiftElement = nullptr;
            node<T>* right = root->insert(element, upshiftElement);

            if (right) {
                // need to create new root
                node<T>* newRoot = new node<T>(m, false);

                // set data
                newRoot->data[0] = upshiftElement;
                newRoot->n = 1;

                // place children
                newRoot->children[0] = root;
                newRoot->children[1] = right;

                // set root
                root = newRoot;
            }
        }

        T* search(int key) {
            return root->search(key);
        }

        bool erase(int key) {
            if (root->leaf) {
                // determine if in root data
                for (int i = 0; i < root->n; i++) {
                    if (root->data[i]->key == key) {
                        root->data[i] = nullptr;
                        shiftLeft<T*>(root->data, i, root->n);
                        return true;
                    }
                }
                return false;
            }
            else {
                root->erase(key);
            }
        }
    };
};

#endif