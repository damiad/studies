#include <bits/stdc++.h>
using namespace std;

class node {
public:
    long long count;
    long long key;
    long long size;
    node* left, *right;
};

class SplayTree {
public:
    SplayTree() {
        root_splay = NULL;
        w = 0;
    }

    //wywołuje funkcje co zmienia drzewko i wrzuca wynik na szczyt
    //wypisuje root
    void get_splay(long long j2) {
        long long j = (j2 + w) % root_splay->size;
        root_splay = splay(root_splay, j + 1); //może +1
        w = root_splay->key;
        cout << w << "\n";
    }

    void insert_splay(long long j2, long long x, long long k) {

        if (root_splay == NULL) {
            root_splay = newNode(k, x, k);
            return;
        }

        long long j = (j2 + w) % (root_splay->size + 1);

        //zwraca pierwszy przedział z wartością większą równą j
        //wsadzamy nad nim gdy left+middle==j
        //rozbijamy go na left+middle=j i wrzucamy nad nim...
        root_splay = splay(root_splay, j);

        if (sizeLeftMidNode(root_splay) == j) { //wstawiamy za
            node* new_root = newNode(k, x, k + root_splay->size);
            new_root->left = root_splay;
            new_root->right = root_splay->right;
            root_splay->right = NULL;
            root_splay->size = sizeLeftMidNode(root_splay);
            root_splay = new_root;
        } else if (j == 0) { //bo jak nie zero to byśmy byli w left...
            node* new_root = newNode(k, x, k);
            root_splay->size += k; // wstawiamy przed
            root_splay->left = new_root;
        } else {      //rozbijamy i wstawiamy pomiędzy rozbite
            node* T1 = new node();
            T1->left = root_splay->left;
            T1->right = NULL;
            T1->count = j - sizeNode(T1->left); //>0
            T1->key = root_splay->key;
            T1->size = j;//left size + mid count
            node* new_root = newNode(k, x, k + root_splay->size);
            root_splay->size -= T1->size;
            root_splay->count -= T1->count; //>0
            root_splay->left = NULL;
            new_root->left = T1;
            new_root->right = root_splay;
            root_splay = new_root;
        }
    }
private:
    node* root_splay;
    long long w;

    long long sizeNode(node* x) {
        if (x == NULL) return 0;
        return x->size;
    }

    long long sizeLeftMidNode(node* x) {
        if (x == NULL) return 0;
        return sizeNode(x->left) + x->count;
    }

    long long sizeRightMidNode(node* x) {
        if (x == NULL) return 0;
        return sizeNode(x->right) + x->count;
    }

    void updateSizeNode(node* x) {
        if (x != NULL)
            x->size = x->count + sizeNode(x->left) + sizeNode(x->right);
    }

    node* newNode(long long count, long long key, long long size) {
        node* Node = new node();
        Node->count = count;
        Node->key = key;
        Node->size = size;
        Node->left = Node->right = NULL;
        return (Node);
    }

    node* rightRotate(node* x) {
        node* y = x->left;
        long long size_copy = x->size;
        x->size = sizeRightMidNode(x) + sizeNode(y->right);
        x->left = y->right;
        y->right = x;
        y->size = size_copy;
        return y;
    }

    node* leftRotate(node* x) {
        node* y = x->right;
        long long size_copy = x->size;
        x->size = sizeLeftMidNode(x) + sizeNode(y->left);
        x->right = y->left;
        y->left = x;
        y->size = size_copy;
        return y;
    }

    bool is_found(node* root, long long index) {
        if (root->left == NULL && root->count >= index) return true;
        if (sizeNode(root->left) < index && sizeLeftMidNode(root) >= index) return true;
        return false;
    }


    //zwraca pierwszy przedział z wartością większą równą j
    // This function modifies the tree and returns the new root
    node* splay(node* root, long long index) {
        // Base cases: root is NULL orkey is present at root
        if (is_found(root, index) == true) return root;

        // Key lies in left subtree
        if (sizeNode(root->left) >= index) {

            if (is_found(root->left, index))
                return rightRotate(root);

            // Zig-Zig (Left Left)
            if (sizeNode(root->left->left) >= index) {

                // First recursively bring the
                // key as root of left-left
                root->left->left = splay(root->left->left, index);
                // Do first rotation for root,
                // second rotation is done after else
                root = rightRotate(root);

            } else { // Zig-Zag (Left Right) klucz po prawej i nie null
                // First recursively bring
                // the key as root of left-right
                root->left->right = splay(root->left->right,
                                          index - sizeLeftMidNode(root->left));

                // Do first rotation for root->left
                // if (root->left->right != NULL)
                root->left = leftRotate(root->left);
            }

            // Do second rotation for root
            return rightRotate(root);
            // return (root->left == NULL) ? root : rightRotate(root);

        } else { // Key lies in right subtree
            index -= sizeLeftMidNode(root);
            if (is_found(root->right, index))
                return leftRotate(root);

            // Zag-Zig (Right Left)
            if (sizeNode(root->right->left) >= index) {

                // Bring the key as root of right-left
                root->right->left = splay(root->right->left, index);

                // Do first rotation for root->right
                root->right = rightRotate(root->right);
            } else { // Zag-Zag (Right Right) klucz po prawej i nie null
                // Bring the key as root of
                // right-right and do first rotation
                root->right->right = splay(root->right->right,
                                           index - sizeLeftMidNode(root->right));
                root = leftRotate(root);
            }

            // Do second rotation for root
            return leftRotate(root);
            // return (root->right == NULL) ? root : leftRotate(root);
        }
    }
};

int main() {

    SplayTree st;
    long long m;
    cin >> m;
    char c;
    long long j2, x, k;
    for (long long i = 0; i < m; i++) {
        cin >> c >> j2;
        if (c == 'g') st.get_splay(j2);
        else {
            cin >> x >> k;
            st.insert_splay(j2, x, k);
        }
    }

}