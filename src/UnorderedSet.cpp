#include "../include/UnorderedSet.h"


/**
* @brief Default constructor for the UnorderedSet class.
*
* Initializes the set with size 0 and a null root pointer.
*/
template<typename Key>
UnorderedSet<Key>::UnorderedSet() {
    setSize = 0;
    root = 0;
}


/**
* @brief Destructor for the UnorderedSet class.
*
* Recursively clears all nodes and deallocates memory associated with the set.
*/
template<typename Key>
UnorderedSet<Key>::~UnorderedSet() {
    clearRecursive(root);
};


/**
 * @brief Returns an iterator pointing to the first element in the set.
 *
 * If the set is empty, the returned iterator will be equivalent to end().
 * Otherwise, it returns an iterator pointing to the leftmost element,
 * which is the smallest element in the set.
 *
 * @return Iterator pointing to the first element or end() if the set is empty.
 */
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


/**
 * @brief Returns an iterator pointing past the last element in the set.
 *
 * The returned iterator does not point to any actual element in the set.
 * It can be used to check if another iterator has reached the end.
 * Dereferencing the end iterator is undefined behavior.
 *
 * @return Iterator pointing past the last element.
 */
template <typename Key>
typename UnorderedSet<Key>::Iterator UnorderedSet<Key>::end() const {
    return Iterator(nullptr);
}


/**
 * @brief Inserts a new key into the unordered set.
 *
 * If the key is already present in the set, the function does nothing and returns false.
 * Otherwise, the key is inserted, and any RED-RED violation in the underlying Red-Black Tree
 * is corrected. The size of the set is also updated.
 *
 * @param key The key value to be inserted into the set.
 * @return True if the insertion was successful. False if the key was already present.
 */
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


/**
 * @brief Searches for a key in the unordered set.
 *
 * This method traverses the underlying Red-Black Tree to determine
 * if the specified key is present in the set or not.
 *
 * @param key The key value to be searched for in the set.
 * @return True if the key is found in the set. False otherwise.
 */
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


/**
 * @brief Erases a key from the unordered set.
 *
 * This method attempts to remove the specified key from the set.
 * The deletion process involves handling three cases:
 * 1. Node with no children.
 * 2. Node with one child.
 * 3. Node with two children.
 *
 * After the key has been removed, the method ensures the properties
 * of the Red-Black Tree are preserved, especially the Black height
 * property. This might involve multiple rotations and color changes.
 *
 * @param key The key value to be erased from the set.
 * @return True if the key was found and successfully removed. False otherwise.
 */
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

    // no child
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

    // one child
    else if ((target->left && !target->right) || (!target->left && target->right)) {
        deleteOneChild(target);
        delete target;
    }

    // two children
    else {
        Node<Key>* successor = target->right;

        while (successor->left)
            successor = successor->left;

        target->key = successor->key;

        parent = successor->parent;
        if (parent->left == successor)
            parent->left = successor->right;
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


/**
 * @brief Clears the contents of the unordered set.
 *
 * This method removes all elements from the unordered set.
 * It uses a recursive helper function to delete nodes in
 * a post-order manner to ensure proper deallocation.
 * After clearing the set, the root is set to nullptr and the
 * size is reset to zero.
 */
template<typename Key>
void UnorderedSet<Key>::clear() {
    clearRecursive(root);
    root = nullptr;
    setSize = 0;
}


/**
 * @brief Retrieves the number of elements in the unordered set.
 *
 * This method returns the number of elements contained in the
 * unordered set, represented by the 'setSize' member variable.
 *
 * @return size_t - The number of elements in the set.
 */
template<typename Key>
size_t UnorderedSet<Key>::size() const {
    return setSize;
}


/**
 * @brief Updates the size of the UnorderedSet.
 *
 * This method recalculates the size of the entire set by invoking the
 * 'getSize()' method with the root node. The result is then assigned
 * to the 'setSize' member variable, ensuring it remains updated.
 */
template<typename Key>
void UnorderedSet<Key>::updateSize() {
    setSize = getSize(root);
}


/**
 * @brief Recursively calculates the size of the subtree rooted at the given node.
 *
 * This method traverses the subtree rooted at the specified node and returns the
 * number of nodes in that subtree. It uses a post-order traversal approach to
 * calculate the size, which includes the current node, its left subtree, and its right subtree.
 *
 * @param node Pointer to the root node of the subtree whose size is to be calculated.
 * @return size_t - The number of nodes in the subtree rooted at the given node.
 */
template<typename Key>
size_t UnorderedSet<Key>::getSize(Node<Key> *node) const {
    if (node == nullptr)
        return 0;
    else
        return getSize(node->left) + 1 + getSize(node->right);
}


/**
 * @brief Fixes the Red-Red color violation in the Red-Black tree.
 *
 * When a Red-Red violation occurs, this function is responsible for
 * balancing the colors by a combination of recoloring and rotations.
 * The Red-Black tree properties are restored after these adjustments.
 *
 * @param node A pointer to the Node<Key> that may cause a violation
 * due to its color and its parent's color both being red.
 *
 * Note:
 * 1. If the uncle node is red, only recoloring is needed.
 * 2. If the uncle node is black, rotations are applied depending on the
 *    position of the node in relation to its parent.
 * 3. After any adjustments, the root node is always recolored to black.
 */
template<typename Key>
void UnorderedSet<Key>::fixRedRedViolation(Node<Key> *node) {
    Node<Key> *parent = nullptr;
    Node<Key> *grandParent = nullptr;
    Color tempColor = Color::RED;

    while ((node != root) && (node->color != Color::BLACK) &&
           (node->parent->color == Color::RED)) {

        parent = node->parent;
        grandParent = node->parent->parent;

        // Node Parent is left child of Grand-parent
        if (parent == grandParent->left) {
            Node<Key> *uncle_pt = grandParent->right;

            // TODO: Uncle is also red
            //  Only Recoloring required
            if (uncle_pt != nullptr && uncle_pt->color == Color::RED) {
                grandParent->color = Color::RED;
                parent->color = Color::BLACK;
                uncle_pt->color = Color::BLACK;
                node = grandParent;
            }
            else {
                // TODO: Node is right child of its parent
                //  Left-rotation required
                if (node == parent->right) {
                    rotateLeft(parent);
                    node = parent;
                    parent = node->parent;
                }

                // TODO: Node is left child of its parent
                //  Right-rotation required
                rotateRight(grandParent);
                tempColor = parent->color;
                parent->color = grandParent->color;
                grandParent->color = tempColor;
                node = parent;
            }
        }

        // Node Parent is right child of Grand-parent
        else {
            Node<Key> *uncle_pt = grandParent->left;

            // TODO: Uncle is also red
            //  Only Recoloring required
            if ((uncle_pt != nullptr) && (uncle_pt->color == Color::RED)) {
                grandParent->color = Color::RED;
                parent->color = Color::BLACK;
                uncle_pt->color = Color::BLACK;
                node = grandParent;
            }
            else {
                // TODO: Node is left child of its parent
                //  Right-rotation required
                if (node == parent->left) {
                    rotateRight(parent);
                    node = parent;
                    parent = node->parent;
                }

                // TODO: Node is right child of its parent
                //  Left-rotation required
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


/**
 * @brief Performs a left rotation on the specified node of the Red-Black tree.
 *
 * A left rotation is applied when certain imbalances occur in the tree
 * during insertion or deletion operations. The method modifies the tree
 * structure by re-arranging nodes and updating their parent-child relationships.
 *
 * @param node A pointer to the Node<Key> that acts as the pivot for the left rotation.
 *
 *        node                       right
 *       /    \                     /    \
 *      A    right        =>      node    C
 *          /    \              /    \
 *         B      C            A      B
 *
 * Note:
 * 1. If the node undergoing rotation has a right child, that child becomes
 *    the new parent after rotation.
 * 2. The previous parent of the node (if any) will now point to this right child.
 * 3. The left child of the right child (if any) will become the new right child of the node.
 */
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


/**
 * @brief Performs a right rotation on the specified node of the Red-Black tree.
 *
 * A right rotation is applied when certain imbalances occur in the tree
 * during insertion or deletion operations. The method modifies the tree
 * structure by re-arranging nodes and updating their parent-child relationships.
 *
 * @param node A pointer to the Node<Key> that acts as the pivot for the right rotation.
 *
 *        node                       left
 *       /    \                     /    \
 *     left    C        =>        A     node
 *    /    \                           /    \
 *   A      B                         B      C
 *
 * Note:
 * 1. If the node undergoing rotation has a left child, that child becomes
 *    the new parent after rotation.
 * 2. The previous parent of the node (if any) will now point to this left child.
 * 3. The right child of the left child (if any) will become the new left child of the node.
 */
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


/**
 * @brief Deletes a node with only one child from the Red-Black tree.
 *
 * This method is specifically designed to handle the scenario where the node
 * to be deleted has exactly one child (either left or right). The method ensures
 * that the Red-Black tree properties are maintained after the node's deletion.
 *
 * @param node A pointer to the Node<Key> that is to be deleted.
 *             This node should have either a left child or a right child, but not both.
 *
 * If the node being deleted is the root, its single child becomes the new root.
 * The color of the child is set to BLACK.
 * The parent pointer of the child is updated to point to the grandparent of the node.
 * If the child's color is BLACK (after deletion), the tree may violate the Red-Black
 * properties, hence 'deleteFix()' is called to fix any potential violations.
 *
 * Note:
 * 1. The method assumes that the node provided as the parameter has only one child.
 * 2. The 'Color::BLUE' indicates a deleting (leaf)node which arises during deletion in
 *    a Red-Black tree. This is a temporary state used to handle deletion scenarios
 *    and will be resolved to maintain the Red-Black tree properties.
 */
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


/**
 * @brief Fixes the Red-Black tree after a deletion operation.
 *
 * This method is used to restore the properties of the Red-Black tree after a node has been
 * deleted. Deletion may cause imbalances and property violations, which are addressed by this
 * method through a series of rotations and color changes.
 *
 * @param node A pointer to the Node<Key> that is being fixed after the deletion.
 *
 * This method operates in a loop until the node is either the root or its color is not BLUE.
 * During each iteration, the sibling node of the given node is identified.
 * Depending on the color of the sibling and the colors of the sibling's children, various
 * operations are performed - rotations (either left or right) and color changes.
 * These operations are aimed at balancing the tree and ensuring that the Red-Black properties
 * are maintained. Once the tree is fixed, the color of the node is set to BLACK.
 *
 * Note:
 * 1. The BLUE color here represents a double black situation caused by the deletion in a Red-Black tree.
 * 2. The main aim of the fix operation is to remove the double black (BLUE) by redistributing the
 *    color or by merging nodes.
 * 3. The while loop ensures that the tree is fixed in an iterative manner, starting from the
 *    deleted node's position and moving up the tree if necessary.
 * 4. The function takes into account various cases like whether the sibling is red or black,
 *    or if one of the sibling's children is red, and so on.
 *
 */
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


/**
 * @brief Recursively clears the Red-Black tree.
 *
 * This method recursively traverses the Red-Black tree in a post-order
 * and deletes all the nodes, effectively clearing the tree.
 *
 * @param node A pointer to the current Node<Key> being processed.
 *
 * If the node is nullptr, the function returns immediately.
 * The method calls itself recursively for the left child of the node.
 * The method calls itself recursively for the right child of the node.
 * If reached to the leaf node, delete node
 */
template<typename Key>
void UnorderedSet<Key>::clearRecursive(Node<Key> *node) {
    if (node == NULL) return;
    clearRecursive(node->left);
    clearRecursive(node->right);
    delete node;
}
