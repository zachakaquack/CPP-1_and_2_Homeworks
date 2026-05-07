#pragma once

#include "node.h"
#include <cstddef>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>

template <typename T> class LinkedList {
  public:
    LinkedList(const bool &isNaturalOrder = true)
        : m_isNaturalOrder(isNaturalOrder) {};
    Node<T> *root() const { return this->m_root; }
    size_t size() const { return this->m_size; }

    bool contains(const T &x) const {
        Node<T> *current = this->root();
        while (current->next() != nullptr) {

            // duplicate found, don't do anything
            const bool listContainsX = (current->data() == x);
            if (listContainsX) {
                return true;
            }

            current = current->next();
        }

        // current->next() != nullptr does not actually check the last node,
        // so we have to check it manually
        if (current->data() == x) {
            return true;
        }

        return false;
    }

    // removes A SPECIFIC, KNOWN, ELEMENT >:(((
    void remove(const T &element) {

        Node<T> *node = this->root();
        Node<T> *prev_node = nullptr;

        // FIRST check if the node is at the start
        if (node->data() == element && prev_node == nullptr) {
            this->m_root = node->next();
            delete node;
            return; // we're already done :D
        }

        // but if the element isn't at the start, we loop
        for (size_t i = 0; i < this->size(); i += 1) {

            // advance by one...
            prev_node = node;
            node = node->next();

            if (node->data() != element) {
                continue;
            }

            // if its at the end, just set the ending ptr to nullptr
            const bool lastItemInList = (node->next() == nullptr);
            if (lastItemInList) {
                prev_node->setNext(nullptr);
                delete node;
            }

            // if not last item, just remove the one in the middle then
            // delete the node
            if (!lastItemInList) {
                prev_node->setNext(node->next());
                delete node;
            }

            this->m_size -= 1;
            return;
        }
    }

    // add something to the list. takes just T and constructs the node itself
    bool append(const T &x) {

        Node<T> *node = new Node<T>(x);

        if (this->root() == nullptr) {
            this->m_root = node;
            this->m_size += 1;
            return true;
        }

        // root is guarenteed to be set at this point
        Node<T> *current = this->root();
        Node<T> *previous = nullptr;

        if (current->data() == x) {
            return false;
        }

        while (true) {

            // duplicate found, don't do anything
            const bool nextIsDuplicate = (current->data() == x);
            if (nextIsDuplicate) {
                return false;
            }

            const bool correctNaturalPositionFound =
                (!m_isNaturalOrder && current->data() < x);
            const bool correctUnNaturalPositionFound =
                (m_isNaturalOrder && current->data() > x);
            const bool correctPositionFound =
                correctNaturalPositionFound || correctUnNaturalPositionFound;

            // case where we just started looping
            if (previous == nullptr) {

                // if we're at the right spot, then add
                if (correctPositionFound) {
                    node->setNext(current);
                    this->m_root = node;
                    this->m_size += 1;
                    return true;
                }
            }

            // found the correct position, so add
            if (correctPositionFound) {
                previous->setNext(node);
                node->setNext(current);
                this->m_size += 1;
                return true;
            }

            // end of the list, just append
            if (current->next() == nullptr) {
                current->setNext(node);
                this->m_size += 1;
                return true;
            }

            // just iterate forward
            previous = current;
            current = current->next();
        }
    }

    // list.get(index) is the same as list[index]
    // basically iterate through the list until we reach what we want
    Node<T> *get(const size_t &index) const {
        if (index > size_t(this->size()) - 1) {
            std::ostringstream os;
            os << "Index " << index
               << " out of range! (list size: " << this->size() << ")";
            throw std::out_of_range(os.str());
        }

        Node<T> *node = this->root();
        for (size_t i = 0; i < index; i += 1) {
            if (node->next() != nullptr) {
                node = node->next();
            }
        }
        return node;
    }

    class Iterator {
      public:
        Iterator() {}
        Iterator(Node<T> *node) { current = node; }

        T operator*() {
            T deref = this->current->data();
            return deref;
        }

        Iterator operator++() {
            current = current->next();
            return *this;
        }

        Iterator operator++(int) {
            Node<T> *temp = current;
            current = current->next();
            return temp;
        }

        bool operator!=(const Iterator &rhs) { return current != rhs.current; }

      private:
        Node<T> *current = nullptr;
    };

    // returns the first node and the "last" node
    Iterator begin() const { return Iterator(m_root); }
    Iterator end() const { return Iterator(); }

    Node<T> *operator[](const int &index) const { return this->get(index); }

    friend std::ostream &operator<<(std::ostream &out, const LinkedList &x) {
        for (size_t i = 0; i < x.size(); i += 1) {
            const Node<T> *currentNode = x.get(i);
            out << "Current Memory : " << currentNode << '\n';
            out << "Next Memory : " << currentNode->next() << '\n';
            out << "Value : " << currentNode->data() << '\n';
        }
        return out;
    }

  private:
    // starting node
    Node<T> *m_root = nullptr;

    // overhead but who cares
    size_t m_size = 0;

    // ordering bs
    bool m_isNaturalOrder = true;
};
