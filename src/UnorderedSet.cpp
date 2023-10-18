#include "../include/UnorderedSet.h"
#include "../include/Stack.h"
#include <iostream>


template<typename Key>
UnorderedSet<Key>::UnorderedSet() {
    setSize = 0;
    root = 0;
}

template<typename Key>
UnorderedSet<Key>::~UnorderedSet() {
    clearRecursive(root);
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
    Node<Key>* tempTarget = nullptr;
    Node<Key>* targetChild = nullptr;
    Color targetColor;

    if (!search(key)) return false;

    while (target) {
        if (key < target->key) target = target->left;
        else if (key > target->key) target = target->right;
        else {
            targetColor = target->color;
            break;
        }
    }

    if (target == root && target->left == nullptr && target->right == nullptr) {
        root = nullptr;
    }
    else {
        tempTarget = target;
        if (target->left == nullptr) {
            targetChild = target->right;
        } else if (target->right == nullptr) {
            targetChild = target->left;
        } else {
            tempTarget = target->right;
            while (tempTarget->left != nullptr) {
                tempTarget = tempTarget->left;
            }
            targetColor = tempTarget->color;
            targetChild = tempTarget->right;
        }

        // Update the parent of targetChild
        if (targetChild != nullptr) {
            targetChild->parent = target->parent;
        }

        // Update the parent's left or right child pointer to point to targetChild
        if (target->parent == nullptr) {
            root = targetChild;
            if (!root) root = tempTarget;
            root->color = Color::BLACK;
        } else if (target == target->parent->left) {
            target->parent->left = targetChild;
        } else {
            target->parent->right = targetChild;
        }

        // Handle the case where the node has only one child using deleteOneChild
        if (targetChild != nullptr) {
            deleteOneChild(target);
        }

        if (targetColor == Color::BLACK) {
            deleteFix(targetChild);
        }
    }

    delete target;
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
            Node Parent is left child of Grand-parent */
        if (parent == grandParent->left)
        {

            Node<Key> *uncle_pt = grandParent->right;

            /* Case : 1
               Uncle is also red
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
                   Node is right child of its parent
                   Left-rotation required */
                if (node == parent->right)
                {
                    rotateLeft(parent);
                    node = parent;
                    parent = node->parent;
                }

                /* Case : 3
                   Node is left child of its parent
                   Right-rotation required */
                rotateRight(grandParent);
                tempColor = parent->color;
                parent->color = grandParent->color;
                grandParent->color = tempColor;
                node = parent;
            }
        }

            /* Case : B
               Node Parent is right child of Grand-parent */
        else
        {
            Node<Key> *uncle_pt = grandParent->left;

            /*  Case : 1
                Uncle is also red
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
                   Node is left child of its parent
                   Right-rotation required */
                if (node == parent->left)
                {
                    rotateRight(parent);
                    node = parent;
                    parent = node->parent;
                }

                /* Case : 3
                   Node is right child of its parent
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
template<typename Key>
void UnorderedSet<Key>::deleteOneChild(Node<Key> *node) {
    Node<Key>* child = (node->left != nullptr) ? node->left : node->right;

    // Detach 'node' from its parent
    if (node->parent == nullptr) {
        root = child; // 'node' was the root
    } else if (node == node->parent->left) {
        node->parent->left = child;
    } else {
        node->parent->right = child;
    }

    // Update the parent of 'child' if it exists
    if (child != nullptr) {
        child->parent = node->parent;
    }

    // Delete 'node' and adjust Red-Black Tree properties if it was black
    if (node->color == Color::BLACK) {
        deleteFix(child);
    }
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
    root->color = Color::BLACK;
}


template<typename Key>
void UnorderedSet<Key>::clearRecursive(Node<Key> *node) {
    if (node == NULL) return;
    clearRecursive(node->left);
    clearRecursive(node->right);
    delete node;
}

