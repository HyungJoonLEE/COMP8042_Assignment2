#include "../include/UnorderedSet.h"
#include "../include/Stack.h"


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
    Node<Key>* parent = nullptr;

    if (!search(key)) return false;

    while (target) {
        if (key < target->key) target = target->left;
        else if (key > target->key) target = target->right;
        else {
            break;
        }
    }

    parent = target->parent;

    // Node with no child
    if (!target->left && !target->right) {
        if (!parent) {
            // Node is root
            delete root;
            root = nullptr;
        }
        else {
            if (parent->left == target)
                parent->left = nullptr;
            else
                parent->right = nullptr;

            if (target->color == Color::BLACK) {
                target->color = Color::BLUE;
                deleteFix(target);
            }
            delete target;
        }
    }

    // Node with one child
    else if ((target->left && !target->right) || (!target->left && target->right)) {
        deleteOneChild(target);
        delete target;
    }

    // Node with two children
    else {
        Node<Key>* successor = target->right;

        while (successor->left)
            successor = successor->left;

        target->key = successor->key; // Copy successor's value to target

        parent = successor->parent;
        if (parent->left == successor)
            parent->left = successor->right;  // Assuming successor is always a leaf or has one child (right)
        else
            parent->right = successor->right;

        if (successor->right) {
            successor->right->parent = successor->parent;
        }

        if (successor->color == Color::RED)
            delete successor;
        else {
            successor->color == Color::BLUE;
            deleteFix(successor);
            delete successor;
        }
    }

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
           (node->parent->color == Color::RED)) {

        parent = node->parent;
        grandParent = node->parent->parent;

        /* Node Parent is left child of Grand-parent */
        if (parent == grandParent->left) {
            Node<Key> *uncle_pt = grandParent->right;

            /* Case : 1
               Uncle is also red
               Only Recoloring required */
            if (uncle_pt != nullptr && uncle_pt->color == Color::RED) {
                grandParent->color = Color::RED;
                parent->color = Color::BLACK;
                uncle_pt->color = Color::BLACK;
                node = grandParent;
            }
            else {
                /* Case : 2
                   Node is right child of its parent
                   Left-rotation required */
                if (node == parent->right) {
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

        /* Node Parent is right child of Grand-parent */
        else {
            Node<Key> *uncle_pt = grandParent->left;

            /*  Case : 1
                Uncle is also red
                Only Recoloring required */
            if ((uncle_pt != nullptr) && (uncle_pt->color == Color::RED)) {
                grandParent->color = Color::RED;
                parent->color = Color::BLACK;
                uncle_pt->color = Color::BLACK;
                node = grandParent;
            }
            else {
                /* Case : 2
                   Node is left child of its parent
                   Right-rotation required */
                if (node == parent->left) {
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
    Node<Key>* child = (node->left) ? node->left : node->right;  // Determine the existing child

    if (!node->parent) {
        // Node is root
        root = child;
        child->color = Color::BLACK;
        if (child) {
            child->parent = nullptr;
        }
    } else {
        if (node->parent->left == node)
            node->parent->left = child;
        else
            node->parent->right = child;

        if (child) {
            child->parent = node->parent;
        }
    }

    if (child->color == Color::BLACK) {
        child->color = Color::BLUE;
        deleteFix(child);
    }
}


template<typename Key>
void UnorderedSet<Key>::deleteFix(Node<Key> *node) {
    Node<Key>* sibling;

    while (node != root && node->color == Color::BLUE) {
        if (node == node->parent->left) {
            sibling = node->parent->right;

            if (!sibling) break;  // Exit the loop if sibling is nullptr

            if (sibling->color == Color::RED) {
                sibling->color = Color::BLACK;
                node->parent->color = Color::RED;
                rotateLeft(node->parent);
                sibling = node->parent->right;
            }

            if ((!sibling->left || sibling->left->color == Color::BLACK) &&
                (!sibling->right || sibling->right->color == Color::BLACK)) {
                sibling->color = Color::RED;
                node = node->parent;
            } else {
                if (!sibling->right || sibling->right->color == Color::BLACK) {
                    if (sibling->left) sibling->left->color = Color::BLACK;
                    sibling->color = Color::RED;
                    rotateRight(sibling);
                    sibling = node->parent->right;
                }
                sibling->color = node->parent->color;
                node->parent->color = Color::BLACK;
                if (sibling->right) sibling->right->color = Color::BLACK;
                rotateLeft(node->parent);
                node = root;
            }
        } else {
            sibling = node->parent->left;

            if (!sibling) break;  // Exit the loop if sibling is nullptr

            if (sibling->color == Color::RED) {
                sibling->color = Color::BLACK;
                node->parent->color = Color::RED;
                rotateRight(node->parent);
                sibling = node->parent->left;
            }

            if ((!sibling->right || sibling->right->color == Color::BLACK) &&
                (!sibling->left || sibling->left->color == Color::BLACK)) {
                sibling->color = Color::RED;
                node = node->parent;
            } else {
                if (!sibling->left || sibling->left->color == Color::BLACK) {
                    if (sibling->right) sibling->right->color = Color::BLACK;
                    sibling->color = Color::RED;
                    rotateLeft(sibling);
                    sibling = node->parent->left;
                }
                sibling->color = node->parent->color;
                node->parent->color = Color::BLACK;
                if (sibling->left) sibling->left->color = Color::BLACK;
                rotateRight(node->parent);
                node = root;
            }
        }
    }

    node->color = Color::BLACK;
}


template<typename Key>
void UnorderedSet<Key>::clearRecursive(Node<Key> *node) {
    if (node == NULL) return;
    clearRecursive(node->left);
    clearRecursive(node->right);
    delete node;
}

