#pragma once

template <typename T> class Node {

  private:
    T *m_data;
    Node *m_parent = nullptr;
    Node *m_left = nullptr;
    Node *m_right = nullptr;

  public:
    // Constructor
    Node() {}
    Node(T data) { this->m_data = new T{data}; }
    ~Node() {
        delete m_data;
        // TODO: fix deleting
        // delete this->m_left;
        // delete this->m_right;
    }

    // Getters
    T *data() { return this->m_data; }
    Node *parent() { return this->m_parent; }
    Node *left() { return this->m_left; }
    Node *right() { return this->m_right; }

    // Setters
    void setData(T *data) { this->m_data = data; }
    void setData(T data) { this->m_data = new T{data}; }
    void setParent(Node *newParent) { this->m_parent = newParent; }
    void setLeft(Node *newLeft) { this->m_left = newLeft; }
    void setRight(Node *newRight) { this->m_right = newRight; }

    // these cant be const for some reason, it crashes
    friend bool operator==(Node<T> &lhs, Node<T> &rhs) {
        return *lhs.data() == *rhs.data();
    }
    friend bool operator<(Node<T> &lhs, Node<T> &rhs) {
        return *lhs.data() < *rhs.data();
    }
    friend bool operator>(Node<T> &lhs, Node<T> &rhs) { return rhs < lhs; }
};
