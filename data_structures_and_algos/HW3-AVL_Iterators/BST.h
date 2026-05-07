#pragma once

#include "Node.h"
#include <iomanip>
#include <stdexcept>
#include <vector>

// Usage:
// T is the typename of the payload data
// If T is a user-defined type, the ==, <, and > operators must be defined
// for use in this BST

template <typename T> class BST {
  public:
    // Constructor
    BST() { this->m_root = nullptr; }

    // Destructor
    ~BST() {
        // Remove all nodes, freeing their memory
        deleteAll(this->m_root);
    }

    Node<T> *root() { return this->m_root; }

    void setRoot(Node<T> *newRoot) {
        this->m_root = newRoot;
        return;
    }

    // Get minimum data value
    Node<T> *findMinimum(Node<T> *current) {

        if (current->left() != nullptr) {
            if (current->left()->data() == nullptr) {
                return current;
            }
        }

        return findMinimum(current->left());
    }

    // Get maximum data value
    Node<T> *findMaximum(Node<T> *current) {

        if (current->right() != nullptr) {
            if (current->right()->data() == nullptr) {
                return current;
            }
        }

        return findMaximum(current->right());
    }

    // Tree Operations

    // Insert new node
    Node<T> *insert(T newData) {

        // Empty BST, make this the root node
        if (this->m_root == nullptr) {
            this->m_root = new Node<T>(newData);

            Node<T> *leftDummyNode = new Node<T>();
            Node<T> *rightDummyNode = new Node<T>();

            leftDummyNode->setParent(this->m_root);
            rightDummyNode->setParent(this->m_root);

            this->m_root->setLeft(leftDummyNode);
            this->m_root->setRight(rightDummyNode);

            return this->m_root;
        }

        return this->f_insert(this->m_root, newData);
    }

    // removes A SPECIFIC, KNOWN, ELEMENT >:(((
    bool remove(T target) {

        // Search BST for target
        // If found, delete the node and associated payload data, return true
        // If not found, return false
        return this->f_remove(this->m_root, target);
    }

    // Search for a data value
    Node<T> *search(T target) {

        // Search BST for target
        // If found, return target (entire if user-defined type)
        // If not found, return notFound
        return f_search(this->m_root, target);
    }

    // Traverse Ascending
    std::vector<T> traverseAscending() {
        std::vector<T> result;
        this->f_traverseAscending(this->m_root, result);
        return result;
    }

    // Traverse Descending
    std::vector<T> traverseDescending() {
        std::vector<T> result;
        this->f_traverseDescending(this->m_root, result);
        return result;
    }

    // Diagnostic Dump
    std::vector<std::string> dump() {
        std::vector<std::string> result;
        this->f_dump(this->m_root, result);
        return result;
    }

    // -------------------------------------------
    // ------------ PRIVATE START NOW ------------
    // -------------------------------------------

  private:
    // root of the entire tree
    Node<T> *m_root;

    Node<T> *f_insert(Node<T> *current, T data) {

        // Recursive Insert routine
        if (data == *current->data()) {
            // BST does not support duplicate values
            // Ignore this Insert request
            return nullptr;
        }

        const bool leftIsNullptr = (current->left() == nullptr);
        const bool rightIsNullptr = (current->right() == nullptr);

        bool isLCDummyNode = false;
        if (!leftIsNullptr) {
            isLCDummyNode = (current->left()->data() == nullptr);
        }

        bool isRCDummyNode = false;
        if (!rightIsNullptr) {
            isRCDummyNode = (current->right()->data() == nullptr);
        }

        // go left downwards the tree
        if (data < *current->data()) {
            if (isLCDummyNode) {
                // left child is a dummy node, so we are at the minimum value in
                // the tree now we can move / create a new dummy node
                Node<T> *oldDummyNode = current->left();
                oldDummyNode->setData(data);

                // set the old dummy node to have a new DN
                Node<T> *newDummyNode = new Node<T>;
                oldDummyNode->setLeft(newDummyNode);
                newDummyNode->setParent(oldDummyNode);

                return oldDummyNode;

            } else if (!leftIsNullptr) {
                return this->f_insert(current->left(), data);
            }

            Node<T> *newNode = new Node<T>(data);

            newNode->setParent(current);
            current->setLeft(newNode);

            return newNode;
        }

        // go right downwards the tree
        if (data > *current->data()) {
            if (isRCDummyNode) {
                // right child is a dummy node, so we are at the maximum value
                // in the tree now we can move / create a new dummy node

                Node<T> *oldDummyNode = current->right();
                oldDummyNode->setData(data);

                // set the old dummy node to have a new DN
                Node<T> *newDummyNode = new Node<T>;
                oldDummyNode->setRight(newDummyNode);
                newDummyNode->setParent(oldDummyNode);

                return oldDummyNode;

            } else if (!rightIsNullptr) {
                return this->f_insert(current->right(), data);
            }

            Node<T> *newNode = new Node<T>(data);

            newNode->setParent(current);
            current->setRight(newNode);

            return newNode;
        }

        // somehow got through, panic
        throw std::runtime_error(
            "CHECK F_INSERT; SOMEHOW GOT THROUGH ALL THREE IF STATEMENTS!!!!");
        return nullptr;
    }

    // Recursive Remove routine
    bool f_remove(Node<T> *current, T target) {

        const bool leftIsNullptr = (current->left() == nullptr);
        const bool rightIsNullptr = (current->right() == nullptr);

        // go left down the tree
        if (target < *current->data()) {
            if (leftIsNullptr) {
                return false;
            }

            return this->f_remove(current->left(), target);
        }

        // go right down the tree
        if (target > *current->data()) {
            if (rightIsNullptr) {
                return false;
            }

            return this->f_remove(current->right(), target);
        }

        // guarenteed return true

        // target == current->data
        if (leftIsNullptr && rightIsNullptr) {

            // Case 1: Leaf node with no children
            if (current->parent()->left() == current) {

                // Current node is left child of its parent
                current->parent()->setLeft(current->left());

            } else {

                // Current node is right child of its parent
                current->parent()->setRight(current->left());
            }

            // Free payload memory
            delete current;

            return true;
        }

        // case 2: single child on left
        if (!leftIsNullptr && rightIsNullptr) {
            if (current->parent()->left() == current) {

                // Current node is left child of its parent
                current->parent()->setLeft(current->left());

            } else {

                // Current node is right child of its parent
                current->parent()->setRight(current->left());
            }

            current->left()->setParent(current->parent());

            // Free entire Node
            delete current;

            return true;
        }

        // case 3: single child on right
        if (leftIsNullptr && !rightIsNullptr) {

            // Current node is left child of its parent
            if (current->parent()->left() == current) {
                current->parent()->setLeft(current->right());
            } else {
                // Current node is right child of its parent
                current->parent()->setRight(current->right());
            }

            current->right()->setParent(current->parent());

            // Free entire Node
            delete current;
            return true;
        }

        // case 4: children on both sides
        // Replace current Node's payload with
        // payload from minimum Node in right subtree
        // ... then delete the min node in right subtree
        Node<T> *minimumRightNode = findMinimum(current->right());

        // Replace data in current node with data from minRight node
        current->setData(minimumRightNode->data());

        // This node has no left child, but could have a right child
        const bool nodeHasRightChild = (minimumRightNode != nullptr);

        if (nodeHasRightChild) {

            // Single right child
            if (minimumRightNode->parent()->left() == minimumRightNode) {

                // minimumRightNode node is left child of its parent
                minimumRightNode->parent()->setLeft(minimumRightNode->right());

            } else {

                // minimumRightNode node is right child of its parent
                minimumRightNode->parent()->setRight(minimumRightNode->right());
            }

        } else {
            // No children, so just delete the node
            if (minimumRightNode->parent()->left() == minimumRightNode) {

                // minimumRightNode node is left child of its parent
                minimumRightNode->parent()->setLeft(nullptr);

            } else {

                // minimumRightNode node is right child of its parent
                minimumRightNode->parent()->setRight(nullptr);
            }
        }

        delete minimumRightNode;
        return true;
    }

    Node<T> *f_search(Node<T> *current, T target) {
        if (target == *current->data()) {
            return current;
        }

        const bool leftIsNullptr = (current->left() == nullptr);
        const bool rightIsNullptr = (current->right() == nullptr);

        if (target < *current->data()) {

            if (leftIsNullptr) {
                return nullptr;
            }

            return f_search(current->left(), target);

        } else if (target > *current->data()) {
            if (rightIsNullptr) {
                return nullptr;
            }

            return f_search(current->right(), target);
        }

        throw std::runtime_error(
            "CHECK F_SEARCH; SOMEHOW GOT THROUGH ALL TWO IF STATEMENTS!!!!");
        return nullptr;
    }

    void f_traverseAscending(Node<T> *current, std::vector<T> &result) {

        // Recursive Traverse routine
        if (current == nullptr) {
            return;
        }

        const bool leftIsNullptr = (current->left() == nullptr);
        const bool rightIsNullptr = (current->right() == nullptr);

        if (!leftIsNullptr) {
            this->f_traverseAscending(current->left(), result);
        }

        result.push_back(current->data());

        if (!rightIsNullptr) {
            this->f_traverseAscending(current->right(), result);
        }
    }

    void f_traverseDescending(Node<T> *current, std::vector<T> &result) {

        // Recursive Traverse routine

        if (current == nullptr) {
            return;
        }

        if (current->right() != nullptr) {
            this->f_traverseDescending(current->right(), result);
        }

        result.push_back(current->data());

        if (current->left() != nullptr) {
            this->f_traverseDescending(current->left(), result);
        }
    }

    void f_dump(Node<T> *current, std::vector<std::string> &result) {

        // Recursive Dump routine
        if (current == nullptr) {
            return;
        }

        const bool leftIsNullptr = (current->left() == nullptr);
        const bool rightIsNullptr = (current->right() == nullptr);

        if (!leftIsNullptr) {
            this->f_dump(current->left(), result);
        }

        // Format a dump of the current Node
        std::stringstream ss;

        // Crashes on windows specifically for some reason
        /*
        ss << "Node@" << std::setw(6) << current
           << ": data = " << current->data() << ", parent=" << current->parent()
           << ", left=" << current->left() << ", right=" << current->right();
           */

        result.push_back(ss.str());

        if (!rightIsNullptr) {
            this->f_dump(current->right(), result);
        }
    }

    void deleteAll(Node<T> *current) {
        if (current == nullptr) {
            return;
        }

        // Recursive deleteAll routine
        // Delete left subtree
        if (current->left() != nullptr) {
            deleteAll(current->left());
        }

        // Delete right subtree
        if (current->right() != nullptr) {
            deleteAll(current->right());
        }

        // Delete current node and it's data
        delete current;
    }
};
