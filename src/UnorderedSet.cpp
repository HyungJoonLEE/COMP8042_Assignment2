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


template<typename Key>
bool UnorderedSet<Key>::insert(const Key &key) {
    Node<Key> *cNode = nullptr, *pNode = nullptr;

    auto* newNode = new Node<Key>(key);
    newNode->color = Color::RED;
    newNode->left = nullptr;
    newNode->right = nullptr;

    if (!root) {
        root = newNode;
        root->color = Color::BLACK;
        root->parent = nullptr;
        setSize++;
        return true;
    }

    cNode = root;
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

    if (setSize > 3)
        fixRedRedViolation(newNode);

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
    return setSize;
}


template<typename Key>
void UnorderedSet<Key>::updateSize() {
    getSize(root);
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
    while(node != root && node->parent->color != Color::RED) {
        Node<Key> *grandparent = node->parent->parent;
        Node<Key> *uncle = (node->parent == grandparent->left) ? grandparent->right : grandparent->left;
        bool side = (node->parent == grandparent->left) ? true : false;

        if (uncle && uncle->color == Color::RED) {
            node->parent->color = Color::BLACK;
            uncle->color = Color::BLACK;
            grandparent->color = Color::RED;
            node = grandparent;
        }
        else {
            if (node == (side ? node->parent->right : node->parent->left)) {
                node = node->parent;
                side ? rotateLeft(node) : rotateLeft(node);
            }

            node->parent->color = Color::BLACK;
            grandparent->color = Color::RED;
            side ? rotateRight(grandparent) : rotateLeft(grandparent);
        }
    }
    root->color = Color::BLACK;
}


template<typename Key>
void UnorderedSet<Key>::rotateLeft(Node<Key> *node) {
    Node<Key>* ptr = nullptr;

    ptr = node->right;
    node->right = ptr->left;
    if (ptr->left != nullptr) {
        ptr->left->parent = node;
    }

    if (!node->parent) root = ptr;
    else if (node == node->parent->left) {
        node->parent->left = ptr;
    }
    else node->parent->right = ptr;
    node->parent = ptr;
    ptr->left = node;
}


template<typename Key>
void UnorderedSet<Key>::rotateRight(Node<Key> *node) {
    Node<Key>* ptr = nullptr;

    ptr = node->left;
    node->left = ptr->right;
    if (ptr->right != nullptr) {
        ptr->right->parent = node;
    }

    if (!node->parent) root = ptr;
    else if (node == node->parent->left) {
        node->parent->left = ptr;
    }
    else node->parent->right = ptr;
    node->parent = ptr;
    ptr->right = node;
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

