#include "../include/UnorderedSet.h"
#include "../include/Stack.h"


template<typename Key>
UnorderedSet<Key>::UnorderedSet() {
    setSize = 0;
    root = 0;
}

template<typename Key>
UnorderedSet<Key>::~UnorderedSet() {

};


template <typename Key>
typename UnorderedSet<Key>::Iterator UnorderedSet<Key>::begin() const {
    Node<Key>* temp = root;
    if (!temp)
        return Iterator(temp);
    while(temp->left) {
        temp = temp->left;
    }
    return Iterator(temp);
}


template <typename Key>
typename UnorderedSet<Key>::Iterator UnorderedSet<Key>::end() const {
    return Iterator(nullptr);
}


template<typename Key>
bool UnorderedSet<Key>::insert(const Key &key) {
    Node<Key> *current = nullptr, *parent = nullptr;

    auto* newNode = new Node<Key> (key);
    newNode->color = Color::RED;
    newNode->key = key;


    if (!root) {
        root = newNode;
        root->color = Color::BLACK;
        updateSize();
        return true;
    }
    else {
        if (search(key)) {
            delete newNode;
            return false;
        }

        current = root;
        while (current) {
            parent = current;
            if (key < current->key) current = current->left;
            else if (key > current->key) current = current->right;
        }

        if (newNode->key < parent->key) parent->left = newNode;
        else parent->right = newNode;
        newNode->parent = parent;
        updateSize();

        // RED-RED Violation
        if (parent->color == Color::RED) {
            fixRedRedViolation(newNode);
        }
    }
    return true;
}


template<typename Key>
bool UnorderedSet<Key>::search(const Key &key) const {
    Node<Key>* current = root;
    while (current) {
        if (key < current->key) current = current->left;
        else if (key > current->key) current = current->right;
        else return true;  // FOUND
    }
    return false;
}


template<typename Key>
bool UnorderedSet<Key>::erase(const Key &key) {
    Node<Key>* target = root;
    Node<Key>* successor = nullptr;
    int size = 0;

    if (!search(key)) return false;

    while (target) {
        if (key < target->key) target = target->left;
        else if (key > target->key) target = target->right;
        else {
            break;
        }
    }

    size = getSize(target) - 1;

    if (size == 0) {
        if (target == root) root = nullptr;
        else {
            if (target->parent->left == target) target->parent->left = nullptr;
            if (target->parent->right == target) target->parent->right = nullptr;
        }
    }
    else if (size == 1) deleteOneChild(target);
    else {
        successor = target->right;
        while (successor->left != nullptr) {
            successor = successor->left;
        }

        successor->left = target->left;
        successor->right = target->right;
        successor->color = target->color;
        successor->parent = target->parent;
        if (target->parent == nullptr) root = successor;
    }

    delete(target);
    setSize--;

    return true;
}


template<typename Key>
void UnorderedSet<Key>::clear() {
    clearRecursive(root);
    root = nullptr;
    setSize = 0;
}


template<typename Key>
size_t UnorderedSet<Key>::size() const {
    return setSize;
}


template<typename Key>
void UnorderedSet<Key>::updateSize() {
    setSize = getSize(root);
}


//Recursively calculates the size of the subtree rooted at the given node:
template<typename Key>
size_t UnorderedSet<Key>::getSize(Node<Key> *node) const {
    if (node == nullptr)
        return 0;
    else
        return getSize(node->left) + 1 + getSize(node->right);
}


template<typename Key>
void UnorderedSet<Key>::fixRedRedViolation(Node<Key> *node) {
    Node<Key> *parent = nullptr;
    Node<Key> *grandParent = nullptr;
    Color tempColor = Color::RED;

    while ((node != root) && (node->color != Color::BLACK) &&
           (node->parent->color == Color::RED))
    {

        parent = node->parent;
        grandParent = node->parent->parent;

        /*  Case : A
            Parent of pt is left child of Grand-parent of pt */
        if (parent == grandParent->left)
        {

            Node<Key> *uncle_pt = grandParent->right;

            /* Case : 1
               The uncle of pt is also red
               Only Recoloring required */
            if (uncle_pt != nullptr && uncle_pt->color == Color::RED)
            {
                grandParent->color = Color::RED;
                parent->color = Color::BLACK;
                uncle_pt->color = Color::BLACK;
                node = grandParent;
            }

            else
            {
                /* Case : 2
                   pt is right child of its parent
                   Left-rotation required */
                if (node == parent->right)
                {
                    rotateLeft(parent);
                    node = parent;
                    parent = node->parent;
                }

                /* Case : 3
                   pt is left child of its parent
                   Right-rotation required */
                rotateRight(grandParent);
                tempColor = parent->color;
                parent->color = grandParent->color;
                grandParent->color = tempColor;
                node = parent;
            }
        }

            /* Case : B
               Parent of pt is right child of Grand-parent of pt */
        else
        {
            Node<Key> *uncle_pt = grandParent->left;

            /*  Case : 1
                The uncle of pt is also red
                Only Recoloring required */
            if ((uncle_pt != nullptr) && (uncle_pt->color == Color::RED))
            {
                grandParent->color = Color::RED;
                parent->color = Color::BLACK;
                uncle_pt->color = Color::BLACK;
                node = grandParent;
            }
            else
            {
                /* Case : 2
                   pt is left child of its parent
                   Right-rotation required */
                if (node == parent->left)
                {
                    rotateRight(parent);
                    node = parent;
                    parent = node->parent;
                }

                /* Case : 3
                   pt is right child of its parent
                   Left-rotation required */
                rotateLeft(grandParent);
                tempColor = parent->color;
                parent->color = grandParent->color;
                grandParent->color = tempColor;
                node = parent;
            }
        }
    }

    root->color = Color::BLACK;
}


template<typename Key>
void UnorderedSet<Key>::rotateLeft(Node<Key> *node) {
    Node<Key> *right = node->right;

    node->right = right->left;

    if (node->right != nullptr)
        node->right->parent = node;

    right->parent = node->parent;

    if (node->parent == nullptr)
        root = right;

    else if (node == node->parent->left)
        node->parent->left = right;

    else
        node->parent->right = right;

    right->left = node;
    node->parent = right;
}


template<typename Key>
void UnorderedSet<Key>::rotateRight(Node<Key> *node) {
    Node<Key> *left = node->left;

    node->left = left->right;

    if (node->left != nullptr)
        node->left->parent = node;

    left->parent = node->parent;

    if (node->parent == nullptr)
        root = left;

    else if (node == node->parent->left)
        node->parent->left = left;

    else
        node->parent->right = left;

    left->right = node;
    node->parent = left;
}


//Deletes a node with only one child in the Red-Black Tree:
//void rb_transplant(red_black_tree *t, tree_node *u, tree_node *v) {
//    if(u->parent == t->NIL)
//        t->root = v;
//    else if(u == u->parent->left)
//        u->parent->left = v;
//    else
//        u->parent->right = v;
//    v->parent = u->parent;
//}


//Deletes a node with only one child in the Red-Black Tree:
template<typename Key>
void UnorderedSet<Key>::deleteOneChild(Node<Key> *node) {
    if (node->parent) {
        if (node->left == nullptr) {                    // Deleting node only has a right child
            if (node->parent->left == node)
                node->parent->left = node->right;       // Deleting node is parent' left
            else
                node->parent->right = node->right;      // Deleting node is parent' right

            node->right->parent = node->parent;
        }
        if (node->right == nullptr) {                   // Deleting node only has a left child
            if (node->parent->left == node)
                node->parent->left = node->left;        // Deleting node is parent' left
            else
                node->parent->right = node->left;

            node->left->parent = node->parent;
        }
    }
    else {
        if (node->left == nullptr) root = node->right;
        if (node->right == nullptr) root = node->left;
        root->parent = nullptr;
        root->color = Color::BLACK;
    }
    if (node == root) root = 0;
}


template<typename Key>
void UnorderedSet<Key>::deleteFix(Node<Key> *node) {
    Node<Key>* s;
    while (node != root && node->color == Color::BLACK) {
        if (node == node->parent->left) {
            s = node->parent->right;
            if (s->color == Color::RED) {
                s->color = Color::BLACK;
                node->parent->color = Color::RED;
                rotateLeft(node->parent);
                s = node->parent->right;
            }

            if (s->left->color == Color::BLACK && s->right->color == Color::BLACK) {
                s->color = Color::RED;
                node = node->parent;
            } else {
                if (s->right->color == Color::BLACK) {
                    s->left->color = Color::BLACK;
                    s->color = Color::RED;
                    rotateRight(s);
                    s = node->parent->right;
                }
                s->color = node->parent->color;
                node->parent->color = Color::BLACK;
                s->right->color = Color::BLACK;
                rotateLeft(node->parent);
                node = root;
            }
        } else {
            s = node->parent->left;
            if (s->color == Color::RED) {
                s->color = Color::BLACK;
                node->parent->color = Color::RED;
                rotateRight(node->parent);
                s = node->parent->left;
            }
            if (s->right->color == Color::BLACK && s->right->color == Color::BLACK) {
                s->color = Color::RED;
                node = node->parent;
            } else {
                if (s->left->color == Color::BLACK) {
                    s->right->color = Color::BLACK;
                    s->color = Color::RED;
                    rotateLeft(s);
                    s = node->parent->left;
                }
                s->color = node->parent->color;
                node->parent->color = Color::BLACK;
                s->left->color = Color::BLACK;
                rotateRight(node->parent);
                node = root;
            }
        }
    }
    node->color = Color::BLACK;
}


template<typename Key>
void UnorderedSet<Key>::clearRecursive(Node<Key> *node) {

}

