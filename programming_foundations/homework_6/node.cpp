#include "node.h"
#include <memory>

using namespace std;
ChainLink Node::setData(const ChainLink &x) {
    this->m_data = x;
    return x;
}

shared_ptr<Node> Node::setNext(const shared_ptr<Node> &x) {
    this->m_next = x;
    return x;
}
