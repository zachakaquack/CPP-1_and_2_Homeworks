#pragma once

template <typename T> class Node {
  public:
    Node() {};

    // funky constructor bullshit
    Node(const T &x) : m_data(x) {};
    ~Node() {
        // delete this->m_next;
        this->m_next = nullptr;
    }

    // the next node in the list
    Node<T> *next() const { return this->m_next; }
    Node<T> *setNext(Node<T> *x) {
        this->m_next = x;
        return x;
    }

    // the data
    T data() const { return this->m_data; }
    T setData(const T &x) {
        this->m_data = x;
        return x;
    }

  private:
    T m_data;
    Node<T> *m_next = nullptr;
};
