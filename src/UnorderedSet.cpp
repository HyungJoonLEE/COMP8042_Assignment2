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
    return Iterator(root);
}


template <typename Key>
typename UnorderedSet<Key>::Iterator UnorderedSet<Key>::end() const {
    Node<Key>* temp;

    temp = root;
    while(temp->right) {
        temp = temp->right;
    }

    if (temp->left) return Iterator(temp->left);
    else return Iterator(temp);
}


// TODO:
template<typename Key>
bool UnorderedSet<Key>::insert(const Key &key) {
    Node<Key>* cNode = root;
    Node<Key>* pNode = nullptr;
    auto* newNode = new Node<Key>(key);
    newNode->color = Color::RED;

    if (!root) {
        root = newNode;
        root->color = Color::BLACK;
        setSize++;
        return true;
    }

    while (cNode) {
        pNode = cNode;
        if (key < cNode->key) cNode = cNode->left;
        else if (key > cNode->key) cNode = cNode->right;
        else {
            // Key exist in the tree - free memory
            delete newNode;
            return false;
        }
    }

    if (key < pNode->key) pNode->left = newNode;
    else pNode->right = newNode;
    newNode->parent = pNode;
    setSize++;

//
//    while(root) {
//        gets
//    }
//

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
    Node<Key>* current = root;

//    while (current) {
//        if (key < current->key) current = current->left;
//        else if (key > current->key) current = current->right;
//        else {
//            if (!current->left) {
//                // Node with only one child or no child
//                deleteOneChild(current);
//                if (!(*current)) {
//                    current = current->left;
//                }
//            }
//            else if (!current->right) {
//                deleteOneChild(current);
//                if (!(*current)) {
//                    current = current->right;
//                }
//            }
//            else {
//                // Node with two children
//                if (current == (*current).left) {
//                    (*current).right = current->right;
//                    (*current).left = current->left;
//                    deleteOneChild(current);
//                }
//            }
//            setSize--;
//            return true;
//        }
//    }
    return false;
}


template<typename Key>
void UnorderedSet<Key>::clear() {
    clearRecursive(root);
    root = nullptr;
    setSize = 0;
}


template<typename Key>
size_t UnorderedSet<Key>::size() const {
    return getSize(root);
}


template<typename Key>
void UnorderedSet<Key>::updateSize() {

}


//Recursively calculates the size of the subtree rooted at the given node:
template<typename Key>
size_t UnorderedSet<Key>::getSize(Node<Key> *node) const {
    if (node == nullptr)
        return 0;
    else
        return(getSize(node->left) + 1 + getSize(node->right));
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
    free(node);
}


template<typename Key>
void UnorderedSet<Key>::deleteFix(Node<Key> *node) {

}


template<typename Key>
void UnorderedSet<Key>::clearRecursive(Node<Key> *node) {

}

