#pragma once

#include "linked_list.h"
#include <cstddef>
#include <ostream>
namespace dsa {

// the set class is basically an interface for the linkedlist
template <typename T> class set {
  public:
    set(const bool &isNaturalOrder = true) {
        this->m_list = LinkedList<T>(isNaturalOrder);
    };
    ~set() {};

    // call linkedlist's append function
    bool add(const T &x) { return this->m_list.append(x); }

    // call linkedlist's remove function
    void remove(const T &x) { this->m_list.remove(x); }

    // check if linked list's m_next == nullptr
    bool empty() { return (this->m_list.root() == nullptr); }

    // call linkedlist's size function
    size_t size() { return this->m_list.size(); }

    // call linkedlist's contains function
    bool contains(const T &x) { return this->m_list.contains(x); }

    // good luck
    typename LinkedList<T>::Iterator begin() { return this->m_list.begin(); }
    typename LinkedList<T>::Iterator end() { return this->m_list.end(); }

    friend std::ostream &operator<<(std::ostream &out, const set &x) {
        out << x.m_list;
        return out;
    }

  private:
    LinkedList<T> m_list;
};
}; // namespace dsa
