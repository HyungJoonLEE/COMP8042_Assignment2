#include "../include/UnorderedSet.h"
#include "../include/Stack.h"


template<typename Key>
UnorderedSet<Key>::UnorderedSet() {
    setSize = 0;
}

template<typename Key>
UnorderedSet<Key>::~UnorderedSet() {

};


template <typename Key>
typename UnorderedSet<Key>::Iterator UnorderedSet<Key>::begin() const {
    Node<Key>* node = root;
    while (node && node->left) {
        node = node->left;
    }
    return Iterator(node);
}


template <typename Key>
typename UnorderedSet<Key>::Iterator UnorderedSet<Key>::end() const {
    return Iterator(nullptr);
}


template<typename Key>
bool UnorderedSet<Key>::insert(const Key &key) {
    // This is a simplified insertion without handling Red-Black Tree properties
    Node<Key>* newNode = new Node<Key>(key);
    if (!root) {
        root = newNode;
        setSize++;
        return true;
    }

    Node<Key>* current = root;
    Node<Key>* parent = nullptr;

    while (current) {
        parent = current;
        if (key < current->key) {
            current = current->left;
        } else if (key > current->key) {
            current = current->right;
        } else {
            // Duplicate key, not inserting
            delete newNode;
            return false;
        }
    }

    if (key < parent->key) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }
    newNode->parent = parent;
    setSize++;

    // TODO: Handle Red-Black Tree properties after insertion
    // fixRedRedViolation(newNode);

    return true;

}


template<typename Key>
bool UnorderedSet<Key>::search(const Key &key) const {
    Node<Key>* current = root;
    while (current) {
        if (key < current->key) {
            current = current->left;
        } else if (key > current->key) {
            current = current->right;
        } else {
            // Key found
            return true;
        }
    }
    return false; // Key not found

}


template<typename Key>
bool UnorderedSet<Key>::erase(const Key &key) {
    // This is a simplified deletion without handling Red-Black Tree properties
    Node<Key>* current = root;
    while (current) {
        if (key < current->key) {
            current = current->left;
        } else if (key > current->key) {
            current = current->right;
        } else {
            // Key found, delete the node
            if (!current->left || !current->right) {
                // Node with only one child or no child
                deleteOneChild(current);
            } else {
                // Node with two children
                Node<Key>* successor = Iterator(current).operator++().currentNode;
                current->key = successor->key;
                deleteOneChild(successor);
            }
            setSize--;
            return true;
        }
    }
    return false; // Key not found
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

}


template<typename Key>
size_t UnorderedSet<Key>::getSize(Node<Key> *node) const {
    return setSize;
}


template<typename Key>
void UnorderedSet<Key>::fixRedRedViolation(Node<Key> *node) {

}


template<typename Key>
void UnorderedSet<Key>::rotateLeft(Node<Key> *node) {

}


template<typename Key>
void UnorderedSet<Key>::rotateRight(Node<Key> *node) {

}


template<typename Key>
void UnorderedSet<Key>::deleteOneChild(Node<Key> *node) {

}


template<typename Key>
void UnorderedSet<Key>::deleteFix(Node<Key> *node) {

}


template<typename Key>
void UnorderedSet<Key>::clearRecursive(Node<Key> *node) {

}

