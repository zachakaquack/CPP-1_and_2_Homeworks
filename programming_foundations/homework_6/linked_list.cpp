#include "linked_list.h"
#include <stdexcept>
#include <string>

using namespace std;
shared_ptr<Node> LinkedList::setRoot(const shared_ptr<Node> &x) {
    this->m_root = x;
    return x;
}

shared_ptr<Node> LinkedList::append(shared_ptr<Node> &x) {

    this->m_size += 1;

    if (this->root() == nullptr) {
        this->setRoot(x);
        return x;
    }

    this->last()->setNext(x);

    return x;
}

shared_ptr<Node> LinkedList::last() {

    if (this->root() == nullptr) {
        throw out_of_range("You did not set the root node!!!");
    }

    shared_ptr<Node> last = this->root();

    while (last->next() != nullptr) {
        last = last->next();
    }

    return last;
}

shared_ptr<Node> LinkedList::get(const int index) {
    if (index > int(this->size()) - 1) {
        throw out_of_range(
            "Index " + to_string(index) +
            " out of range! (list size: " + to_string(this->size()) + ")");
    }

    shared_ptr<Node> node = this->root();
    for (int i = 0; i < index; i += 1) {
        if (node->next() != nullptr) {
            node = node->next();
        }
    }
    return node;
}
shared_ptr<Node> LinkedList::operator[](const int &index) {
    return this->get(index);
}
