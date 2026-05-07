#pragma once

#include "BST.h"

namespace dsa {
template <typename T> class avl_tree {
  private:
    // init it :)
    BST<T> m_tree{};

    bool _checkBalance(Node<T> *b);
    void _balance(Node<T> *b);

    // rotates clockwise (right) by default
    void _rotate(Node<T> *t, Node<T> *m, Node<T> *b, bool rotatingCCW);

  public:
    avl_tree() {}
    ~avl_tree() {}

    class const_forward_iterator;
    class const_reverse_iterator;

    bool insert(const T &x);
    bool contains(const T &x);
    bool remove(const T &x);

    // iterator bullshit
    const_forward_iterator cbegin();
    const_forward_iterator cend();
    const_reverse_iterator crbegin();
    const_reverse_iterator crend();
};

// implementation details only below here

template <typename T> bool avl_tree<T>::_checkBalance(Node<T> *b) {
    // if either parent does not exist, it should be balanced
    // (if there are only two nodes, it is not possible to be unbalanced)

    // skip dummy nodes
    if (b->data() == nullptr) {
        return true;
    }

    // less than 3 nodes, do not balance
    if (b->parent() == nullptr || b->parent()->parent() == nullptr) {
        return true;
    }

    Node<T> *m = b->parent();
    Node<T> *t = m->parent();

    if ((m->left() != nullptr && m->right() != nullptr) ||
        (t->left() != nullptr && t->right() != nullptr)) {
        // TODO: check if the whole tree is balanced, "balance factor"?
        return true;
    }

    return false;
}

template <typename T> void avl_tree<T>::_balance(Node<T> *b) {

    if (this->_checkBalance(b)) {
        return;
    }

    // general process is to identify if it's either 'stacked' or 'linear'.
    // linear balancing are the easiest by far, so we treat that as the
    // default. if the three nodes are treated as stacked, then we transform
    // the stacked nodes into linear nodes.

    Node<T> *m = b->parent();
    Node<T> *t = m->parent();

    // t > m < b (m is left of t, and b is right of m)
    const bool stackedLeftRightNodes = (*t > *m && *m < *b);
    // t < m > b (m is right of t, and b is left of m)
    const bool stackedRightLeftNodes = (*t < *m && *m > *b);

    // check if not linear in 2 ways, left then right or right then left.
    if (stackedLeftRightNodes) {
        t->setLeft(b);
        b->setLeft(m);
        b->setParent(t);
        m->setParent(b);
        m->setRight(nullptr);
    } else if (stackedRightLeftNodes) {
        t->setRight(b);
        b->setRight(m);
        b->setParent(t);
        m->setParent(b);
        m->setLeft(nullptr);
    }
    if (stackedLeftRightNodes || stackedRightLeftNodes) {
        // what was labled b is now m, and vice versa
        //  we can not do this outside of an if statement because we may
        //  have started linear
        b = m;
        m = b->parent();
    }

    // verifying linear status before rotating, cant use the previous
    //  variables because it may have been linear to start
    // t > m > b (linear to the left / we have to rotate CCW)
    const bool rotatingCCW = !(*t > *m && *m > *b);
    this->_rotate(t, m, b, rotatingCCW);
}

template <typename T>
void avl_tree<T>::_rotate(Node<T> *t, Node<T> *m, Node<T> *b,
                          bool rotatingCCW) {
    Node<T> *p = t->parent();

    if (p != nullptr) {
        // these being const crashes the program
        Node<T> *left = p->left();
        Node<T> *right = p->right();
        if (*left == *t) {
            p->setLeft(m);
        } else if (*right == *t) {
            p->setRight(m);
        }
    } else {
        this->m_tree.setRoot(t);
    }

    m->setParent(p);
    t->setParent(m);

    if (rotatingCCW) {
        m->setLeft(t);
        t->setRight(nullptr);
    } else {
        m->setRight(t);
        t->setLeft(nullptr);
    }
}

template <typename T> bool avl_tree<T>::insert(const T &x) {
    // TODO: Check if rotated correctly?
    Node<T> *result = this->m_tree.insert(x);
    if (result == nullptr) {
        return false;
    }
    this->_balance(result);
    return true;
}

template <typename T> bool avl_tree<T>::contains(const T &x) {
    const Node<T> *result = this->m_tree.search(x);

    if (result == nullptr) {
        return false;
    }

    return true;
}

template <typename T> bool avl_tree<T>::remove(const T &x) {
    // TODO: Check if rotated correctly after deletion, may be a non issue
    return this->m_tree.remove(x);
};

template <typename T> class avl_tree<T>::const_forward_iterator {
private:
    Node<T>* currentNode = nullptr;

public:
    const_forward_iterator() {}
    const_forward_iterator(Node<T>* node) { this->currentNode = node; };

    // prefix decrement
    const_forward_iterator operator--() {
        if (this->currentNode->left() != nullptr) {
            this->currentNode = this->currentNode->left();

            if (this->currentNode->right() == nullptr) {
                return *this;
            }

            while (this->currentNode->right() != nullptr) {
                this->currentNode = this->currentNode->right();
            }
            return *this;
        }

        // make sure its not the dummy node
        if (this->currentNode->data() == nullptr) {
            this->currentNode = this->currentNode->parent();
            return *this;
        }

        // find parent that is next in order
        while (*this->currentNode->parent()->data() >
            *this->currentNode->data()) {
            this->currentNode = this->currentNode->parent();
        }
        // do it one more time so that parent is actually the one we want
        this->currentNode = this->currentNode->parent();
        return *this;
    }

    const_forward_iterator operator--(int) {

        // the incrementing logic is within the prefix function
        --*this;
        return *this;
    }

    // prefix increment
    const_forward_iterator& operator++() {
        if (this->currentNode->right() != nullptr) {
            this->currentNode = this->currentNode->right();

            if (this->currentNode->left() == nullptr) {
                return *this;
            }

            while (this->currentNode->left() != nullptr) {
                this->currentNode = this->currentNode->left();
            }
            return *this;
        }

        if (this->currentNode->data() == nullptr) {
            this->currentNode = this->currentNode->parent();
            return *this;
        }

        // find parent that is next in order
        while (*this->currentNode->parent()->data() <
            *this->currentNode->data()) {
            this->currentNode = this->currentNode->parent();
        }

        // do it one more time so that parent is actually the one we want
        this->currentNode = this->currentNode->parent();
        return *this;
    }

    // postfix increment
    const_forward_iterator operator++(int) {
        // the incrementing logic is within the prefix function
        ++*this;
        return *this;
    }

    bool operator==(const const_forward_iterator& rhs) {
        return this->currentNode == rhs.currentNode;
    }
    bool operator!=(const const_forward_iterator& rhs) {
        return !(this->currentNode == rhs.currentNode);
    }
    T operator*() { return *this->currentNode->data(); }
};

template <typename T> class avl_tree<T>::const_reverse_iterator {
private:
    Node<T>* currentNode = nullptr;

public:
    const_reverse_iterator() {}
    const_reverse_iterator(Node<T>* node) { this->currentNode = node; };

    // prefix decrement
    const_reverse_iterator operator++() {
        if (this->currentNode->left() != nullptr) {
            this->currentNode = this->currentNode->left();

            if (this->currentNode->right() == nullptr) {
                return *this;
            }

            while (this->currentNode->right() != nullptr) {
                this->currentNode = this->currentNode->right();
            }
            return *this;
        }

        // make sure its not the dummy node
        if (this->currentNode->data() == nullptr) {
            this->currentNode = this->currentNode->parent();
            return *this;
        }

        // find parent that is next in order
        while (*this->currentNode->parent()->data() >
            *this->currentNode->data()) {
            this->currentNode = this->currentNode->parent();
        }

        // do it one more time so that parent is actually the one we want
        this->currentNode = this->currentNode->parent();
        return *this;
    }

    const_reverse_iterator operator++(int) {

        // the incrementing logic is within the prefix function
        ++*this;
        return *this;
    }

    // prefix increment
    const_reverse_iterator& operator--() {
        if (this->currentNode->right() != nullptr) {
            this->currentNode = this->currentNode->right();

            if (this->currentNode->left() == nullptr) {
                return *this;
            }

            while (this->currentNode->left() != nullptr) {
                this->currentNode = this->currentNode->left();
            }
            return *this;
        }

        if (this->currentNode->data() == nullptr) {
            this->currentNode = this->currentNode->parent();
            return *this;
        }

        // find parent that is next in order
        while (*this->currentNode->parent()->data() <
            *this->currentNode->data()) {
            this->currentNode = this->currentNode->parent();
        }

        // do it one more time so that parent is actually the one we want
        this->currentNode = this->currentNode->parent();
        return *this;
    }

    // postfix increment
    const_reverse_iterator operator--(int) {
        // the incrementing logic is within the prefix function
        --*this;
        return *this;
    }

    bool operator==(const const_reverse_iterator& rhs) {
        return this->currentNode == rhs.currentNode;
    }
    bool operator!=(const const_reverse_iterator& rhs) {
        return !(this->currentNode == rhs.currentNode);
    }
    T operator*() { return *this->currentNode->data(); }
};

template <typename T>
typename avl_tree<T>::const_forward_iterator avl_tree<T>::cbegin() {

    Node<T> *min = this->m_tree.findMinimum(this->m_tree.root());

    return const_forward_iterator(min);
};
template <typename T>
typename avl_tree<T>::const_forward_iterator avl_tree<T>::cend() {
    // TODO: this is supposed to use a dummy pointer to designate the end
    // has been found i believe
    //
    Node<T> *max = this->m_tree.findMaximum(this->m_tree.root());
    Node<T> *maxDummyNode = max->right();

    return const_forward_iterator(maxDummyNode);
}

template <typename T>
typename avl_tree<T>::const_reverse_iterator avl_tree<T>::crbegin() {

    Node<T> *max = this->m_tree.findMaximum(this->m_tree.root());

    return const_reverse_iterator(max);
};
template <typename T>
typename avl_tree<T>::const_reverse_iterator avl_tree<T>::crend() {
    // TODO: this is supposed to use a dummy pointer to designate the end
    // has been found i believe
    //
    Node<T> *min = this->m_tree.findMinimum(this->m_tree.root());
    Node<T> *minDummyNode = min->left();

    return const_reverse_iterator(minDummyNode);
}

}; // namespace dsa
