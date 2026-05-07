#pragma once

#include "chain_link.h"
#include <memory>

using namespace std;
class Node {
  public:
    Node(const ChainLink &data) : m_data(data) { this->setData(data); }
    ~Node() {}

    ChainLink data() { return this->m_data; }
    ChainLink setData(const ChainLink &x);

    shared_ptr<Node> next() { return this->m_next; }
    shared_ptr<Node> setNext(const shared_ptr<Node> &x);

  private:
    ChainLink m_data;
    shared_ptr<Node> m_next;
};
