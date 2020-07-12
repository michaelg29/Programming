#ifndef BTREE_HPP
#define BTREE_HPP

namespace btree {
    template <typename T>
    void shiftRight(T* arr, int i, int& size) {
        for (int idx = size - 1; idx >= i; idx--) {
            arr[idx + 1] = arr[idx];
        }
        size++;
    }

    template <typename T>
    void shiftLeft(T* arr, int i, int& size) {
        for (; i < size - 1; i++) {
            arr[i] = arr[i + 1];
        }
        size--;
    }

    template <typename T>
    class node {
    public:
        node<T>** children;	// array of child pointers
        T** data;       // array of data (has keys)

        int m;                  // tree degree
        int n;                  // current number of keys
        int min;                // min no keys = ceil(m / 2) - 1

        bool leaf;              // true if no children

        node(int m, bool leaf = true)
            : m(m), min(ceil(m / 2.) - 1), leaf(leaf), n(0) {
            // allocate memory for arrays
            children = new node<T> * [m + 1];
            data = new T * [m];
        }

        node<T>* insert(T* element, T*& upshiftElement) {
            /*
                if overflow, return new created child and update reference for new parent
            */

            // find ordered position for element
            int i = 0;
            while (i < n && element->key > data[i]->key) {
                i++;
            }

            if (leaf) {
                // allocate space and assign
                shiftRight<T*>(data, i, n);
                data[i] = element;

                if (n == m) {
                    // overflow
                    return split(upshiftElement);
                }
            }
            else {
                // pass down to child
                node<T>* right = children[i]->insert(element, upshiftElement);

                if (right) {
                    // overflow in child, returned min and right branch

                    // set data
                    shiftRight<T*>(data, i, n);
                    data[i] = upshiftElement;

                    // add child
                    int _n = n;
                    shiftRight<node<T>*>(children, i + 1, _n);
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

        T*& search(int key) {
            int i = 0;
            for (; i < n && key >= data[i]->key; i++) {
                if (key == data[i]->key) {
                    // found element
                    return data[i];
                }
            }

            // if no children, no other places to look
            return leaf ? nullptr : children[i]->search(key);
        }

        void traverse(void(itemViewer)(T* data)) {
            for (int i = 0; i < n; i++) {
                if (!leaf) {
                    // has children
                    children[i]->traverse(itemViewer);
                }
                itemViewer(data[i]);
            }
            if (!leaf) {
                // get rightmost child
                children[n]->traverse(itemViewer);
            }
        }

        /*
            return vals
            -1:	child not found
            0:	successful deletion
            1:	cannot delete because violates min
        */
        int erase(int key) {
            // find location
            bool found = false;
            int i = 0;
            while (i < n && key >= data[i]->key) {
                if (key == data[i]->key) {
                    // found element
                    found = true;
                    break;
                }
                i++;
            }

            if (found) {
                // key in data
                if (leaf) {
                    // remove item
                    data[i] = nullptr;
                    shiftLeft<T*>(data, i, n);
                }
                else {
                    // left child: children[i], right child: children[i + 1]

                    // substitute from left
                    int _n = children[i]->n;
                    if (_n > children[i]->min) {
                        T* repl = getPredecessor(i);
                        // recursively delete largest key from left
                        children[i]->erase(repl->key);
                        // borrow largest key from left
                        data[i] = repl;
                        return 0;
                    }

                    // substitute from right
                    _n = children[i + 1]->n;
                    if (_n > children[i]->min) {
                        T* repl = getSuccessor(i + 1);
                        // recursively delete smallest key from right
                        children[i + 1]->erase(repl->key);
                        // borrow smallest key from right
                        data[i] = repl;
                        return 0;
                    }

                    // both children have min no keys (no possible substitution)
                    data[i] = nullptr;
                    shiftLeft<T*>(data, i, n);
                    mergeChildren(i);
                }

                // if violates minimum condition, return that case
                return n >= min ? 0 : 1;
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
                    // deletion in child violates minimum condition
                    /*
                        - look to its siblings, if either sibling has more than min, borrow from it
                        - else, merge with sibling and parent

                        will have left sibling if i > 0
                        will have right sibling if i < n - 1
                    */

                    bool hasLeft = i > 0;
                    bool hasRight = i < n + 1; // + 1 because data already removed

                    if (hasLeft && children[i - 1]->n > children[i]->min) {
                        borrow(i, true);
                        return 0;
                    }

                    if (hasRight && children[i + 1]->n > children[i]->min) {
                        borrow(i, false);
                        return 0;
                    }

                    // merge with sibling and parent
                    if (hasLeft) {
                        // parent is data[i - 1]

                        // insert into target child
                        shiftRight<T*>(children[i]->data, 0, children[i]->n);
                        children[i]->data[0] = data[i - 1];

                        mergeChildren(i - 1);
                        shiftLeft(data, i - 1, n);

                        return n >= min ? 0 : 1;
                    }
                    if (hasRight) {
                        // parent is data[i]

                        // insert into right sibling
                        shiftRight<T*>(children[i + 1]->data, 0, children[i + 1]->n);
                        children[i + 1]->data[0] = data[i];

                        mergeChildren(i);
                        shiftLeft<T*>(data, i, n);
                       
                        return n >= min ? 0 : 1;
                    }
                }

                return -1;
            }
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

    private:
        node<T>* split(T*& upshiftElement) {
            node<T>* right = new node<T>(m, leaf);

            upshiftElement = data[min];
            data[min] = nullptr;

            // data
            for (int idx = min + 1; idx < n; idx++) {
                right->data[idx - min - 1] = data[idx];
                data[idx] = nullptr;
                right->n++;
            }
            n -= right->n + 1;  // +1 to account for divisor

            // children
            if (!leaf) {
                for (int idx = min + 1; idx <= m; idx++) {
                    right->children[idx - min - 1] = children[idx];
                    children[idx] = nullptr;
                }
            }

            return right;
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
            if (ogN == 0) {
                ogN++;
            }

            // data
            for (int i = 0; i < children[leftIdx + 1]->n; i++) {
                T* c = child->data[child->n];
                T* s = sibling->data[i];
                child->data[child->n++] = sibling->data[i];
            }

            // children
            if (!child->leaf) {
                for (int i = 0; i <= sibling->n; i++) {
                    node<T>* c = child->children[ogN + i - 1];
                    node<T>* s = sibling->children[i];
                    child->children[ogN + i] = sibling->children[i];
                }
            }

            // left shift other children
            int _n = n + 1;
            shiftLeft<node<T>*>(children, leftIdx + 1, _n);
        }

        void borrow(int base, bool toLeft) {
            bool isLeaf = children[base]->leaf;

            // bring parent down
            /*
                if to left, parent is data[base - 1]
                if to right, parent is data[base]
            */

            if (toLeft) {
                // shift data over
                shiftRight<T*>(children[base]->data, 0, children[base]->n);
                // insert parent element
                children[base]->data[0] = data[base - 1];

                // substitute greatest child from left sibling
                int _n = children[base - 1]->n;
                data[base - 1] = children[base - 1]->data[_n - 1];

                // update sibling
                children[base - 1]->data[_n - 1] = nullptr;
                children[base - 1]->n--;

                if (!isLeaf) {
                    // also bring over child
                    int noChildren = children[base]->n;
                    shiftRight<node<T>*>(children[base]->children, 0, noChildren);

                    children[base]->children[0] = children[base - 1]->children[_n];
                    children[base - 1]->children[_n] = nullptr;
                }
            }
            else {
                // insert parent element
                children[base]->data[children[base]->n] = data[base];
                children[base]->n++;

                // substitute smallest child from right sibling
                int _n = children[base + 1]->n;
                data[base] = children[base + 1]->data[0];

                // update sibling
                shiftLeft<T*>(children[base + 1]->data, 0, children[base + 1]->n);
                children[base + 1]->data[children[base + 1]->n] = nullptr; // remove last key (extra)

                if (!isLeaf) {
                    // also bring over child
                    int noChildren = children[base]->n;

                    children[base]->children[noChildren] = children[base + 1]->children[0];
                    _n++;
                    shiftLeft<node<T>*>(children[base + 1]->children, 0, _n);
                    children[base + 1]->children[_n] = nullptr; // remove last child (extra)
                }
            }
        }
    };

    template <typename T>
    class BTree {
    public:
        int m;			// degree of tree

        node<T>* root;	// root node of tree

        BTree(int m)
            : m(m) {
            // allocate for root
            root = new node<T>(m, true);
            root->min = 1;
        }

        void traverse(void(itemViewer)(T* data)) {
            root->traverse(itemViewer);
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

                // set children
                newRoot->children[0] = root;
                newRoot->children[1] = right;

                // reset root in class
                root = newRoot;
            }
        }

        T*& operator[](int key) {
            return root->search(key);
        }

        T*& search(int key) {
            return root->search(key);
        }

        void clear() {
            clearNode(root);
        }

        bool erase(int key) {
            int res = root->erase(key);

            if (root->n == 0) {
                // no more keys
                root = root->children[0];
            }

            return res == 0;
        }

    private:
        void clearNode(node<T>* n) {
            for (int i = 0; i < n->n; i++) {
                if (!n->leaf) {
                    clearNode(n->children[i]);
                    n->children[i] = nullptr;
                }
                n->data[i] = nullptr;
            }
            if (!n->leaf) {
                clearNode(n->children[n->n]);
                n->children[n->n] = nullptr;
            }
            n->children = nullptr;
            n->data = nullptr;
        }
    };
}

#endif