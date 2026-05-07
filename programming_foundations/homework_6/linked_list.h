#pragma once

#include "node.h"
#include <cstddef>
#include <memory>

using namespace std;
class LinkedList {
  public:
    LinkedList() {};
    ~LinkedList() {}

    shared_ptr<Node> root() { return this->m_root; }
    shared_ptr<Node> setRoot(const shared_ptr<Node> &x);

    shared_ptr<Node> append(shared_ptr<Node> &x);
    shared_ptr<Node> last();

    size_t size() { return this->m_size; };

    shared_ptr<Node> get(const int index);
    shared_ptr<Node> operator[](const int &index);

  private:
    shared_ptr<Node> m_root = nullptr;
    size_t m_size = 0;
};
